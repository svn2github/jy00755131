#ifndef __COMMON_CONFIG_H__
#define __COMMON_CONFIG_H__

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

#pragma intrinsic (_InterlockedIncrement,	\
					_InterlockedDecrement,	\
					_InterlockedExchangeAdd,	\
					_InterlockedExchange,	\
					_InterlockedCompareExchange)

#define __in__	
#define __out__	
#define	__in_out__
/*
	__DEBUG_PCI_WRITE_PORT_PERFORMANCE__	测试当前并口卡写入性能
	__OUTPUT_ALL_IO_INFO					输出所有数据接收发送内容
	__DEBUG_IT__							调试过滤代码
	__TEMP_ABANDON_REPEAT_PACK__			临时使用分包操作
*/

enum{
	SAMPLE_DATA_PACKAGE_SIZE = 72,
	DEST_IP_BEGIN_POS	= 20,
	DEST_PORT_BEGIN_POS	= 24,
	CRC_BEGIN_POS		= 252,
	MAX_STRING_SIZE	=	260,
	MAX_PACK_NET_SIZE = 1460,
	CURRENT_SEGD_MINI_DATA_LENGTH = 4,
	RECV_PACK_SERIAL_NO_POS	= 4,
	SEND_PACK_SERIAL_NO_POS	= 4,
	SEND_PACK_SERIAL_NO_LENGTH	= 4,
	RECV_PACK_MAX_SERIAL_NO	= 0xFF,

	DATA_INPUT_PACKAGE_SIZE = 0x100,
	DATA_OUTPUT_PACKAGE_SIZE = 0x80,
	MAX_PARALLE_PORT_INPUT_BUFFER_SIZE = 8,
	SYN_WORD_ONE = 0x11,
	SYN_WORD_TWO = 0x22,
	SYN_WORD_THREE = 0x33,
	SYN_WORD_FOUR = 0x44,

	THE_FIRST_ROUTE_ID_ASSIGNED = 0x100,
	APP_FAULT	=	-1, //!< 返回值失败
	APP_SUCCESS =	0x0,//!< 返回值成功
	MAX_SAMPLING_TIME = 0x1F400, //!< 最大采集时间为128000毫秒
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
//#	define __MATRIX_DEBUG__
#endif //_DEBUG

#define __matrix_zero_Memory(addr, size)	memset((addr), 0x0, (size))

#endif //__COMMON_CONFIG_H__