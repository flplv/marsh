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

#ifndef AREA_H_
#define AREA_H_

#include "types.h"

void point_clear(point_t *tgt);
void point_set(point_t * tgt, dim_t x, dim_t y);

void area_clear(area_t *tgt);
void area_clear(area_t* tgt);
void area_set(area_t * area, dim_t x, dim_t y, dim_t width, dim_t height);
void area_set_start_point(area_t * area, point_t point);
void area_set_start_xy(area_t * area, dim_t x, dim_t y);
void area_set_size(area_t * area, dim_t width, dim_t height);

point_t area_start_point(area_t * area);
point_t area_end_point(area_t * area);

dim_t area_value(area_t * area);

bool area_contains_point(const area_t * area, point_t point);

#endif /* AREA_H_ */
