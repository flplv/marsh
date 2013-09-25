/*
 * ccodeimage.cpp
 *
 *  Created on: Mar 23, 2013
 *      Author: fanl
 */

#include "ccodeimage.h"
#include "image.h"
#include <CImg.h>
#include <fstream>

using namespace std;
using namespace cimg_library;

c_code_image::c_code_image(const image & image)
{
	processed_img = &image;
}

c_code_image::~c_code_image()
{
}

void c_code_image::generate_icon(const char * filename, const char* path_to_output_c)
{
	ofstream code;
	code.open(path_to_output_c);

	code << "#include \"bitmap_data.h\"" << endl << endl;
	code << "static const uint8_t _" << filename << "_data[] =" << endl << "{";

	CImg<unsigned char> &img = processed_img->get_CImg();

	for (int row = 0; row < img.height(); row ++)
	{
		code << endl << "\t";

		for (int col = 0; col < img.width(); col++)
		{
			char ch[10];
			snprintf(ch, 10, "0x%02X, ", *img.data(col, row, 0, 0));
			code << ch;
		}
	}

	code << endl << "};" << endl << endl;

	code << "const struct s_bitmap _" << filename << " =" << endl;
	code << "{" << endl;
	code << "\t_" <<  filename << "_data," << endl;
	code << "\t" << img.width() << "," << endl;
	code << "\t" << img.height() << "," << endl;
	code << "\tTRUE," << endl;
	code << "\tBITMAP_BUFFER_8BPP," << endl;
	code << "};" << endl;

	code << endl;

	code << "const bitmap_t *"<< filename <<"_bitmap = &_" << filename << ";" << endl;

	code.close();
}

void c_code_image::generate_16bpp(const char* filename, const char* path_to_output_c)
{
	ofstream code;
	code.open(path_to_output_c);

	code << "#include \"bitmap_data.h\"" << endl << endl;
	code << "static const uint16_t _" << filename << "_data[] =" << endl << "{";

	CImg<unsigned char> &img = processed_img->get_CImg();

	for (int row = 0; row < img.height(); row ++)
	{
		code << endl << "\t";

		for (int col = 0; col < img.width(); col++)
		{
			char ch[10];
			unsigned char red = *img.data(col, row, 0, 0);
			unsigned char green = *img.data(col, row, 0, 1);
			unsigned char blue = *img.data(col, row, 0, 2);
			snprintf(ch, 10, "0x%04X, ", (((red>>3)<<11) | ((green>>2)<<5) | ((blue)>>3)));
			code << ch;
		}
	}

	code << endl << "};" << endl << endl;

	code << "const struct s_bitmap _" << filename << " =" << endl;
	code << "{" << endl;
	code << "\t_" <<  filename << "_data," << endl;
	code << "\t" << img.width() << "," << endl;
	code << "\t" << img.height() << "," << endl;
	code << "\tFALSE," << endl;
	code << "\tBITMAP_BUFFER_16BPP," << endl;
	code << "};" << endl;

	code << endl;

	code << "const bitmap_t *"<< filename <<"_bitmap = &_" << filename << ";" << endl;

	code.close();
}
