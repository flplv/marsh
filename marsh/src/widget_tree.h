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


#ifndef WIDGET_TREE_H_
#define WIDGET_TREE_H_

#include "types.h"

widget_t * widget_root(widget_t * child);
widget_t * widget_parent(widget_t * child);
widget_t * widget_child(widget_t * parent);
widget_t * widget_left_sibling(widget_t * sibling);
widget_t * widget_right_sibling(widget_t * sibling);
widget_t * widget_last(widget_t * sibling);
widget_t * widget_last_child(widget_t * parent);
size_t widget_num_of_children(const widget_t * parent);

/* This method delete a widget, its creator, and all tree behind it, including the
 * creator of each node. It creates and deletion event that propagates to obj and its
 * children calling widget_delete for each. */
void widget_tree_delete(widget_t * obj);

void widget_tree_draw(widget_t *);
void widget_tree_press(widget_t *, int x, int y);
void widget_tree_release(widget_t *, int x, int y);
void widget_tree_click(widget_t *, int x, int y);
void widget_tree_refresh_dimension(widget_t *);

#endif
