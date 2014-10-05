/*
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

#include "yla_stack.h"
#include "yla_test.h"

int test_init_n()
{
    yla_stack stack;
    yla_stack_init(&stack, 3);

    yla_int_type result = 0;

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must empty just after creating");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "stack size more then 0 couldn't be full just after creating");
    YLATEST_ASSERT_FALSE(yla_stack_top(&stack, &result), "no top can be accessible just after creating");

    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 33), "stack with deep 3 could able to push 1 value");
    YLATEST_ASSERT_TRUE(yla_stack_top(&stack, &result), "stack with deep 3 could able to push 1 value");

    YLATEST_ASSERT_FALSE(yla_stack_is_empty(&stack), "stack must NOT empty after 1 push");
    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 45), "stack with deep 3 could able to push 2 value");
    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 77), "stack with deep 3 could able to push 3 value");

    YLATEST_ASSERT_TRUE(yla_stack_is_full(&stack), "stack with deep 3 must be full after 3 push");
    YLATEST_ASSERT_FALSE(yla_stack_push(&stack, 99), "stack with deep 3 could NOT able to push 4 value");
    YLATEST_ASSERT_TRUE(yla_stack_is_full(&stack), "stack with deep 3 must be full after 3 push");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 1 value");
    YLATEST_ASSERT_TRUE(result==77, "incorrect value after 1 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 2 value");
    YLATEST_ASSERT_TRUE(result==45, "incorrect value after 2 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 3 value");
    YLATEST_ASSERT_TRUE(result==33, "incorrect value after 3 pull");


    YLATEST_ASSERT_FALSE(yla_stack_pull(&stack, &result), "stack with deep 3 could NOT able to pull 4 value");

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must be empty after pull last value");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "stack couldn't be full after last pull");
}

YLATEST_BEGIN(yla_stack_test)
  YLATEST_ADD_TEST_CASE(test_init_n)
YLATEST_END

/*
int yla_stack_push(yla_stack* stack, yla_int_type value);
int yla_stack_pull(yla_stack* stack, yla_int_type *result);

int yla_stack_set_deep(yla_stack* stack, size_t index, yla_int_type value);
int yla_stack_get_deep(yla_stack* stack, size_t index, yla_int_type *result);

int yla_stack_top(yla_stack* stack, yla_int_type *result);

int yla_stack_is_empty(yla_stack* stack);
int yla_stack_is_full(yla_stack* stack);
*/
