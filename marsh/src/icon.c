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
#include "icon.h"
#include "bitmap_data/bitmap_data.h"


struct s_icon_instance
{
	color_t color;
	bitmap_t *bitmap;

	my_log_t * log;
	widget_t *glyph;
	widget_interface_t *self_reference;
};


static bool ready_to_draw(icon_t * obj)
{
	if (!dimension_good(widget_get_dimension(obj->glyph)))
		return false;

	if (obj->bitmap == NULL)
		return false;

	return true;
}

static void draw(icon_t * obj)
{
	PTR_CHECK(obj, "icon");

	if (!ready_to_draw(obj))
	{
		my_log(ERROR, __FILE__, __LINE__, "Object not initialized properly, can't draw.", obj->log);
		return;
	}

	canvas_t *canv = canvas_new(widget_get_dimension(obj->glyph));
	canvas_set_color(canv, color_to_pixel(obj->color));

	if (obj->bitmap->bitmap_data_width == BITMAP_BUFFER_8BPP)
	{
		canvas_draw_alpha_bitmap_8bpp(canv, (BUFFER_PTR_RDOLY)obj->bitmap->bitmap, 0, 0, obj->bitmap->width, obj->bitmap->height);
	}
	else
	{
		my_log(ERROR, __FILE__, __LINE__, "Bad bitmap_data_width", obj->log);
	}

	canvas_delete(canv);
}

icon_t * icon_new()
{
	icon_t * obj = (icon_t *) malloc(sizeof(struct s_icon_instance));
	MEMORY_ALLOC_CHECK(obj);

	obj->log = my_log_new("Icon", MESSAGE);
	obj->self_reference = widget_interface_new(obj, (void(*)(void *))draw, (void(*)(void *))icon_delete);
	obj->glyph = widget_new(obj->self_reference);
	obj->color = color(255,255,255);
	obj->bitmap = NULL;

	return obj;
}

void icon_delete(icon_t * const obj)
{
	PTR_CHECK(obj, "icon");

	my_log_delete(obj->log);
	widget_delete(obj->glyph);
	widget_interface_delete(obj->self_reference);

	free(obj);
}

static void set_size(icon_t * obj, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "icon");

	dimension_set_size(widget_get_dimension(obj->glyph), width, height);
	dimension_set_rest_if_possible(widget_get_dimension(obj->glyph));
}

static bool is_bitmap_icon(bitmap_t* bitmap)
{
	if (!bitmap->single_channel)
		return false;

	if (bitmap->bitmap_data_width != BITMAP_BUFFER_8BPP && bitmap->bitmap_data_width != BITMAP_BUFFER_1BPP)
		return false;

	return true;
}

void icon_set_bitmap(icon_t * obj, bitmap_t * bitmap)
{
	PTR_CHECK(obj, "icon");

	if (!is_bitmap_icon(bitmap))
	{
		LOG_ERROR("icon", "Selected bitmap is not an icon");
		return;
	}

	obj->bitmap = bitmap;
	set_size(obj, bitmap->width, bitmap->height);
}

void icon_set_position(icon_t * obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "icon");

	dimension_set_start_position(widget_get_dimension(obj->glyph), x, y);
	dimension_set_rest_if_possible(widget_get_dimension(obj->glyph));
}

void icon_set_color_html(icon_t * obj, const char *html_color_code)
{
	PTR_CHECK(obj, "icon");

	obj->color = color_html(html_color_code);
}

widget_t *icon_get_widget(const icon_t * obj)
{
	PTR_CHECK_RETURN (obj, "icon", NULL);

	return obj->glyph;
}
