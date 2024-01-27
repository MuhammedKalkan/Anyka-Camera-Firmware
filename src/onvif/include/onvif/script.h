/*
 *
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 *
 *  By downloading, copying, installing or using the software you agree to this license.
 *  If you do not agree to this license, do not download, install,
 *  Copy or use the software.
 *
 *  Copyright (C) 2012, JUAN, Co, All Rights Reserved.
 *
 *  Created on: Nov 19, 2015
 *	Author: Frank Law
 *
 */

#include <stdarg.h>
#include <stdlib.h>
#include <NkUtils/types.h>

#if !defined(N1_DEVICE_TEST_SCRIPT_H_)
#define N1_DEVICE_TEST_SCRIPT_H_
NK_CPP_EXTERN_BEGIN

static inline NK_Int
SCRIPT(NK_PChar fmt, ...)
{
	NK_Char script[1024 * 4];
	va_list var;

	va_start(var, fmt);
	vsnprintf(script, sizeof(script), fmt, var);
	va_end(var);

	return system(script);
}

static inline NK_SSize
SCRIPT2(NK_PChar result, NK_Size result_max, NK_PChar fmt, ...)
{
	NK_Char script[1024 * 4];
	va_list var;
	FILE *fID = NK_Nil;
	NK_SSize readn = -1;

	va_start(var, fmt);
	vsnprintf(script, sizeof(script), fmt, var);
	va_end(var);

	fID = popen(script, "rb");
	if (!fID) {
		return -1;
	}

	readn = fread(result, 1, result_max, fID);
	pclose(fID);
	fID = NK_Nil;

	return readn;
}


NK_CPP_EXTERN_END
#endif /* N1_DEVICE_TEST_SCRIPT_H_ */
