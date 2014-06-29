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

#include "widget_event.h"
#include "widget_tree.h"
#include "widget_private.h"
#include "event.h"
#include "types.h"

#include "helper/linked_list.h"

static bool uid_cmp(event_code_t seed, widget_event_handler_t * node)
{
	PTR_CHECK_RETURN(node, "widget_event", false);

	if (seed == node->code)
		return true;

	return false;
}

static widget_event_handler_t * get_handler_from_code(widget_event_handler_t * list_root, event_code_t uid_number)
{
	widget_event_handler_t * found_node;

	if (!list_root)
		return NULL;

	found_node = linked_list_find(list_root, head, uid_cmp, uid_number);

	return found_node;
}

int widget_event_install_handler (widget_t * widget, event_code_t code, widget_event_handler_f * handler)
{
	widget_event_handler_t * existent_event_handler;
	widget_event_handler_t * new_event_handler;

	PTR_CHECK_RETURN(widget, "widget_event", -1);

	existent_event_handler = get_handler_from_code(widget->event_handler_list, code);

	if (existent_event_handler)
	{
		existent_event_handler->function = (int(*)(widget_t *, event_t *))handler;
		return 0;
	}

	new_event_handler = (typeof(new_event_handler)) malloc (sizeof(*new_event_handler));
	MEMORY_ALLOC_CHECK_RETURN(new_event_handler, -1);

	new_event_handler->code = code;
	new_event_handler->function = (int(*)(widget_t *, event_t *))handler;
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

	PTR_CHECK_RETURN(widget, "widget_event", widget_event_not_consumed);
	PTR_CHECK_RETURN(event, "widget_event", widget_event_not_consumed);

	handler = linked_list_find(widget->event_handler_list, head, event_comparator, event_code(event));

	if (!handler)
		return widget_event_not_consumed;

	ASSERT_RETURN(handler->function, "widget_event", widget_event_not_consumed);

	return handler->function(widget, event);
}

static int __attribute__((noinline)) widget_event_commit_impl(widget_t * widget, event_t * event)
{
	return event_process(widget, event);
}

static int __attribute__((noinline)) widget_event_commit_internal(widget_t * self, event_t * event, int propagation_mask)
{
	widget_t * child;
	bool event_consumed = false;

	PTR_CHECK_RETURN(self, "widget_event", widget_event_not_consumed);

	if (!(propagation_mask & event_prop_bottom_up))
	{
		/* Consume in current widget */
		if (widget_event_commit_impl(self, event) == widget_event_consumed)
			if (!(propagation_mask & event_prop_persistent))
				return widget_event_consumed;
	}

	/* Route to children */
	child = widget_child(self);

	if (propagation_mask & event_prop_right_to_left)
		child = widget_last(child);

	while(child)
	{
		/* Next is taken before executing event in case it is a deletion and child cease to exist */
		widget_t * next_child;

		if (propagation_mask & event_prop_right_to_left)
			next_child =  widget_left_sibling(child);
		else
			next_child =  widget_right_sibling(child);


		// XXX recursiveness
		if (widget_event_commit_internal(child, event, propagation_mask) == widget_event_consumed)
			event_consumed = true;

		if (event_consumed && (!(propagation_mask & event_prop_persistent)))
			break;

		child = next_child;
	}

	if (propagation_mask & event_prop_bottom_up)
	{
		/* Consume in current widget */
		if (widget_event_commit_impl(self, event) == widget_event_consumed)
			if (!(propagation_mask & event_prop_persistent))
				return widget_event_consumed;
	}

	if (propagation_mask & event_prop_persistent)
		return widget_event_not_consumed;

	return event_consumed ? widget_event_consumed : widget_event_not_consumed;
}

bool widget_event_emit(widget_t * widget, event_t * event)
{
	int result;
	int propagation_mask;
	widget_t * start_widget;

	PTR_CHECK_RETURN(widget, "widget_event", false);
	PTR_CHECK_RETURN(event, "widget_event", false);

	propagation_mask = event_propagation_mask(event);

	if (propagation_mask & event_prop_from_root)
		start_widget = widget_root(widget);
	else
		start_widget = widget;

	result = widget_event_commit_internal(start_widget, event, propagation_mask);
	event_delete(event);

	return (result == widget_event_not_consumed) ? false : true;
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
