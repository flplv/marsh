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
#include "button_engine.h"
#include "widget.h"
#include "widget_interface.h"
#include "dimension.h"
#include "signalslot2.h"
#include "interact.h"
#include "interaction_engine.h"

struct s_button_engine
{
	widget_t *glyph;
	widget_interface_t *self_reference;

	slot2_t *press;
	slot2_t *release;

	widget_t **current_state;

	widget_t *state_normal;
	widget_t *state_press;

	widget_t *state_on;
	widget_t *state_off;
	widget_t *state_on_to_off;
	widget_t *state_off_to_on;
};

static void draw(button_engine_t * obj)
{
	PTR_CHECK(obj, "buttonEngine");

	if (!obj->current_state) {
		global_my_log(ERROR, __FILE__, __LINE__, "No Logic selected.", "buttonEngine");
		return;
	}

	if ((*obj->current_state) == NULL) {
		return;
	}

	widget_draw(*obj->current_state);
}

static bool valid_interaction(button_engine_t * obj, size_t x, size_t y)
{
	if (dimension_contains(
			widget_get_dimension(*obj->current_state), x, y))
		return true;

	return false;
}

static void action_press(button_engine_t * obj, size_t x, size_t y)
{
	if (!valid_interaction(obj, x, y))
		return;

	obj->current_state = &obj->state_press;
	draw(obj);
}
static void action_release(button_engine_t * obj)
{
	if (obj->current_state != &obj->state_press)
		return;

	obj->current_state = &obj->state_normal;
	draw(obj);
}

static void toggle_press(button_engine_t * obj, size_t x, size_t y)
{
	if (!valid_interaction(obj, x, y))
		return;

	if (obj->current_state == &obj->state_off)
		obj->current_state = &obj->state_off_to_on;
	else
		obj->current_state = &obj->state_on_to_off;

	draw(obj);
}
static void toggle_release(button_engine_t * obj, size_t x, size_t y)
{
	if (obj->current_state != &obj->state_on_to_off)
		if (obj->current_state != &obj->state_off_to_on)
			return;

	if (valid_interaction(obj, x, y))
	{
		if (obj->current_state == &obj->state_on_to_off)
			obj->current_state = &obj->state_off;
		else
			obj->current_state = &obj->state_on;
	}
	else
	{
		if (obj->current_state == &obj->state_on_to_off)
			obj->current_state = &obj->state_on;
		else
			obj->current_state = &obj->state_off;
	}

	draw(obj);
}

static void bad_config_message(void)
{
	global_my_log(ERROR, __FILE__, __LINE__, "No Logic selected.", "buttonEngine");
}

button_engine_t* button_engine_new()
{
	button_engine_t * obj = (button_engine_t *) calloc(1, sizeof(struct s_button_engine));
	MEMORY_ALLOC_CHECK(obj);

	obj->self_reference = widget_interface_new(obj, (void(*)(void*))draw, (void(*)(void*))button_engine_delete);
	obj->glyph = widget_new(obj->self_reference);

	obj->press = slot2_new();
	obj->release = slot2_new();

	slot2_set(obj->press, (slot2_func)bad_config_message, obj);
	slot2_set(obj->release, (slot2_func)bad_config_message, obj);

	signal2_t *if_sig;

	if_sig = interaction_engine_get_press_signal(widget_get_interaction_engine(obj->glyph));
	slot2_connect(obj->press, if_sig);
	if_sig = interaction_engine_get_release_signal(widget_get_interaction_engine(obj->glyph));
	slot2_connect(obj->release, if_sig);

	return obj;
}

void button_engine_delete(button_engine_t *obj)
{
	PTR_CHECK(obj, "buttonEngine");

	widget_delete(obj->glyph);
	widget_interface_delete(obj->self_reference);
	slot2_delete(obj->press);
	slot2_delete(obj->release);

	free(obj);
}

widget_t *button_engine_get_widget(button_engine_t *obj)
{
	PTR_CHECK_RETURN (obj, "buttonEngine", NULL);

	return obj->glyph;
}

static void set_action_slots(button_engine_t* obj)
{
	slot2_set(obj->press, (slot2_func) action_press, obj);
	slot2_set(obj->release, (slot2_func) action_release, obj);
}

static void set_toggle_slots(button_engine_t* obj)
{
	slot2_set(obj->press, (slot2_func) toggle_press, obj);
	slot2_set(obj->release, (slot2_func) toggle_release, obj);
}

void button_engine_select_toggle_logic(button_engine_t *obj)
{
	PTR_CHECK(obj, "buttonEngine");

	set_toggle_slots(obj);
	obj->current_state = &obj->state_off;
}

void button_engine_select_action_logic(button_engine_t *obj)
{
	PTR_CHECK(obj, "buttonEngine");

	set_action_slots(obj);
	obj->current_state = &obj->state_normal;
}

static void dimension_update(button_engine_t *obj)
{
	dimension_clear(
			widget_get_dimension(obj->glyph));

	if (obj->state_normal)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_normal));

	if (obj->state_off)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_off));

	if (obj->state_off_to_on)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_off_to_on));

	if (obj->state_on)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_on));

	if (obj->state_on_to_off)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_on_to_off));

	if (obj->state_press)
		dimension_merge(
				widget_get_dimension(obj->glyph),
				widget_get_dimension(obj->state_press));
}

void button_engine_set_action_normal_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_normal = gobj;
	dimension_update(obj);
}

void button_engine_set_action_onpress_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_press = gobj;
	dimension_update(obj);
}

void button_engine_set_toggle_on_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_on = gobj;
	dimension_update(obj);
}

void button_engine_set_toggle_off_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_off = gobj;
	dimension_update(obj);
}

void button_engine_set_toggle_on_to_off_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_on_to_off = gobj;
	dimension_update(obj);
}

void button_engine_set_toggle_off_to_on_state(button_engine_t *obj, widget_t *gobj)
{
	PTR_CHECK(obj, "buttonEngine");

	obj->state_off_to_on = gobj;
	dimension_update(obj);
}
