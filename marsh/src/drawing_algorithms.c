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

#include <stdbool.h>
#include "helper/helper_types.h"
#include "helper/checks.h"
#include "helper/number.h"
#include "types.h"
#include "canvas_private.h"
#include "color.h"

enum circle_arcs
{
	CIRCLE_NONE = 0,
	CIRCLE_NEN = 0x01,
	CIRCLE_NEE = 0x02,
	CIRCLE_SEE = 0x04,
	CIRCLE_SES = 0x08,
	CIRCLE_SWW = 0x10,
	CIRCLE_SWS = 0x20,
	CIRCLE_NWN = 0x40,
	CIRCLE_NWW = 0x80,
	CIRCLE_NE  = CIRCLE_NEN | CIRCLE_NEE,
	CIRCLE_NW  = CIRCLE_NWN | CIRCLE_NWW,
	CIRCLE_SE  = CIRCLE_SEE | CIRCLE_SES,
	CIRCLE_SW  = CIRCLE_SWW | CIRCLE_SWS,
	CIRCLE_N   = CIRCLE_NE | CIRCLE_NW,
	CIRCLE_S   = CIRCLE_SE | CIRCLE_SW,
	CIRCLE_E   = CIRCLE_NE | CIRCLE_SE,
	CIRCLE_W   = CIRCLE_NW | CIRCLE_SW,
	CIRCLE_ALL = CIRCLE_N | CIRCLE_S
};


static __inline void dot(const pixel_t color, size_t x, size_t y, pixel_t *to, size_t line_incr_width)
{
	*(to + x + y * line_incr_width) = color;
}

static __inline void horizontal_line(const pixel_t color, size_t x, size_t y, int32_t width, pixel_t *to, size_t line_incr_width)
{
	size_t xi = (get_sig(width)==1)?(x):(x+width);
	size_t length = (get_sig(width)==1)?(width):(-width);

	to += xi + y * line_incr_width;

	while(length--)
		*to++ = color;
}

static __inline void vertical_line(const pixel_t color, size_t x, size_t y, int32_t height, pixel_t *to, size_t line_incr_width)
{
	size_t yi = (get_sig(height)==1)?(y):(y+height);
	size_t length = (get_sig(height)==1)?(height):(-height);

	to += x + yi * line_incr_width;

	while(length--) {
		*to = color;
		to += line_incr_width;
	}
}

static __inline void circle_kernel(pixel_t* to, const pixel_t color, enum circle_arcs arc, int32_t x_center, int32_t x, int32_t y_center, int32_t y, int32_t line_incr_width)
{
	if (arc & CIRCLE_NEN) dot(color, (x_center + x), (y_center - y), to, line_incr_width); //NEN
	if (arc & CIRCLE_NEE) dot(color, (x_center + y), (y_center - x), to, line_incr_width); //NEE
	if (arc & CIRCLE_SEE) dot(color, (x_center + y), (y_center + x), to, line_incr_width); //SEE
	if (arc & CIRCLE_SES) dot(color, (x_center + x), (y_center + y), to, line_incr_width); //SES
	if (arc & CIRCLE_SWW) dot(color, (x_center - y), (y_center + x), to, line_incr_width); //SWW
	if (arc & CIRCLE_SWS) dot(color, (x_center - x), (y_center + y), to, line_incr_width); //SWS
	if (arc & CIRCLE_NWW) dot(color, (x_center - y), (y_center - x), to, line_incr_width); //NWW
	if (arc & CIRCLE_NWN) dot(color, (x_center - x), (y_center - y), to, line_incr_width); //NWN
}

