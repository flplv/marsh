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

#include "signalslot.h"
#include "helper/stack.h"
#include "helper/checks.h"
#include "helper/log.h"

struct s_signal
{
	my_stack_t *slots_stack;
	MODULE_PRIVATE_DATA_DECLARATION;
};

struct s_slot
{
	slot_func func;
	slot_arg arg0;
	BOOL set;
	MODULE_PRIVATE_DATA_DECLARATION;
};

#define SIG_MOD_SIG (ADDRESS_TO_SIGNATURE_CAST)signal_create
#define SLO_MOD_SLO (ADDRESS_TO_SIGNATURE_CAST)slot_create

static void slot_call(slot_t *);

signal_t *signal_create()
{
	signal_t * obj = (signal_t *)calloc(1, sizeof(struct s_signal));
	INSTANCE_SET(obj, SIG_MOD_SIG);

	obj->slots_stack = stack_create(sizeof(slot_t *));

	return obj;
}

void signal_destroy(signal_t * obj)
{
	PTR_CHECK(obj, "signal");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal");

	stack_destroy(obj->slots_stack);

	INSTANCE_CLEAR(obj);

	free(obj);
}

void signal_emit(signal_t *obj)
{
	uint32_t n_of_slots;
	slot_t *slot_to_be_called = NULL;

	PTR_CHECK(obj, "signal");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal");


	n_of_slots = stack_size(obj->slots_stack);

	while(n_of_slots--)
	{
		stack_get(obj->slots_stack, (BUFFER_PTR)&slot_to_be_called, n_of_slots);
		slot_call(slot_to_be_called);
	}

}

static BOOL signal_is_connected(signal_t *obj, slot_t *slot)
{
	uint32_t n_of_slots;
	slot_t *slot_to_be_checked = NULL;

	PTR_CHECK_RETURN(obj, "signal", FALSE);
	INSTANCE_CHECK_RETURN(obj, SIG_MOD_SIG, "signal", FALSE);

	n_of_slots = stack_size(obj->slots_stack);

	while(n_of_slots--)
	{
		stack_get(obj->slots_stack, (BUFFER_PTR)&slot_to_be_checked, n_of_slots);
		if(slot == slot_to_be_checked)
			return TRUE;
	}

	return FALSE;
}

static void signal_connect(signal_t *obj, slot_t *slot)
{
	PTR_CHECK(obj, "signal");
	INSTANCE_CHECK(obj, SIG_MOD_SIG, "signal");

	if (signal_is_connected(obj, slot))
		return;

	stack_add(obj->slots_stack, (BUFFER_PTR)&slot);
}

slot_t *slot_create()
{
	slot_t * obj = (slot_t *)calloc(1, sizeof(struct s_slot));
	INSTANCE_SET(obj, SLO_MOD_SLO);

	return obj;
}

void slot_destroy(slot_t * obj)
{
	PTR_CHECK(obj, "slot");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot");


	INSTANCE_CLEAR(obj);
	free(obj);
}

void slot_set(slot_t *obj, slot_func function, slot_arg arg)
{
	PTR_CHECK(obj, "slot");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot");

	if (!function) {
		LOG_ERROR("slot", "bad slot function");
		return;
	}

	obj->func = function;
	obj->arg0 = arg;
	obj->set = TRUE;
}

void slot_connect(slot_t *obj, signal_t* signal)
{
	PTR_CHECK(obj, "slot");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot");

	signal_connect(signal, obj);
}

static void slot_call(slot_t *obj)
{
	PTR_CHECK(obj, "slot");
	INSTANCE_CHECK(obj, SLO_MOD_SLO, "slot");

	if (!obj->set) {
		LOG_ERROR("slot", "no slot function set");
		return;
	}

	if (!obj->func) {
		LOG_ERROR("slot", "bad slot function");
		return;
	}

	obj->func(obj->arg0);
}
