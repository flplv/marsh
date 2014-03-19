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

#include "text.h"
#include "font.h"
#include "helper/checks.h"
#include "helper/number.h"
#include "helper/my_string.h"
#include "font.h"
#include "color.h"
#include "dimension.h"
#include "widget.h"
#include "widget_interface.h"
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
	BOOL ref_is_set;

	slot_t *string_update_slot;
	widget_interface_t *self_reference;
	widget_t *glyph;

	MODULE_PRIVATE_DATA_DECLARATION;
};

#define SIGNATURE_TEXT (ADDRESS_TO_SIGNATURE_CAST)&text_create

static BOOL string_is_set(text_t* obj)
{
	if (my_string_len(obj->string))
		return TRUE;

	return FALSE;
}

static BOOL font_is_set(text_t* obj)
{
	if(!obj->font)
		return FALSE;

	return TRUE;
}

static BOOL ref_pos_is_set(text_t* obj)
{
	if(!obj->ref_is_set)
		return FALSE;

	return TRUE;
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
	dimension_set_size(widget_get_dimension(obj->glyph), width, height);

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

	dimension_set_start_position(widget_get_dimension(obj->glyph), x, y);

	dimension_set_rest_if_possible(widget_get_dimension(obj->glyph));
}

static void string_changed(text_t* obj)
{
	update_position_and_size(obj);
}

static BOOL ready_to_draw(text_t * obj)
{
	if (!color_check(obj->color))
		return FALSE;
	if (!dimension_good(widget_get_dimension(obj->glyph)))
		return FALSE;
	if (my_string_len(obj->string) == 0)
		return FALSE;

	return TRUE;
}

static void text_draw(text_t * obj)
{
	canvas_t *canv;

	if (!ready_to_draw(obj)) {
		LOG_ERROR("text", "unable to draw");
		return;
	}

	canv = canvas_create(widget_get_dimension(obj->glyph));
	canvas_set_color(canv, color_to_pixel(obj->color));
	if (obj->just == TEXT_LEFT_JUST)
		font_draw_left_just(obj->font, obj->string, canv);
	else if (obj->just == TEXT_CENTER_JUST)
		font_draw_center_just(obj->font, obj->string, canv);
	else if (obj->just == TEXT_RIGHT_JUST)
		font_draw_right_just(obj->font, obj->string, canv);
	canvas_destroy(canv);
}
text_t* text_create()
{
	text_t * obj;

	obj = (text_t *)calloc(1, sizeof(text_t));
	MEMORY_ALLOC_CHECK(obj);
	INSTANCE_SET(obj, SIGNATURE_TEXT);

	obj->string = my_string_create();

	obj->string_update_slot = slot_create();
	slot_set(obj->string_update_slot, (slot_func)string_changed, (slot_arg)obj);
	slot_connect(obj->string_update_slot, my_string_get_update_signal(obj->string));

	obj->self_reference = widget_interface_create(obj, (void(*)(void *))text_draw, (void(*)(void *))text_destroy);
	obj->glyph = widget_create(obj->self_reference);

	obj->just = TEXT_LEFT_JUST;

	obj->color = color_null();

	return obj;
}

void text_destroy(text_t* obj)
{
	PTR_CHECK(obj, "text");
	INSTANCE_CHECK(obj, SIGNATURE_TEXT, "text");

	widget_destroy(obj->glyph);
	widget_interface_destroy(obj->self_reference);

	slot_destroy(obj->string_update_slot);
	my_string_destroy(obj->string);

	INSTANCE_CLEAR(obj);
	free(obj);
}

my_string_t* text_get_string(text_t* obj)
{
	PTR_CHECK_RETURN(obj, "text", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_TEXT, "text", NULL);

	return obj->string;
}


void text_set_font(text_t* obj, font_t *font)
{
	PTR_CHECK(obj, "text");
	INSTANCE_CHECK(obj, SIGNATURE_TEXT, "text");

	obj->font = font;
	update_position_and_size(obj);
}

void text_set_reference_position(text_t* obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "text");
	INSTANCE_CHECK(obj, SIGNATURE_TEXT, "text");

	obj->ref_is_set = TRUE;
	obj->ref_x = x;
	obj->ref_y = y;

	update_position_and_size(obj);
}

void text_set_color_html(text_t* obj, const char* html_color_code)
{
	PTR_CHECK(obj, "text");
	INSTANCE_CHECK(obj, SIGNATURE_TEXT, "text");

	obj->color = color_html(html_color_code);
}

void text_set_justification(text_t * obj, enum e_text_justification just)
{
	PTR_CHECK(obj, "text");
	INSTANCE_CHECK(obj, SIGNATURE_TEXT, "text");

	obj->just = just;
	update_position_and_size(obj);
}

widget_t* text_get_widget(text_t* const obj)
{
	PTR_CHECK_RETURN(obj, "text", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_TEXT, "text", NULL);

	return obj->glyph;
}

