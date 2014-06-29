/*
 *  Copyright (C) 2013 to 2014 by Felipe Lavratti
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

#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <glibmm/fileutils.h>
#include <iostream>
#include <sigc++/sigc++.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

#include <X11/X.h>
#include <X11/Xlib.h>

#include "marshmallowthread.h"

class MyArea: public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();
	Glib::RefPtr<Gdk::Pixbuf> m_PxlBuf;

protected:
	//Override default signal handler:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

MyArea::MyArea()
{
	try
	{
		m_PxlBuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 800, 480);
	} catch (const Gdk::PixbufError& ex)
	{
//		std::cerr << "PixbufError: " << ex.what() << std::endl;
	}

	if (m_PxlBuf)
		set_size_request(m_PxlBuf->get_width() / 2, m_PxlBuf->get_height() / 2);
}

MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!m_PxlBuf)
		return false;

	Gdk::Cairo::set_source_pixbuf(cr, m_PxlBuf, 0, 0);
	cr->paint();

	return true;
}

MyArea *pArea;
char * pVirtFb = NULL;

Glib::RefPtr<Gtk::Application> gSimuApp;

marshmallow_thread *pMarsh;

void * VirtualInputClickHandler(GdkEventButton * ev)
{
	static bool pressed = false;

	int x = ev->x;
	int y = ev->y;

	if (!pressed)
	{
		if (ev->type == GDK_BUTTON_PRESS)
		{
			std::cout << "PressStart at " << x << ", " << y << ", " << std::endl;

			pressed = true;
			pMarsh->press(x, y);
		}
	}
	else
	{
		if (ev->type == GDK_BUTTON_RELEASE)
		{
			std::cout << "PressStop at " << x << ", " << y << ", " << std::endl;

			pressed = false;
			pMarsh->release(x, y);
		}
	}
	return NULL;
}

int main(int argc, char **argv)
{
	gdk_threads_init();
	gdk_threads_enter();

	gSimuApp = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	Gtk::Window win;
	win.set_title("RE8000 Simulador");
	win.set_default_size(800, 480);

	/* Virtual Framebuffer area */
	pArea = new MyArea;

	/* Input */
	pArea->add_events(Gdk::BUTTON_PRESS_MASK);
	pArea->signal_button_press_event().connect(sigc::ptr_fun(&VirtualInputClickHandler));
	pArea->add_events(Gdk::BUTTON_RELEASE_MASK);
	pArea->signal_button_release_event().connect(sigc::ptr_fun(&VirtualInputClickHandler));
	gdk_threads_leave();

	/* Fb */
	pVirtFb = new char[800 * 480 * 2];

	/* Gtk Window call */
	win.add(*pArea);
	pArea->show();


	pMarsh = new marshmallow_thread;

	return gSimuApp->run(win);
}

void VirtualFb_Refresh(void)
{
	char * pGtkDrawBuffer = (char *) pArea->m_PxlBuf->get_pixels();
	short * pSh;

	for (int i = 0; i < (800 * 480); i++)
	{
		pSh = (short *) (pVirtFb + i * 2);
		*pGtkDrawBuffer++ = (((*pSh >> 11) & 0x1F) << 3) | 0x7;
		*pGtkDrawBuffer++ = (((*pSh >> 5) & 0x3F) << 2) | 0x3;
		*pGtkDrawBuffer++ = ((*pSh & 0x1F) << 3) | 0x7;
	}
	gdk_threads_enter();
	pArea->hide();
	pArea->show();
	gdk_threads_leave();
}
