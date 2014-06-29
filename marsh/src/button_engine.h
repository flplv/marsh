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

#ifndef BUTTONENGINE_H_
#define BUTTONENGINE_H_

#include "types.h"

button_engine_t *button_engine_new(widget_t * parent);
void button_engine_delete(button_engine_t *);

widget_t *button_engine_get_widget(button_engine_t *);

/* Action Logic Functions */
void button_engine_select_action_logic(button_engine_t *);

void button_engine_set_action_normal_state(button_engine_t *, widget_t *);
void button_engine_set_action_onpress_state(button_engine_t *, widget_t *);

/* Toggle Logic Functions */
void button_engine_select_toggle_logic(button_engine_t *);

void button_engine_set_toggle_on_state(button_engine_t *, widget_t *);
void button_engine_set_toggle_off_state(button_engine_t *, widget_t *);
void button_engine_set_toggle_on_to_off_state(button_engine_t *, widget_t *);
void button_engine_set_toggle_off_to_on_state(button_engine_t *, widget_t *);

#endif /* BUTTONENGINE_H_ */
