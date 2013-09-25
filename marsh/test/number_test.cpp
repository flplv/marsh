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
#include "helper/number.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(number)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(number, number)
{
	CHECK_TRUE(charactere_is_hex('f'));
	CHECK_FALSE(charactere_is_hex('G'));

	CHECK_TRUE(charactere_is_math_sig('+'));
	CHECK_FALSE(charactere_is_math_sig('.'));

	CHECK_TRUE(charactere_is_space(' '));
	CHECK_FALSE(charactere_is_space('G'));

	CHECK_TRUE(charactere_is_null('\0'));
	CHECK_FALSE(charactere_is_null('2'));

	CHECK_TRUE(charactere_hex_to_int('F') == 15);

	CHECK_EQUAL(-255, string_hex_to_int("-FF"));

	CHECK_EQUAL(10, string_hex_to_int("+0A"));

	CHECK_EQUAL(10, string_hex_to_int("+0A+829"));
	CHECK_EQUAL(32, string_hex_to_int("   \t\n 0020+829"));

	CHECK_EQUAL(-1, string_hex_to_int("FFFFFFFF"));

	char str[10];
	int_to_string(12345, str, 10);
	STRCMP_EQUAL("12345", str)
}
