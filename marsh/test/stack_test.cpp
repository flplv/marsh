/*
 *  Copyright (C) 2013 by Felipe Lavratti
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in the
 *  Software without restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 *  Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 *  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

extern "C" {
#include "helper/stack.h"
#include "helper/stack.c"
}

TEST_GROUP(stack)
{
	my_stack_t *cut;
	struct s_item{
		int32_t a;
		uint8_t b;
	} clone_stack[10];

	void setup()
	{
		cut = stack_new(sizeof(struct s_item));
	}

	void teardown()
	{
		stack_delete(cut);
	}
};

TEST(stack, addAndRead)
{
	for (int var = 0; var < 10; ++var) {
		clone_stack[var].a = var;
		clone_stack[var].b = var%2;
		const int res = stack_add(cut, (BUFFER_PTR_RDOLY)&clone_stack[var]);
		CHECK_EQUAL(var, res);
	}

	struct s_item read_item;
	stack_get(cut, (BUFFER_PTR)&read_item, 8);
	CHECK_EQUAL(clone_stack[8].a, read_item.a);
	CHECK_EQUAL(clone_stack[8].b, read_item.b);
}

TEST(stack, addthree)
{
	CHECK_EQUAL(2, cut->buffer_total_slots);
	CHECK_EQUAL(0, cut->used_slots);
	stack_add(cut, (uint8_t *)&clone_stack[0]);
	CHECK_EQUAL(1, cut->used_slots);
	stack_add(cut, (uint8_t *)&clone_stack[0]);
	CHECK_EQUAL(2, cut->used_slots);
	stack_add(cut, (uint8_t *)&clone_stack[0]);
	CHECK_EQUAL(3, cut->used_slots);
	CHECK_EQUAL(4, cut->buffer_total_slots);
}

TEST(stack, instance)
{
	CHECK_EQUAL(2, cut->buffer_total_slots);
}

