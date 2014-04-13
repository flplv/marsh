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

#include <stdlib.h>

#include "helper/log.h"
#include "helper/checks.h"

#include "widget_private.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_event.h"
#include "interact.h"
#include "interaction_engine.h"

widget_t * widget_new(widget_t * parent, void * report_instance, void (*report_draw)(void *), void (*report_delete)(void *))
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	dimension_clear(&obj->dim);

	obj->creator_instance = report_instance;
	obj->creator_draw = report_draw;
	obj->creator_delete = report_delete;

	obj->interaction = interaction_engine_new(&obj->dim);

	widget_tree_register(obj, parent);

	widget_event_init(&obj->event_handler_list);

	return obj;
}

void widget_delete_instance_only(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	interaction_engine_delete(obj->interaction);

	widget_tree_unregister(obj);
	free(obj);
}

void widget_delete(widget_t * obj)
{
	widget_t * child;

	PTR_CHECK(obj, "widget");

	while((child = widget_last_child(obj)))
		widget_delete(child);

	if (obj->creator_delete && obj->creator_instance)
	{
		obj->creator_delete(obj->creator_instance);
	}
	else
	{
		widget_delete_instance_only(obj);
	}
}

void widget_draw(widget_t * obj)
{
	widget_t * child;

	PTR_CHECK(obj, "widget");

	if (obj->creator_draw && obj->creator_instance)
	{
		obj->creator_draw(obj->creator_instance);
	}

	child = widget_child(obj);

	while (child)
	{
		widget_draw(child);
		child = widget_right_sibling(child);
	}
}
///* TODO TODO TODO: Stoped here: recreate the interaction engine inside widget */
//bool widget_press(widget_t * obj, int x, int y)
//{
//	widget_t * child;
//
//	PTR_CHECK_RETURN(obj, "widget", false);
//
//	if (dimension_contains(&obj->dim, x, y))
//	{
//		interaction_engine_press(obj->interaction, x, y);
//		return true;
//	}
//
//	child = obj->child;
//
//	while (child)
//	{
//		if (widget_press(child, x, y))
//			return true;
//
//		child = child->right;
//	}
//
//	return false;
//}
//
//bool widget_release(widget_t * obj, int x, int y)
//{
//
//}

dimension_t * widget_get_dimension(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);

	return &obj->dim;
}

interaction_engine_t * widget_get_interaction_engine(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);

	return obj->interaction;
}
