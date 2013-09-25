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
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(color)
{
	color_t cut;
	void setup()
	{
		cut = color_html("#806020");
	}

	void teardown()
	{
	}
};

TEST(color, typeSize)
{
	CHECK_TRUE (sizeof(int) == sizeof(struct s_color));
}

TEST(color, html_to_data)
{
	CHECK_FALSE(color_check_html_code(""));
	CHECK_FALSE(color_check_html_code("ffffff"));
	CHECK_FALSE(color_check_html_code("%FFFFFF"));
	CHECK_FALSE(color_check_html_code("#FfFfFG"));
	CHECK_FALSE(color_check_html_code("#.00000"));
	CHECK_FALSE(color_check_html_code("#00000ç"));
	CHECK_FALSE(color_check_html_code("#00z000"));

}

TEST(color, colorGood)
{
	CHECK_FALSE(color_check(0));
	CHECK_TRUE(color_check(cut));
}

TEST(color, colorConvert16bpp)
{
	color_t color, color2;
	pixel_t foregroundColor;

	color = color_html("#880000");
	foregroundColor = color_to_pixel(color);
	CHECK_EQUAL(0x8800, foregroundColor);

	color = color_html("#008400");
	foregroundColor = color_to_pixel(color);
	CHECK_EQUAL(0x0420, foregroundColor);

	color = color_html("#000088");
	foregroundColor = color_to_pixel(color);
	CHECK_EQUAL(0x0011, foregroundColor);

	color = color_html("#FFFFFF");
	foregroundColor = color_to_pixel(color);
	color2 = color_from_pixel(foregroundColor);
	CHECK_EQUAL(color, color2);

	color = color_html("#808080");
	foregroundColor = color_to_pixel(color);
	color2 = color_from_pixel(foregroundColor);
	CHECK_EQUAL(color, color2);
}

TEST (color, blend)
{
	color_t color_red = color(255,0,0);
	color_t color_blue = color(0,255,0);
	color_t color_blended = color_alpha_blend(color_red, color_blue, 127);

	CHECK_EQUAL(color(128,127,0), color_blended);
}

TEST(color, color)
{
	CHECK_EQUAL(color(0x10, 0x20, 0x30), color_html("#102030"));
}
