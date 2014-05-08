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

#include "signalslot.h"
#include "event.h"
#include "widget_private.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_event.h"

widget_t * widget_new(widget_t * parent, void * report_instance, void (*report_draw)(void *), void (*report_delete)(void *))
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	area_clear(&obj->dim);

	obj->creator_instance = report_instance;
	obj->creator_draw = report_draw;
	obj->creator_delete = report_delete;

	obj->click_signal = signal_new();
	obj->press_signal = signal_new();
	obj->release_signal = signal_new();

	widget_tree_register(obj, parent);

	widget_event_init(&obj->event_handler_list);
	widget_event_install_handler(obj, event_code_interaction_click, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_interaction_release, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_interaction_press, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_draw, default_draw_event_handler);
	widget_event_install_handler(obj, event_code_delete, default_delete_event_handler);

	obj->pressed = false;

	return obj;
}

void widget_delete_instance(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	widget_event_deinit(&obj->event_handler_list);
	widget_tree_unregister(obj);

	signal_delete(obj->click_signal);
	signal_delete(obj->press_signal);
	signal_delete(obj->release_signal);

	free(obj);
}

void widget_virtual_delete(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_delete && obj->creator_instance)
	{
		obj->creator_delete(obj->creator_instance);
	}
	else
	{
		widget_delete_instance(obj);
	}
}

void widget_delete(widget_t * obj)
{
	event_t * deletion_event;

	PTR_CHECK(obj, "widget");

	deletion_event = event_new(event_code_delete, NULL, NULL);
	PTR_CHECK(deletion_event, "widget");

	widget_event_commit(obj, deletion_event);
}

void widget_draw(widget_t * obj)
{
	event_t * draw_event;

	PTR_CHECK(obj, "widget");

	draw_event = event_new(event_code_draw, NULL, NULL);
	PTR_CHECK(draw_event, "widget");

	widget_event_commit(obj, draw_event);
}

void widget_click(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget");

	interaction_event = event_new(event_code_interaction_click, &data, NULL);
	PTR_CHECK(interaction_event, "widget");

	widget_event_commit(obj, interaction_event);
}

void widget_press(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget");

	interaction_event = event_new(event_code_interaction_press, &data, NULL);
	PTR_CHECK(interaction_event, "widget");

	widget_event_commit(obj, interaction_event);
}

void widget_release(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget");

	interaction_event = event_new(event_code_interaction_release, &data, NULL);
	PTR_CHECK(interaction_event, "widget");

	widget_event_commit(obj, interaction_event);
}

area_t * widget_area(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);
	return &obj->dim;
}

void widget_process_click(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	signal_emit(obj->click_signal);
}

void widget_process_release(widget_t * obj)
{
	bool shall_click = false;
	PTR_CHECK(obj, "widget");

	if (obj->pressed)
	{
		shall_click = true;
		obj->pressed = false;
	}

	signal_emit(obj->release_signal);

	/* TODO: Create advanced click algorithm, do deal with false releases. */
	if (shall_click)
		signal_emit(obj->click_signal);
}

void widget_process_press(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->pressed = true;

	signal_emit(obj->press_signal);
}

void widget_process_draw(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_draw && obj->creator_instance)
	{
		obj->creator_draw(obj->creator_instance);
	}
}

signal_t * widget_click_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->click_signal;
}

signal_t * widget_release_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->release_signal;
}

signal_t * widget_press_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->press_signal;
}
