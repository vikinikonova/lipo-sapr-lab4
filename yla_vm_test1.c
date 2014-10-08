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

static int test_gencode()
{
    char buf[100];
    char *ptr = buf;
    
    put_commd(&ptr, 0x12);
    put_value(&ptr, 0x3456);

    YLATEST_ASSERT_TRUE(buf[0] == 0x12, "put_commd");
    YLATEST_ASSERT_TRUE(buf[1] == 0x34, "put_value");
    YLATEST_ASSERT_TRUE(buf[2] == 0x56, "put_value");
    
    return 0;
}

static int test_init_null()
{
    yla_vm vm;
    char buf[100];

    YLATEST_ASSERT_FALSE(yla_vm_init(&vm, NULL, 100), "init NULL");
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm)==YLA_VM_ERROR_NO_PROGRAM_CODE, "init NULL");
    
    return 0;
}

static int test_init_0()
{
    yla_cop_type code[1];
    yla_vm vm;

    YLATEST_ASSERT_FALSE(yla_vm_init(&vm, code, 0), "init 0 size code");
    YLATEST_ASSERT_TRUE(yla_vm_last_error(&vm)==YLA_VM_ERROR_NO_PROGRAM_CODE, "init 0 size code");

    return 0;
}

YLATEST_BEGIN(yla_vm_test1)
  YLATEST_ADD_TEST_CASE(test_gencode)
  YLATEST_ADD_TEST_CASE(test_init_null)
  YLATEST_ADD_TEST_CASE(test_init_0)
YLATEST_END
