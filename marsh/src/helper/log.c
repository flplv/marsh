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

#include "helper/number.h"

#include "log.h"
#include "log_private.h"
#include <stdio.h>
#include <string.h>

char output_prefix_string[20];
char line_output_string[20];

int _stdout_output_impl(const char * str)
{
	const char *pCh = str;
	while (*pCh)
		putchar(*pCh++);
	fflush(stdout);
	return pCh - str;
}
int (*marshmallow_terminal_output)(const char * str) = _stdout_output_impl;

my_log_t * my_log_create(const char * module_tag, enum e_log_lvl default_lvl)
{
	my_log_t * log = (my_log_t *)malloc(sizeof(struct s_my_log_instance));

	log->module_tag = module_tag;
	log->default_my_log_level = default_lvl;
	log->stream_output = marshmallow_terminal_output;
	return log;
}
void my_log_destroy(my_log_t * const obj)
{
	if(obj)
		free(obj);
}

void my_log(enum e_log_lvl lvl, const char *file, int line, const char * text, my_log_t * const owner)
{
	if (!owner)
		return;

	if (!owner->stream_output)
		return;
	if (lvl > owner->default_my_log_level)
		return;

	if (lvl == ERROR)
	{
		strncpy(output_prefix_string, "error: ", 20);
	}
	else if (lvl == WARNING)
	{
		strncpy(output_prefix_string, "warning: ", 20);
	}
	else if (lvl == MESSAGE)
	{
		strncpy(output_prefix_string, "message: ", 20);
	}

	int_to_string(line, line_output_string, 20);

	owner->stream_output("\n");
	owner->stream_output(file);
	owner->stream_output(":");
	owner->stream_output(line_output_string);
	owner->stream_output(": ");
	owner->stream_output(output_prefix_string);
	owner->stream_output(owner->module_tag);
	owner->stream_output(": ");
	owner->stream_output(text);
}

void my_log_continue(const char * text, my_log_t * const owner)
{
	if (!owner)
		return;

	if (!owner->stream_output)
		return;

	owner->stream_output(text);
}

void my_log_continue_new_line(const char * text, my_log_t * const owner)
{
	int i;

	if (!owner)
		return;

	if (!owner->stream_output)
		return;

	owner->stream_output("\n");
	i = strlen(owner->module_tag);
	while (i--)
		owner->stream_output(" ");
	owner->stream_output("       ");
	owner->stream_output(text);
}

bool my_log_assert(bool test, enum e_log_lvl lvl, const char *file, int line, const char * text, my_log_t * const owner)
{
	if (!owner)
		return false;

	if (!test)
	{
		my_log(lvl, file, line, "Assertion Failed: ", owner);
		my_log_continue(text, owner);
		return true;
	}
	return false;
}

void global_my_log(enum e_log_lvl lvl, const char *file, int line, const char * text, const char * owner)
{
	struct s_my_log_instance log;

	if (!owner)
		return;
	if (!text)
		return;

	log.default_my_log_level = MESSAGE;
	log.module_tag = owner;
	log.stream_output = marshmallow_terminal_output;
	my_log(lvl, file, line, text, &log);
}
