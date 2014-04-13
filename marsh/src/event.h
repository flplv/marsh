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

#ifndef EVENT_H_
#define EVENT_H_

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

enum e_event_propagation_policy
{
	event_propagate_from_root,   // Event starts its propagation from the root widget down the tree.
	event_propagate_from_current // Event starts its propagation from the widget that triggered it down.
};

enum e_event_life_policy
{
	event_life_persistent,  // Event don't stop after triggered an action.
	event_life_single		// Event stop propagation after triggered an action once.
};

enum e_event_default_unique_id
{
	event_uid_press = 1,
	event_uid_release = 2,
	event_uid_click = 3,
};

bool event_is_user_event_uid(event_uid_t event_unique_id);
event_uid_t event_get_uid_from_name(const char * event_uid_name);

event_uid_t event_reserve_new_uid(enum e_event_propagation_policy prop_policy, enum e_event_life_policy life_cycle, const char * event_name);
void event_unreserve_all_uids(void);

event_t * event_new(event_uid_t event_unique_id, void * data, void (*free_data)(void *));
void event_delete(event_t *);

#endif /* WIDGET_EVENT_H_ */
