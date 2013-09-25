/*
 * image.cpp
 *
 *  Created on: Mar 23, 2013
 *      Author: fanl
 */

#include "image.h"
#include <cerrno>
#include <CImg.h>
#include <iostream>

using namespace cimg_library;
using namespace std;

image::image()
{
	img = NULL;
}

image::~image()
{
	if (img) delete img;
}

void image::load(const char* path_to_image)
{
	img = new CImg<unsigned char>(path_to_image);

	cout << "Image Width: " << img->width() << endl;
	cout << "Image Height: " << img->height() << endl;
	cout << "Image Depth: " << img->depth() << endl;
}

void image::check()
{
	if (!img)
	{
		cout << "Bad usage of class image";
		exit(-1);
	}
}

void image::display(const char * title)
{
	img->display(title);
}

void image::merge()
{
	if (img->depth() > 1)
	{
		cout << "Merging Channels..." << endl;
		CImg<unsigned char> original_img(*img);
		delete img;
		img = new CImg<unsigned char>(original_img.width(), original_img.height(), 1, 1, 0);
		*img = original_img.RGBtoHSI().get_channel(2);
	}
}

void image::create_icon_from_alpha()
{
	check();

	cout << "Getting Icon from Alpha Channel" << endl;

	CImg<unsigned char> white(img->width(),img->height(), 1, 1, 255);

	CImg<unsigned char> render(img->width(),img->height(), 1, 1);
	render.fill(0);
	render.draw_image(0,0,0,0, white, img->channel(img->depth()), 1, 255);

	delete img;
	img = new CImg<unsigned char>(render.width(), render.height(), 1, 1, 0);
	*img = render;
}

