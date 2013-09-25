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

#include <iostream>
#include "marshmallowthread.h"

extern "C"
{
#include <pthread.h>
}

#include "framebuffer.h"
#include "widget.h"
#include "text.h"
#include "rectangle.h"
#include "icon.h"
#include "image.h"
#include "container.h"
#include "button_engine.h"
#include "signalslot.h"
#include "signalslot2.h"
#include "interaction_engine.h"
#include "interact.h"

struct marshmallow_thread_private
{
	bool thread_running;
	pthread_t thread_id;
	pthread_mutex_t thread_mutex;

	pthread_cond_t thread_cond;

	struct sInteraction
	{
		enum { PRESS, RELEASE } type;
		bool set;
		int x;
		int y;
	} interaction;
};

marshmallow_thread::marshmallow_thread()
{
	p = new struct marshmallow_thread_private;
	p->thread_running = true;

    pthread_mutex_init(&p->thread_mutex, NULL);
    pthread_cond_init(&p->thread_cond, NULL);
    pthread_create(&p->thread_id, NULL, (void*(*)(void*))marshmallow_thread::thread_handler, this);
}

marshmallow_thread::~marshmallow_thread()
{
	void * pVoid;
	p->thread_running = false;
	pthread_join(p->thread_id, &pVoid);

	delete p;
}

void marshmallow_thread::press(int x, int y)
{
	pthread_mutex_lock(&p->thread_mutex);

	p->interaction.x = x;
	p->interaction.y = y;
	p->interaction.type = marshmallow_thread_private::sInteraction::PRESS;
	p->interaction.set = true;

	pthread_cond_signal(&p->thread_cond);
	pthread_mutex_unlock(&p->thread_mutex);
}

void marshmallow_thread::release(int x, int y)
{
	pthread_mutex_lock(&p->thread_mutex);

	p->interaction.x = x;
	p->interaction.y = y;
	p->interaction.type = marshmallow_thread_private::sInteraction::RELEASE;
	p->interaction.set = true;

	pthread_cond_signal(&p->thread_cond);
	pthread_mutex_unlock(&p->thread_mutex);
}

