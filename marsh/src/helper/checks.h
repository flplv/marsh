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

#ifndef OBJCHECK_H_
#define OBJCHECK_H_

#include "log.h"
#include "fatal.h"

#define MEMORY_ALLOC_CHECK_RETURN(__obj, __ret__) \
	if (!__obj) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Failed to alloc memory", "Malloc"); \
 		return __ret__; \
	}

#define MEMORY_ALLOC_CHECK(__obj) \
	if (!__obj) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Failed to alloc memory", "Malloc"); \
 		return; \
	}

#define PTR_CHECK(__ptr, __module__) \
	if (!__ptr) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Invalid Pointer", __module__); \
		return; \
	}

#define PTR_CHECK_SILENT(__ptr, __module__) \
	if (!__ptr) { \
		return; \
	}

#define PTR_CHECK_RETURN(__ptr, __module__, __ret__) \
	if (!__ptr) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Invalid Pointer", __module__); \
		return __ret__; \
	}

#define ASSERT(__condition, __module__) \
	if (!__condition) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Assertion " #__condition " failed.", __module__); \
		return; \
	}

#define ASSERT_RETURN(__condition, __module__, __ret__) \
	if (!(__condition)) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Assertion " #__condition " failed.", __module__); \
		return __ret__; \
	}

#endif /* OBJCHECK_H_ */
