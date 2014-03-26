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

#include "virtual_canvas.h"
#include "helper/checks.h"

struct s_virtual_canvas
{
	dim_t x, y, width, height, xend, yend;
};

static dim_t end_pos(dim_t start_pos, dim_t length)
{
	return start_pos + length;
}

virtual_canvas_t * virtual_canvas_new(void)
{
	virtual_canvas_t * obj = (virtual_canvas_t *) malloc(sizeof(struct s_virtual_canvas));
	MEMORY_ALLOC_CHECK(obj);

	return obj;
}

void virtual_canvas_delete(virtual_canvas_t * obj)
{
	PTR_CHECK(obj, "virtual_canvas");
	free(obj);
}

void virtual_canvas_set_dimension(virtual_canvas_t * obj, dim_t x, dim_t y, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "virtual_canvas");

	obj->x = x;
	obj->y = y;
	obj->width = width;
	obj->height = height;
	obj->xend = end_pos(x, width);
	obj->yend = end_pos(y, height);
}
