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

#include "my_string.h"
#include "checks.h"
#include "signalslot.h"

struct s_my_string
{
	char * str_data;
	size_t str_len;
	size_t mem_size;
	signal_t *update_signal;
};

static void _expand(my_string_t *obj, size_t size)
{
	if (!obj->str_data)
	{
		obj->str_data = (char *)calloc(size, sizeof(char));
	}
	else
	{
		obj->str_data = (char *)realloc(obj->str_data, size * sizeof(char));
	}

	obj->mem_size = size;
}

static void _clear(my_string_t * obj)
{
	if (obj->str_data)
		free (obj->str_data);

	obj->str_data = (char*)calloc(1, sizeof(char));
	obj->str_len = 0;
	obj->mem_size = 1;
}

static void _set(my_string_t * obj, const char *str)
{
	char *my_str = obj->str_data;
	size_t len = 0;

	while(*str)
	{
		*my_str++ = *str++;
		len++;
	}
	*my_str = '\0';
	obj->str_len = len;
}


my_string_t* my_string_new(void)
{
	my_string_t* obj = (my_string_t *)calloc(1, sizeof(my_string_t));
	MEMORY_ALLOC_CHECK(obj);

	obj->str_data = (char*)calloc(1, sizeof(char));
	obj->str_len = 0;
	obj->mem_size = 1;

	obj->update_signal = signal_new();

	return obj;
}

void my_string_delete(my_string_t *obj)
{
	PTR_CHECK(obj, "my_string");

	if (obj->str_data)
		free(obj->str_data);

	signal_delete(obj->update_signal);

	free(obj);
}

void my_string_clear(my_string_t * obj)
{
	PTR_CHECK(obj, "my_string");

	_clear(obj);
}

size_t my_string_set(my_string_t *obj, const char* str)
{
	size_t len = my_strlen(str);

	PTR_CHECK_RETURN(obj, "my_string", 0);

	if ((len+1) > obj->mem_size)
	{
		_clear(obj);
		_expand(obj, len*2);
	}

	_set(obj, str);

	signal_emit(obj->update_signal);

	return obj->str_len;
}

const char* my_string_get(my_string_t* obj)
{
	PTR_CHECK_RETURN(obj, "my_string", (char *) 0);

	return obj->str_data;
}

size_t my_string_len(my_string_t* obj)
{
	PTR_CHECK_RETURN(obj, "my_string", 0);

	return obj->str_len;
}

signal_t * my_string_get_update_signal(my_string_t * obj)
{
	PTR_CHECK_RETURN(obj, "my_string", (signal_t *) 0);

	return obj->update_signal;
}

size_t my_strlen(const char* str)
{
	size_t len = 0;

	PTR_CHECK_RETURN(str, "my_string", 0);

	while (*str++)
		len++;
	return len;
}

