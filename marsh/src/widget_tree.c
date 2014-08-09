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

#include "widget_private.h"
#include "widget_tree.h"
#include "event.h"

/*
 * TODO Add the non recursive widget_tree traversal system, to override
 * recursiveness found on event propagation.
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

void widget_tree_delete(widget_t * obj)
{
	event_t * deletion_event;

	PTR_CHECK(obj, "widget_tree");

	deletion_event = event_new(event_code_delete, NULL, NULL);
	PTR_CHECK(deletion_event, "widget_tree");

	widget_event_emit(obj, deletion_event);
}

bool widget_tree_ancestors_visible(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget_tree", false);

	widget_t * parent = widget_parent(obj);

	while (parent)
	{
		if (!widget_visible(parent))
			return false;

		parent = widget_parent(parent);
	}

	return true;
}

void widget_tree_draw(widget_t * obj)
{
	event_t * draw_event;

	PTR_CHECK(obj, "widget_tree");

	if (!widget_tree_ancestors_visible(obj))
		return;

	draw_event = event_new(event_code_draw, NULL, NULL);
	PTR_CHECK(draw_event, "widget_tree");

	widget_event_emit(obj, draw_event);
}

void widget_tree_click(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget_tree");

	interaction_event = event_new(event_code_interaction_click, &data, NULL);
	PTR_CHECK(interaction_event, "widget_tree");

	widget_event_emit(obj, interaction_event);
}

void widget_tree_press(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget_tree");

	interaction_event = event_new(event_code_interaction_press, &data, NULL);
	PTR_CHECK(interaction_event, "widget_tree");

	widget_event_emit(obj, interaction_event);
}

void widget_tree_release(widget_t * obj, int x, int y)
{
	event_t * interaction_event;
	interaction_event_data_t data;

	data.interaction_point.x = x;
	data.interaction_point.y = y;

	PTR_CHECK(obj, "widget_tree");

	interaction_event = event_new(event_code_interaction_release, &data, NULL);
	PTR_CHECK(interaction_event, "widget_tree");

	widget_event_emit(obj, interaction_event);
}

/* TODO: This is bad pattern, event to refresh a dimension is bad,
 * the canvas dimension should be computed at the draw event handler.
 */
void widget_tree_refresh_dimension(widget_t * obj)
{
	PTR_CHECK(obj, "widget_tree");

	if (!widget_child(obj))
	{
		widget_refresh_dim(obj);
	}
	else
	{
		event_t * event;

		event = event_new(event_code_refresh_dim, NULL, NULL);
		PTR_CHECK(event, "widget_tree");

		widget_event_emit(obj, event);
	}
}
