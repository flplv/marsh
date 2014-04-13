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
#include "event.h"
#include "event.c"
}

TEST_GROUP(event)
{
	void setup()
	{
	}

	void teardown()
	{
		event_unreserve_all_uids();
		DISABLE_INTERCEPTION;
	}
};

TEST(event, linked_list)
{
	POINTERS_EQUAL(&press_event, uid_list_root);
	POINTERS_EQUAL(&release_event, uid_list_root->head.next);
	POINTERS_EQUAL(&click_event, uid_list_root->head.next->next);
	POINTERS_EQUAL(NULL, uid_list_root->head.next->next->next);
}

TEST(event, reservation)
{
	LONGS_EQUAL(4, event_reserve_new_uid(event_propagate_from_current, event_life_single, "test_event"));

	LONGS_EQUAL(4, event_get_uid_from_name("test_event"));
	LONGS_EQUAL(3, event_get_uid_from_name("default_click"));
	LONGS_EQUAL(2, event_get_uid_from_name("default_release"));
	LONGS_EQUAL(1, event_get_uid_from_name("default_press"));

	event_unreserve_all_uids();
}

static void my_free(void * data) { free (data); }

TEST(event, instances)
{
	ENABLE_INTERCEPTION;

	event_t * cut;
	cut = event_new(event_uid_press, NULL, NULL);
	CHECK_TRUE(cut);
	event_delete(cut);

	cut = event_new(event_uid_press, malloc(1024), my_free);
	CHECK_TRUE(cut);
	event_delete(cut);

	int user_unique_id;
	user_unique_id = event_reserve_new_uid(event_propagate_from_current, event_life_single, "custom");
	cut = event_new(user_unique_id, malloc(1024), my_free);
	CHECK_TRUE(cut);
	event_delete(cut);
	event_unreserve_all_uids();

	cut = event_new(3141, NULL, NULL);
	CHECK_FALSE(cut);
	event_delete(cut);
}

