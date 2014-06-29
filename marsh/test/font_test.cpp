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

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

extern "C" {
#include "font.h"
#include "font.c"
#include "helper/my_string.h"
}

TEST_GROUP(font)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(font, sizes)
{
	my_string_t * str;
	str = my_string_new();
	my_string_set(str, "a");

	CHECK_EQUAL(ubuntu_monospace_16->height, height(ubuntu_monospace_16));
	CHECK_EQUAL(ubuntu_monospace_16->width[ubuntu_monospace_16->index['a']], font_string_width(ubuntu_monospace_16, str));

	my_string_set(str, "abc");
	CHECK_EQUAL(24, font_string_width(ubuntu_monospace_16, str));

	my_string_set(str, "abc\ndef");
	CHECK_EQUAL(24, font_string_width(ubuntu_monospace_16, str));

	my_string_delete(str);
}

