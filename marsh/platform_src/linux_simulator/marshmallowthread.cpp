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
#include "event.h"
#include "widget.h"
#include "widget_tree.h"
#include "text.h"
#include "rectangle.h"
#include "icon.h"
#include "image.h"
#include "button_engine.h"
#include "signalslot.h"
#include "signalslot2.h"

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
	framebuffer_new();
	event_pool_init();

	widget_t *screen;
	screen = widget_new(NULL, NULL, NULL, NULL);
	widget_area(screen)->width = 800;
	widget_area(screen)->height = 480;

	rectangle_t *bg = rectangle_new(screen);
	rectangle_set_fill_color_html(bg, "#004000");
	rectangle_set_position(bg, 5, 5);
	rectangle_set_size(bg, 790, 470);

	rectangle_t *obj1 = rectangle_new(rectangle_get_widget(bg));
	rectangle_t *obj2 = rectangle_new(rectangle_get_widget(bg));
	rectangle_t *obj3 = rectangle_new(rectangle_get_widget(bg));

	rectangle_set_position(obj1, 0, 0);
	rectangle_set_size(obj1, 100, 100);
	rectangle_set_fill_color_html(obj1, "#800000");

	rectangle_set_position(obj2, 50, 50);
	rectangle_set_size(obj2, 100, 100);
	rectangle_set_fill_color_html(obj2, "#008000");

	rectangle_set_position(obj3, 100, 100);
	rectangle_set_size(obj3, 100, 100);
	rectangle_set_fill_color_html(obj3, "#000080");


	rectangle_t *rounded, *square_border, *rounded_border, *frame, *rounded_frame;
	rounded = rectangle_new(rectangle_get_widget(bg));
	square_border = rectangle_new(rectangle_get_widget(bg));
	rounded_border = rectangle_new(rectangle_get_widget(bg));
	frame = rectangle_new(rectangle_get_widget(bg));
	rounded_frame = rectangle_new(rectangle_get_widget(bg));

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

	rectangle_t *icon_bg1 = rectangle_new(screen);
	rectangle_set_position(icon_bg1, 0, 300);
	rectangle_set_size(icon_bg1, 50, 50);
	rectangle_set_fill_color_html(icon_bg1, "#404040");
	rectangle_set_border_color_html(icon_bg1, "#FF4040");
	rectangle_set_border_tickness(icon_bg1, 5);

	icon_t *wifi = icon_new(screen);
	icon_set_color_html(wifi, "#00FF00");
	icon_set_position(wifi, 10, 310);
	icon_set_bitmap(wifi, wifi_icon_bitmap);

	icon_t *wifi2 = icon_new(screen);
	icon_set_color_html(wifi2, "#00FFFF");
	icon_set_position(wifi2, 20, 320);
	icon_set_bitmap(wifi2, wifi_icon_bitmap);

	icon_t *wifi3 = icon_new(screen);
	icon_set_color_html(wifi3, "#FFFFFF");
	icon_set_position(wifi3, 30, 330);
	icon_set_bitmap(wifi3, wifi_icon_bitmap);

	image_t *lena = image_new(screen);
	image_set_bitmap(lena, lena_bitmap);
	image_set_position(lena, 120, 205);

	text_t *txt1 = text_new(screen);
	text_set_color_html(txt1, "#C0C0C0");
	text_set_font(txt1, ubuntu_monospace_16);
	text_set_reference_position(txt1, 10, 400);
	my_string_set(text_get_string(txt1), "Hello World!\nHow beautiful is it?");

	text_t *txt2 = text_new(screen);
	text_set_color_html(txt2, "#D0D0D0");
	text_set_font(txt2, ubuntu_monospace_16);
	text_set_justification(txt2, TEXT_CENTER_JUST);
	text_set_reference_position(txt2, 400, 400);
	my_string_set(text_get_string(txt2), "Hello World!\nHow beautiful is it?");

	text_t *txt3 = text_new(screen);
	text_set_color_html(txt3, "#E0E0E0");
	text_set_font(txt3, ubuntu_monospace_16);
	text_set_justification(txt3, TEXT_RIGHT_JUST);
	text_set_reference_position(txt3, 790, 400);
	my_string_set(text_get_string(txt3), "Hello World!\nHow beautiful is it?");

	slot_t *lena_slot = slot_new();
	slot_set(lena_slot, (slot_func)lena_onclick, NULL);

	slot_connect(lena_slot, widget_click_signal(image_get_widget(lena)));

	widget_tree_draw(screen);
	framebuffer_inform_written_area(0, 0, framebuffer_width(), framebuffer_height());

	self->main = screen;
	self->root_pointer = self->main;

	while (self->p->thread_running)
	{
		pthread_cond_wait(&self->p->thread_cond, &self->p->thread_mutex);

		if (self->p->interaction.set)
		{
			if (self->p->interaction.type == self->p->interaction.PRESS)
				widget_tree_press(self->root_pointer,
						self->p->interaction.x, self->p->interaction.y);
			else if (self->p->interaction.type == self->p->interaction.RELEASE)
				widget_tree_release(self->root_pointer,
						self->p->interaction.x, self->p->interaction.y);
		}

		framebuffer_inform_written_area(0, 0, framebuffer_width(), framebuffer_height());
		pthread_mutex_unlock(&self->p->thread_mutex);
	}

	text_delete(txt1);
	text_delete(txt2);
	text_delete(txt3);

	event_pool_deinit();
	framebuffer_delete();

	return NULL;
}

void marshmallow_thread::lena_onclick()
{
	std::cout << "Lena" << std::endl;
}

void marshmallow_thread::goto_popup(marshmallow_thread* self)
{
//	widget_tree_draw(self->root_pointer);
}

void marshmallow_thread::goto_main(marshmallow_thread* self)
{
	self->root_pointer = self->main;
	widget_tree_draw(self->root_pointer);
}
