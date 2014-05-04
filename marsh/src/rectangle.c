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

#include "helper/checks.h"
#include "helper/number.h"
#include "helper/log.h"
#include "color.h"
#include "area.h"
#include "canvas.h"
#include "widget.h"
#include "rectangle.h"
#include "drawing_algorithms.h"


struct s_rectangle_instance
{
	color_t fill_color;
	bool is_filled;
	color_t border_color;
	dim_t border_tickness;
	bool has_border;
	dim_t corner_radius;
	my_log_t * log;
	widget_t *glyph;
};

static bool bad_corner_radius(rectangle_t * obj)
{
	dim_t least_dim  = get_smaller( widget_area(obj->glyph)->width,
			widget_area(obj->glyph)->height);

	if ( obj->corner_radius > (least_dim/2))
		return true;

	return false;
}

static bool ready_to_draw(rectangle_t * obj)
{
	if (!area_value(widget_area(obj->glyph)))
		return false;

	if (!obj->is_filled && !obj->has_border)
		return false;

	return true;
}

static void abstract_delete(void * obj)
{
	rectangle_delete((rectangle_t *)obj);
}

static void decode_and_draw(rectangle_t* obj)
{
	canvas_t *canv = canvas_new(widget_area(obj->glyph));

	if (obj->corner_radius)
	{
		if (obj->is_filled) {
			draw_solid_round_rectangle(canv, color_to_pixel(obj->fill_color), obj->corner_radius);
		}
		if (obj->has_border) {
			draw_round_rectangle(canv,  color_to_pixel(obj->border_color), obj->border_tickness, obj->corner_radius);
		}
	}
	else
	{
		if (obj->is_filled) {
			draw_solid_rectangle(canv, color_to_pixel(obj->fill_color));
		}
		if (obj->has_border) {
			draw_rectangle(canv, color_to_pixel(obj->border_color), obj->border_tickness);
		}
	}

	canvas_delete(canv);
}

static void draw(rectangle_t * obj)
{
	dim_t least_dim;;

	PTR_CHECK(obj, "rectangle");

	if (!ready_to_draw(obj))
	{
		my_log(ERROR, __FILE__, __LINE__, "Object not initialized properly, can't draw.", obj->log);
		return;
	}

	if (bad_corner_radius(obj))
	{
		my_log(WARNING, __FILE__, __LINE__, "Bad corner radius. Radius has been reduced.", obj->log);
		least_dim  = get_smaller( widget_area(obj->glyph)->width,
							widget_area(obj->glyph)->height);
		obj->corner_radius = least_dim / 2;
	}

	decode_and_draw(obj);
}

rectangle_t * rectangle_new(widget_t * parent)
{
	rectangle_t * obj = (rectangle_t *) calloc(1, sizeof(struct s_rectangle_instance));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	obj->log = my_log_new("Rectangle", MESSAGE);
	obj->glyph = widget_new(parent, obj, (void(*)(void *))draw, abstract_delete);
	obj->fill_color = color(0,0,0);
	obj->is_filled = false;
	obj->border_color = color(0,0,0);
	obj->border_tickness = 1;
	obj->has_border = false;
	obj->corner_radius = 0;

	return obj;
}

void rectangle_delete(rectangle_t * const obj)
{
	PTR_CHECK(obj, "rectangle");

	my_log_delete(obj->log);
	widget_delete_instance(obj->glyph);

	free(obj);
}

void rectangle_set_size(rectangle_t * const obj, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "rectangle");

	area_set_size(widget_area(obj->glyph), width, height);
}

void rectangle_set_position(rectangle_t * const obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "rectangle");

	area_set_start_xy(widget_area(obj->glyph), x, y);
}

void rectangle_set_fill_color_html(rectangle_t * const obj, const char *html_color_code)
{
	PTR_CHECK(obj, "rectangle");

	obj->fill_color = color_html(html_color_code);
	obj->is_filled = true;
}

widget_t *rectangle_get_widget(rectangle_t * const obj)
{
	PTR_CHECK_RETURN (obj, "rectangle", NULL);

	return obj->glyph;
}

void rectangle_set_border_tickness(rectangle_t * const obj, dim_t tickness)
{
	PTR_CHECK(obj, "rectangle");

	obj->border_tickness = tickness;
	obj->has_border = true;
}

void rectangle_set_border_color_html(rectangle_t * const obj, const char* html_color_code)
{
	PTR_CHECK(obj, "rectangle");

	obj->border_color = color_html(html_color_code);
	obj->has_border = true;
}

void rectangle_set_rounded_corner_radius(rectangle_t * const obj, dim_t radius)
{
	PTR_CHECK(obj, "rectangle");

	obj->corner_radius = radius;
}
