#ifndef __CONFIG_H__
#define __CONFIG_H__

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include <intrin.h>
#include <stdarg.h>

// pcap header files
#include "../include/pcap/pcap.h"

#pragma intrinsic (_InterlockedIncrement,	\
					_InterlockedDecrement,	\
					_InterlockedExchangeAdd,	\
					_InterlockedExchange,	\
					_InterlockedCompareExchange)
#define __in__	
#define __out__	
#define	__in_out__
enum{
	MAX_STRING_SIZE	=	260,
	MAX_PACK_NET_SIZE = 1460,
	APP_FAULT	=	-1, //!< 返回值失败
	APP_SUCCESS =	0x0,//!< 返回值成功
};

#define HANDLE_DIALOG_MESSAGE(hwnd, message, function_proc)	\
	case (message): return (SetDlgMsgResult(hwnd, message, HANDLE_##message((hwnd), (wParam), (lParam), (function_proc))))

#if defined(_UNICODE) || defined(UNICODE)
#	define	__MATRIX_TEXT(x)	L##x
	typedef	wchar_t	char_t;	
#	define	__matrix_fsopen			_wfsopen
#	define	__matrix_sprintf_s		swprintf_s
#	define	__matrix_strcat_s		wcscat_s
#	define	__matrix_strnlen_s		wcsnlen_s
#	define	__matrix_setlocale		_wsetlocale
#	define	__matrix_vsprintf_s		vswprintf_s
#	define	__matrix_stricmp		_wcsicmp	
#	define	__matrix_strcpy_s		wcscpy_s
#	define	__matrix_strstr			wcsstr
#	define	__matrix_splitpath_s	_wsplitpath_s
#else
#	define __MATRIX_TEXT(x)	x
	typedef	char	char_t;
#	define	__matrix_fsopen	_fsopen
#	define	__matrix_sprintf_s	sprintf_s
#	define	__matrix_strcat_s	strcat_s
#	define	__matrix_strnlen_s	strnlen_s
#	define	__matrix_setlocale	setlocale
#	define	__matrix_vsprintf_s	vsprintf_s
#	define	__matrix_stricmp	_stricmp
#	define	__matrix_strcpy_s	strcpy_s
#	define	__matrix_strstr		strstr
#	define	__matrix_splitpath_s	_splitpath_s
#endif

#ifdef _DEBUG
#	define __MATRIX_DEBUG__
#endif //_DEBUG

#define __matrix_zero_Memory(addr, size)	memset((addr), 0x0, (size))

#endif //__COMMON_CONFIG_H__