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

#ifndef CANVAS_PRIVATE_H_
#define CANVAS_PRIVATE_H_

struct s_canvas
{
	pixel_t *memory_start;
	size_t height;
	size_t width;
	size_t line_incrementation_width;
};

#define CANVAS_START_RECT(__canvas, __x, __y, __to) pixel_t *__to = __canvas->memory_start + __x + __y * __canvas->line_incrementation_width
#define CANVAS_START_BITMAP(__canvas, __x, __y, __to) pixel_t *__to = __canvas->memory_start + __x + __y * __canvas->line_incrementation_width
#define CANVAS_START_CIRC_CENTER_RADIUS(__canvas, __xc, __yc, __r, __to) \
	pixel_t *__to = __canvas->memory_start + __xc-__r + (__yc-__r) * __canvas->width
#define CANVAS_TO(__canvas, __x, __y) ((__canvas)->memory_start + (__x) + ((__y) * (__canvas)->line_incrementation_width))
#define CANVAS_GO_DOWN(__canvas, __to, __amount) to += (__amount) * __canvas->line_incrementation_width
#define CANVAS_GO_LEFT(__canvas, __to, __amount) to -= __amount


#endif /* CANVAS_PRIVATE_H_ */
