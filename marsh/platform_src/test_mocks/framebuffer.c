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

#include "framebuffer.h"
#include <string.h>

#include "area.h"

#define FRAMEBUFFER_WIDTH  800UL
#define FRAMEBUFFER_HEIGHT 480UL

#define PIXEL_PTR_LINE_INCREMENT_VAL   FRAMEBUFFER_WIDTH
#define PIXEL_PTR_PIXEL_INCREMENT_VAL  1UL

static pixel_t* pFb;

static bool _fb_not_initd(void)
{
	if (!pFb)
		return true;
	else
		return false;
}

void framebuffer_init()
{
	if (!_fb_not_initd())
		free(pFb);

	pFb = (pixel_t *) calloc(FRAMEBUFFER_HEIGHT * FRAMEBUFFER_WIDTH, sizeof(pixel_t));
}

void framebuffer_deinit()
{
	if (_fb_not_initd())
		return;

	if (pFb)
		free(pFb);
	pFb = NULL;
}

pixel_t* framebuffer_start()
{
	if (_fb_not_initd())
		return NULL;

	return pFb;
}

pixel_t* framebuffer_at(pixel_t x, pixel_t y)
{
	if (_fb_not_initd())
		return NULL;

	return pFb + x*PIXEL_PTR_PIXEL_INCREMENT_VAL + y*PIXEL_PTR_LINE_INCREMENT_VAL;
}

size_t framebuffer_width()
{
	return FRAMEBUFFER_WIDTH;
}

size_t framebuffer_height()
{
	return FRAMEBUFFER_HEIGHT;
}

const area_t * framebuffer_area()
{
	static area_t ret;

	area_set(&ret, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	return &ret;
}

void framebuffer_inform_written_area(size_t x, size_t y, size_t width, size_t height)
{
	x = *(int*)&x;
	y = *(int*)&y;
	width = *(int*)&width;
	height = *(int*)&height;
}

