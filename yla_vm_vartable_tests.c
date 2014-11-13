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

static int test_zero_vm()
{
    yla_int_type result = -1;

    YLATEST_ASSERT_FALSE(yla_vm_set_var(NULL, 0, 10), "zero vm");
    YLATEST_ASSERT_FALSE(yla_vm_get_var(NULL, 0, &result), "zero vm");

    return 0;
}

static int test_zero_table()
{
    yla_cop_type prg[HEADER_SIZE + 1];
    yla_cop_type *ptr = prg;
    yla_int_type result = -1;

    put_header(&ptr, 0, 0, 1);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    yla_vm_init(&vm, prg, HEADER_SIZE + 1);

    YLATEST_ASSERT_FALSE(yla_vm_set_var(&vm, 0, 10), "set 0 for zero var table");
    YLATEST_ASSERT_FALSE(yla_vm_get_var(&vm, 0, &result), "set 0 for zero var table");

    yla_vm_do_command(&vm);
    yla_vm_done(&vm);

    return 0;
}

static int test_table_ok_internal(yla_int_type val)
{
    yla_cop_type prg[HEADER_SIZE + 1];
    yla_cop_type *ptr = prg;
    yla_int_type result = -1;

    put_header(&ptr, 0, 1, 1);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    yla_vm_init(&vm, prg, HEADER_SIZE + 1);

    YLATEST_ASSERT_TRUE(yla_vm_set_var(&vm, 0, val), "set 0 for zero var table");
    YLATEST_ASSERT_TRUE(yla_vm_get_var(&vm, 0, &result), "set 0 for zero var table");
    YLATEST_ASSERT_TRUE(result == val, "var changed after load");

    yla_vm_do_command(&vm);
    yla_vm_done(&vm);

    return 0;
}

static int test_table_ok()
{
    test_table_ok_internal(10);
    test_table_ok_internal(7432);
}

static int test_load_ok_internal(yla_int_type val)
{
    yla_cop_type prg[HEADER_SIZE + 4];
    yla_cop_type *ptr = prg;

    yla_int_type result = (yla_int_type)-1;
    yla_int_type buf[100];

    put_header(&ptr, 1, 1, 4);
    put_commd(&ptr, CLOAD);
    put_value(&ptr, 0x0);
    put_commd(&ptr, CHALT);

    yla_vm vm;

    yla_vm_init(&vm, prg, HEADER_SIZE + 4);

    YLATEST_ASSERT_TRUE(yla_vm_set_var(&vm, 0, val), "set 0 for non-zero var table");

    YLATEST_ASSERT_TRUE(yla_vm_do_command(&vm) == 1, "LOAD command performing error");

    YLATEST_ASSERT_TRUE(yla_vm_stack_trace(&vm, buf, 1) == 0, "stack must have 1 value");
    YLATEST_ASSERT_TRUE(buf[0] == val, "incorrect value in stack after load var");
    YLATEST_ASSERT_TRUE(yla_vm_get_var(&vm, 0, &result), "get value from VM must work OK");
    YLATEST_ASSERT_TRUE(result == val, "var changed after load");

    yla_vm_do_command(&vm);
    yla_vm_done(&vm);

    return 0;
}

static int test_load_ok()
{
    YLATEST_ASSERT_FALSE(test_load_ok_internal(0), "0");
    YLATEST_ASSERT_FALSE(test_load_ok_internal(3841), "random value");
    YLATEST_ASSERT_FALSE(test_load_ok_internal((yla_int_type)-1), "-1");
    YLATEST_ASSERT_FALSE(test_load_ok_internal(1), "1");

    return 0;
}

YLATEST_BEGIN(yla_vm_vartable_tests)
  YLATEST_ADD_TEST_CASE(test_zero_vm)
  YLATEST_ADD_TEST_CASE(test_zero_table)
  YLATEST_ADD_TEST_CASE(test_table_ok)
  YLATEST_ADD_TEST_CASE(test_load_ok)
YLATEST_END
