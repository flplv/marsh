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
#include "area.h"

extern char * pVirtFb;

#define FRAMEBUFFER_WIDTH  800UL
#define FRAMEBUFFER_HEIGHT 480UL

#define PIXEL_PTR_LINE_INCREMENT_VAL   FRAMEBUFFER_WIDTH
#define PIXEL_PTR_PIXEL_INCREMENT_VAL  1UL

void framebuffer_new()
{
}

void framebuffer_delete()
{
}

pixel_t* framebuffer_start()
{
	return (pixel_t *)pVirtFb;
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

pixel_t* framebuffer_at(pixel_t x, pixel_t y)
{
	pixel_t* pFb = (pixel_t *)pVirtFb;
	return pFb + x*PIXEL_PTR_PIXEL_INCREMENT_VAL + y*PIXEL_PTR_LINE_INCREMENT_VAL;
}

extern void VirtualFb_Refresh(void);
void framebuffer_inform_written_area(size_t x, size_t y, size_t width, size_t height)
{
	VirtualFb_Refresh();
}

