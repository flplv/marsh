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

#ifndef DRAWING_ALGORITHMS_H_
#define DRAWING_ALGORITHMS_H_

#include "types.h"
#include "helper/helper_types.h"

void draw_solid_rectangle(const canvas_t *canv, pixel_t color);
void draw_solid_round_rectangle(const canvas_t *canv, pixel_t color, size_t round_radius);
void draw_round_rectangle(const canvas_t *canv, pixel_t color, size_t line_width, size_t round_radius);
void draw_rectangle(const canvas_t *canv, pixel_t color, size_t line_width);
void draw_circle(const canvas_t *canv, pixel_t color);
void draw_bitmap_1bpp(const canvas_t *canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void draw_bitmap(const canvas_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void draw_alpha_bitmap_8bpp(const canvas_t *canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);


#endif /* DRAWING_ALGORITHMS_H_ */
