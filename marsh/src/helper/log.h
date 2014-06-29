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

#ifndef LOG_H_
#define LOG_H_

#include "types.h"

enum e_log_lvl {
	ERROR,
	WARNING,
	MESSAGE
};

typedef struct s_my_log_instance my_log_t;


my_log_t * my_log_new(const char * title, enum e_log_lvl default_lvl);
void my_log_delete(my_log_t * const obj);

void my_log(enum e_log_lvl, const char *file, int line, const char *, my_log_t *);
void my_log_continue(const char *, my_log_t *);
void my_log_continue_new_line(const char *, my_log_t *);
bool my_log_assert(bool test, enum e_log_lvl lvl, const char *file, int line, const char * text, my_log_t * const);

void global_my_log(enum e_log_lvl lvl, const char *file, int line, const char * text, const char * owner);

#define LOG_ERROR(__owner, __text) global_my_log(ERROR, __FILE__, __LINE__, __text, __owner)

#endif /* LOG_H_ */
