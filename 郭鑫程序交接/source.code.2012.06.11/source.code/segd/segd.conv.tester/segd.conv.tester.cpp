// segd.conv.tester.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "segd.conv.tester.h"
#include "..\segd.api\segd.api.h"

#include <stdio.h>
#include <bitset>
#include <string>
#include <iostream>

#include <shlwapi.h>

#pragma comment(lib, "shlwapi")

#define MAX_LOADSTRING 100
#define APP_FAULT -1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INIT_MODULE __init_module_ptr = NULL;
SET_MODULE_OPTION __set_option_ptr = NULL;;
RELEASE_MODULE __release_module_ptr = NULL;
GET_ERROR_INFO __get_last_error_ptr = NULL;
SHOW_WRITING_PROGRESS __show_writing_progress_ptr = NULL;

using namespace std; 

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	int error_code = 0x0;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SEGDCONVTESTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)){return FALSE;}

	HMODULE instance = LoadLibrary(__TEXT("segd.api.dll"));

	__init_module_ptr = (INIT_MODULE)GetProcAddress(instance, "init_module");
	__set_option_ptr = (SET_MODULE_OPTION)GetProcAddress(instance, "set_module_option");
	__release_module_ptr = (RELEASE_MODULE)GetProcAddress(instance, "release_module");
	__get_last_error_ptr = (GET_ERROR_INFO)GetProcAddress(instance, "get_error_info");
	__show_writing_progress_ptr = (SHOW_WRITING_PROGRESS)GetProcAddress(instance, "show_writing_progress");

	SEGD_API_INFO sai;

	SecureZeroMemory(&sai, sizeof(sai));
	sai.listen_port_ = 25360;
	sai.config_path_ = __TEXT("args.conf");

	error_code = __init_module_ptr(&sai);
	if(error_code == APP_FAULT)	MessageBox(NULL, __get_last_error_ptr(&error_code), __TEXT("Error info"), MB_OK);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SEGDCONVTESTER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	__release_module_ptr();
	FreeLibrary(instance);
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SEGDCONVTESTER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SEGDCONVTESTER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void test_driver_set_config_option()
{

	int error_code = 0x0;
	error_code = __set_option_ptr(__TEXT("datetime"), __TEXT("11.236.15.27.10"));
	if(error_code == APP_FAULT)	MessageBox(NULL, __get_last_error_ptr(&error_code), __TEXT("Error info"), MB_OK);
}

void test_driver_run_segd_conv()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	TCHAR buf[MAX_PATH] = {0}; 

	ZeroMemory( &si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi));

	// Start the child process.  
	if( !CreateProcess( __TEXT("E:\\works\\segd\\segd.conv\\release\\segd.conv.exe"), // No module name (use command line)
		__TEXT("--data-file E:\\20111025\\809.4k.dat"),  // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,// No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		wnsprintf(buf, sizeof(buf)/sizeof(buf[0]), __TEXT("CreateProcess failed (%d)\n"), GetLastError());
		MessageBox(NULL, buf, L"error", MB_OK);
		return;
	}

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

void test_show_segd_progress(HWND hwnd)
{
	__show_writing_progress_ptr(GetDesktopWindow(), L"SEGD文件生成进度条");
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_SET_CONFIG_OPTION:
			test_driver_set_config_option();
			break;
		case IDM_TEST_RUNSEGD_CONV:
			test_driver_run_segd_conv();
			break;
		case IDM_TEST_SHOWSEG:
			test_show_segd_progress(hWnd);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
