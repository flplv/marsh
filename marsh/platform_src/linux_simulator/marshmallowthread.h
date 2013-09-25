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

#ifndef MARSHMALLOWTHREAD_H_
#define MARSHMALLOWTHREAD_H_

#include "types.h"

class marshmallow_thread
{
public:
	marshmallow_thread();
	virtual ~marshmallow_thread();
	static void *thread_handler(marshmallow_thread *);

	void press(int x, int y);
	void release(int x, int y);

private:
	struct marshmallow_thread_private *p;
	static void lena_onclick();
	static void goto_popup(marshmallow_thread* self);
	static void goto_main(marshmallow_thread* self);

	widget_t *root_pointer, *popup, *main;
};

#endif /* MARSHMALLOWTHREAD_H_ */
