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

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

extern "C" {

#include "helper/my_string.h"
#include "helper/my_string.c"
}

TEST_GROUP(my_string)
{
	my_string_t * cut;
	void setup()
	{
		cut = my_string_new();
	}

	void teardown()
	{
		my_string_delete(cut);
	}
};

TEST(my_string, instance)
{
	STRCMP_EQUAL("", my_string_get(cut));
	my_string_set(cut, "tweedledee ");
	STRCMP_EQUAL("tweedledee ", my_string_get(cut));

	my_string_set(cut, "tweedledee tweedledee");
	STRCMP_EQUAL("tweedledee tweedledee", my_string_get(cut));

	my_string_set(cut, "tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee");
	STRCMP_EQUAL("tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee tweedledee", my_string_get(cut));
}
