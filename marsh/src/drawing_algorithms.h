/*
 * drawing_algorithms.h
 *
 *  Created on: Mar 26, 2014
 *      Author: fanl
 */

#ifndef DRAWING_ALGORITHMS_H_
#define DRAWING_ALGORITHMS_H_

#include "types.h"

void draw_solid_rectangle(const canvas_t *canv, pixel_t color);
void draw_solid_round_rectangle(const canvas_t *canv, pixel_t color, size_t round_radius);
void draw_round_rectangle(const canvas_t *canv, pixel_t color, size_t line_width, size_t round_radius);
void draw_rectangle(const canvas_t *canv, pixel_t color, size_t line_width);
void draw_circle(const canvas_t *canv, pixel_t color);
void draw_bitmap_1bpp(const canvas_t *canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void draw_bitmap(const canvas_t *canv, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);
void draw_alpha_bitmap_8bpp(const canvas_t *canv, pixel_t color, BUFFER_PTR_RDOLY bitmap, size_t x, size_t y, size_t width, size_t height);


#endif /* DRAWING_ALGORITHMS_H_ */
