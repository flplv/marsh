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
#include "types.h"
#include "signalslot.h"
#include "signalslot2.h"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(signalslot)
{
	signal_t * signal;
	signal2_t * signal2;
	slot_t * slot;
	slot2_t * slot2;

	void setup()
	{
		signal = signal_create();
		slot = slot_create();
		signal2 = signal2_create();
		slot2 = slot2_create();
		called = false;

	}

	void teardown()
	{
		signal_destroy(signal);
		slot_destroy(slot);
		signal2_destroy(signal2);
		slot2_destroy(slot2);
		DISABLE_INTERCEPTION;
	}

	bool called;
	static void callme(TEST_GROUP_CppUTestGroupsignalslot * self)
	{
		self->called = true;
	}
};

TEST(signalslot, Instance)
{
	ENABLE_INTERCEPTION;

	CHECK_TRUE(signal != NULL);
	CHECK_TRUE(slot != NULL);
	signal_destroy(signal);
	signal_destroy(signal);
	STRCMP_CONTAINS("signal", intercepted_output[2]);
	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
	slot_destroy(slot);
	slot_destroy(slot);
	STRCMP_CONTAINS("slot", intercepted_output[2]);
	STRCMP_CONTAINS("Invalid Instance", intercepted_output[0]);
}


TEST(signalslot, action)
{
	CHECK_FALSE(called);

	slot_set(slot, (slot_func)callme, (slot_arg)this);
	slot_connect(slot, signal);
	signal_emit(signal);

	CHECK_TRUE(called);
}

TEST(signalslot, action2)
{
	called = false;
	CHECK_FALSE(called);

	slot2_set(slot2, (slot2_func)callme, (slot_arg)this);
	slot2_connect(slot2, signal2);
	signal2_emit(signal2, 0, 0);

	CHECK_TRUE(called);
}
