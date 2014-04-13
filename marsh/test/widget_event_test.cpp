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
#include "types.h"
#include "widget_event.h"
#include "widget_event.c"
}

TEST_GROUP(widget_event)
{
	void setup()
	{
	}

	void teardown()
	{
		DISABLE_INTERCEPTION;
	}
};

TEST(widget_event, leaks)
{
	//TODO: Add checks on returns of the install
	widget_t wid;
	widget_event_init(&wid.event_handler_list);

	CHECK_TRUE(0 == widget_event_install_handler(&wid, 13, NULL));
	CHECK_FALSE(-1 == widget_event_install_handler(&wid, 13, NULL));

	widget_event_deinit(&wid.event_handler_list);
}
