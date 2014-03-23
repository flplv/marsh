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
#include "dimension.h"
#include "signalslot.h"
#include "signalslot2.h"
#include "interaction_engine.h"
#include "interaction_engine.c"
}

#include "mocks/terminal_intercepter.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"

TEST_GROUP(interaction_engine)
{
	interaction_engine_t * cut;
	dimension_t dim;
	void setup()
	{
		dimension_clear(&dim);
		dimension_set_start_position(&dim, 0, 0);
		dimension_set_size(&dim, 100, 100);
		dimension_set_rest_if_possible(&dim);
		cut = interaction_engine_new(&dim);
	}

	void teardown()
	{
		interaction_engine_delete(cut);
	}
};

bool called = false;
static void callme(void*)
{
	called =  true;
}

TEST(interaction_engine, click1)
{
	slot_t *slot = slot_new();

	slot_connect(slot, interaction_engine_get_click_signal(cut));
	slot_set(slot, callme, NULL);

	called = false;
	CHECK_FALSE(called);

	interaction_engine_click(cut);

	CHECK_TRUE(called);

	slot_delete(slot);
}

TEST(interaction_engine, pressrelease)
{
	slot_t *slot = slot_new();

	slot_connect(slot, interaction_engine_get_click_signal(cut));
	slot_set(slot, callme, NULL);

	called = false;
	CHECK_FALSE(called);

	interaction_engine_press(cut, 0, 0);
	interaction_engine_release(cut, 0, 0);

	prioritary_release = NULL;

	CHECK_TRUE(called);

	slot_delete(slot);
}

