/*
 * canvas_private.h
 *
 *  Created on: Mar 26, 2014
 *      Author: fanl
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
