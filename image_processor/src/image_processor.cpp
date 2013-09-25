#include "arguments.h"
#include "image.h"
#include "ccodeimage.h"
#include <cstdlib>

int main(int argc, char * const * argv)
{
	arguments args(argc, argv);

	if (!args.check())
		exit(-1);

	image image;
	image.load(args.op_filename());

	if (args.op_help())
	{
		args.print_help();
		exit (0);
	}

	if (args.op_grayscale())
	{
		image.merge();
	}

	if (args.op_alpha() && args.op_icon())
	{
		image.display("Original");
		image.create_icon_from_alpha();
		image.display("Result");

		c_code_image output(image);
		output.generate_icon("wifi_icon", "wifi_icon.c");
		exit (0);
	}

	if (args.op_16bpp())
	{
		image.display("Result");

		c_code_image output(image);
		output.generate_16bpp("lena", "lena.c");
		exit (0);
	}

	return 0;
}
