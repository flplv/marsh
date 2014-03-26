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

#ifndef CANVAS_H_
#define CANVAS_H_

#include "types.h"

canvas_legacy_t * canvas_legacy_new_fullscreen(void);
canvas_legacy_t * canvas_legacy_new(dimension_t *);

void canvas_legacy_draw_solid_rectangle(const canvas_legacy_t *canv);
void canvas_legacy_draw_solid_round_rectangle(const canvas_legacy_t *canv, size_t round_radius);
void canvas_legacy_draw_round_rectangle(const canvas_legacy_t *canv, size_t line_width, size_t round_radius);
void canvas_legacy_draw_rectangle(const canvas_legacy_t *canv, size_t line_width);
void canvas_legacy_draw_circle(const canvas_legacy_t *canv);
void canvas_legacy_draw_bitmap_1bpp(const canvas_legacy_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void canvas_legacy_draw_bitmap(const canvas_legacy_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void canvas_legacy_draw_alpha_bitmap_8bpp(const canvas_legacy_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);

void canvas_legacy_set_color(canvas_legacy_t *canv, pixel_t);
size_t canvas_legacy_get_width(const canvas_legacy_t *canv);

void canvas_legacy_delete(canvas_legacy_t *);

#endif /* CANVAS_H_ */
