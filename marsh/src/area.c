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

#include "area.h"

#include <string.h>

void point_clear(point_t *tgt)
{
	PTR_CHECK(tgt, "area");
	memset(tgt, 0x00, sizeof(*tgt));
}

void area_clear(area_t *tgt)
{
	PTR_CHECK(tgt, "area");
	memset(tgt, 0x00, sizeof(*tgt));
}


void point_set(point_t * tgt, dim_t x, dim_t y)
{
	PTR_CHECK(tgt, "area");
	tgt->x = x;
	tgt->y = y;
}

void area_set(area_t * area, dim_t x, dim_t y, dim_t width, dim_t height)
{
	PTR_CHECK(area, "area");

	area->x = x;
	area->y = y;
	area->width = width;
	area->height = height;
}

void area_set_start_point(area_t * area, point_t p)
{
	PTR_CHECK(area, "area");

	area->x = p.x;
	area->y = p.y;
}

void area_set_start_xy(area_t * area, dim_t x, dim_t y)
{
	PTR_CHECK(area, "area");

	area->x = x;
	area->y = y;
}

void area_set_size(area_t * area, dim_t width, dim_t height)
{
	PTR_CHECK(area, "area");

	area->width = width;
	area->height = height;
}

point_t area_start_point(area_t * area)
{
	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));
	return (point_t){area->x, area->y};
}

point_t area_end_point(area_t * area)
{
	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));

	return (point_t){area->x + area->width - 1, area->y + area->height - 1};
}

dim_t area_value(area_t * area)
{
	PTR_CHECK_RETURN(area, "area", false);
	return (get_abs(area->width) * get_abs(area->height));
}

bool area_contains_point(const area_t * area, point_t point)
{
	PTR_CHECK_RETURN(area, "area", false);

	if (point.x < area->x)
		return false;

	if (point.y < area->y)
		return false;

	if (point.x >= (area->x + area->width))
		return false;

	if (point.y >= (area->y + area->height))
		return false;

	return true;
}
