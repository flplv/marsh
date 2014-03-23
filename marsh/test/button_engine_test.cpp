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
#include "button_engine.h"
#include "widget.h"
#include "rectangle.h"
#include "framebuffer.h"
#include "color.h"
#include "interact.h"
#include "interaction_engine.h"
#include "signalslot.h"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

static bool action_called = false;
static void action(void)
{
	action_called = true;
}

TEST_GROUP(buttonEngine)
{
	button_engine_t *cut;
	void setup()
	{
		action_called = false;
		cut = button_engine_new();
	}

	void teardown()
	{
		button_engine_delete(cut);
		DISABLE_INTERCEPTION;
	}
};

//TEST(buttonEngine, instance)
//{
//	ENABLE_INTERCEPTION;
//	button_engine_delete(cut);
//	button_engine_delete(cut);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
//}

TEST(buttonEngine, bad)
{
	rectangle_t *normal, *onpress;

	normal = rectangle_new();
	onpress = rectangle_new();

	rectangle_set_fill_color_html(normal, "#800000");
	rectangle_set_fill_color_html(onpress, "#FF0000");
	rectangle_set_size(normal, 100, 100);
	rectangle_set_size(onpress, 100, 100);
	rectangle_set_position(normal, 10, 10);
	rectangle_set_position(onpress, 10, 10);

	button_engine_set_action_normal_state(cut, rectangle_get_widget(normal));
	button_engine_set_action_onpress_state(cut, rectangle_get_widget(onpress));

	framebuffer_new();

	ENABLE_INTERCEPTION;
	widget_draw(button_engine_get_widget(cut));
	STRCMP_CONTAINS("No Logic selected", intercepted_output[0]);

	framebuffer_delete();

	rectangle_delete(normal);
	rectangle_delete(onpress);
}


TEST(buttonEngine, action)
{
	rectangle_t *normal, *onpress;

	normal = rectangle_new();
	onpress = rectangle_new();

	rectangle_set_fill_color_html(normal, "#800000");
	rectangle_set_fill_color_html(onpress, "#FF0000");
	rectangle_set_size(normal, 100, 100);
	rectangle_set_size(onpress, 100, 100);
	rectangle_set_position(normal, 10, 10);
	rectangle_set_position(onpress, 10, 10);

	slot_t *slot = slot_new();
	slot_set(slot, (slot_func)action, NULL);

	slot_connect(slot,
			interaction_engine_get_click_signal(
					widget_get_interaction_engine(
							button_engine_get_widget(cut)))
				);

	button_engine_set_action_normal_state(cut, rectangle_get_widget(normal));
	button_engine_set_action_onpress_state(cut, rectangle_get_widget(onpress));

	framebuffer_new();

	CHECK_EQUAL(0x0000, *framebuffer_at(10, 10));
	button_engine_select_action_logic(cut);
	widget_draw(button_engine_get_widget(cut));
	CHECK_EQUAL(color_to_pixel(color_html("#800000")), *framebuffer_at(10, 10));
	interact_press(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#FF0000")), *framebuffer_at(10, 10));
	interact_release(button_engine_get_widget(cut), 5, 5);
	CHECK_EQUAL(color_to_pixel(color_html("#800000")), *framebuffer_at(10, 10));

	CHECK_FALSE(action_called);
	interaction_engine_click(widget_get_interaction_engine(button_engine_get_widget(cut)));
	CHECK_TRUE(action_called);

	framebuffer_delete();

	rectangle_delete(normal);
	rectangle_delete(onpress);
	slot_delete(slot);
}

TEST(buttonEngine, toggle)
{
	rectangle_t *off, *on, *on_to_off, *off_to_on;

	off = rectangle_new();
	on = rectangle_new();
	on_to_off = rectangle_new();
	off_to_on = rectangle_new();

	rectangle_set_fill_color_html(off, "#800000");
	rectangle_set_fill_color_html(off_to_on, "#FF0000");
	rectangle_set_fill_color_html(on, "#008000");
	rectangle_set_fill_color_html(on_to_off, "#00FF00");

	rectangle_set_size(off, 100, 100);
	rectangle_set_size(off_to_on, 100, 100);
	rectangle_set_position(off, 10, 10);
	rectangle_set_position(off_to_on, 10, 10);

	rectangle_set_size(on, 100, 100);
	rectangle_set_size(on_to_off, 100, 100);
	rectangle_set_position(on, 10, 10);
	rectangle_set_position(on_to_off, 10, 10);

	button_engine_set_toggle_on_state(cut, rectangle_get_widget(on));
	button_engine_set_toggle_off_state(cut, rectangle_get_widget(off));
	button_engine_set_toggle_on_to_off_state(cut, rectangle_get_widget(on_to_off));
	button_engine_set_toggle_off_to_on_state(cut, rectangle_get_widget(off_to_on));

	button_engine_select_toggle_logic(cut);

	framebuffer_new();
	CHECK_EQUAL(0x0000, *framebuffer_at(10, 10));

	widget_draw(button_engine_get_widget(cut));
	CHECK_EQUAL(color_to_pixel(color_html("#800000")), *framebuffer_at(10, 10));

	interact_press(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#FF0000")), *framebuffer_at(10, 10));

	interact_release(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#008000")), *framebuffer_at(10, 10));

	interact_press(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#00FF00")), *framebuffer_at(10, 10));

	interact_release(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#800000")), *framebuffer_at(10, 10));

	interact_press(button_engine_get_widget(cut), 10, 10);
	CHECK_EQUAL(color_to_pixel(color_html("#FF0000")), *framebuffer_at(10, 10));

	interact_release(button_engine_get_widget(cut), 5, 5);
	CHECK_EQUAL(color_to_pixel(color_html("#800000")), *framebuffer_at(10, 10));


	framebuffer_delete();

	rectangle_delete(on);
	rectangle_delete(off);
	rectangle_delete(on_to_off);
	rectangle_delete(off_to_on);
}
