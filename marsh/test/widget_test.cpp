/*
 *  Copyright (C) 2013 to 2014 by Felipe Lavratti
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
		widget_delete_instance_only(cut);
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
	widget_delete(cut2);
	widget_delete_instance_only(cut2);
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

	widget_tree_delete(cut2);

	CHECK_TRUE(cut->tree.child == NULL);
}

TEST(Widget, dimension)
{
	widget_t * parent;
	widget_t * child;
	widget_t * child2;

	parent = widget_new(NULL, NULL, NULL, NULL);
	child = widget_new(parent, NULL, NULL, NULL);
	child2 = widget_new(child, NULL, NULL, NULL);

	widget_set_area(parent, 10, 10, 200, 200);
	widget_set_area(child, 10, 10, 10, 10);
	widget_set_area(child2, 10, 10, 10, 10);

	CHECK_EQUAL(10, parent->absolute_dim.x);
	CHECK_EQUAL(10, parent->absolute_dim.y);

	CHECK_EQUAL(20, child->absolute_dim.x);
	CHECK_EQUAL(20, child->absolute_dim.y);

	CHECK_EQUAL(30, child2->absolute_dim.x);
	CHECK_EQUAL(30, child2->absolute_dim.y);

	CHECK_FALSE(widget_canvas_cropped(parent));
	CHECK_FALSE(widget_canvas_cropped(child));
	CHECK_TRUE(widget_canvas_cropped(child2));

	widget_set_area(parent, 20, 20, 200, 200);

	CHECK_EQUAL(20, parent->absolute_dim.x);
	CHECK_EQUAL(20, parent->absolute_dim.y);

	CHECK_EQUAL(30, child->absolute_dim.x);
	CHECK_EQUAL(30, child->absolute_dim.y);

	CHECK_EQUAL(40, child2->absolute_dim.x);
	CHECK_EQUAL(40, child2->absolute_dim.y);

	CHECK_FALSE(widget_canvas_cropped(parent));
	CHECK_FALSE(widget_canvas_cropped(child));
	CHECK_TRUE(widget_canvas_cropped(child2));

	widget_set_area(child, 10, 10, 20, 20);

	CHECK_FALSE(widget_canvas_cropped(parent));
	CHECK_FALSE(widget_canvas_cropped(child));
	CHECK_FALSE(widget_canvas_cropped(child2));

	widget_tree_delete(parent);
}


TEST(Widget, visibility)
{
	widget_t * parent;
	widget_t * child;
	widget_t * child2;

	parent = widget_new(NULL, NULL, NULL, NULL);
	child = widget_new(parent, NULL, NULL, NULL);
	child2 = widget_new(child, NULL, NULL, NULL);

	CHECK_TRUE(widget_tree_ancestors_visible(child2));
	CHECK_TRUE(widget_visible(child2));

	CHECK_TRUE(widget_visible(parent));
	widget_hide(parent);
	CHECK_FALSE(widget_visible(parent));

	CHECK_FALSE(widget_tree_ancestors_visible(child2));

	widget_show(parent);
	CHECK_TRUE(widget_visible(parent));

	CHECK_TRUE(widget_tree_ancestors_visible(child2));

	widget_hide(child);
	CHECK_FALSE(widget_tree_ancestors_visible(child2));

	widget_tree_delete(parent);
}

