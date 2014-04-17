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

#include "widget_event.h"
#include "widget_tree.h"
#include "widget_private.h"
#include "event.h"
#include "types.h"

#include "helper/linked_list.h"

enum
{
	event_processed,
	event_not_processed,
	stop_propagation,
	continue_propagation,
};

static bool uid_cmp(event_code_t seed, widget_event_handler_t * node)
{
	PTR_CHECK_RETURN(node, "widget_event", false);

	if (seed == node->code)
		return true;

	return false;
}

static bool has_code_already_installed(widget_event_handler_t * list_root, event_code_t uid_number)
{
	widget_event_handler_t * found_node;

	if (!list_root)
		return false;

	found_node = linked_list_find(list_root, head, uid_cmp, uid_number);

	if (found_node)
		return true;

	return false;
}

int widget_event_install_handler (widget_t * widget, event_code_t uid, bool (*handler)(widget_t * widget, event_t * event))
{
	widget_event_handler_t * new_event_handler;

	PTR_CHECK_RETURN(widget, "widget_event", -1);

	if (has_code_already_installed(widget->event_handler_list, uid))
		return 0;

	new_event_handler = (typeof(new_event_handler)) malloc (sizeof(*new_event_handler));
	MEMORY_ALLOC_CHECK_RETURN(new_event_handler, -1);

	new_event_handler->code = uid;
	new_event_handler->function = handler;
	linked_list_init(new_event_handler, head);

	if (!widget->event_handler_list)
		widget->event_handler_list = new_event_handler;
	else
		linked_list_insert_after(linked_list_last(widget->event_handler_list, head), new_event_handler, head);

	return 0;
}

static bool event_comparator(event_code_t code, widget_event_handler_t * handler)
{
	PTR_CHECK_RETURN(handler, "widget_event", false);

	return handler->code == code;
}

static int event_process (widget_t * widget, event_t * event)
{
	widget_event_handler_t * handler;

	PTR_CHECK_RETURN(widget, "widget_event", event_not_processed);
	PTR_CHECK_RETURN(event, "widget_event", event_not_processed);

	handler = linked_list_find(widget->event_handler_list, head, event_comparator, event_code(event));

	if (!handler)
		return event_not_processed;

	ASSERT_RETURN(handler->function, "widget_event", event_not_processed);

	if (handler->function(widget, event))
		return event_processed;

	return event_not_processed;
}

static int __attribute__((noinline)) widget_event_commit_impl(widget_t * widget, event_t * event)
{
	if (event_process(widget, event) == event_processed)
	{
		enum e_event_life_policy life = event_life_policy(event);

		if (life == event_life_single)
		{
			return stop_propagation;
		}
	}

	return continue_propagation;
}

static int __attribute__((noinline)) widget_event_commit_internal(widget_t * self, event_t * event)
{
	widget_t * child;

	PTR_CHECK_RETURN(self, "widget_event", stop_propagation);

	/* Consume in self widget */
	if (widget_event_commit_impl(self, event) == stop_propagation)
		return stop_propagation;

	/* Route to children */
	child = widget_child(self);
	while(child)
	{
		// TODO XXX ATENTION XXX TODO: recursiveness
		if (widget_event_commit_internal(child, event) == stop_propagation)
			return stop_propagation;

		child = widget_right_sibling(child);
	}

	return continue_propagation;
}

enum e_event_result widget_event_commit(widget_t * self, event_t * event)
{
	widget_t * start_widget;
	enum e_event_propagation_policy propagation;

	propagation = event_propagation_policy(event);

	if (propagation == event_propagate_from_root)
		start_widget = widget_root(self);
	else
		start_widget = self;


	if (widget_event_commit_internal(start_widget, event) == continue_propagation)
	{
		event_delete(event);
		return event_not_consumed;
	}

	event_delete(event);
	return event_consumed;
}

void widget_event_init(widget_event_handler_t ** widget_event_lists_root_ptr)
{
	PTR_CHECK(widget_event_lists_root_ptr, "widget_event");

	*widget_event_lists_root_ptr = NULL;
}

void widget_event_deinit(widget_event_handler_t ** widget_event_lists_root_ptr)
{
	PTR_CHECK(widget_event_lists_root_ptr, "widget_event");

	if (*widget_event_lists_root_ptr)
		linked_list_free(*widget_event_lists_root_ptr, head);

	*widget_event_lists_root_ptr = NULL;
}
