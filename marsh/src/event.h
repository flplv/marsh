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

#ifndef EVENT_H_
#define EVENT_H_

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

#define event_prop_default         0x0
#define event_prop_from_root       0x1
#define event_prop_bottom_up       0x2
#define event_prop_persistent 0x4
#define event_prop_right_to_left   0x8

void event_pool_init(void);
void event_pool_deinit(void);

event_code_t event_pool_new_code(int propagation_mask, const char * event_name);
event_code_t event_pool_code_from_name(const char * event_name);
void event_pool_remove_created_codes(void);

/* If no free_data function is passed, data is not freed when event is deleted */
event_t * event_new(event_code_t event_code, void * data, void (*free_data)(void *));
void event_delete(event_t *);

event_code_t event_code(event_t * event);
const void * event_data(event_t * event);
int event_propagation_mask(event_t * event);

#endif /* WIDGET_EVENT_H_ */