void *marshmallow_thread::thread_handler(marshmallow_thread* self)
{
	framebuffer_create();

	rectangle_t *bg = rectangle_create();
	rectangle_set_fill_color_html(bg, "#004000");
	rectangle_set_position(bg, 5, 5);
	rectangle_set_size(bg, 790, 470);

	rectangle_t *obj1 = rectangle_create();
	rectangle_t *obj2 = rectangle_create();
	rectangle_t *obj3 = rectangle_create();

	rectangle_set_position(obj1, 0, 0);
	rectangle_set_size(obj1, 100, 100);
	rectangle_set_fill_color_html(obj1, "#800000");

	rectangle_set_position(obj2, 50, 50);
	rectangle_set_size(obj2, 100, 100);
	rectangle_set_fill_color_html(obj2, "#008000");

	rectangle_set_position(obj3, 100, 100);
	rectangle_set_size(obj3, 100, 100);
	rectangle_set_fill_color_html(obj3, "#000080");

	container_t *cont = container_create();

	container_add(cont, rectangle_get_widget(obj1));
	container_add(cont, rectangle_get_widget(obj2));
	container_add(cont, rectangle_get_widget(obj3));


	rectangle_t *rounded, *square_border, *rounded_border, *frame, *rounded_frame;
	rounded = rectangle_create();
	square_border = rectangle_create();
	rounded_border = rectangle_create();
	frame = rectangle_create();
	rounded_frame = rectangle_create();

	rectangle_set_position(rounded, 210, 50);
	rectangle_set_size(rounded, 100, 100);
	rectangle_set_fill_color_html(rounded, "#808080");
	rectangle_set_rounded_corner_radius(rounded, 20);

	rectangle_set_position(square_border, 320, 50);
	rectangle_set_size(square_border, 100, 100);
	rectangle_set_fill_color_html(square_border, "#808080");
	rectangle_set_border_color_html(square_border, "#c0c0c0");

	rectangle_set_position(rounded_border, 430, 50);
	rectangle_set_size(rounded_border, 100, 100);
	rectangle_set_fill_color_html(rounded_border, "#808080");
	rectangle_set_border_color_html(rounded_border, "#c0c0c0");
	rectangle_set_rounded_corner_radius(rounded_border, 13);

	rectangle_set_position(frame, 540, 50);
	rectangle_set_size(frame, 100, 100);
	rectangle_set_border_color_html(frame, "#c0c0c0");
	rectangle_set_border_tickness(frame, 5);

	rectangle_set_position(rounded_frame, 650, 50);
	rectangle_set_size(rounded_frame, 100, 100);
	rectangle_set_border_color_html(rounded_frame, "#c0c0c0");
	rectangle_set_rounded_corner_radius(rounded_frame, 13);
	rectangle_set_border_tickness(rounded_frame, 5);

	container_add(cont, rectangle_get_widget(rounded));
	container_add(cont, rectangle_get_widget(square_border));
	container_add(cont, rectangle_get_widget(rounded_border));
	container_add(cont, rectangle_get_widget(frame));
	container_add(cont, rectangle_get_widget(rounded_frame));


	rectangle_t *normal, *onpress;

	normal = rectangle_create();
	onpress = rectangle_create();

	rectangle_set_fill_color_html(normal, "#800000");
	rectangle_set_fill_color_html(onpress, "#FF0000");
	rectangle_set_size(normal, 100, 100);
	rectangle_set_size(onpress, 100, 100);
	rectangle_set_position(normal, 400, 200);
	rectangle_set_position(onpress, 400, 200);

	button_engine_t *button1;
	button_engine_t *button2;
	button1 = button_engine_create();
	button2 = button_engine_create();

	button_engine_set_action_normal_state(button1, rectangle_get_widget(normal));
	button_engine_set_action_onpress_state(button1, rectangle_get_widget(onpress));
	button_engine_select_action_logic(button1);

	rectangle_t *off, *on, *on_to_off, *off_to_on;

	off = rectangle_create();
	on = rectangle_create();
	on_to_off = rectangle_create();
	off_to_on = rectangle_create();

	rectangle_set_fill_color_html(off, "#800000");
	rectangle_set_fill_color_html(off_to_on, "#FF0000");
	rectangle_set_fill_color_html(on, "#008000");
	rectangle_set_fill_color_html(on_to_off, "#00FF00");

	rectangle_set_size(off, 100, 100);
	rectangle_set_size(off_to_on, 100, 100);
	rectangle_set_position(off, 600, 200);
	rectangle_set_position(off_to_on, 600, 200);

	rectangle_set_size(on, 100, 100);
	rectangle_set_size(on_to_off, 100, 100);
	rectangle_set_position(on, 600, 200);
	rectangle_set_position(on_to_off, 600, 200);

	button_engine_set_toggle_on_state(button2, rectangle_get_widget(on));
	button_engine_set_toggle_off_state(button2, rectangle_get_widget(off));
	button_engine_set_toggle_on_to_off_state(button2, rectangle_get_widget(on_to_off));
	button_engine_set_toggle_off_to_on_state(button2, rectangle_get_widget(off_to_on));

	button_engine_select_toggle_logic(button2);

	text_t *txt1 = text_create();
	text_set_color_html(txt1, "#C0C0C0");
	text_set_font(txt1, ubuntu_monospace_16);
	text_set_reference_position(txt1, 10, 400);
	my_string_set(text_get_string(txt1), "Hello World!\nHow beautiful is it?");

	text_t *txt2 = text_create();
	text_set_color_html(txt2, "#D0D0D0");
	text_set_font(txt2, ubuntu_monospace_16);
	text_set_justification(txt2, TEXT_CENTER_JUST);
	text_set_reference_position(txt2, 400, 400);
	my_string_set(text_get_string(txt2), "Hello World!\nHow beautiful is it?");

	text_t *txt3 = text_create();
	text_set_color_html(txt3, "#E0E0E0");
	text_set_font(txt3, ubuntu_monospace_16);
	text_set_justification(txt3, TEXT_RIGHT_JUST);
	text_set_reference_position(txt3, 790, 400);
	my_string_set(text_get_string(txt3), "Hello World!\nHow beautiful is it?");

	rectangle_t *icon_bg1 = rectangle_create();
	rectangle_set_position(icon_bg1, 0, 300);
	rectangle_set_size(icon_bg1, 50, 50);
	rectangle_set_fill_color_html(icon_bg1, "#404040");
	rectangle_set_border_color_html(icon_bg1, "#FF4040");
	rectangle_set_border_tickness(icon_bg1, 5);

	icon_t *wifi = icon_create();
	icon_set_color_html(wifi, "#00FF00");
	icon_set_position(wifi, 10, 310);
	icon_set_bitmap(wifi, wifi_icon_bitmap);

	icon_t *wifi2 = icon_create();
	icon_set_color_html(wifi2, "#00FFFF");
	icon_set_position(wifi2, 20, 320);
	icon_set_bitmap(wifi2, wifi_icon_bitmap);

	icon_t *wifi3 = icon_create();
	icon_set_color_html(wifi3, "#FFFFFF");
	icon_set_position(wifi3, 30, 330);
	icon_set_bitmap(wifi3, wifi_icon_bitmap);

	image_t *lena = image_create();
	image_set_bitmap(lena, lena_bitmap);
	image_set_position(lena, 120, 205);

	slot_t *lena_slot = slot_create();
	slot_set(lena_slot, (slot_func)lena_onclick, NULL);

	slot_connect(lena_slot, interaction_engine_get_click_signal(
								widget_get_interaction_engine(
									image_get_widget(lena))));

	container_t *screen;
	screen = container_create();
	container_add(screen, rectangle_get_widget(bg));
	container_add(screen, image_get_widget(lena));
	container_add(screen, text_get_widget(txt1));
	container_add(screen, text_get_widget(txt2));
	container_add(screen, text_get_widget(txt3));
	container_add(screen, button_engine_get_widget(button1));
	container_add(screen, button_engine_get_widget(button2));
	container_add(screen, container_get_widget(cont));
	container_add(screen, rectangle_get_widget(icon_bg1));
	container_add(screen, icon_get_widget(wifi));
	container_add(screen, icon_get_widget(wifi2));
	container_add(screen, icon_get_widget(wifi3));

	container_draw(screen);
	framebuffer_inform_written_area(0, 0, framebuffer_width(), framebuffer_height());

	rectangle_t *popup_button_bg_on = rectangle_create();
	rectangle_set_fill_color_html(popup_button_bg_on, "#006080");
	rectangle_set_size(popup_button_bg_on, 100, 50);
	rectangle_set_position(popup_button_bg_on, 350, 330);
	rectangle_set_border_color_html(popup_button_bg_on, "#FFFFFF");
	rectangle_set_border_tickness(popup_button_bg_on, 3);
	rectangle_set_rounded_corner_radius(popup_button_bg_on, 4);

	rectangle_t *popup_button_bg_off = rectangle_create();
	rectangle_set_fill_color_html(popup_button_bg_off, "#0080C0");
	rectangle_set_size(popup_button_bg_off, 100, 50);
	rectangle_set_position(popup_button_bg_off, 350, 330);
	rectangle_set_border_color_html(popup_button_bg_off, "#0080FF");
	rectangle_set_border_tickness(popup_button_bg_off, 3);
	rectangle_set_rounded_corner_radius(popup_button_bg_off, 4);

	text_t *popup_button_text_on = text_create();
	text_set_color_html(popup_button_text_on, "#FFFFFF");
	text_set_reference_position(popup_button_text_on, 400, 345);
	text_set_justification(popup_button_text_on, TEXT_CENTER_JUST);
	text_set_font(popup_button_text_on, ubuntu_monospace_16);
	my_string_set(text_get_string(popup_button_text_on), "Bye bye!");

	container_t *popup_button_on = container_create();
	container_add(popup_button_on, rectangle_get_widget(popup_button_bg_on));
	container_add(popup_button_on, text_get_widget(popup_button_text_on));

	button_engine_t *popup_button = button_engine_create();
	button_engine_set_action_normal_state(popup_button, container_get_widget(popup_button_on));
	button_engine_set_action_onpress_state(popup_button, rectangle_get_widget(popup_button_bg_off));
	button_engine_select_action_logic(popup_button);


	rectangle_t *popup_background = rectangle_create();
	rectangle_set_fill_color_html(popup_background, "#FFFFFF");
	rectangle_set_position(popup_background, 200, 90);
	rectangle_set_size(popup_background, 400, 300);

	text_t *popup_text = text_create();
	text_set_color_html(popup_text, "#000000");
	text_set_reference_position(popup_text, 400, 210);
	text_set_justification(popup_text, TEXT_CENTER_JUST);
	text_set_font(popup_text, ubuntu_monospace_16);
	my_string_set(text_get_string(popup_text), "Pop up BRO!!");

	container_t *screen2 = container_create();
	container_add(screen2, rectangle_get_widget(popup_background));
	container_add(screen2, text_get_widget(popup_text));
	container_add(screen2, button_engine_get_widget(popup_button));

	self->main = container_get_widget(screen);
	self->popup = container_get_widget(screen2);
	self->root_pointer = self->main;

	slot_t *popup_enter_slot = slot_create();
	slot_set(popup_enter_slot, (slot_func)marshmallow_thread::goto_popup, self);
	slot_connect(popup_enter_slot,
				interaction_engine_get_click_signal(
					widget_get_interaction_engine(
						button_engine_get_widget(button1))));

	slot_t *main_enter_slot = slot_create();
	slot_set(main_enter_slot, (slot_func)marshmallow_thread::goto_main, self);
	slot_connect(main_enter_slot,
				interaction_engine_get_click_signal(
					widget_get_interaction_engine(
						button_engine_get_widget(popup_button))));

	while (self->p->thread_running)
	{
		pthread_cond_wait(&self->p->thread_cond, &self->p->thread_mutex);

		if (self->p->interaction.set)
		{
			if (self->p->interaction.type == self->p->interaction.PRESS)
				interact_press(self->root_pointer,
						self->p->interaction.x, self->p->interaction.y);
			else if (self->p->interaction.type == self->p->interaction.RELEASE)
				interact_release(self->root_pointer,
						self->p->interaction.x, self->p->interaction.y);
		}

		framebuffer_inform_written_area(0, 0, framebuffer_width(), framebuffer_height());
		pthread_mutex_unlock(&self->p->thread_mutex);
	}

	button_engine_destroy(button1);
	rectangle_destroy(normal);
	rectangle_destroy(onpress);
	container_destroy(cont);
	text_destroy(txt1);
	text_destroy(txt2);
	text_destroy(txt3);

	framebuffer_destroy();

	return NULL;
}

void marshmallow_thread::lena_onclick()
{
	std::cout << "Lena" << std::endl;
}

void marshmallow_thread::goto_popup(marshmallow_thread* self)
{
	self->root_pointer = self->popup;
	widget_draw(self->root_pointer);
}

void marshmallow_thread::goto_main(marshmallow_thread* self)
{
	self->root_pointer = self->main;
	widget_draw(self->root_pointer);
}
