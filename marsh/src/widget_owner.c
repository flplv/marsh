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



#include "widget.h"
#include "widget_private.h"
#include "widget_owner.h"

widget_owner_t * widget_owner_create(void *owner, void (*draw_func)(void *), void (*destroy_func)(void *))
{
	widget_owner_t * obj = (widget_owner_t *)malloc(sizeof(struct s_widget_owner));
	MEMORY_ALLOC_CHECK(obj);
	INSTANCE_SET(obj, (ADDRESS_TO_SIGNATURE_CAST)&widget_owner_create);

	obj->owner_instance = owner;
	obj->draw = draw_func;
	obj->destroy = destroy_func;

	return obj;
}

void widget_owner_destroy(widget_owner_t * const obj)
{
	PTR_CHECK(obj, "widgetOwnerCallback");
	INSTANCE_CHECK(obj, (ADDRESS_TO_SIGNATURE_CAST)&widget_owner_create, "widgetOwnerCallback");
	INSTANCE_CLEAR(obj);
	free(obj);
}

