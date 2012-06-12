// segd.viewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../../common/config.h"
#include "../../common/log.h"
#include "../../common/utility.h"
#include "segd.viewer.h"
#include "segd.editctrl.h"
#include <CommCtrl.h>

//global variable
extern Application __theApp;
HWND __editctrl = NULL;

bool editctrl_create(HINSTANCE instance, RECT* rect_ptr, HWND parent_window_handle, LPCTSTR title)
{
	bool is_ok = false;
	int x = (rect_ptr->right - rect_ptr->left)/4 + 1, 
		y = 2 * (rect_ptr->bottom - rect_ptr->top) / 3 + 1, 
		wd = 3 * (rect_ptr->right - rect_ptr->left)/4 - 1, 
		hi = (rect_ptr->bottom - rect_ptr->top) / 3 - 1;


	__editctrl = CreateWindowEx(0, WC_EDIT, title, 
									WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_CENTER,
									x, y, wd, hi, parent_window_handle, (HMENU)IDC_SEGDVIEWER, instance, NULL);
	
	if(IsWindow(__editctrl)) is_ok = true;

	return is_ok;
}
