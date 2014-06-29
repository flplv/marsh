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
#include <string.h>
#include "area.h"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(area)
{
	area_t dim;

	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(area, contains)
{
	area_set_size(&dim, 10, 10);
	area_set_start_xy(&dim, 10, 10);

	CHECK_TRUE(area_contains_point(&dim,  (point_t){10, 10}));
	CHECK_TRUE(area_contains_point(&dim,  (point_t){15, 15}));
	CHECK_TRUE(area_contains_point(&dim,  (point_t){19, 19}));
	CHECK_FALSE(area_contains_point(&dim, (point_t){20, 20}));
}

TEST(area, compare)
{
	area_t a, b;

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 0, 0, 100, 100);

	CHECK_TRUE(memcmp(&a, &b, sizeof(a)) == 0);
	CHECK_TRUE(area_same(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 10, 10, 80, 80);

	CHECK_FALSE(memcmp(&a, &b, sizeof(a)) == 0);
	CHECK_FALSE(area_same(&a, &b));
}

TEST(area, negative)
{
	area_t a;

	area_set(&a, 0, 0, 100, 100);

	CHECK_EQUAL(0, area_start_point(&a).x);
	CHECK_EQUAL(0, area_start_point(&a).y);
	CHECK_EQUAL(100, area_end_point(&a).x);
	CHECK_EQUAL(100, area_end_point(&a).y);

	area_set(&a, -1, -1, -100, -100);

	CHECK_EQUAL(-1, area_start_point(&a).x);
	CHECK_EQUAL(-1, area_start_point(&a).y);
	CHECK_EQUAL(-101, area_end_point(&a).x);
	CHECK_EQUAL(-101, area_end_point(&a).y);
}

TEST(area, intersects)
{
	area_t a, b;

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 10, 10, 80, 80);

	CHECK_TRUE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 0, 0, 100, 100);

	CHECK_TRUE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 10, 10, 90, 90);

	CHECK_TRUE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, -10, -10, 800, 800);

	CHECK_TRUE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, -10, -10, -80, -80);

	CHECK_FALSE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 100, 100, -20, -20);

	CHECK_TRUE(area_intersects(&a, &b));

	area_set(&a, 0, 0, 100, 100);
	area_set(&b, 100, 100, 20, 20);

	CHECK_FALSE(area_intersects(&a, &b));
}


