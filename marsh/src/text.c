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

#include "text.h"
#include "font.h"
#include "helper/checks.h"
#include "helper/number.h"
#include "helper/my_string.h"
#include "font.h"
#include "color.h"
#include "area.h"
#include "widget.h"
#include "signalslot.h"
#include "canvas.h"
#include "framebuffer.h"

struct s_text
{
	my_string_t *string;
	font_t *font;
	color_t color;
	enum e_text_justification just;
	dim_t ref_x, ref_y;
	bool ref_is_set;

	slot_t *string_update_slot;
	widget_t *glyph;
};

static bool string_is_set(text_t* obj)
{
	if (my_string_len(obj->string))
		return true;

	return false;
}

static bool font_is_set(text_t* obj)
{
	if(!obj->font)
		return false;

	return true;
}

static bool ref_pos_is_set(text_t* obj)
{
	if(!obj->ref_is_set)
		return false;

	return true;
}

static void update_position_and_size(text_t *obj)
{
	dim_t width;
	dim_t height;
	dim_t x;
	dim_t y;

	if (!string_is_set(obj))
		return;

	if (!font_is_set(obj))
		return;

	if (!ref_pos_is_set(obj))
		return;

	width = font_string_width(obj->font, obj->string);
	height = font_string_height(obj->font, obj->string);
	widget_set_dim(obj->glyph, width, height);

	y = obj->ref_y;

	if (obj->just == TEXT_LEFT_JUST)
	{
		x = obj->ref_x;
	}
	else if (obj->just == TEXT_CENTER_JUST)
	{
		x = obj->ref_x - width / 2;
	}
	else if (obj->just == TEXT_RIGHT_JUST)
	{
		x = obj->ref_x - width;
	}
	else
	{
		LOG_ERROR("text", "Invalid Justification");
	}

	widget_set_pos(obj->glyph, x, y);
}

static void string_changed(text_t* obj)
{
	update_position_and_size(obj);
}

static bool ready_to_draw(text_t * obj)
{
	if (!color_check(obj->color))
		return false;
	if (!area_value(widget_area(obj->glyph)))
		return false;
	if (my_string_len(obj->string) == 0)
		return false;

	return true;
}

static void text_draw(text_t * obj, const area_t * limiting_canvas_area)
{
	if (!ready_to_draw(obj)) {
		LOG_ERROR("text", "unable to draw");
		return;
	}

	area_t canvas_area = widget_compute_canvas_area(obj->glyph, limiting_canvas_area);
	canvas_t *canv = canvas_new(&canvas_area);

	if (obj->just == TEXT_LEFT_JUST)
		font_draw_left_just(obj->font, obj->string, color_to_pixel(obj->color), canv);
	else if (obj->just == TEXT_CENTER_JUST)
		font_draw_center_just(obj->font, obj->string, color_to_pixel(obj->color), canv);
	else if (obj->just == TEXT_RIGHT_JUST)
		font_draw_right_just(obj->font, obj->string, color_to_pixel(obj->color), canv);

	canvas_delete(canv);
}
text_t* text_new(widget_t * parent)
{
	text_t * obj;

	obj = (text_t *)calloc(1, sizeof(text_t));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	obj->string = my_string_new();

	obj->string_update_slot = slot_new();
	slot_set(obj->string_update_slot, (slot_func)string_changed, (slot_arg)obj);
	slot_connect(obj->string_update_slot, my_string_get_update_signal(obj->string));

	obj->glyph = widget_new(parent, obj, (void(*)(void *, const area_t *))text_draw, (void(*)(void *))text_delete);

	obj->just = TEXT_LEFT_JUST;

	obj->color = color_null();

	return obj;
}

void text_delete(text_t* obj)
{
	PTR_CHECK(obj, "text");

	widget_delete_instance_only(obj->glyph);

	slot_delete(obj->string_update_slot);
	my_string_delete(obj->string);

	free(obj);
}

my_string_t* text_get_string(text_t* obj)
{
	PTR_CHECK_RETURN(obj, "text", NULL);

	return obj->string;
}


void text_set_font(text_t* obj, font_t *font)
{
	PTR_CHECK(obj, "text");

	obj->font = font;
	update_position_and_size(obj);
}

void text_set_reference_position(text_t* obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "text");

	obj->ref_is_set = true;
	obj->ref_x = x;
	obj->ref_y = y;

	update_position_and_size(obj);
}

void text_set_color_html(text_t* obj, const char* html_color_code)
{
	PTR_CHECK(obj, "text");

	obj->color = color_html(html_color_code);
}

void text_set_justification(text_t * obj, enum e_text_justification just)
{
	PTR_CHECK(obj, "text");

	obj->just = just;
	update_position_and_size(obj);
}

widget_t* text_get_widget(text_t* const obj)
{
	PTR_CHECK_RETURN(obj, "text", NULL);

	return obj->glyph;
}

