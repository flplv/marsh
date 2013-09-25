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

#ifndef OBJECTSTACK_H_
#define OBJECTSTACK_H_


#include "widget.h"

typedef struct s_object_stack object_stack_t;

object_stack_t *object_stack_create(void);
void object_stack_destroy(object_stack_t * const);

void object_stack_add(object_stack_t *, widget_t *);
uint32_t object_stack_size(object_stack_t *);

widget_t * object_stack_get_object(object_stack_t *, uint32_t);

#define OBJECTSTACK_INVALID_INDEX -1
uint32_t object_stack_get_object_index(object_stack_t *, const widget_t * const);

widget_t *object_stack_get_object_on_top(object_stack_t *, dim_t x, dim_t y);

#endif /* OBJECTSTACK_H_ */
