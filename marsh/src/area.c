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

#include "helper/checks.h"
#include "helper/number.h"

#include "area.h"
#include "framebuffer.h"
#include "helper/number.h"

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

area_t * area_alloc(void)
{
	return (area_t *)calloc(1, sizeof(struct s_area));
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

point_t area_start_point(const area_t * area)
{
	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));
	return (point_t){area->x, area->y};
}

point_t area_end_point(const area_t * area)
{
	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));

	return (point_t){area->x + area->width, area->y + area->height};
}

point_t area_start_point_abs(const area_t * area)
{
	point_t ret;

	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));

	if (area->width < 0)
		ret.x = area_end_point(area).x;
	else
		ret.x = area->x;

	if (area->height < 0)
		ret.y = area_end_point(area).y;
	else
		ret.y = area->y;

	return ret;
}

point_t area_end_point_abs(const area_t * area)
{
	PTR_CHECK_RETURN(area, "area", ((point_t){0, }));

	return (point_t){area->x + area->width, area->y + area->height};
}

bool area_intersects(const area_t * first, const area_t * second)
{
	PTR_CHECK_RETURN(first, "area", false);
	PTR_CHECK_RETURN(second, "area", false);

    if (area_end_point_abs(first).x <= area_start_point_abs(second).x
    	|| area_end_point_abs(second).x <= area_start_point_abs(first).x)
        return false;

    if (area_end_point_abs(first).y <= area_start_point_abs(second).y
        	|| area_end_point_abs(second).y <= area_start_point_abs(first).y)
            return false;

    return true;
}

void area_set_intersection(area_t *tgt, const area_t * first, const area_t * second)
{
	PTR_CHECK(tgt, "area");
	PTR_CHECK(first, "area");
	PTR_CHECK(second, "area");

    if(area_intersects(first, second))
    {
        area_set(tgt,
        		get_bigger(first->x, second->x),
        		get_bigger(first->y, second->y),
        		get_smaller(area_end_point(first).x, area_end_point(second).x) - get_bigger(first->x, second->x),
        		get_smaller(area_end_point(first).y, area_end_point(second).y) - get_bigger(first->y, second->y));
    }
    else
    {
        area_set(tgt, 0, 0, 0, 0);
    }
}

dim_t area_value(const area_t * area)
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

bool area_same(const area_t * first, const area_t * second)
{
	if (memcmp(first, second, sizeof(*first)) == 0)
		return true;

	return false;
}
