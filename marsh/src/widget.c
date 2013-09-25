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

#define SIGNATURE_WIDGET (ADDRESS_TO_SIGNATURE_CAST)&widget_create

widget_t * widget_create(widget_owner_t * owner)
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK(obj);

	INSTANCE_SET(obj, SIGNATURE_WIDGET);

	dimension_clear(&obj->dim);
	obj->owner = owner;

	obj->interaction = interaction_engine_create(&obj->dim);

	return obj;
}

void widget_destroy(widget_t * const obj)
{
	PTR_CHECK(obj, "widget");
	INSTANCE_CHECK(obj, SIGNATURE_WIDGET, "widget");
	interaction_engine_destroy(obj->interaction);
	INSTANCE_CLEAR(obj);
	free(obj);
}

void widget_destroy_owner(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	INSTANCE_CHECK(obj, SIGNATURE_WIDGET, "widget");

	if (obj->owner->destroy && obj->owner->owner_instance)
	{
		obj->owner->destroy(obj->owner->owner_instance);
	}
}

void widget_draw(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	INSTANCE_CHECK(obj, SIGNATURE_WIDGET, "widget");

	obj->owner->draw(obj->owner->owner_instance);
}

dimension_t *widget_get_dimension(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_WIDGET, "widget", NULL);

	return &obj->dim;
}

pos_t widget_get_position(widget_t * obj)
{
	pos_t bad_ret;
	pos_clear(&bad_ret);

	PTR_CHECK_RETURN (obj, "widget", bad_ret);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_WIDGET, "widget", bad_ret);

	if (!dimension_good(&obj->dim))
		return bad_ret;

	return obj->dim.pos;
}

area_t widget_get_size(widget_t * obj)
{
	area_t bad_ret;
	area_clear(&bad_ret);

	PTR_CHECK_RETURN (obj, "widget", bad_ret);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_WIDGET, "widget", bad_ret);

	if (!dimension_good(&obj->dim))
		return bad_ret;

	return obj->dim.size;
}

interaction_engine_t* widget_get_interaction_engine(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);
	INSTANCE_CHECK_RETURN(obj, SIGNATURE_WIDGET, "widget", NULL);

	return obj->interaction;
}
