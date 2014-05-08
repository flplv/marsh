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
#include "helper/linked_list.h"
#include "types.h"
#include "widget.h"
#include "widget_event.h"
#include "area.h"

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
	int (*function)(widget_t *, event_t *);

	linked_list_t head;
};
typedef struct s_widget_event_handler_node widget_event_handler_t;

struct s_widget
{
	area_t dim;
	signal_t * click_signal;
	signal_t * press_signal;
	signal_t * release_signal;

	void * creator_instance;
	void (*creator_draw)(void *);
	void (*creator_delete)(void *);

	struct s_widget_tree tree;
	widget_event_handler_t * event_handler_list;

	bool pressed;
};

void widget_event_init(widget_event_handler_t ** widget_event_lists_root_ptr);
void widget_event_deinit(widget_event_handler_t ** widget_event_lists_root_ptr);

void widget_tree_register(widget_t * self, widget_t * parent);
void widget_tree_unregister(widget_t * self);

enum e_widget_event_handler_result default_interaction_event_handler(widget_t *, event_t *);
enum e_widget_event_handler_result default_delete_event_handler(widget_t *, event_t *);
enum e_widget_event_handler_result default_draw_event_handler(widget_t *, event_t *);

#endif /* WIDGETPRIVATE_H_ */
