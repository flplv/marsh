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
#include "interact.h"
#include "interaction_engine.h"
#include "dimension.h"
#include "signalslot.h"
#include "signalslot2.h"

static interaction_engine_t *prioritary_release = NULL;

struct s_interaction_engine {
	signal_t *click_sig;
	signal2_t *press_sig;
	signal2_t *release_sig;
	dimension_t *dimension;

	bool is_interactable;
	bool pressing;
};


interaction_engine_t* interaction_engine_new(dimension_t * owner_dimension)
{
	interaction_engine_t *obj = (interaction_engine_t *)calloc(1, sizeof (struct s_interaction_engine));
	MEMORY_ALLOC_CHECK(obj);

	obj->click_sig = signal_new();
	obj->press_sig = signal2_new();
	obj->release_sig = signal2_new();
	obj->dimension = owner_dimension;
	obj->is_interactable = true;

	return obj;
}

void interaction_engine_delete(interaction_engine_t* obj)
{
	PTR_CHECK(obj, "interaction_interface");
	signal_delete(obj->click_sig);
	signal2_delete(obj->press_sig);
	signal2_delete(obj->release_sig);
	free(obj);
}

void interaction_engine_click(interaction_engine_t *obj)
{
	PTR_CHECK(obj, "interaction_interface");

	signal_emit(obj->click_sig);
}

static void set_prio_release(interaction_engine_t* obj)
{
	if (prioritary_release)
	{
		LOG_ERROR("interaction_interface", "prioritary_release colision");
	}

	prioritary_release = obj;
}
void interaction_engine_emmit_pending_release(size_t x, size_t y)
{
	if (!prioritary_release)
		return;

	signal2_emit(prioritary_release->release_sig, x, y);
	prioritary_release = NULL;
}

void interaction_engine_press(interaction_engine_t *obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "interaction_interface");

	if (obj->is_interactable)
	{
		set_prio_release(obj);
		obj->pressing = true;
	}

	signal2_emit(obj->press_sig, x, y);
}

void interaction_engine_release(interaction_engine_t *obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "interaction_interface");

	if (obj->is_interactable)
	{
		if (obj->pressing)
		{
			if (dimension_contains(obj->dimension, x, y))
				interaction_engine_click(obj);
		}
		obj->pressing = false;
	}


	signal2_emit(obj->release_sig, x, y);
}

signal2_t * interaction_engine_get_press_signal(interaction_engine_t *obj)
{
	PTR_CHECK_RETURN(obj, "interaction_interface", NULL);

	return obj->press_sig;
}

signal_t * interaction_engine_get_click_signal(interaction_engine_t *obj)
{
	PTR_CHECK_RETURN(obj, "interaction_interface", NULL);

	return obj->click_sig;
}

signal2_t * interaction_engine_get_release_signal(interaction_engine_t *obj)
{
	PTR_CHECK_RETURN(obj, "interaction_interface", NULL);

	return obj->release_sig;
}

void interaction_engine_disable_interaction_logic(interaction_engine_t* obj)
{
	PTR_CHECK(obj, "interaction_interface");

	obj->is_interactable = false;
}
