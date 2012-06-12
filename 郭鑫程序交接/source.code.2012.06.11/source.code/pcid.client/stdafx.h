// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>

#include "resource.h"

enum{
	MAX_STRING_SIZE	= 256,
	MAX_PACK_NET_SIZE = 1460,	
};

#define HANDLE_DIALOG_MESSAGE(hwnd, message, function_proc)	\
	case (message): return (SetDlgMsgResult(hwnd, message, HANDLE_##message((hwnd), (wParam), (lParam), (function_proc))))


