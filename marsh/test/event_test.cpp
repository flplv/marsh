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
		event_pool_init();
	}

	void teardown()
	{
		event_pool_deinit();
		DISABLE_INTERCEPTION;
	}
};

TEST(event, linked_list_default_events)
{
	CHECK_TRUE(event_list_root);
	CHECK_TRUE(event_list_root->head.next);
	CHECK_TRUE(event_list_root->head.next->next);
	CHECK_TRUE(event_list_root->head.next->next->next);
	CHECK_TRUE(event_list_root->head.next->next->next->next);
	CHECK_FALSE(event_list_root->head.next->next->next->next->next);
}

TEST(event, reservation)
{
	LONGS_EQUAL(10, event_pool_new_code(event_prop_default, "test_event"));

	LONGS_EQUAL(10, event_pool_code_from_name("test_event"));
	LONGS_EQUAL(event_code_delete, event_pool_code_from_name("default_delete"));
	LONGS_EQUAL(event_code_draw, event_pool_code_from_name("default_draw"));
	LONGS_EQUAL(event_code_interaction_click, event_pool_code_from_name("default_click"));
	LONGS_EQUAL(event_code_interaction_release, event_pool_code_from_name("default_release"));
	LONGS_EQUAL(event_code_interaction_press, event_pool_code_from_name("default_press"));

	event_pool_remove_created_codes();
}

TEST(event, event_quality)
{
	ENABLE_INTERCEPTION;

	event_t * cut;
	cut = event_new(event_code_interaction_press, NULL, NULL);
	CHECK_TRUE(cut);

	CHECK_EQUAL(event_code_interaction_press, event_code(cut));
	CHECK_EQUAL(event_prop_default | event_prop_right_to_left, event_propagation_mask(cut));

	event_delete(cut);
}

static void my_free(void * data) { free (data); }

TEST(event, instances)
{
	ENABLE_INTERCEPTION;

	event_t * cut;
	cut = event_new(event_code_interaction_press, NULL, NULL);
	CHECK_TRUE(cut);
	event_delete(cut);

	cut = event_new(event_code_interaction_press, malloc(1024), my_free);
	CHECK_TRUE(cut);
	event_delete(cut);

	int user_unique_id;
	user_unique_id = event_pool_new_code(event_prop_default, "custom");
	cut = event_new(user_unique_id, malloc(1024), my_free);
	CHECK_TRUE(cut);
	event_delete(cut);
	event_pool_remove_created_codes();

	cut = event_new(3141, NULL, NULL);
	CHECK_FALSE(cut);
	event_delete(cut);
}

