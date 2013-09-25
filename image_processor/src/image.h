/*
 * image.h
 *
 *  Created on: Mar 23, 2013
 *      Author: fanl
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <CImg.h>

class image
{
public:
	image();
	virtual ~image();
	void load(const char * path_to_image);
	void create_icon_from_alpha();
	void display(const char * title = NULL);
	void merge();

	cimg_library::CImg<unsigned char>& get_CImg() const { return *img;};

protected:
	cimg_library::CImg<unsigned char> *img;

private:
	void check();
};

#endif /* IMAGE_H_ */
