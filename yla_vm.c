/*
    Virtual Machine

    This file is part of YLA VM (Yet another Language for Academic purpose: Virtual Machine).

    YLA VM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <string.h>

#include "yla_cop.h"
#include "yla_vm.h"


int yla_vm_get_value(yla_vm *vm, yla_stack_type *value);

int yla_vm_set_var(yla_vm *vm, size_t index, yla_stack_type value);
int yla_vm_get_var(yla_vm *vm, size_t index, yla_stack_type *value);

yla_stack_type yla_vm_get_value_internal(unsigned char *start);


int yla_vm_read_header(yla_vm *vm, unsigned char *program, size_t program_size);
int yla_vm_check_magic(unsigned char** program);
int yla_vm_read_sizes(yla_vm *vm, unsigned char **program);


/*
Public functions
*/

int yla_vm_init(yla_vm *vm, unsigned char *program, size_t program_size)
{
	if (!vm) {
		return 0;
	}

	if (!yla_vm_read_header(vm, program, program_size)) {
		vm->last_error = ERROR_NO_PROGRAM_CODE;
		return 0;
	}
	
	yla_stack_init(&vm->stack, vm->stack_size);
	vm->vartable = calloc(vm->vartable_size, sizeof(yla_stack_type));
	vm->code = malloc(vm->code_size);
	memcpy(vm->code, program + HEADER_SIZE, program_size - HEADER_SIZE);

	return 1;
}

int yla_vm_done(yla_vm *vm)
{
	if (!vm) {
		return 0;
	}

	if (vm->code) {
		free(vm->code);
	}

	if (vm->vartable) {
		free(vm->code);
	}

	yla_stack_done(&vm->stack);
	
	return 1;
}


/*
Private functions
*/

int yla_vm_get_value(yla_vm *vm, yla_stack_type *value)
{
	if (vm->pc + sizeof(yla_stack_type) >= vm->pc) {
		vm->last_error = ERROR_CODE_SEG_EXCEED;
		return 0;
	}
	
	*value = yla_vm_get_value_internal(vm->code);
	vm->pc += sizeof(yla_stack_type);

	return 1;
}

yla_stack_type yla_vm_get_value_internal(unsigned char *start)
{
	unsigned int full_value = 0;
	size_t i=0;
	for (i=0; i<sizeof(yla_stack_type); ++i) {
		full_value <<= 1;
		unsigned char byte = *start++;
		full_value |= byte;
	}
	return full_value;
}

int yla_vm_read_header(yla_vm *vm, unsigned char *program, size_t program_size)
{
	if (program == NULL) {
		return 0;
	}
	if (program_size < HEADER_SIZE) {
		return 0;
	}
	if (!yla_vm_check_magic(&program)) {
		return 0;
	}
	if (!yla_vm_read_sizes(vm, &program)) {
		return 0;
	}
	
	return 1;
}

int yla_vm_check_magic(unsigned char** program)
{
	yla_stack_type magic_value;
	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE1) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE2) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	magic_value = yla_vm_get_value_internal(*program);
	if (magic_value != MAGIC_CODE3) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	return 1;
}

int yla_vm_read_sizes(yla_vm *vm, unsigned char **program)
{
	vm->stack_size = yla_vm_get_value_internal(*program);
	if (vm->code_size > MAX_STACK_SIZE) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	vm->vartable_size = yla_vm_get_value_internal(*program);
	if (vm->vartable_size > MAX_VARTABLE_SIZE) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	vm->code_size = yla_vm_get_value_internal(*program);
	if (vm->code_size > MAX_CODE_SIZE) {
		return 0;
	}
	*program += sizeof(yla_stack_type);

	return 1;
}

/*
Vartable
*/
int yla_vm_get_var(yla_vm *vm, size_t index, yla_stack_type *value)
{
	if (!vm) {
		return 0;
	}
	if (index >= vm->vartable_size) {
		return 0;
	}

	*value = vm->vartable[index];

	return 1;
}

int yla_vm_set_var(yla_vm *vm, size_t index, yla_stack_type value)
{
	if (!vm) {
		return 0;
	}
	if (index >= vm->vartable_size) {
		return 0;
	}

	vm->vartable[index] = value;
}

/*
Perform command by code of operation
*/
int do_command(yla_vm *vm, unsigned char cop)
{
	yla_stack_type op1;
	yla_stack_type op2;
	yla_stack_type res;

	switch(cop) {

		case CNOP:	
			break;

		case CPUSH:
			if (!yla_vm_get_value(vm, &res)) {
				return 0;
			}
			if (!yla_stack_push(&vm->stack, res)) {
				return 0;
			}
			break;

		case CADD:
			if (!yla_stack_pull(&vm->stack, &op1)) {
				return 0;
			}
			if (!yla_stack_pull(&vm->stack, &op2)) {
				return 0;
			}
			res = op2 + op1;
			if (!yla_stack_push(&vm->stack, res)) {
				return 0;
			}
			break;

		case CSUB: 
			if (!yla_stack_pull(&vm->stack, &op1)) {
				return 0;
			}
			if (!yla_stack_pull(&vm->stack, &op2)) {
				return 0;
			}
			res = op2 - op1;
			if (!yla_stack_push(&vm->stack, res)) {
				return 0;
			}
			break;

		case CMULT:
			if (!yla_stack_pull(&vm->stack, &op1)) {
				return 0;
			}
			if (!yla_stack_pull(&vm->stack, &op2)) {
				return 0;
			}
			res = op2 * op1;
			if (!yla_stack_push(&vm->stack, res)) {
				return 0;
			}
			break;

		case CDIV: 
			if (!yla_stack_pull(&vm->stack, &op1)) {
				return 0;
			}
			if (!yla_stack_pull(&vm->stack, &op2)) {
				return 0;
			}
			if (op1==0) {
				vm->last_error = ERROR_DIV_BY_ZERO;
				return 0;
			}
			res = op2 / op1;
			if (!yla_stack_push(&vm->stack, res)) {
				return 0;
			}
			break;

		case CHALT:
			return -1;

		default:
			vm->last_error = ERROR_UNKNOWN_COMMAND;
			return 0;
	}
	return 1;
}
