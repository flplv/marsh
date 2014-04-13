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

#include "canvas.h"
#include "canvas_private.h"
#include "color.h"
#include "framebuffer.h"
#include "dimension.h"
#include "helper/number.h"
#include "helper/checks.h"
#include "types.h"
#include <math.h>


canvas_t* canvas_new_fullscreen()
{
	canvas_t * canv = (canvas_t*)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(canv, NULL);

	canv->memory_start = framebuffer_start();
	canv->height = framebuffer_height();
	canv->width = framebuffer_width();
	canv->line_incrementation_width = framebuffer_width();

	return canv;
}

canvas_t* canvas_new(dimension_t * dim)
{
	canvas_t * canv = (canvas_t*)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(canv, NULL);

	if (!dimension_good(dim))
	{
		LOG_ERROR("canvas", "dimension not good");
		return NULL;
	}

	canv->memory_start = framebuffer_at(dim->pos.start.x, dim->pos.start.y);
	canv->height = dim->size.height;
	canv->width = dim->size.width;
	canv->line_incrementation_width = framebuffer_width();

	return canv;
}

canvas_t * canvas_new_sub_canvas(canvas_t* canv, size_t x, size_t y, size_t width, size_t height)
{
	canvas_t *sub_canvas;
	PTR_CHECK_RETURN(canv, "canvas", NULL);

	sub_canvas = (canvas_t*)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(sub_canvas, NULL);

	sub_canvas->memory_start = canv->memory_start + x + y * canv->line_incrementation_width;
	sub_canvas->height = height;
	sub_canvas->width = width;
	sub_canvas->line_incrementation_width = canv->line_incrementation_width;

	return sub_canvas;
}

void canvas_delete(canvas_t *canv)
{
	PTR_CHECK(canv, "canvas");
	free(canv);
}

size_t canvas_get_width(const canvas_t *canv)
{
	PTR_CHECK_RETURN(canv, "canvas", 0);
	return canv->width;
}