static __inline void solid_circle_kernel(pixel_t* to, const pixel_t color, enum circle_arcs arc, int32_t x_center, int32_t x, int32_t y_center, int32_t y, int32_t line_incr_width)
{
	if (arc & CIRCLE_NEN) horizontal_line(color, x_center, (y_center - y),  + x, to, line_incr_width); //NEN
	if (arc & CIRCLE_NEE) horizontal_line(color, x_center, (y_center - x),  + y, to, line_incr_width); //NEE
	if (arc & CIRCLE_SEE) horizontal_line(color, x_center, (y_center + x),  + y, to, line_incr_width); //SEE
	if (arc & CIRCLE_SES) horizontal_line(color, x_center, (y_center + y),  + x, to, line_incr_width); //SES
	if (arc & CIRCLE_SWW) horizontal_line(color, x_center, (y_center + x),  - y, to, line_incr_width); //SWW
	if (arc & CIRCLE_SWS) horizontal_line(color, x_center, (y_center + y),  - x, to, line_incr_width); //SWS
	if (arc & CIRCLE_NWW) horizontal_line(color, x_center, (y_center - x),  - y, to, line_incr_width); //NWW
	if (arc & CIRCLE_NWN) horizontal_line(color, x_center, (y_center - y),  - x, to, line_incr_width); //NWN
}

static __inline void circle_loop(pixel_t* to, const pixel_t color, enum circle_arcs arc, size_t radius, size_t x_center, size_t y_center, size_t line_incrementation_width)
{
	int32_t d1 = 3 - (2 * radius);
	int32_t x = 0;
	int32_t y = radius;
	bool rov = true;
	while (rov)
	{
		if (x >= y)
		{
			rov = false;
		}
		if (d1 < 0)
		{
			d1 = d1 + (4 * x) + 6;
		}
		else
		{
			d1 = d1 + 4 * (x - y) + 10;
			y = y - 1;
		}
		circle_kernel(to, color, arc, x_center, x, y_center, y, line_incrementation_width);
		x++;
	}
}

static __inline void solid_circle_loop(pixel_t* to, const pixel_t color, enum circle_arcs arc, size_t radius, size_t x_center, size_t y_center, size_t line_increment_width)
{
	int32_t d1 = 3 - (2 * radius);
	int32_t x = 0;
	int32_t y = radius;
	bool rov = true;
	while (rov)
	{
		if (x >= y)
		{
			rov = false;
		}
		if (d1 < 0)
		{
			d1 = d1 + (4 * x) + 6;
		}
		else
		{
			d1 = d1 + 4 * (x - y) + 10;
			y = y - 1;
		}
		solid_circle_kernel(to, color, arc, x_center, x, y_center, y, line_increment_width);
		x++;
	}
}

static void solid_rectangle(pixel_t * to, pixel_t color, size_t width, size_t height, size_t line_increment_width)
{
	size_t i,j;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			*to++ = color;
		}
		to += line_increment_width;
		to -= width;
	}
}

void draw_circle(const canvas_t *canv, pixel_t color)
{
	PTR_CHECK(canv, "draw_algorithms");

	if (color_check(color))
	{
		LOG_ERROR("draw_algorithms", "invalid color");
		return;
	}

	size_t radius = ((canv->width>canv->height)?(canv->width/2):(canv->height/2));
	size_t x_center = canv->width/2;
	size_t y_center = canv->height/2;

	CANVAS_START_CIRC_CENTER_RADIUS(canv, x_center, y_center, radius, to);
	circle_loop(to, color, CIRCLE_ALL, radius, x_center, y_center,canv->line_incrementation_width);
}

