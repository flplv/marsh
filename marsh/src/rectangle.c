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
#include "dimension.h"
#include "canvas.h"
#include "widget.h"
#include "widget_interface.h"
#include "rectangle.h"


struct s_rectangle_instance
{
	MODULE_PRIVATE_DATA_DECLARATION;
	color_t fill_color;
	BOOL is_filled;
	color_t border_color;
	dim_t border_tickness;
	BOOL has_border;
	dim_t corner_radius;
	my_log_t * log;
	widget_t *glyph;
	widget_interface_t *self_reference;
};

#define SIGNATURE_RECTANGLE (ADDRESS_TO_SIGNATURE_CAST)&rectangle_create

static BOOL bad_corner_radius(rectangle_t * obj)
{
	dim_t least_dim  = get_smaller( widget_get_dimension(obj->glyph)->size.width,
			widget_get_dimension(obj->glyph)->size.height);

	if ( obj->corner_radius > (least_dim/2))
		return TRUE;

	return FALSE;
}

static BOOL ready_to_draw(rectangle_t * obj)
{
	if (!dimension_good(widget_get_dimension(obj->glyph)))
		return FALSE;

	if (!obj->is_filled && !obj->has_border)
		return FALSE;

	return TRUE;
}

static void abstract_destroy(void * obj)
{
	rectangle_destroy((rectangle_t *)obj);
}

static void decode_and_draw(rectangle_t* obj)
{
	canvas_t *canv = canvas_create(widget_get_dimension(obj->glyph));

	if (obj->corner_radius)
	{
		if (obj->is_filled) {
			canvas_set_color(canv, color_to_pixel(obj->fill_color));
			canvas_draw_solid_round_rectangle(canv, obj->corner_radius);
		}
		if (obj->has_border) {
			canvas_set_color(canv, color_to_pixel(obj->border_color));
			canvas_draw_round_rectangle(canv, obj->border_tickness, obj->corner_radius);
		}
	}
	else
	{
		if (obj->is_filled) {
			canvas_set_color(canv, color_to_pixel(obj->fill_color));
			canvas_draw_solid_rectangle(canv);
		}
		if (obj->has_border) {
			canvas_set_color(canv, color_to_pixel(obj->border_color));
			canvas_draw_rectangle(canv, obj->border_tickness);
		}
	}

	canvas_destroy(canv);
}

static void draw(rectangle_t * obj)
{
	dim_t least_dim;;

	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	if (!ready_to_draw(obj))
	{
		my_log(ERROR, __FILE__, __LINE__, "Object not initialized properly, can't draw.", obj->log);
		return;
	}

	if (bad_corner_radius(obj))
	{
		my_log(WARNING, __FILE__, __LINE__, "Bad corner radius. Radius has been reduced.", obj->log);
		least_dim  = get_smaller( widget_get_dimension(obj->glyph)->size.width,
							widget_get_dimension(obj->glyph)->size.height);
		obj->corner_radius = least_dim / 2;
	}

	decode_and_draw(obj);
}

rectangle_t * rectangle_create()
{
	rectangle_t * obj = (rectangle_t *) calloc(1, sizeof(struct s_rectangle_instance));
	MEMORY_ALLOC_CHECK(obj);
	INSTANCE_SET(obj, SIGNATURE_RECTANGLE);

	obj->log = my_log_create("Rectangle", MESSAGE);
	obj->self_reference = widget_interface_create(obj, (void(*)(void *))draw, abstract_destroy);
	obj->glyph = widget_create(obj->self_reference);
	obj->fill_color = color(0,0,0);
	obj->is_filled = FALSE;
	obj->border_color = color(0,0,0);
	obj->border_tickness = 1;
	obj->has_border = FALSE;
	obj->corner_radius = 0;

	return obj;
}

void rectangle_destroy(rectangle_t * const obj)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	my_log_destroy(obj->log);
	widget_destroy(obj->glyph);
	widget_interface_destroy(obj->self_reference);

	INSTANCE_CLEAR(obj);
	free(obj);
}

void rectangle_set_size(rectangle_t * const obj, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	dimension_set_size(widget_get_dimension(obj->glyph), width, height);
	dimension_set_rest_if_possible(widget_get_dimension(obj->glyph));
}

void rectangle_set_position(rectangle_t * const obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	dimension_set_start_position(widget_get_dimension(obj->glyph), x, y);
	dimension_set_rest_if_possible(widget_get_dimension(obj->glyph));
}

void rectangle_set_fill_color_html(rectangle_t * const obj, const char *html_color_code)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	obj->fill_color = color_html(html_color_code);
	obj->is_filled = TRUE;
}

widget_t *rectangle_get_widget(rectangle_t * const obj)
{
	PTR_CHECK_RETURN (obj, "rectangle", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_RECTANGLE, "rectangle", NULL);

	return obj->glyph;
}

void rectangle_set_border_tickness(rectangle_t * const obj, dim_t tickness)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	obj->border_tickness = tickness;
	obj->has_border = TRUE;
}

void rectangle_set_border_color_html(rectangle_t * const obj, const char* html_color_code)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	obj->border_color = color_html(html_color_code);
	obj->has_border = TRUE;
}

void rectangle_set_rounded_corner_radius(rectangle_t * const obj, dim_t radius)
{
	PTR_CHECK(obj, "rectangle");
	INSTANCE_CHECK(obj, SIGNATURE_RECTANGLE, "rectangle");

	obj->corner_radius = radius;
}
