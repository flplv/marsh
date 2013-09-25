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

#ifndef INTERACTION_ENGINE_H_
#define INTERACTION_ENGINE_H_

#include "types.h"

interaction_engine_t * interaction_engine_create(dimension_t * owner_dimension);
void interaction_engine_destroy(interaction_engine_t *);

void interaction_engine_click(interaction_engine_t *obj);
void interaction_engine_press(interaction_engine_t *obj, dim_t x, dim_t y);
void interaction_engine_release(interaction_engine_t *obj, dim_t x, dim_t y);
void interaction_engine_disable_interaction_logic(interaction_engine_t *obj);

signal_t *interaction_engine_get_click_signal(interaction_engine_t *);
signal2_t *interaction_engine_get_press_signal(interaction_engine_t *);
signal2_t *interaction_engine_get_release_signal(interaction_engine_t *);

void interaction_engine_emmit_pending_release(size_t x, size_t y);

#endif /* INTERACTION_ENGINE_H_ */
