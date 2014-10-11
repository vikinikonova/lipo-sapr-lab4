/*
    Virtual machine Test #1

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

#include "yla_vm.h"
#include "yla_cop.h"
#include "yla_test.h"
#include "yla_test_gencode.h"

static int test_null_vm()
{
    yla_int_type buf[100];

    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(NULL, buf, 100) == -1, "NULL vm");

    return 0;
}

static int test_zero_stack()
{
    yla_cop_type prg[HEADER_SIZE + 1];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 0, 0, 1);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    yla_int_type buf[100];

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 1), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(&vm, buf, 100) == 0, "stack must be empty");
    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == -1, "halt expected")
    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(&vm, buf, 100) == 0, "stack must be empty");
    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

static void fill_int_type_arr(yla_int_type *buf, size_t size, yla_int_type value)
{
    int i;
    for (i=0; i<100; i++) {
        buf[i] = value;
    }
}

static int test_push()
{
    yla_cop_type prg[HEADER_SIZE + 4];
    yla_cop_type *ptr = prg;

    put_header(&ptr, 1, 0, 4);
    put_commd(&ptr, CPUSH);
    put_value(&ptr, 0x1234);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    yla_int_type buf[100];
    fill_int_type_arr(buf, 100, -1);

    YLATEST_ASSERT_TRUE(yla_vm_init(&vm, prg, HEADER_SIZE + 4), "normal");
    YLATEST_ASSERT_TRUE(yla_vm_run(&vm), "normal")

    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(&vm, NULL, 0) == 1, "stack must have 1 value");
    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(&vm, buf, 1) == 0, "stack must copy 1 value OK");
    YLATEST_ASSERT_TRUE(buf[0] == 0x1234, "incorrect stack value");

    YLATEST_ASSERT_TRUE(yla_vm_done(&vm), "normal");

    return 0;
}

YLATEST_BEGIN(yla_vm_test_stack_trace)
  YLATEST_ADD_TEST_CASE(test_null_vm)
  YLATEST_ADD_TEST_CASE(test_zero_stack)
  YLATEST_ADD_TEST_CASE(test_push)
YLATEST_END
