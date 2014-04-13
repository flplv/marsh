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

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "types.h"

rectangle_t * rectangle_new(widget_t * parent);
void rectangle_delete(rectangle_t * const);

void rectangle_set_position(rectangle_t * const, dim_t x, dim_t y);
void rectangle_set_size(rectangle_t * const, dim_t width, dim_t height);
void rectangle_set_fill_color_html(rectangle_t * const, const char * html_color_code);
void rectangle_set_border_tickness(rectangle_t * const, dim_t tickness);
void rectangle_set_border_color_html(rectangle_t * const, const char *html_color_code);
void rectangle_set_rounded_corner_radius(rectangle_t * const, dim_t radius);

widget_t *rectangle_get_widget(rectangle_t * const);

#endif /* RECTANGLE_H_ */
