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

#include "event.h"

#include <string.h>

#include "helper/linked_list.h"
#include "helper/checks.h"

struct s_event
{
	event_code_t code;
	void * data;
	void (*free_data)(void *);
};

struct s_event_code_info
{
	linked_list_t head;

	event_code_t code_number;
	char name[32];

	int propagation_mask;
};

static const int client_uuid_start_code = 10;
static bool initd = false;
static struct s_event_code_info * event_list_root = NULL;

#define event_user_uid_start 100

static bool name_comparator (const char * seed, struct s_event_code_info * node)
{
	return (strcmp(seed, node->name) == 0);
}

static bool code_comparator (event_code_t seed, struct s_event_code_info * node)
{
	return (seed == node->code_number);
}

static void create_default_events()
{
	struct s_event_code_info * press_event;
	struct s_event_code_info * click_event;
	struct s_event_code_info * release_event;
	struct s_event_code_info * draw_event;
	struct s_event_code_info * delete_event;


	click_event = (typeof(click_event))calloc(1, sizeof(*click_event));
	MEMORY_ALLOC_CHECK(click_event);

	click_event->code_number = event_code_interaction_click;
	linked_list_init(click_event,head);
	strcpy(click_event->name, "default_click");
	click_event->propagation_mask = event_prop_default;



	release_event = (typeof(release_event))calloc(1, sizeof(*release_event));
	MEMORY_ALLOC_CHECK(release_event);

	release_event->code_number = event_code_interaction_release;
	linked_list_init(release_event,head);
	strcpy(release_event->name, "default_release");
	release_event->propagation_mask = event_prop_default;



	press_event = (typeof(press_event))calloc(1, sizeof(*press_event));
	MEMORY_ALLOC_CHECK(press_event);

	press_event->code_number = event_code_interaction_press;
	linked_list_init(press_event,head);
	strcpy(press_event->name, "default_press");
	press_event->propagation_mask = event_prop_default;



	draw_event = (typeof(draw_event))calloc(1, sizeof(*draw_event));
	MEMORY_ALLOC_CHECK(draw_event);

	draw_event->code_number = event_code_draw;
	linked_list_init(draw_event,head);
	strcpy(draw_event->name, "default_draw");
	draw_event->propagation_mask = event_prop_default;



	delete_event = (typeof(delete_event))calloc(1, sizeof(*delete_event));
	MEMORY_ALLOC_CHECK(delete_event);

	delete_event->code_number = event_code_delete;
	linked_list_init(delete_event,head);
	strcpy(delete_event->name, "default_delete");
	delete_event->propagation_mask = event_prop_force_all_nodes | event_prop_outside_first;


	/* This order matters */
	event_list_root = press_event;
	linked_list_insert_after(linked_list_last(event_list_root, head), release_event, head);
	linked_list_insert_after(linked_list_last(event_list_root, head), click_event, head);
	linked_list_insert_after(linked_list_last(event_list_root, head), draw_event, head);
	linked_list_insert_after(linked_list_last(event_list_root, head), delete_event, head);
}

void event_pool_init(void)
{
	ASSERT(!initd, "event_pool");

	create_default_events();

	initd = true;
}

void event_pool_deinit(void)
{
	linked_list_free(event_list_root, head);
	event_list_root = NULL;
	initd = false;
}

static bool code_valid(event_code_t code)
{
	struct s_event_code_info * found_node;

	if (code <= 0)
		return false;

	found_node = linked_list_find(event_list_root, head, code_comparator, code);
	if (found_node)
		return true;

	return false;
}

event_code_t event_code(event_t * event)
{
	ASSERT_RETURN(event, "event", -1);

	return event->code;
}

const void * event_data(event_t * event)
{
	ASSERT_RETURN(event, "event", NULL);

	return event->data;
}

int event_propagation_mask(event_t * event)
{
	ASSERT_RETURN(event, "event", event_prop_default);
	ASSERT_RETURN(initd, "event_pool", event_prop_default);

	struct s_event_code_info * found_node = linked_list_find(event_list_root, head, code_comparator, event->code);

	if (found_node)
		return found_node->propagation_mask;

	return event_prop_default;
}

event_code_t event_pool_code_from_name(const char * event_uid_name)
{
	PTR_CHECK_RETURN(event_uid_name, "event", -1);
	ASSERT_RETURN(initd, "event_pool", -1);

	struct s_event_code_info * found_node = linked_list_find(event_list_root, head, name_comparator, event_uid_name);

	if (found_node)
		return found_node->code_number;

	return -1;
}

event_code_t event_pool_new_code(int propagation_mask, const char * event_name)
{
	struct s_event_code_info * last_uid;
	struct s_event_code_info * new_uid;

	ASSERT_RETURN(initd, "event_pool", -1);

	last_uid = linked_list_last(event_list_root, head);
	ASSERT_RETURN(last_uid, "event", -1);

	new_uid = (typeof(new_uid))calloc(1, sizeof(*new_uid));
	MEMORY_ALLOC_CHECK_RETURN(new_uid, -1);

	linked_list_init(new_uid, head);
	if ((last_uid->code_number + 1) < client_uuid_start_code)
		new_uid->code_number = client_uuid_start_code;
	else
		new_uid->code_number = last_uid->code_number + 1;
	new_uid->propagation_mask = propagation_mask;
	strncpy(new_uid->name, event_name, 32);

	linked_list_insert_after(last_uid, new_uid, head);

	return new_uid->code_number;
}

void event_pool_remove_created_codes(void)
{
	event_pool_deinit();
	event_pool_init();
}

event_t * event_new(event_code_t event_unique_id, void * data, void (*free_data)(void *))
{
	event_t * new_event;

	ASSERT_RETURN(initd, "event_pool", NULL);

	if (!code_valid(event_unique_id))
		return NULL;

	new_event = (typeof(new_event))calloc(1, sizeof(*new_event));
	MEMORY_ALLOC_CHECK_RETURN(new_event, NULL);

	new_event->data = data;
	new_event->free_data = free_data;
	new_event->code = event_unique_id;

	return new_event;
}

void event_delete(event_t * event)
{
	PTR_CHECK(event, "event");

	if (event->data)
	{
		if (event->free_data)
			event->free_data(event->data);
	}

	free(event);
}

