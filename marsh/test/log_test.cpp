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
#include "helper/log_private.h"
}
#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(log)
{
	my_log_t * cut;
	void setup()
	{
		cut = my_log_new("TEST", MESSAGE);
		cut->stream_output = output_intercepter;
	}

	void teardown()
	{
		my_log_delete(cut);
	}
};

TEST(log, instance)
{
	my_log(MESSAGE, __FILE__, __LINE__, "message", cut);
	STRCMP_CONTAINS("message", intercepted_output[3]);
	my_log(WARNING, __FILE__, __LINE__, "warning", cut);
	STRCMP_CONTAINS("warning", intercepted_output[3]);
	my_log(ERROR, __FILE__, __LINE__, "error", cut);
	STRCMP_CONTAINS("error", intercepted_output[3]);
}
