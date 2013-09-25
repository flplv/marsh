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

#include "signalslot2.h"
#include "helper/stack.h"
#include "helper/checks.h"
#include "helper/log.h"

struct s_signal2
{
	my_stack_t *slot2s_stack;
	MODULE_PRIVATE_DATA_DECLARATION;
};

struct s_slot2
{
	slot2_func func;
	slot_arg arg0;
	BOOL set;
	MODULE_PRIVATE_DATA_DECLARATION;
};

#define SIG_MOD_SIG (ADDRESS_TO_SIGNATURE_CAST)signal2_create
#define SLO_MOD_SLO (ADDRESS_TO_SIGNATURE_CAST)slot2_create

static void slot2_call(slot2_t *, size_t, size_t);

signal2_t *signal2_create()
{
	signal2_t * obj = (signal2_t *)calloc(1, sizeof(struct s_signal2));
	INSTANCE_SET(obj, SIG_MOD_SIG);

	obj->slot2s_stack = stack_create(sizeof(slot2_t *));

	return obj;
}

void signal2_destroy(signal2_t * obj)
{
	PTR_CHECK(obj, "signal2");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal2");

	stack_destroy(obj->slot2s_stack);

	INSTANCE_CLEAR(obj);

	free(obj);
}

void signal2_emit(signal2_t *obj, size_t x, size_t y)
{
	uint32_t n_of_slot2s;
	slot2_t *slot2_to_be_called = NULL;

	PTR_CHECK(obj, "signal2");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal2");


	n_of_slot2s = stack_size(obj->slot2s_stack);

	while(n_of_slot2s--)
	{
		stack_get(obj->slot2s_stack, (BUFFER_PTR)&slot2_to_be_called, n_of_slot2s);
		slot2_call(slot2_to_be_called, x, y);
	}

}

static BOOL signal2_is_connected(signal2_t *obj, slot2_t *slot2)
{
	uint32_t n_of_slot2s;
	slot2_t *slot2_to_be_checked = NULL;

	PTR_CHECK_RETURN(obj, "signal2", FALSE);
	INSTANCE_CHECK_RETURN(obj, SIG_MOD_SIG, "signal2", FALSE);

	n_of_slot2s = stack_size(obj->slot2s_stack);

	while(n_of_slot2s--)
	{
		stack_get(obj->slot2s_stack, (BUFFER_PTR)&slot2_to_be_checked, n_of_slot2s);
		if(slot2 == slot2_to_be_checked)
			return TRUE;
	}

	return FALSE;
}

static void signal2_connect(signal2_t *obj, slot2_t *slot2)
{
	PTR_CHECK(obj, "signal2");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal2");

	if (signal2_is_connected(obj, slot2))
		return;

	stack_add(obj->slot2s_stack, (BUFFER_PTR)&slot2);
}

slot2_t *slot2_create()
{
	slot2_t * obj = (slot2_t *)calloc(1, sizeof(struct s_slot2));
	INSTANCE_SET(obj, SLO_MOD_SLO);

	return obj;
}

void slot2_destroy(slot2_t * obj)
{
	PTR_CHECK(obj, "slot2");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot2");


	INSTANCE_CLEAR(obj);
	free(obj);
}

void slot2_set(slot2_t *obj, slot2_func function, slot_arg arg)
{
	PTR_CHECK(obj, "slot2");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot2");

	if (!function) {
		LOG_ERROR("slot2", "bad slot2 function");
		return;
	}

	obj->func = function;
	obj->arg0 = arg;
	obj->set = TRUE;
}

void slot2_connect(slot2_t *obj, signal2_t* signal2)
{
	PTR_CHECK(obj, "slot2");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot2");

	signal2_connect(signal2, obj);
}

static void slot2_call(slot2_t *obj, size_t x, size_t y)
{
	PTR_CHECK(obj, "slot2");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot2");

	if (!obj->set) {
		LOG_ERROR("slot2", "no slot2 function set");
		return;
	}

	if (!obj->func) {
		LOG_ERROR("slot2", "bad slot2 function");
		return;
	}

	obj->func(obj->arg0, x, y);
}
