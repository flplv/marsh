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

#ifndef widget_H_
#define widget_H_

#include "types.h"

/*
 * Widget is a base class for widgets.
 *
 *
 * Caution must be taken on the destruction of a widget, read the following comments.
 */

widget_t * widget_new(widget_t * parent, void * creator_instance, void (*creator_draw)(void *), void (*creator_delete)(void *));

/*
 * Widget destruction destroy the widget itself and all its children, as well its
 * children's children, and so on.
 *
 * If the widget is created without a creator destructor (creator_instance and creator_delete),
 * the widget will call widget_delete_instance_only from widget_delete, otherwise, it is
 * expected that creator destructor call widget_delete_instance_only.
 */

/* Only use this method if you hold the ownership of the widget, or created directly
   the widget through widget_new. This is the real widget_t type destructor. */
void widget_delete_instance(widget_t * obj);

/* Use this method to call the widget creator (rectangle, etc.) destroy function.
   This is the virtual call of the widget destructor.
   If, during creation, there is no creator_delete or creator_instance, no creator destructor
   will be called, than widget_delete_instance_only will be called internally.   */
void widget_virtual_delete(widget_t * obj);

/* This method delete a widget, its creator, and all tree behind it, including the
 * creator of each node.  */
void widget_delete(widget_t * obj);

/*
 * TODO: Change the widget patter from polymorphism to decorator. This way all this deletion mass
 * will be gone.
 * I.e.: instead a image class contain a widget, a widget will contain a decorator,
 * that is the image class itself.
 */

area_t * widget_area(widget_t *);

void widget_draw(widget_t *);
void widget_press(widget_t *, int x, int y);
void widget_release(widget_t *, int x, int y);
void widget_click(widget_t *, int x, int y);

void widget_process_click(widget_t * obj);
void widget_process_release(widget_t * obj);
void widget_process_press(widget_t * obj);
void widget_process_draw(widget_t * obj);

signal_t * widget_click_signal(widget_t * obj);
signal_t * widget_release_signal(widget_t * obj);
signal_t * widget_press_signal(widget_t * obj);

#endif /* widget_H_ */
