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

extern "C" {
#include "framebuffer.h"
#include "dimension.h"
#include "widget.h"
#include "widget_owner.h"
#include "widget_private.h"
#include "rectangle.h"
#include "object_stack.h"
#include "draw_manager.h"

#include "dimension.c"
#include "draw_manager.c"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(drawManager)
{
	static void draw1mock(void *)
	{
		output_intercepter("draw1");
	}
	static void draw2mock(void *)
	{
		output_intercepter("draw2");
	}
	static void draw3mock(void *)
	{
		output_intercepter("draw3");
	}
	static void destroy1mock (void * const)
	{
		output_intercepter("destroy1");
	}
	static void destroy2mock (void * const)
	{
		output_intercepter("destroy2");
	}
	static void destroy3mock (void * const)
	{
		output_intercepter("destroy3");
	}

	draw_manager_t * cut;
	widget_owner_t *owner1, *owner2, *owner3;
	widget_t *obj1, *obj2, *obj3;
	object_stack_t * stack;

	void setup()
	{
		framebuffer_create();
		owner1 = widget_owner_create(this, draw1mock, destroy1mock);
		owner2 = widget_owner_create(this, draw2mock, destroy2mock);
		owner3 = widget_owner_create(this, draw3mock, destroy3mock);
		obj1 = widget_create(owner1);
		obj2 = widget_create(owner2);
		obj3 = widget_create(owner3);

		stack = object_stack_create();
		cut = draw_manager_create();

		dimension_set_size(&obj1->dim, 100, 100);
		dimension_set_start_position(&obj1->dim, 100, 100);
		dimension_set_rest_if_possible(&obj1->dim);

		dimension_set_size(&obj2->dim, 100, 100);
		dimension_set_start_position(&obj2->dim, 200, 200);
		dimension_set_rest_if_possible(&obj2->dim);

		dimension_set_size(&obj3->dim, 100, 100);
		dimension_set_start_position(&obj3->dim, 300, 300);
		dimension_set_rest_if_possible(&obj3->dim);

		object_stack_add(stack, obj1);
		object_stack_add(stack, obj2);
		object_stack_add(stack, obj3);
	}

	void teardown()
	{
		draw_manager_destroy(cut);
		object_stack_destroy(stack);
		widget_destroy(obj3);
		widget_destroy(obj2);
		widget_destroy(obj1);
		widget_owner_destroy(owner1);
		widget_owner_destroy(owner2);
		widget_owner_destroy(owner3);
		framebuffer_destroy();
	}
};

TEST(drawManager, stackConnection)
{
	marshmallow_terminal_output = output_intercepter;
	CHECK_FALSE(check_stack(cut));
	marshmallow_terminal_output = _stdout_output_impl;
	draw_manager_assossiate_drawing_stack(cut, stack);
	POINTERS_EQUAL(stack, cut->assossiated_stack);
	CHECK_TRUE(check_stack(cut));
}

TEST(drawManager, drawOrder)
{
	draw_manager_assossiate_drawing_stack(cut, stack);
	draw_manager_draw(cut, obj2);

	STRCMP_EQUAL("draw2", intercepted_output[1]);
	STRCMP_EQUAL("draw3", intercepted_output[0]);

	draw_manager_draw_all(cut);
	STRCMP_EQUAL("draw1", intercepted_output[2]);
	STRCMP_EQUAL("draw2", intercepted_output[1]);
	STRCMP_EQUAL("draw3", intercepted_output[0]);
}
