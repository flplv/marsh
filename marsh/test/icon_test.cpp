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
#include "framebuffer.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_private.h"
#include "bitmap_data/bitmaps.h"
#include "event.h"
#include "icon.h"
#include "icon.c"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(icon)
{
	icon_t * cut;
	void setup()
	{
		marshmallow_terminal_output = output_intercepter;
		framebuffer_init();
		event_pool_init();
		cut = icon_new(NULL);
	}

	void teardown()
	{
		icon_delete(cut);
		event_pool_deinit();
		framebuffer_deinit();
		marshmallow_terminal_output = _stdout_output_impl;
	}
};

TEST(icon, Instance)
{
//	icon_delete(cut);
//	icon_delete(cut);
//	STRCMP_CONTAINS("icon", intercepted_output[2]);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(icon, DestroyOwner)
{
	icon_t * cut2;
	cut2 = icon_new(NULL);
	widget_delete(icon_get_widget(cut2));
//	icon_delete(cut2);
//	STRCMP_CONTAINS("icon", intercepted_output[2]);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(icon, Draw)
{
	cut->glyph->creator_draw(cut);
	STRCMP_CONTAINS("Object not initialized properly, can't draw.", intercepted_output[0]);

	CHECK_EQUAL(0x0000, *framebuffer_at(1, 10));
	icon_set_position(cut, 0, 0);
	icon_set_bitmap(cut, wifi_icon_bitmap);
	widget_tree_draw(icon_get_widget(cut));
	CHECK_EQUAL(0xFFFF, *framebuffer_at(1, 10));
}

