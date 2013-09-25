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

#include <string.h>
#include <ctype.h>

#include "helper/log.h"
#include "helper/number.h"

#include "color.h"
#include "color_private.h"

const struct s_color invalid_color = {0, 0, 0, INVALID_COLOR_FLAG};

color_t color_null(void)
{
	return *(color_t *)&invalid_color;
}

BOOL color_check_html_code(const char * html_color_code)
{
	uint32_t size = strlen(html_color_code);

	if(size != 7)
		return FALSE;
	if(html_color_code[0] != '#')
		return FALSE;

	while(--size)
	{
		if (!charactere_is_hex(html_color_code[size]))
			return FALSE;
	}

	return TRUE;
}

static BOOL _color_check(const color_t color)
{
	if (((struct s_color*)&color)->flag == VALID_COLOR_FLAG)
		return TRUE;
	return FALSE;
}

color_t color(uint8_t red, uint8_t green, uint8_t blue)
{
	struct s_color return_color;
	return_color.red = red;
	return_color.green = green;
	return_color.blue = blue;
	return_color.flag = VALID_COLOR_FLAG;
	return *(color_t*)&return_color;
}

color_t color_html(const char* html_color_code)
{
	struct s_color return_color;
	char processing_color[3];

	if (!color_check_html_code(html_color_code)) {
		global_my_log(ERROR, __FILE__, __LINE__, ": Bad Html Code", "Color");
		return color_null();
	}

	processing_color[0] = html_color_code[1];
	processing_color[1] = html_color_code[2];
	processing_color[2] = '\0';
	return_color.red = string_hex_to_int(processing_color);
	processing_color[0] = html_color_code[3];
	processing_color[1] = html_color_code[4];
	processing_color[2] = '\0';
	return_color.green = string_hex_to_int(processing_color);
	processing_color[0] = html_color_code[5];
	processing_color[1] = html_color_code[6];
	processing_color[2] = '\0';
	return_color.blue = string_hex_to_int(processing_color);
	return_color.flag = VALID_COLOR_FLAG;

	return *(color_t*)&return_color;
}

pixel_t color_to_pixel(color_t color)
{
	struct s_color * pColor = (struct s_color *)&color;

	uint8_t red = pColor->red;
	uint8_t green = pColor->green;
	uint8_t blue = pColor->blue;

	if (!_color_check(color)) {
		global_my_log(ERROR, __FILE__, __LINE__, ": Bad Color", "Color");
		return 0x0;
	}

	return (((red>>3)<<11) | ((green>>2)<<5) | ((blue)>>3));
}

color_t color_from_pixel(const pixel_t pixel_color)
{
	color_t ret_color = color(0,0,0);
	struct s_color *p_ret_color = (struct s_color *)&ret_color;
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	red = (pixel_color >> 8) & 0xF8;
	red |= (red&0x08)?(0x7):(0x0);

	green = (pixel_color >> 3) & 0xFC;
	green |= (green&0x04)?(0x3):(0x0);

	blue = (pixel_color << 3) & 0xF8;
	blue |= (green&0x08)?(0x7):(0x0);

	p_ret_color->red = red;
	p_ret_color->green = green;
	p_ret_color->blue = blue;

	return ret_color;
}

BOOL color_check(color_t color)
{
	return _color_check(color);
}

static uint8_t alpha_blend(uint8_t a, uint8_t b, uint8_t i)
{
	uint32_t result = (uint32_t)a * (255 - (uint32_t)i) / 255 + (uint32_t)b * (uint32_t)i / 255;

	if (result > 255) return 255;
	return (uint8_t) result;
}

color_t color_alpha_blend(color_t color_to, color_t color_from, uint8_t alpha)
{
	color_t ret_color = color(0,0,0);
	struct s_color *p_ret_color = (struct s_color *)&ret_color;
	struct s_color *p_color_to = (struct s_color *)&color_to;
	struct s_color *p_color_from = (struct s_color *)&color_from;

	if (!_color_check(color_to)) {
		LOG_ERROR("color", "Bad arg 1 color");
		return 0x0;
	}

	if (!_color_check(color_from)) {
		LOG_ERROR("color", "Bad arg 2 color");
		return 0x0;
	}

	if (alpha == 0)
		return color_to;
	if (alpha == 255)
		return color_from;

	p_ret_color->red = alpha_blend(p_color_to->red, p_color_from->red, alpha);
	p_ret_color->green = alpha_blend(p_color_to->green, p_color_from->green, alpha);
	p_ret_color->blue = alpha_blend(p_color_to->blue, p_color_from->blue, alpha);

	return ret_color;
}
