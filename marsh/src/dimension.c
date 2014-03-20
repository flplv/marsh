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

#include "dimension.h"
#include "helper/number.h"
#include "helper/checks.h"
#include "helper/log.h"

#include <string.h>

void pos_clear(pos_t *tgt)
{
	PTR_CHECK(tgt, "dimension");
	memset(tgt, 0x00, sizeof(pos_t));
}

void area_clear(area_t *tgt)
{
	PTR_CHECK(tgt, "dimension");
	memset(tgt, 0x00, sizeof(area_t));
}

void pos_set(pos_t* tgt, dim_t start_x, dim_t start_y, area_t *size)
{
	PTR_CHECK(tgt, "dimension");
	tgt->start.x = start_x;
	tgt->start.y = start_y;
	tgt->end.x = start_x + size->width;
	tgt->end.y = start_y + size->height;
}

void area_set(area_t* tgt, dim_t wid, dim_t hei)
{
	PTR_CHECK(tgt, "dimension");
	tgt->width = wid;
	tgt->height = hei;
}

static bool configured(const dimension_t* tgt)
{
	bool bad = false;

	if (tgt->size_set == false)
		bad = true;

	if (tgt->pos_start_set == false)
		bad = true;

	if (tgt->pos_end_set == false)
		bad = true;

	if (bad)
	{
		return false;
	}

	return true;
}

void dimension_set_rest_if_possible(dimension_t * tgt)
{
	PTR_CHECK(tgt, "dimension");

	if (tgt->size_set)
	{
		if (tgt->pos_start_set)
		{
			tgt->pos.end.x = tgt->pos.start.x + tgt->size.width - 1;
			tgt->pos.end.y = tgt->pos.start.y + tgt->size.height - 1;
			tgt->pos_end_set = true;
		}
		else if (tgt->pos_end_set)
		{
			tgt->pos.start.x = tgt->pos.end.x - tgt->size.width + 1;
			tgt->pos.start.y = tgt->pos.end.y - tgt->size.height + 1;
			tgt->pos_start_set = true;
		}
	}
	else
	{
		if (!tgt->pos_start_set)
			return;
		if (!tgt->pos_end_set)
			return;

		tgt->size.width  = tgt->pos.end.x - tgt->pos.end.x + 1;
		tgt->size.height = tgt->pos.end.y - tgt->pos.end.y + 1;
		tgt->size_set = true;
	}
}

bool dimension_good(const dimension_t *tgt)
{
	bool bad = false;

	PTR_CHECK_RETURN (tgt, "dimension", bad);

	if (!configured(tgt)) {
		return false;
	}

	if (tgt->pos.start.x < 0)
		bad = true;
	if (tgt->pos.start.y < 0)
		bad = true;
	if (tgt->size.width < 0)
		bad = true;
	if (tgt->size.height < 0)
		bad = true;

	if (bad)
		return false;

	return true;
}

void dimension_merge(dimension_t* tgt, const dimension_t* from)
{
	PTR_CHECK(tgt, "dimension");
	PTR_CHECK(from, "dimension");

	if (!dimension_good(from)) {
		global_my_log(ERROR, __FILE__, __LINE__, "\"from\" Badly configured", "dimension");
		return;
	}

	if (dimension_good(tgt))
	{
		tgt->pos.start.x = get_smaller(tgt->pos.start.x, from->pos.start.x);
		tgt->pos.start.y = get_smaller(tgt->pos.start.y, from->pos.start.y);
		tgt->pos.end.x = get_bigger(tgt->pos.end.x, from->pos.end.x);
		tgt->pos.end.y = get_bigger(tgt->pos.end.y, from->pos.end.y);
		tgt->size_set = false;
		dimension_set_rest_if_possible(tgt);
	}
	else
	{
		*tgt = *from;
	}
}

void dimension_set_start_position(dimension_t* tgt, dim_t start_x, dim_t start_y)
{
	PTR_CHECK(tgt, "dimension");

	tgt->pos.start.x = start_x;
	tgt->pos.start.y = start_y;
	tgt->pos_start_set = true;
}

void dimension_set_end_position(dimension_t* tgt, dim_t end_x, dim_t end_y)
{
	PTR_CHECK(tgt, "dimension");

	tgt->pos.end.x = end_x;
	tgt->pos.end.y = end_y;
	tgt->pos_end_set = true;
}

void dimension_set_size(dimension_t* tgt, dim_t width, dim_t height)
{
	PTR_CHECK(tgt, "dimension");

	tgt->size.height = height;
	tgt->size.width = width;
	tgt->size_set = true;
}

bool dimension_contains(const dimension_t* dim, dim_t x, dim_t y)
{
	if (!dimension_good(dim)) {
		global_my_log(ERROR, __FILE__, __LINE__, "\"dim\" Badly configured", "dimension");
		return false;
	}

	if (x < dim->pos.start.x)
		return false;

	if (y < dim->pos.start.y)
		return false;

	if (x > dim->pos.end.x)
		return false;

	if (y > dim->pos.end.y)
		return false;

	return true;
}

void dimension_clear(dimension_t* tgt)
{
	memset(tgt, 0x00, sizeof(dimension_t));
}
