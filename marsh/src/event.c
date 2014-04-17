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
	/* Do not edit this order,
	 * order dependente static initialization
	 * of the struct members below */

	linked_list_t head;

	event_code_t code_number;
	char name[32];

	enum e_event_propagation_policy propagation_policy;
	enum e_event_life_policy life_policy;
};

static struct s_event_code_info click_event = {{NULL}, event_code_click, "default_click", event_propagate_from_current, event_life_single};
static struct s_event_code_info release_event = {{&click_event.head}, event_code_release, "default_release", event_propagate_from_current, event_life_single};
static struct s_event_code_info press_event = {{&release_event.head}, event_code_press, "default_press", event_propagate_from_current, event_life_single};

struct s_event_code_info * uid_list_root = &press_event;

#define event_user_uid_start 100

static bool code_valid(event_code_t code)
{
	if (code <= 0)
		return false;

	if (code > (linked_list_last(uid_list_root, head)->code_number))
		return false;

	return true;
}

static bool name_comparator (const char * seed, struct s_event_code_info * node)
{
	return (strcmp(seed, node->name) == 0);
}

static bool code_comparator (event_code_t seed, struct s_event_code_info * node)
{
	return (seed == node->code_number);
}

event_code_t event_code(event_t * event)
{
	ASSERT_RETURN(event, "event", -1);

	return event->code;
}

enum e_event_life_policy event_life_policy(event_t * event)
{
	ASSERT_RETURN(event, "event", (enum e_event_life_policy)-1);

	struct s_event_code_info * found_node = linked_list_find(uid_list_root, head, code_comparator, event->code);

	if (found_node)
		return found_node->life_policy;

	return (enum e_event_life_policy)-1;
}

enum e_event_propagation_policy event_propagation_policy(event_t * event)
{
	ASSERT_RETURN(event, "event", (enum e_event_propagation_policy)-1);

	struct s_event_code_info * found_node = linked_list_find(uid_list_root, head, code_comparator, event->code);

	if (found_node)
		return found_node->propagation_policy;

	return (enum e_event_propagation_policy)-1;
}

event_code_t event_code_from_name(const char * event_uid_name)
{
	PTR_CHECK_RETURN(event_uid_name, "event", -1);

	struct s_event_code_info * found_node = linked_list_find(uid_list_root, head, name_comparator, event_uid_name);

	if (found_node)
		return found_node->code_number;

	return -1;
}

event_code_t event_reserve_new_code(enum e_event_propagation_policy prop_policy, enum e_event_life_policy life_cycle, const char * event_name)
{
	struct s_event_code_info * last_uid;
	struct s_event_code_info * new_uid;

	last_uid = linked_list_last(uid_list_root, head);
	ASSERT_RETURN(last_uid, "event", -1);

	new_uid = (typeof(new_uid))calloc(1, sizeof(*new_uid));
	MEMORY_ALLOC_CHECK_RETURN(new_uid, -1);

	linked_list_init(new_uid, head);
	new_uid->code_number = last_uid->code_number + 1;
	new_uid->life_policy = life_cycle;
	new_uid->propagation_policy = prop_policy;
	strncpy(new_uid->name, event_name, 32);

	linked_list_insert_after(last_uid, new_uid, head);

	return new_uid->code_number;
}

void event_unreserve_all_uids(void)
{
	linked_list_free(container_of(click_event.head.next, struct s_event_code_info, head), head);
	click_event.head.next = NULL;
}

event_t * event_new(event_code_t event_unique_id, void * data, void (*free_data)(void *))
{
	event_t * new_event;

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

