/*
    Generate code for test Virtual Machine

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

#include "yla_test_gencode.h"
#include "yla_type.h"

void put_commd(yla_cop_type** prg, yla_cop_type value)
{
	**prg = value;
	(*prg)++;
}

void put_value(yla_cop_type** prg, yla_int_type value)
{
	int i;
	unsigned char* ptr = (unsigned char*)&value;
	for (i=0; i<sizeof(yla_int_type); i++)
		put_commd(prg, ptr[i]);
}

void put_header(yla_cop_type** prg, size_t stack_size, size_t vartable_size, size_t code_size)
{
    put_value(prg, MAGIC_CODE1);
    put_value(prg, MAGIC_CODE2);
    put_value(prg, MAGIC_CODE3);
    put_value(prg, stack_size);
    put_value(prg, vartable_size);
    put_value(prg, code_size);
}