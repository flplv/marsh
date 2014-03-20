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

#ifndef POS_H_
#define POS_H_

#include "types.h"


bool dimension_good(const dimension_t *tgt);
void dimension_clear(dimension_t* tgt);

void dimension_merge(dimension_t *tgt, const dimension_t *from);

void dimension_set_start_position(dimension_t *tgt, dim_t start_x, dim_t start_y);
void dimension_set_end_position(dimension_t *tgt, dim_t end_x, dim_t end_y);
void dimension_set_size(dimension_t *tgt, dim_t width, dim_t height);
void dimension_set_rest_if_possible(dimension_t * tgt);

bool dimension_contains(const dimension_t *, dim_t x, dim_t y);

void pos_clear(pos_t *tgt);
void area_clear(area_t *tgt);

#endif /* POS_H_ */
