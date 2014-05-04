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
#include "color.h"
#include "color_private.h"
#include "event.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_private.h"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

static bool called = false;
static void call(void *){
	called = true;
}

TEST_GROUP(Widget)
{
	widget_t * cut;

	void setup()
	{
		marshmallow_terminal_output = output_intercepter;
		event_pool_init();
		cut = widget_new(NULL, NULL, NULL, NULL);
		called = false;
	}

	void teardown()
	{
		event_pool_deinit();
		widget_delete_instance(cut);
		marshmallow_terminal_output = _stdout_output_impl;
	}
};

TEST(Widget, instance)
{
	CHECK_EQUAL((void*)0, cut->creator_instance);
	CHECK_EQUAL((void(*)(void*))0, cut->creator_draw);
	CHECK_EQUAL((void(*)(void*))0, cut->creator_delete);
	CHECK_TRUE((void*)cut->click_signal);
	CHECK_TRUE((void*)cut->press_signal);
	CHECK_TRUE((void*)cut->release_signal);

	CHECK_TRUE(cut->event_handler_list);
	CHECK_EQUAL((void*)0, cut->tree.child);
	CHECK_EQUAL((void*)0, cut->tree.parent);
	CHECK_EQUAL((void*)0, cut->tree.left);
	CHECK_EQUAL((void*)0, cut->tree.right);
}

TEST(Widget, destroy)
{
	widget_t * cut2;
	cut2 = widget_new(NULL, this, (void(*)(void*))2, call);
	widget_virtual_delete(cut2);
	widget_delete_instance(cut2);
	CHECK_TRUE(called);
}

TEST(Widget, tree_deletion)
{
	CHECK_TRUE(cut->tree.child == NULL);

	widget_t * cut2;
	cut2 = widget_new(cut, NULL, NULL, NULL);

	CHECK_TRUE(cut->tree.child == cut2);

	widget_new(cut2, NULL, NULL, NULL);
	widget_new(cut2, NULL, NULL, NULL);
	widget_new(cut2, NULL, NULL, NULL);
	widget_new(cut2, NULL, NULL, NULL);
	widget_new(widget_child(cut2), NULL, NULL, NULL);
	widget_new(widget_child(cut2), NULL, NULL, NULL);
	widget_new(widget_child(cut2), NULL, NULL, NULL);
	widget_new(widget_last_child(cut2), NULL, NULL, NULL);
	widget_new(widget_last_child(cut2), NULL, NULL, NULL);
	widget_new(widget_last_child(cut2), NULL, NULL, NULL);
	widget_new(widget_last_child(cut2), NULL, NULL, NULL);
	widget_new(widget_last_child(cut2), NULL, NULL, NULL);

	CHECK_EQUAL(1, widget_num_of_children(cut));
	CHECK_EQUAL(4, widget_num_of_children(cut2));
	CHECK_EQUAL(3, widget_num_of_children(widget_child(cut2)));
	CHECK_EQUAL(5, widget_num_of_children(widget_last_child(cut2)));

	widget_delete(cut2);

	CHECK_TRUE(cut->tree.child == NULL);
}
