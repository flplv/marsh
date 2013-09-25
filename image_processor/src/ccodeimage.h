/*
 * ccodeimage.h
 *
 *  Created on: Mar 23, 2013
 *      Author: fanl
 */

#ifndef CCODEIMAGE_H_
#define CCODEIMAGE_H_

class image;

class c_code_image
{
public:
	c_code_image(const image & rgb_image);
	virtual ~c_code_image();

	void generate_icon(const char * filename, const char * path_to_output);
	void generate_16bpp(const char * filename, const char * path_to_output);

private:
	const image * processed_img;
};

#endif /* CCODEIMAGE_H_ */
