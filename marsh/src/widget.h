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
void widget_delete_instance_only(widget_t * obj);

/* Use this method to call the widget creator (rectangle, etc.) destroy function.
   This is the virtual call of the widget destructor.
   If, during creation, there is no creator_delete or creator_instance, no creator destructor
   will be called, than widget_delete_instance_only will be called internally.   */
void widget_delete(widget_t * obj);

/* TODO: both destructors can be merged into one, as long a default destructor, which would
 * be the current widget_delete, is initialized as the destructor interface.  */

interaction_engine_t * widget_get_interaction_engine(widget_t *);

dimension_t * widget_get_dimension(widget_t *);

void widget_draw(widget_t *);
//bool widget_press(widget_t *, int x, int y);
//bool widget_release(widget_t *, int x, int y);

#endif /* widget_H_ */
