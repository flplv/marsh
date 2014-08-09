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
 * the widget will call widget_delete_instance_only from widget_tree_delete, otherwise, it is
 * expected that creator destructor call widget_delete_instance_only.
 */

/* This is the widget_t instance delete, it doesn't delete it's creator,
 * If you are a widget class and created a widget to use as base, this is the delete you
 * need to use. */
void widget_delete_instance_only(widget_t * obj);

/* This is the public delete for a widget, this delete will call its creator (rectangle, image, etc.)
 * delete function. Use this if you want to delete a widget you created not by calling widget_new, but
 * rectangle_new, etc. This is the delete to be used when polymorphism is used.  */
void widget_delete(widget_t * obj);


/*
 * TODO: Change the widget pattern from polymorphism to decorator.
 * I.e.: instead a image class contain a widget, a widget will contain a decorator,
 * the image class itself, that will decorate widget's behavior.
 * Maybe decoration is not the right pattern yet, further research in this subject is needed.
 */

void widget_set_dim(widget_t *, dim_t width, dim_t height);
void widget_set_pos(widget_t *, dim_t x, dim_t y);
void widget_set_area(widget_t *, dim_t x, dim_t y, dim_t width, dim_t height);
const area_t * widget_area(const widget_t *);
const area_t * widget_canvas_area(const widget_t *);
bool widget_canvas_cropped(const widget_t * obj);

void widget_refresh_dim(widget_t * obj);

void widget_click(widget_t * obj);
void widget_release(widget_t * obj);
void widget_press(widget_t * obj);
void widget_draw(widget_t * obj);

signal_t * widget_click_signal(widget_t * obj);
signal_t * widget_release_signal(widget_t * obj);
signal_t * widget_press_signal(widget_t * obj);

void widget_hide(widget_t * obj);
void widget_show(widget_t * obj);
bool widget_visible(widget_t * obj);

#endif /* widget_H_ */
