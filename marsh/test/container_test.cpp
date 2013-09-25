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
#include "helper/log.h"
#include "color.h"
#include "framebuffer.h"
#include "signalslot.h"
#include "widget_private.h"
#include "rectangle.h"
#include "container.h"
#include "container.c"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(WidgetContainer)
{
	container_t * cut;

	void setup()
	{
		cut = container_create();
	}

	void teardown()
	{
		container_destroy(cut);
		DISABLE_INTERCEPTION;
	}
};

TEST(WidgetContainer, instance)
{
	CHECK_TRUE(cut != NULL);
	CHECK_TRUE(INSTANCE_GET(cut) == SIGNATURE_CONTAINER);
	CHECK_TRUE(cut->log != NULL);
	CHECK_TRUE(cut->glyph != NULL);
	CHECK_TRUE(cut->glyph->owner!= NULL);
	CHECK_TRUE(cut->stack != NULL);
	CHECK_TRUE(cut->self_reference->owner_instance != NULL);
	CHECK_TRUE(cut->self_reference->draw  != NULL);
	CHECK_TRUE(cut->self_reference->destroy  != NULL);
	ENABLE_INTERCEPTION;
	container_destroy(cut);
	container_destroy(cut);
	STRCMP_CONTAINS("container", intercepted_output[2]);
	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(WidgetContainer, DestroyFromBase)
{
	ENABLE_INTERCEPTION;
	widget_destroy_owner(container_get_widget(cut));
	container_destroy(cut);
	STRCMP_CONTAINS("container", intercepted_output[2]);
	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(WidgetContainer, AddAndDestroy)
{
	ENABLE_INTERCEPTION;
	rectangle_t *rect = rectangle_create();
	container_add(cut, rectangle_get_widget(rect));
	container_destroy(cut);
}

rectangle_t *rect1;
rectangle_t *rect2;
rectangle_t *rect3;

static bool called = false;
static void call (void) { called = true; };
static bool called2 = false;
static void call2 (void) { called2 = true; };

TEST(WidgetContainer, dimensionsAndDraw)
{
	widget_t *cut_gobj = container_get_widget(cut);

	rect1 = rectangle_create();
	rect2 = rectangle_create();
	rect3 = rectangle_create();

	rectangle_set_size(rect1, 100, 100);
	rectangle_set_size(rect2, 100, 100);
	rectangle_set_size(rect3, 100, 100);
	rectangle_set_position(rect1, 10, 10);
	rectangle_set_position(rect2, 1, 100);
	rectangle_set_position(rect3, 200, 200);

	container_add(cut, rectangle_get_widget(rect1));
	container_add(cut, rectangle_get_widget(rect2));
	container_add(cut, rectangle_get_widget(rect3));

	update_dimensions(cut);

	CHECK_EQUAL(1, cut_gobj->dim.pos.start.x);
	CHECK_EQUAL(10, cut_gobj->dim.pos.start.y);
	CHECK_EQUAL(300-1, cut_gobj->dim.pos.end.x);
	CHECK_EQUAL(300-1, cut_gobj->dim.pos.end.y);

	rectangle_set_fill_color_html(rect1, "#FF0000");
	rectangle_set_fill_color_html(rect2, "#00FF00");
	rectangle_set_fill_color_html(rect3, "#0000FF");

	framebuffer_create();

	CHECK_EQUAL(0x0000, *framebuffer_at(10, 10));
	CHECK_EQUAL(0x0000, *framebuffer_at(1, 100));
	CHECK_EQUAL(0x0000, *framebuffer_at(200, 200));

	container_draw(cut);

	CHECK_EQUAL(color_to_pixel(color_html("#FF0000")), *framebuffer_at(10, 10));
	CHECK_EQUAL(color_to_pixel(color_html("#00FF00")), *framebuffer_at(1, 100));
	CHECK_EQUAL(color_to_pixel(color_html("#0000FF")), *framebuffer_at(200, 200));

	slot2_t *slot = slot2_create();
	slot2_set(slot, (slot2_func)call, NULL);

	slot2_t *slot2 = slot2_create();
	slot2_set(slot2, (slot2_func)call2, NULL);

	signal2_t *if_sig;
	if_sig = interaction_engine_get_press_signal(widget_get_interaction_engine(
			container_get_widget(cut)));
	slot2_connect(slot, if_sig);

	if_sig = interaction_engine_get_release_signal(widget_get_interaction_engine(
			container_get_widget(cut)));
	slot2_connect(slot2, if_sig);

	CHECK_FALSE(called);
	CHECK_FALSE(called2);

	interact_press(container_get_widget(cut), 10, 10);
	interact_release(container_get_widget(cut), 10, 10);

	CHECK_TRUE(called);
	CHECK_TRUE(called2);

	slot2_destroy(slot);
	slot2_destroy(slot2);

	framebuffer_destroy();
}
