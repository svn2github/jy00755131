#ifndef __SEGD_LIST_VIEW_H__
#define __SEGD_LIST_VIEW_H__

#pragma  once

bool	listview_create(HINSTANCE instance, RECT* rect_ptr, 
						HWND parent_window_handle = NULL, LPCTSTR title = NULL);

bool listview_add_item(int index, TCHAR** data);
void listview_clear();

#endif //__SEGD_LIST_VIEW_H__