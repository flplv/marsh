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
#include "widget_private.h"

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
	new_event_handler->handler = handler;
	linked_list_init(new_event_handler, head);

	if (!widget->event_handler_list)
		widget->event_handler_list = new_event_handler;
	else
		linked_list_insert_after(linked_list_last(widget->event_handler_list, head), new_event_handler, head);

	return 0;
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
