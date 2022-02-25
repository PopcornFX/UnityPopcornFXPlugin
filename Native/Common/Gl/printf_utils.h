#pragma once
//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __SAMPLEGL_UTILS_H__
#define __SAMPLEGL_UTILS_H__

#include <cstdarg>
#include <cstdio>

using namespace	PK_NAMESPACE;

#if	defined(PK_PRINTF_TO_CLOG)
#	include <pk_kernel/include/kr_log.h>
#	include <pk_kernel/include/kr_buffer.h>

PK_INLINE void	unity_fprintf(FILE *f, const char *format, ...)
{
	va_list	ap;
	char	buff[2048];
	va_start(ap, format);
	SNativeStringUtils::VSPrintf(buff, format, ap);
//	vsprintf_s(buff, 2048, format, ap);
	va_end(ap);
	if (f == stderr)
		CLog::Log(PK_ERROR, "%s", buff);
	else
		CLog::Log(PK_INFO, "%s", buff);
	// fprintf(f, "%s", buff);
}
#	define fprintf(...) unity_fprintf(__VA_ARGS__)
#	define printf(...) unity_fprintf(stdout, __VA_ARGS__)

#elif	defined(_WIN32)
#	include <Windows.h>

PK_INLINE void	win_fprintf(FILE *f, const char *format, ...)
{
	va_list	ap;
	char	buff[0x2000];
	va_start(ap, format);
	vsprintf_s(buff, 0x2000, format, ap);
	va_end(ap);
	OutputDebugStringA(buff);
	fprintf(f, "%s", buff);
}
#	define fprintf(...)	win_fprintf(__VA_ARGS__)
#	define printf(...)	win_fprintf(stdout, __VA_ARGS__)
#endif

#ifdef	_DEBUG
#	define	ERROR_IF(__call_test, __todo_if_error) \
	do { \
		if (__call_test) \
		{ \
			fprintf(stderr, "Error: %s\n", #__call_test); \
			__todo_if_error; \
		} \
	} while (0)
#else
#	define	ERROR_IF(__call_test, __todo_if_error) \
	do { \
		(__call_test); \
	} while (0)
#endif

#endif /* __SAMPLEGL_UTILS_H__ */
