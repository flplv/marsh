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

#include <stdlib.h>

#include "helper/log.h"
#include "helper/checks.h"

#include "widget_private.h"
#include "widget.h"
#include "interact.h"
#include "interaction_engine.h"

widget_t * widget_new(widget_interface_t * interface)
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK(obj);

	dimension_clear(&obj->dim);
	obj->interface = interface;

	obj->interaction = interaction_engine_new(&obj->dim);

	return obj;
}

void widget_delete(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	interaction_engine_delete(obj->interaction);
	free(obj);
}

void widget_delete_interface(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->interface->destroy && obj->interface->owner_instance)
	{
		obj->interface->destroy(obj->interface->owner_instance);
	}
}

void widget_draw(const widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->interface->draw(obj->interface->owner_instance);
}

dimension_t *widget_get_dimension(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);

	return &obj->dim;
}

interaction_engine_t* widget_get_interaction_engine(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);

	return obj->interaction;
}
