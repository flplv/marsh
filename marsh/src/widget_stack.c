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

#include "helper/fatal.h"
#include "helper/log.h"
#include "helper/checks.h"
#include "helper/stack.h"

#include "widget_stack.h"
#include "widget.h"
#include "dimension.h"

struct s_widget_stack
{
	my_log_t * log;
	my_stack_t *stack;
};

widget_stack_t * widget_stack_new(void)
{
	widget_stack_t * instance;

	instance = (widget_stack_t *)calloc(1, sizeof(widget_stack_t));
	MEMORY_ALLOC_CHECK(instance);

	instance->log = my_log_new("widgetStack", ERROR);
	instance->stack = stack_new(sizeof (widget_t *));

	return instance;
}

void widget_stack_delete(widget_stack_t * const instance)
{
	PTR_CHECK(instance, "widgetStack");

	stack_delete(instance->stack);
	my_log_delete(instance->log);
	free(instance);
}

void widget_stack_add(widget_stack_t * instance, widget_t *graphic_obj)
{
	PTR_CHECK(instance, "widgetStack");

	stack_add(instance->stack, (BUFFER_PTR_RDOLY)&graphic_obj);
}

widget_t * widget_stack_get_object(widget_stack_t * instance, uint32_t index)
{
	widget_t * return_object;

	PTR_CHECK_RETURN (instance, "widgetStack", NULL);

	if (index >= stack_size(instance->stack))
		return NULL;

	stack_get(instance->stack, (BUFFER_PTR)&return_object, index);

	return return_object;
}

widget_t * widget_stack_get_top_object(widget_stack_t * instance, dim_t x, dim_t y)
{
	uint32_t stack_iterator;
	widget_t * object;
	pos_t position;

	PTR_CHECK_RETURN (instance, "widgetStack", NULL);

	stack_iterator = stack_size(instance->stack);

	while(stack_iterator--)
	{
		stack_get(instance->stack, (BUFFER_PTR)&object, stack_iterator);
		position = dimension_get_position(widget_get_dimension(object));

		if (position.start.x <= x && position.start.y <= y)
			if (position.end.x >= x && position.end.y >= y)
				return object;
	}

	return NULL;
}

uint32_t widget_stack_get_object_index(widget_stack_t * instance, const widget_t * object)
{
	uint32_t stack_iterator;
	widget_t * stack_object;

	PTR_CHECK_RETURN (instance, "widgetStack", WIDGET_STACK_INVALID_INDEX);

	stack_iterator = stack_size(instance->stack);

	while(stack_iterator--)
	{
		stack_get(instance->stack, (BUFFER_PTR)&stack_object, stack_iterator);
		if (object == stack_object)
				return stack_iterator;
	}

	my_log(WARNING, __FILE__, __LINE__, "widget_stack_get_object_index: object was not found", instance->log);
	return WIDGET_STACK_INVALID_INDEX;
}

uint32_t widget_stack_size(widget_stack_t * instance)
{
	PTR_CHECK_RETURN (instance, "widgetStack", WIDGET_STACK_INVALID_INDEX);

	return stack_size(instance->stack);
}

widget_t *widget_stack_get_object_on_top(widget_stack_t* instance, dim_t x, dim_t y)
{
	uint32_t stack_iterator;
	widget_t *object;

	PTR_CHECK_RETURN (instance, "widgetStack", NULL);

	stack_iterator = stack_size(instance->stack);

	while(stack_iterator--)
	{
		stack_get(instance->stack, (BUFFER_PTR)&object, stack_iterator);
		if (dimension_contains(widget_get_dimension(object), x, y))
				return object;
	}

	return NULL;
}
