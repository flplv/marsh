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
#include "color.c"
#include "framebuffer.h"
#include "event.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_private.h"
#include "rectangle.h"
#include "rectangle.c"

}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(WidgetRectangle)
{
	rectangle_t * cut;
	void setup()
	{
		marshmallow_terminal_output = output_intercepter;
		framebuffer_new();
		event_pool_init();
		cut = rectangle_new(NULL);
	}

	void teardown()
	{
		rectangle_delete(cut);
		event_pool_deinit();
		framebuffer_delete();
		marshmallow_terminal_output = _stdout_output_impl;
	}
};

TEST(WidgetRectangle, Instance)
{
	CHECK_TRUE(cut != NULL);
	CHECK_TRUE(cut->log != NULL);
	CHECK_TRUE(cut->glyph != NULL);
	CHECK_TRUE(cut->glyph->creator_instance != NULL);
	CHECK_TRUE(cut->glyph->creator_draw  != NULL);
	CHECK_TRUE(cut->glyph->creator_delete != NULL);
//	rectangle_delete(cut);
//	rectangle_delete(cut);
//	STRCMP_CONTAINS("rectangle", intercepted_output[2]);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(WidgetRectangle, DestroyOwner)
{
	rectangle_t * cut2;
	cut2 = rectangle_new(NULL);
	widget_delete(rectangle_get_widget(cut2));
//	rectangle_delete(cut2);
//	STRCMP_CONTAINS("rectangle", intercepted_output[2]);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(WidgetRectangle, Draw)
{
	cut->glyph->creator_draw(cut);
	STRCMP_CONTAINS("Object not initialized properly, can't draw.", intercepted_output[0]);

	CHECK_EQUAL(0x0000, *framebuffer_at(5, 5));
	rectangle_set_position(cut, 0, 0);
	rectangle_set_size(cut, 10, 10);
	rectangle_set_fill_color_html(cut, "#FFFFFF");
	widget_tree_draw(rectangle_get_widget(cut));
	CHECK_EQUAL(0xFFFF, *framebuffer_at(5, 5));
}

TEST(WidgetRectangle, foreground_color)
{
	struct s_color expected_color;
	expected_color.red = 0x80;
	expected_color.green = 0x20;
	expected_color.blue = 0x80;
	expected_color.flag = VALID_COLOR_FLAG;

	rectangle_set_fill_color_html(cut, "#80208");
	STRCMP_CONTAINS("Color", intercepted_output[2]);
	STRCMP_CONTAINS("Bad Html Code", intercepted_output[0]);

	rectangle_set_fill_color_html(cut, "#802080");
	CHECK_EQUAL(*(int*)&expected_color, (int)cut->fill_color);
}

TEST(WidgetRectangle, position_end1)
{
	rectangle_set_size(cut, 10, 20);
	rectangle_set_position(cut, 50, 60);

	CHECK_EQUAL(60, area_end_point(widget_area(cut->glyph)).x);
	CHECK_EQUAL(80, area_end_point(widget_area(cut->glyph)).y);
}
