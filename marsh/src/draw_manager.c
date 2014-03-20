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


#include "draw_manager.h"
#include "widget_stack.h"


struct s_draw_manager_instance
{
	MODULE_PRIVATE_DATA_DECLARATION;
	my_log_t *log;
	widget_stack_t *assossiated_stack;
};

#define SIGNATURE_DRAW_MANAGER (ADDRESS_TO_SIGNATURE_CAST)&draw_manager_create

static bool check_stack(draw_manager_t * instance)
{
	if (!instance->assossiated_stack) {
		global_my_log(ERROR, __FILE__, __LINE__, "draw_manager_assossiateDrawingStack not called.", "drawManager");
		return false;
	}

	return true;
}

draw_manager_t * draw_manager_create(void)
{
	draw_manager_t * instance = (draw_manager_t *)malloc(sizeof(struct s_draw_manager_instance));

	if (!instance) {
		fatal("Malloc failed.");
		return NULL ;
	}

	INSTANCE_SET(instance, SIGNATURE_DRAW_MANAGER);

	instance->assossiated_stack = NULL;

	instance->log = my_log_create("drawManager", ERROR);


	return instance;
}

void draw_manager_destroy(draw_manager_t * const instance)
{
	PTR_CHECK(instance, "drawManager");
	INSTANCE_CHECK(instance, SIGNATURE_DRAW_MANAGER, "drawManager");

	my_log_destroy(instance->log);

	if (check_stack(instance))
			(instance)->assossiated_stack = NULL;

	INSTANCE_CLEAR(instance);
	if (instance)
		free(instance);
}

void draw_manager_assossiate_drawing_stack(draw_manager_t * instance, widget_stack_t * stack_instance_ptr)
{
	PTR_CHECK(instance, "drawManager");
	INSTANCE_CHECK(instance, SIGNATURE_DRAW_MANAGER, "drawManager");

	instance->assossiated_stack = stack_instance_ptr;
}

static void draw_objects(draw_manager_t* instance, int8_t start_offset, int8_t end_offset)
{
	while (start_offset <= end_offset)
	{
		widget_draw(widget_stack_get_object(instance->assossiated_stack, start_offset));
		start_offset++;
	}
}

void draw_manager_draw(draw_manager_t * instance, widget_t * start_object)
{
	int8_t start_offset, end_offset;

	PTR_CHECK(instance, "drawManager");
	INSTANCE_CHECK(instance, SIGNATURE_DRAW_MANAGER, "drawManager");

	if (!check_stack(instance))
			return;

	start_offset = widget_stack_get_object_index(instance->assossiated_stack, start_object);
	if (start_offset == WIDGET_STACK_INVALID_INDEX)
		return;

	end_offset = widget_stack_size(instance->assossiated_stack) - 1;

	draw_objects(instance, start_offset, end_offset);
}

void draw_manager_draw_all(draw_manager_t * instance)
{
	int8_t end_offset;

	PTR_CHECK(instance, "drawManager");
	INSTANCE_CHECK(instance, SIGNATURE_DRAW_MANAGER, "drawManager");

	if (!check_stack(instance))
			return;

	end_offset = widget_stack_size(instance->assossiated_stack) - 1;

	draw_objects(instance, 0, end_offset);
}