void draw_solid_round_rectangle(const canvas_t *canv, pixel_t color, size_t round_radius)
{
	pixel_t *to;
	size_t xi, yi;

	PTR_CHECK(canv, "draw_algorithms");

	if (color_check(color))
	{
		LOG_ERROR("draw_algorithms", "invalid color");
		return;
	}

	xi = 0;
	yi = 0;
	to = CANVAS_TO(canv, xi, yi);
	solid_circle_loop(to, color, CIRCLE_NW, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = canv->width - round_radius*2;
	yi = 0;
	to = CANVAS_TO(canv, xi, yi);
	solid_circle_loop(to, color, CIRCLE_NE, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = 0;
	yi = canv->height-1 - round_radius*2;
	to = CANVAS_TO(canv, xi, yi);
	solid_circle_loop(to, color, CIRCLE_SW, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = canv->width - round_radius*2;
	yi = canv->height-1 - round_radius*2;
	to = CANVAS_TO(canv, xi, yi);
	solid_circle_loop(to, color, CIRCLE_SE, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	solid_rectangle(CANVAS_TO(canv, round_radius, 0), color, canv->width - round_radius*2, round_radius+1, canv->line_incrementation_width);
	solid_rectangle(CANVAS_TO(canv, round_radius, canv->height-1 - round_radius), color, canv->width - round_radius*2, round_radius+1, canv->line_incrementation_width);
	solid_rectangle(CANVAS_TO(canv, 0, round_radius), color, canv->width, canv->height - round_radius*2, canv->line_incrementation_width);

}

void draw_round_rectangle(const canvas_t *canv, pixel_t color, size_t line_width, size_t round_radius)
{
	pixel_t *to;
	size_t xi, yi;
	size_t i;

	PTR_CHECK(canv, "draw_algorithms");

	if (color_check(color))
	{
		LOG_ERROR("draw_algorithms", "invalid color");
		return;
	}

	to = CANVAS_TO(canv, 0, 0);

	for (i = 0; i < line_width; i++)
	{
		horizontal_line(color, round_radius, i, canv->width-1 - round_radius*2, to, canv->line_incrementation_width);
		horizontal_line(color, round_radius, canv->height-1 - i, canv->width - round_radius*2, to, canv->line_incrementation_width);
		vertical_line(color, i, round_radius, canv->height-1 - round_radius*2, to, canv->line_incrementation_width);
		vertical_line(color, canv->width-1 - i, round_radius, canv->height-1 - round_radius*2, to, canv->line_incrementation_width);
	}

	xi = 0;
	yi = 0;
	to = CANVAS_TO(canv, xi, yi);
	circle_loop(to, color, CIRCLE_NW, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = canv->width-1 - round_radius*2;
	yi = 0;
	to = CANVAS_TO(canv, xi, yi);
	circle_loop(to, color, CIRCLE_NE, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = 0;
	yi = canv->height-1 - round_radius*2;
	to = CANVAS_TO(canv, xi, yi);
	circle_loop(to, color, CIRCLE_SW, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	xi = canv->width-1 - round_radius*2;
	yi = canv->height-1 - round_radius*2;
	to = CANVAS_TO(canv, xi, yi);
	circle_loop(to, color, CIRCLE_SE, round_radius, round_radius, round_radius, canv->line_incrementation_width);

	for (i = 1; i < line_width; i++)
	{
		xi = 0;
		yi = 0;
		to = CANVAS_TO(canv, xi, yi);
		circle_loop(to, color, CIRCLE_NW, round_radius - i, round_radius, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_NW, round_radius - i, round_radius - 1, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_NW, round_radius - i, round_radius, round_radius - 1, canv->line_incrementation_width);

		xi = canv->width-1 - round_radius * 2;
		yi = 0;
		to = CANVAS_TO(canv, xi, yi);
		circle_loop(to, color, CIRCLE_NE, round_radius - i, round_radius, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_NE, round_radius - i, round_radius + 1, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_NE, round_radius - i, round_radius, round_radius - 1, canv->line_incrementation_width);

		xi = 0;
		yi = canv->height-1 - round_radius * 2;
		to = CANVAS_TO(canv, xi, yi);
		circle_loop(to, color, CIRCLE_SW, round_radius - i, round_radius, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_SW, round_radius - i, round_radius - 1, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_SW, round_radius - i, round_radius, round_radius + 1, canv->line_incrementation_width);

		xi = canv->width-1 - round_radius * 2;
		yi = canv->height-1 - round_radius * 2;
		to = CANVAS_TO(canv, xi, yi);
		circle_loop(to, color, CIRCLE_SE, round_radius - i, round_radius, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_SE, round_radius - i, round_radius + 1, round_radius, canv->line_incrementation_width);
		circle_loop(to, color, CIRCLE_SE, round_radius - i, round_radius, round_radius + 1, canv->line_incrementation_width);
	}
}

void draw_rectangle(const canvas_t *canv, pixel_t color, size_t line_width)
{
	pixel_t *to;
	size_t i;

	PTR_CHECK(canv, "draw_algorithms");

	if (color_check(color))
	{
		LOG_ERROR("draw_algorithms", "invalid color");
		return;
	}

	to = CANVAS_TO(canv, 0, 0);

	for (i = 0; i < line_width; i++)
	{
		horizontal_line(color, 0, i, canv->width-1, to, canv->line_incrementation_width);
		horizontal_line(color, 0, canv->height-1 - i, canv->width, to, canv->line_incrementation_width);
		vertical_line(color, i, 0, canv->height-1, to, canv->line_incrementation_width);
		vertical_line(color, canv->width-1 - i, 0, canv->height-1, to, canv->line_incrementation_width);
	}
}

void draw_solid_rectangle(const canvas_t *canv, pixel_t color)
{
	PTR_CHECK(canv, "draw_algorithms");

	if (color_check(color))
	{
		LOG_ERROR("draw_algorithms", "invalid color");
		return;
	}

	solid_rectangle(CANVAS_TO(canv, 0, 0), color, canv->width, canv->height, canv->line_incrementation_width);
}

static bool bitmap_bit(BUFFER_PTR_RDOLY bitmap, uint32_t bit_offset)
{
	uint32_t byte_offset = 0;

	while (bit_offset >= 8)
	{
		byte_offset ++;
		bit_offset -= 8;
	}

	if (bitmap[byte_offset] & (1<<(7-bit_offset)))
		return true;

	return false;
}

static uint8_t bitmap_byte(BUFFER_PTR_RDOLY bitmap, uint32_t byte_offset)
{
	return bitmap[byte_offset];
}

void draw_bitmap_1bpp(const canvas_t* canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height)
{
	size_t i,j;

	PTR_CHECK(canv, "draw_algorithms");

	CANVAS_START_RECT(canv, x, y, to);

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if (bitmap_bit(bitmap, i*width + j))
				*to = color;
			to++;
		}
		CANVAS_GO_DOWN(canv, to, 1);
		CANVAS_GO_LEFT(canv, to, width);
	}
}

void draw_bitmap(const canvas_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height)
{
	size_t i,j;
	pixel_t * from = (pixel_t *)bitmap;

	PTR_CHECK(canv, "draw_algorithms");

	CANVAS_START_BITMAP(canv, x, y, to);

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			*to = *from;
			to++;
			from++;
		}
		CANVAS_GO_DOWN(canv, to, 1);
		CANVAS_GO_LEFT(canv, to, width);
	}
}

void draw_alpha_bitmap_8bpp(const canvas_t *canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height)
{
	size_t i,j;
	uint8_t alpha_intensity;
	pixel_t existent_pixel_color;
	pixel_t new_pixel_color;


	PTR_CHECK(canv, "draw_algorithms");

	CANVAS_START_RECT(canv, x, y, to);

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			alpha_intensity = bitmap_byte(bitmap, i*width + j);

			if ( alpha_intensity == 0xFF)
			{
				*to = color;
			}
			else if (alpha_intensity > 0x00)
			{
				existent_pixel_color = *to;
				new_pixel_color = color_to_pixel(color_alpha_blend(color_from_pixel(existent_pixel_color), color_from_pixel(color), alpha_intensity));
				*to = new_pixel_color;
			}

			to++;
		}
		CANVAS_GO_DOWN(canv, to, 1);
		CANVAS_GO_LEFT(canv, to, width);
	}
}
