/*
 * help.cpp
 *
 *  Created on: Mar 23, 2013
 *      Author: fanl
 */

#include "arguments.h"
#include <iostream>
extern "C" {
#include <getopt.h>
}

struct option long_options[] = {
	{"help",      no_argument,       0,  'h' },
	{"grayscale", no_argument,       0,  'g' },
	{"alpha",     no_argument,       0,  'a' },
	{"icon",      no_argument,       0,  'i' },
	{"16bpp",     no_argument,       0,  '1' },
	{0,           0,                 0,   0  }
};

char* arguments::op_filename()
{
	return filename;
}

void arguments::arg_parse_loop()
{
	int opt = 0;
	int long_index = 0;

	while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
	{
		if (opt == 'h')
			help = true;
		if (opt == 'g')
			gray = true;
		if (opt == 'a')
			alph = true;
		if (opt == 'i')
			icon = true;
		if (opt == '1')
			bpp16 = true;
	}
}

arguments::arguments(int main_argc, char * const *main_argv)
{
	argc = main_argc;
	argv = main_argv;
	help = false;
	gray = false;
	alph = false;
	icon = false;
	filename = argv[1];

	arg_parse_loop();
}

arguments::~arguments()
{
}

bool arguments::check()
{
	if (argc < 2 || filename == NULL)
	{
		print_usage();
		return false;
	}
	return true;
}

void arguments::print_usage()
{
	const char * program_name = argv[0];

	if (program_name[0] == '/') {
	std::cout << "Use: " << program_name << " <path_to_image_file> [options]";
	}
	else {
	std::cout << "Use: ./" << program_name << " <path_to_image_file> [options]";
	}
}

void arguments::print_help()
{
	print_usage();

	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -grayscale    This option will merge all channels." << std::endl;
	std::cout << "               The output image will be grayscale." << std::endl;
	std::cout << "" << std::endl;
	std::cout << " -alpha        Use the alpha channel as image source." << std::endl;
	std::cout << "               The output image will be grayscale." << std::endl;
	std::cout << "" << std::endl;
	std::cout << " -icon         Generate an Icon. Icons has support of " << std::endl;
	std::cout << "               pseudo antialiasing." << std::endl;
	std::cout << "" << std::endl;
	std::cout << " -16bpp        Generate an Image of 16bpp." << std::endl;
	std::cout << "" << std::endl;
}


bool arguments::op_help()
{
	if (help)
		return true;

	return false;
}

bool arguments::op_grayscale()
{
	if (gray)
		return true;

	return false;
}

bool arguments::op_alpha()
{
	if (alph)
		return true;

	return false;
}

bool arguments::op_16bpp()
{
	if (bpp16)
		return true;

	return false;
}

bool arguments::op_icon()
{
	if (icon)
		return true;

	return false;
}
