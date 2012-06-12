#pragma once

#include "resource.h"

typedef struct __tagApplication
{
	HINSTANCE	instance_;					// current instance
	TCHAR		title_[MAX_STRING_SIZE];	// The title bar text
	TCHAR		window_class_name_[MAX_STRING_SIZE];// the main window class name
	HWND		main_window_;
}Application, *Application_PTR;

ATOM				MyRegisterClass(HINSTANCE);
BOOL				InitInstance(HINSTANCE, int);
LRESULT	CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR	CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL				init_application(HINSTANCE);
void				on_command(HWND, int, HWND, UINT);
void				on_paint(HWND);
void				on_destroy(HWND);
BOOL				on_create(HWND, LPCREATESTRUCT);
void				on_system_command(HWND, UINT, int, int);
void				on_open_file(HWND);
void				on_save_file(HWND);