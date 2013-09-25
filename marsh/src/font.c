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

#include "font.h"
#include "helper/checks.h"
#include "helper/number.h"
#include "helper/my_string.h"
#include "font_data/font_data.h"
#include "canvas.h"


static dim_t char_bitmap_width(font_t *font)
{
	if (font->bitmap_data_width == FONT_BUFFER_8BITS)
		return 8;

	if (font->bitmap_data_width == FONT_BUFFER_16BITS)
		return 16;

	LOG_ERROR("font", "invalid bitmap_data_width");
	return 0;
}

static dim_t char_bitmap_height(font_t *font)
{
	return font->height;
}


static BUFFER_PTR_RDOLY bitmap(font_t * font, char c)
{
	uint32_t byte_offset;
	uint32_t byte_per_line;
	BUFFER_PTR ret;

	if (font->bitmap_data_width == FONT_BUFFER_8BITS)
		byte_per_line = 1;
	else if (font->bitmap_data_width == FONT_BUFFER_16BITS)
		byte_per_line = 2;
	else
	{
		LOG_ERROR("font", "invalid font setting");
	}

	byte_offset =  byte_per_line * font->index[(uint32_t)c] * font->height;

	ret = (((BUFFER_PTR)font->bitmap) + byte_offset);

	return ret;
}

static dim_t height(font_t * font)
{
	return font->height;
}

dim_t font_char_width(font_t * font, char c)
{
	return font->width[(uint32_t)font->index[(uint32_t)c]];
}

dim_t font_string_width(font_t* font, my_string_t* string)
{
	dim_t width = 0;
	dim_t last_line_width = 0;
	const char * pCh = my_string_get(string);

	while (*pCh)
	{
		if (charactere_is_printable(*pCh)) {
			width += font_char_width(font, *pCh);
		}
		else if (*pCh == '\n')
		{
			if (last_line_width < width)
				last_line_width = width;
			width = 0;
		}

		pCh++;
	}

	if (width < last_line_width)
		return last_line_width;

	return width;
}

dim_t font_string_height(font_t* font, my_string_t* string)
{
	dim_t height = font->height;
	const char * pCh = my_string_get(string);

	while (*pCh)
	{
		if (*pCh == '\n')
		{
			height += font->height;
		}
		pCh++;
	}

	return height;
}

void font_draw_left_just(font_t* font, my_string_t* string, const canvas_t *canv)
{
	const char * pCh = my_string_get(string);
	uint32_t x, y, bitmap_height, bitmap_width;

	bitmap_height = char_bitmap_height(font);
	bitmap_width = char_bitmap_width(font);

	x = 0;
	y = 0;

	while (*pCh)
	{
		if (charactere_is_printable(*pCh)) {
			canvas_draw_bitmap_1bpp(canv, bitmap(font, *pCh), x, y, bitmap_width, bitmap_height);
			x += font_char_width(font, *pCh);
		}
		else if ('\n' == *pCh) {
			x = 0;
			y += height(font);
		}

		pCh++;
	}
}

static dim_t line_width(font_t * font, const char *pCh)
{
	dim_t width = 0;

	while(*pCh)
	{
		if (charactere_is_printable(*pCh)) {
			width += font_char_width(font, *pCh);
		}
		else if (*pCh == '\n')
		{
			return width;
		}

		pCh++;
	}

	return width;
}

void font_draw_center_just(font_t* font, my_string_t* string, const canvas_t *canv)
{
	const char * pCh = my_string_get(string);
	uint32_t x, y, bitmap_height, bitmap_width;

	bitmap_height = char_bitmap_height(font);
	bitmap_width = char_bitmap_width(font);

	x = (canvas_get_width(canv) - line_width(font, pCh))/2;
	y = 0;

	while (*pCh)
	{
		if (charactere_is_printable(*pCh)) {
			canvas_draw_bitmap_1bpp(canv, bitmap(font, *pCh), x, y, bitmap_width, bitmap_height);
			x += font_char_width(font, *pCh);
		}
		else if ('\n' == *pCh) {
			x = (canvas_get_width(canv) - line_width(font, pCh+1))/2;
			y += height(font);
		}

		pCh++;
	}
}


void font_draw_right_just(font_t* font, my_string_t* string, const canvas_t *canv)
{
	const char * pCh = my_string_get(string);
	uint32_t x, y, bitmap_height, bitmap_width;

	bitmap_height = char_bitmap_height(font);
	bitmap_width = char_bitmap_width(font);

	x = (canvas_get_width(canv) - line_width(font, pCh));
	y = 0;

	while (*pCh)
	{
		if (charactere_is_printable(*pCh)) {
			canvas_draw_bitmap_1bpp(canv, bitmap(font, *pCh), x, y, bitmap_width, bitmap_height);
			x += font_char_width(font, *pCh);
		}
		else if ('\n' == *pCh) {
			x = (canvas_get_width(canv) - line_width(font, pCh+1));
			y += height(font);
		}

		pCh++;
	}
}
