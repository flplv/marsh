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

#ifndef WIDGETPRIVATE_H_
#define WIDGETPRIVATE_H_

#include "helper/checks.h"
#include "types.h"
#include "widget.h"
#include "interact.h"
#include "dimension.h"

#include "helper/linked_list.h"

struct s_widget_tree
{
	widget_t * parent;
	widget_t * child;
	widget_t * right;
	widget_t * left;
};

struct s_widget_event_handler_node
{
	event_code_t code;
	bool (*handler)(widget_t * widget, event_t * event);

	linked_list_t head;
};
typedef struct s_widget_event_handler_node widget_event_handler_t;

struct s_widget
{
	dimension_t dim;
	interaction_engine_t *interaction;

	void * creator_instance;
	void (*creator_draw)(void *);
	void (*creator_delete)(void *);

	struct s_widget_tree tree;
	widget_event_handler_t * event_handler_list;
};

void widget_event_init(widget_event_handler_t ** widget_event_lists_root_ptr);
void widget_event_deinit(widget_event_handler_t ** widget_event_lists_root_ptr);

void widget_tree_register(widget_t * self, widget_t * parent);
void widget_tree_unregister(widget_t * self);

#endif /* WIDGETPRIVATE_H_ */
