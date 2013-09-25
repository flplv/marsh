#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

struct option;

class arguments
{
public:
	arguments(int main_argc, char * const *main_argv);
	virtual ~arguments();
	bool check();
	void print_usage();
	void print_help();

	bool op_help();
	bool op_grayscale();
	bool op_alpha();
	bool op_icon();
	bool op_16bpp();

	char * op_filename();

private:
	int argc;
	char * const * argv;
	char * filename;
	void arg_parse_loop();
	bool help;
	bool gray;
	bool alph;
	bool icon;
	bool bpp16;
};

#endif /* ARGUMENTS_H_ */
