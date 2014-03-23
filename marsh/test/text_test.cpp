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

#include <cstring>

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

#include "mocks/terminal_intercepter.h"

extern "C" {
#include "text.h"
#include "text.c"
#include "helper/my_string.h"
}

TEST_GROUP(text)
{
	text_t *cut;

	void setup()
	{
		cut = text_new();
	}

	void teardown()
	{
		text_delete(cut);
		DISABLE_INTERCEPTION;
	}
};

TEST(text, instance)
{
	ENABLE_INTERCEPTION;

	CHECK_TRUE(cut != NULL);
//	text_delete(cut);
//	text_delete(cut);
//	STRCMP_CONTAINS("text", intercepted_output[2]);
//	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}

TEST(text, sizes)
{
	int width;
	int height;

	width = widget_get_dimension(cut->glyph)->size.width;
	height = widget_get_dimension(cut->glyph)->size.height;

	CHECK_EQUAL(0, width);
	CHECK_EQUAL(0, height);

	text_set_reference_position(cut, 0, 0);

	my_string_set(text_get_string(cut), "tweedledum");
	text_set_font(cut, ubuntu_monospace_16);

	width = widget_get_dimension(cut->glyph)->size.width;
	height = widget_get_dimension(cut->glyph)->size.height;
	CHECK_EQUAL(80, width);
	CHECK_EQUAL(16, height);
}

