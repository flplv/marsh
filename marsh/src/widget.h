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
 * Caution must be taken on the destruction of a widget, read the following comments.
 */

widget_t * widget_new(widget_interface_t *);

/* Only use this method if you hold the ownership of the widget, or created directly
   the widget through widget_new. This is the real widget_t type destructor. */
void widget_delete(widget_t * obj);

/* Use this method to call the widget creator (rectangle, etc.) destroy function.
   This is the virtual call of the widget destructor*/
void widget_delete_interface(widget_t * obj);

/* TODO: both destructors can be merged into one, as long a default destructor, which would
 * be the current widget_delete, is initialized as the destructor interface.  */

interaction_engine_t * widget_get_interaction_engine(widget_t *);

dimension_t * widget_get_dimension(widget_t *);


void widget_draw(const widget_t *);

#endif /* widget_H_ */
