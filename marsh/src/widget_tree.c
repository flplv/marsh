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

#include "widget_private.h"
#include "widget_tree.h"

/*
 * TODO:
 * Add the non recursive widget_tree traversal system, to override
 * recursiveness found on widget deletion and event propagation.
 */

void widget_tree_register(widget_t * self, widget_t * parent)
{
	widget_t * last_brother;

	if (!parent)
		return;

	self->tree.parent = parent;

	if (!parent->tree.child)
	{
		parent->tree.child = self;
		return;
	}

	last_brother = widget_last_child(parent);

	if (last_brother)
	{
		last_brother->tree.right = self;
		self->tree.left = last_brother;
	}
}

void widget_tree_unregister(widget_t * self)
{
	if (self->tree.left)
		self->tree.left->tree.right = self->tree.right;

	if (self->tree.right)
		self->tree.right->tree.left = self->tree.left;

	if (self->tree.parent)
		if (self->tree.parent->tree.child == self)
			self->tree.parent->tree.child = self->tree.right;
}

widget_t * widget_root(widget_t * child)
{
	if (!child)
		return NULL;

	while(child->tree.parent)
		child = child->tree.parent;

	return child;
}
widget_t * widget_parent(widget_t * child)
{
	if (!child)
		return NULL;

	return child->tree.parent;
}

widget_t * widget_child(widget_t * parent)
{
	if (!parent)
		return NULL;

	return parent->tree.child;
}

widget_t * widget_left_sibling(widget_t * sibling)
{
	if (!sibling)
		return NULL;

	return sibling->tree.left;
}

widget_t * widget_right_sibling(widget_t * sibling)
{
	if (!sibling)
		return NULL;

	return sibling->tree.right;
}

widget_t * widget_last(widget_t * sibling)
{
	widget_t * widget_iterator = sibling;
	widget_t * last_widget = sibling;

	while (widget_iterator)
	{
		last_widget = widget_iterator;
		widget_iterator = widget_iterator->tree.right;
	}

	return last_widget;
}

widget_t * widget_last_child(widget_t * parent)
{
	return widget_last(widget_child(parent));
}

size_t widget_num_of_children(const widget_t * parent)
{
	int n = 0;
	widget_t * child = parent->tree.child;

	if (!parent)
		return 0;

	while (child)
	{
		n++;
		child = child->tree.right;
	}

	return n;
}
