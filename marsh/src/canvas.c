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

#include "canvas.h"
#include "canvas_private.h"
#include "color.h"
#include "framebuffer.h"
#include "area.h"
#include "helper/number.h"
#include "helper/checks.h"
#include "types.h"
#include <math.h>

/* TODO: Add/fix the negative position or dimension on the canvas, drawing function and widget system.
 *       also, fix the out of canvas printing that happens when parent size is smaller than child's.
 *       1. Canvas should hide the scratch_pad from client, and manage if it uses the fb directly or a scratch pad.
 *       2. Clients writes (or use a writing function) to write to the canvas memory transparently.
 *       3. At the end, client inform canvas it has finished writing and canvas copies the scratch to the fb if necessary.
 *
 *       Issue to be resolved:
 *        - a writing function will be need to set a single pixel instead of writing the pixel right to the memory, this is not optimized,
 *          a better memory writing scheme must be designed, with the same cost as writing directly to memory and transparent to client,
 *          allowing canvas to change the pixel writing order (top-down, bot-up, etc.) accordingly to negative dimensions.
 *
 *          Solution 1.:
 *            Clients always write to the scratch pad without line breaks, canvas copy the scratch pad to the fb considering line breaks and painting directions.
 *            Pros: simple interface
 *            Cons: double copy to set a pixel on the screen.
 */

static pixel_t * scratch_pad = NULL;

void canvas_delete_scratchpad()
{
	if (scratch_pad)
		free(scratch_pad);
	scratch_pad = NULL;
}

canvas_t * canvas_new_fullscreen()
{
	canvas_t * canv = (canvas_t *)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(canv, NULL);

	canv->tgt_memory_start = framebuffer_start();
	canv->height = framebuffer_height();
	canv->width = framebuffer_width();
	canv->line_incrementation_width = framebuffer_width();

	return canv;
}

canvas_t * canvas_new(const area_t * area)
{
	canvas_t * canv = (canvas_t *)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(canv, NULL);

	canv->tgt_memory_start = framebuffer_at(area->x, area->y);
	canv->height = area->height;
	canv->width = area->width;
	canv->line_incrementation_width = framebuffer_width();

	return canv;
}

canvas_t * canvas_new_scratchpad()
{
	canvas_t * canv = canvas_new_fullscreen();
	PTR_CHECK_RETURN(canv, "canvas", NULL);

	if (scratch_pad == NULL)
	{
		scratch_pad = (typeof(scratch_pad))malloc(framebuffer_width() * framebuffer_height() * sizeof(*scratch_pad));
	}

	canv->tgt_memory_start = scratch_pad;

	return canv;
}

canvas_t * canvas_new_sub_canvas(canvas_t * canv, size_t x, size_t y, size_t width, size_t height)
{
	canvas_t *sub_canvas;
	PTR_CHECK_RETURN(canv, "canvas", NULL);

	sub_canvas = (canvas_t *)calloc(1, sizeof(struct s_canvas));
	MEMORY_ALLOC_CHECK_RETURN(sub_canvas, NULL);

	sub_canvas->tgt_memory_start = canv->tgt_memory_start + x + y * canv->line_incrementation_width;
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

bool canvas_scratchpad(const canvas_t *canv)
{
	if (scratch_pad == NULL)
		return false;

	if (scratch_pad != canv->tgt_memory_start)
		return false;

	return true;
}
