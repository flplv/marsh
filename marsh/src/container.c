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

#include "helper/checks.h"
#include "helper/log.h"

#include "container.h"
#include "dimension.h"
#include "widget.h"
#include "widget_interface.h"
#include "widget_stack.h"
#include "draw_manager.h"
#include "signalslot2.h"
#include "interact.h"
#include "interaction_engine.h"

struct s_container_instance
{
	my_log_t * log;
	widget_t *glyph;
	widget_interface_t *self_reference;
	widget_stack_t *stack;
	draw_manager_t *dm;
	slot2_t *press;
	slot2_t *release;
	slot_t *update_dim;
	MODULE_PRIVATE_DATA_DECLARATION;
};

#define SIGNATURE_CONTAINER (ADDRESS_TO_SIGNATURE_CAST)&container_create

static void abstract_draw(void *obj)
{
	container_draw((container_t *)obj);
}

static void abstract_destroy(void * obj)
{
	container_destroy((container_t *)obj);
}

static void press(container_t *obj, size_t x, size_t y)
{
	widget_t *obj_to_click;

	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	obj_to_click = widget_stack_get_object_on_top(obj->stack, x, y);
	if (obj_to_click)
		interaction_engine_press(widget_get_interaction_engine(obj_to_click), x, y);
}

static void release(container_t *obj, size_t x, size_t y)
{
	widget_t *obj_to_click;

	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	obj_to_click = widget_stack_get_object_on_top(obj->stack, x, y);
	if (obj_to_click)
		interaction_engine_release(widget_get_interaction_engine(obj_to_click), x, y);
}

container_t* container_create()
{
	container_t * obj = (container_t *) malloc(sizeof(struct s_container_instance));
	MEMORY_ALLOC_CHECK(obj);

	INSTANCE_SET(obj, SIGNATURE_CONTAINER);
	obj->log = my_log_create("Container", MESSAGE);
	obj->self_reference = widget_interface_create(obj, abstract_draw, abstract_destroy);
	obj->glyph = widget_create(obj->self_reference);
	obj->stack = widget_stack_create();
	obj->dm = draw_manager_create();
	draw_manager_assossiate_drawing_stack(obj->dm, obj->stack);

	obj->press = slot2_create();
	obj->release = slot2_create();

	slot2_set(obj->press, (slot2_func)press, obj);
	slot2_set(obj->release, (slot2_func)release, obj);

	signal2_t *if_sig;
	if_sig = interaction_engine_get_press_signal(widget_get_interaction_engine(obj->glyph));
	slot2_connect(obj->press, if_sig);

	if_sig = interaction_engine_get_release_signal(widget_get_interaction_engine(obj->glyph));
	slot2_connect(obj->release, if_sig);

	interaction_engine_disable_interaction_logic(widget_get_interaction_engine(obj->glyph));

	return obj;
}

static void destroy_all_objects_in_stack(container_t* obj)
{
	widget_t *object_to_be_deleted;
	uint32_t size;

	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	size = widget_stack_size(obj->stack);

	while(size--)
	{
		object_to_be_deleted = widget_stack_get_object(obj->stack, size);
		widget_destroy_owner(object_to_be_deleted);
	}
}

static void update_dimensions(container_t *obj)
{
	uint32_t size;
	widget_t *obj_owned;

	size = widget_stack_size(obj->stack);

	dimension_clear(widget_get_dimension(obj->glyph));

	while(size--)
	{
		obj_owned = widget_stack_get_object(obj->stack, size);
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj_owned));
	}
}


void container_destroy(container_t* const obj)
{
	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	destroy_all_objects_in_stack(obj);

	my_log_destroy(obj->log);
	widget_destroy(obj->glyph);
	widget_interface_destroy(obj->self_reference);
	draw_manager_destroy(obj->dm);
	widget_stack_destroy(obj->stack);
	slot2_destroy(obj->press);
	slot2_destroy(obj->release);

	INSTANCE_CLEAR(obj);
	free(obj);
}

void container_add(container_t* obj, widget_t* graphic_to_be_added)
{
	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	widget_stack_add(obj->stack, graphic_to_be_added);
	update_dimensions(obj);
}

widget_t *container_get_widget(container_t * const obj)
{
	PTR_CHECK_RETURN (obj, "container", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_CONTAINER, "container", NULL);

	return obj->glyph;
}

void container_draw(container_t* obj)
{
	PTR_CHECK(obj, "container");
	INSTANCE_CHECK(obj, SIGNATURE_CONTAINER, "container");

	draw_manager_draw_all(obj->dm);
}
