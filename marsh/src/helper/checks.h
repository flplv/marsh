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

#ifndef OBJCHECK_H_
#define OBJCHECK_H_

#include "log.h"
#include "fatal.h"

#define MEMORY_ALLOC_CHECK(__obj) \
	if (!__obj) { \
		global_my_log(ERROR, __FILE__, __LINE__, "Failed to alloc memory", "Malloc"); \
		fatal("Locked"); \
 		while(1); \
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

#define	SIGNATURE_NULL ((void(*)(void))-1)

#define MODULE_PRIVATE_DATA_DECLARATION void(*__sig_)(void)
#define ADDRESS_TO_SIGNATURE_CAST void(*)(void)

#define INSTANCE_SET(__instance, __signature) __instance->__sig_ = __signature
#define INSTANCE_GET(__instance) __instance->__sig_
#define INSTANCE_CLEAR(__instance) __instance->__sig_ = SIGNATURE_NULL

#define INSTANCE_CHECK(__instance, __signature, __module__)\
	if (__instance->__sig_ != __signature) \
	{ \
		global_my_log(ERROR, __FILE__, __LINE__, "Invalid Instance", __module__); \
		return; \
	}

#define INSTANCE_CHECK_SILENT(__instance, __signature, __module__)\
	if (__instance->__sig_ != __signature) \
	{ \
		return; \
	}

#define INSTANCE_CHECK_RETURN(__instance, __signature, __module__, __RETURN)\
	if (__instance->__sig_ != __signature) \
	{ \
		global_my_log(ERROR, __FILE__, __LINE__, "Invalid Instance", __module__); \
		return __RETURN; \
	}

#endif /* OBJCHECK_H_ */
