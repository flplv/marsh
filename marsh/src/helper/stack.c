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

#include "helper_types.h"
#include "checks.h"
#include "stack.h"
#include <string.h>

struct s_stack
{
	uint32_t item_size;
	uint32_t used_slots;
	uint32_t buffer_total_slots;
	uint8_t *buffer;
};


my_stack_t* stack_new(uint32_t item_size)
{
	my_stack_t *obj;

	if (item_size > 256) {
		global_my_log(ERROR, __FILE__, __LINE__, "item_size > 256", "stack");
		return NULL;
	}

	obj = (my_stack_t*)calloc(1, sizeof(struct s_stack));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	obj->item_size = item_size;

	obj->buffer = (uint8_t *)calloc(2, obj->item_size);
	MEMORY_ALLOC_CHECK_RETURN(obj->buffer, NULL);
	obj->buffer_total_slots = 2;

	obj->used_slots = 0;

	return obj;
}

void stack_delete(my_stack_t *obj)
{
	PTR_CHECK(obj, "stack");

	free(obj->buffer);

	free(obj);
}

uint32_t stack_add(my_stack_t *obj, BUFFER_PTR_RDOLY item)
{
	PTR_CHECK_RETURN (obj, "stack", -1);

	if (obj->used_slots == obj->buffer_total_slots)
	{
		obj->buffer = (uint8_t *)realloc(obj->buffer, obj->buffer_total_slots * 2 * obj->item_size);
		MEMORY_ALLOC_CHECK_RETURN(obj->buffer, -1);
		obj->buffer_total_slots *= 2;
	}

	memcpy( obj->buffer + (obj->used_slots * obj->item_size),
			item,
			obj->item_size );
	obj->used_slots++;

	return obj->used_slots-1;
}

uint32_t stack_size(my_stack_t* obj)
{
	return obj->used_slots;
}

void stack_get(my_stack_t *obj, BUFFER_PTR target, uint32_t pos)
{
	memcpy(target, obj->buffer + obj->item_size*pos, obj->item_size);
}
