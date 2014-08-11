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

#include <stdlib.h>

#include "helper/log.h"
#include "helper/checks.h"

#include "framebuffer.h"
#include "signalslot.h"
#include "widget_private.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_event.h"

widget_t * widget_new(widget_t * parent, void * report_instance, void (*report_draw)(void *, const area_t *), void (*report_delete)(void *))
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	area_clear(&obj->area);

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
	obj->visible = true;

	return obj;
}

void widget_delete_instance_only(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	widget_event_deinit(&obj->event_handler_list);
	widget_tree_unregister(obj);

	signal_delete(obj->click_signal);
	signal_delete(obj->press_signal);
	signal_delete(obj->release_signal);

	free(obj);
}

void widget_delete(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_delete && obj->creator_instance)
	{
		obj->creator_delete(obj->creator_instance);
	}
	else
	{
		widget_delete_instance_only(obj);
	}
}

const area_t * widget_area(const widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);
	return &obj->area;
}

area_t widget_compute_canvas_area(const widget_t *obj, const area_t * limiting_canvas_area)
{
	PTR_CHECK_RETURN(obj, "widget", ((area_t){0,}));

	area_t result_canvas;

	if (limiting_canvas_area)
		area_set_intersection(&result_canvas, limiting_canvas_area, &obj->area);
	else
		result_canvas = obj->area;

	return result_canvas;
}

void widget_set_dim(widget_t *obj, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "widget");

	obj->area.width = width;
	obj->area.height = height;
}

void widget_set_pos(widget_t *obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "widget");

	obj->area.x = x;
	obj->area.y = y;
}

void widget_set_area(widget_t *obj, dim_t x, dim_t y, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "widget");

	obj->area.x = x;
	obj->area.y = y;
	obj->area.width = width;
	obj->area.height = height;
}

void widget_click(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	signal_emit(obj->click_signal);
}

void widget_release(widget_t * obj)
{
	bool shall_click = false;
	PTR_CHECK(obj, "widget");

	if (obj->pressed)
	{
		shall_click = true;
		obj->pressed = false;
	}

	signal_emit(obj->release_signal);

	/* TODO: Create advanced click algorithm, do deal with false releases.
	 * Design suggestion:
	 *  Create a list of clicked widgets, those are reported when an release
	 *  happens. It doesn't make sense to emit an event through the tree
	 *  for a release, since only clicked widgets should respond to the release
	 *  event.
	 *  The linked list should be a separated module called widget_clicked_list.
	 *    widtget_clicked_list is a singleton type module.
	 *  */

	if (shall_click)
		signal_emit(obj->click_signal);
}

void widget_press(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->pressed = true;

	signal_emit(obj->press_signal);
}

void widget_draw(widget_t * obj, const area_t * limiting_canvas_area)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_draw && obj->creator_instance)
	{
		obj->creator_draw(obj->creator_instance, limiting_canvas_area);
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

void widget_hide(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->visible = false;
}

void widget_show(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->visible = true;
}

bool widget_visible(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", false);

	return obj->visible;
}
