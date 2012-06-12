// segd.viewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../../common/config.h"
#include "../../common/log.h"
#include "../../common/utility.h"
#include "segd.viewer.h"
#include "segd.listview.h"
#include "segd.editctrl.h"
#include "segd.treeview.h"
#include "segd.file.format.h"
#include <CommDlg.h>
#include <Shlwapi.h>
#include <list>

#pragma comment(lib, "comdlg32")
#pragma comment(lib, "shlwapi")

//local enumeration
enum __enumTREEVIEW_ITEM_DATA_TYPE
{
	SEGD_GENERAL_HEADER_1_TYPE,
	SEGD_GENERAL_HEADER_2_TYPE,
	SEGD_GENERAL_HEADER_3_TYPE,
	SEGD_SCAN_TYPE_HEADER_TYPE,
	SEGD_EXTENDED_HEADER_TYPE,
	SEGD_EXTERNAL_HEADER_TYPE,
	SEGD_DEMUX_TRACE_HEADER_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_1_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_2_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_3_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_4_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_5_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_6_TYPE,
	SEGD_TRACE_EXTENDED_HEADER_7_TYPE,
};

//local struct
typedef struct __tagTREEVIEW_ITEM_DATA
{
	int type_;
	void* pointer_;
}TREEVIEW_ITEM_DATA, *TREEVIEW_ITEM_DATA_PTR;


// Global Variables:
Application __theApp;
SEGD_FILE_PTR __read_segd_file = NULL;
std::list<TREEVIEW_ITEM_DATA_PTR> __treeview_item_data_list;

void clear_treeview_item_data_list()
{
	std::list<TREEVIEW_ITEM_DATA_PTR>::iterator itr;
	TREEVIEW_ITEM_DATA_PTR item_data_ptr = NULL;

	if(__treeview_item_data_list.size() > 0){
		
		for(itr = __treeview_item_data_list.begin(); itr != __treeview_item_data_list.end(); ++itr){

			item_data_ptr = *itr;
			if(item_data_ptr) free(item_data_ptr);
		}

		__treeview_item_data_list.clear();
	}
}

TREEVIEW_ITEM_DATA_PTR add_treeview_item_data_into_list(int type, void* pointer)
{
	int size = sizeof(TREEVIEW_ITEM_DATA);
	TREEVIEW_ITEM_DATA_PTR ptr = NULL;
	ptr = (TREEVIEW_ITEM_DATA_PTR)malloc(size);
	SecureZeroMemory(ptr, size);
	
	ptr->type_ = type;
	ptr->pointer_ = pointer;
	__treeview_item_data_list.push_back(ptr);

	return ptr;
}

#include "segd.viewer2.h"

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

	memset(&__theApp, 0x0, sizeof(__theApp));

	if(!init_application(hInstance)){

		return FALSE;
	}

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)){

		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SEGDVIEWER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

BOOL init_application(HINSTANCE instance)
{
	BOOL is_ok = TRUE;
	INITCOMMONCONTROLSEX ccx; 

	// Initialize global strings
	__theApp.instance_ = instance;
	LoadString(instance, IDS_APP_TITLE, __theApp.title_, sizeof(__theApp.title_));
	LoadString(instance, IDC_SEGDVIEWER, __theApp.window_class_name_, sizeof(__theApp.window_class_name_));

	MyRegisterClass(instance);

	ccx.dwSize = sizeof(ccx);
	ccx.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;

	is_ok = InitCommonControlsEx(&ccx);

	return is_ok;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SEGDVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SEGDVIEWER);
	wcex.lpszClassName	= __theApp.window_class_name_;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	LONG style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
   __theApp.instance_ = hInstance; // Store instance handle in our global variable
   __theApp.main_window_ = CreateWindowEx(NULL, __theApp.window_class_name_, __theApp.title_, 
										style, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, __theApp.instance_, NULL);

   if (!__theApp.main_window_) return FALSE;

   ShowWindow(__theApp.main_window_, nCmdShow);
   UpdateWindow(__theApp.main_window_);

   return TRUE;
}


void on_command(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	UNREFERENCED_PARAMETER(codeNotify);
	UNREFERENCED_PARAMETER(hwndCtl);

	switch (id)
	{
		case IDM_ABOUT:
			DialogBox(__theApp.instance_, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		case ID_FILE_OPEN:
			on_open_file(hwnd);
			break;
		case ID_FILE_SAVEFILE:
			on_save_file(hwnd);
			break;
	}
}

void on_paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hwnd, &ps);
}

void on_destroy(HWND hwnd)
{
	UNREFERENCED_PARAMETER(hwnd);

	clear_treeview_item_data_list();

	if(__read_segd_file)
		segd_file_close(__read_segd_file);

	PostQuitMessage(0);
}

BOOL on_create(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(lpCreateStruct);

	BOOL is_ok = TRUE;
	int wid = 0,
		hi = 0,
		x = 0,
		y = 0;
	RECT rect = {0};
	RECT desktop_rect = {0};

	GetClientRect(hwnd, &rect);

	is_ok = editctrl_create(__theApp.instance_, &rect, hwnd);
	if(!is_ok) return is_ok;

	is_ok = treeview_create(__theApp.instance_, &rect, hwnd);
	if(!is_ok) return is_ok;

	is_ok = listview_create(__theApp.instance_, &rect, hwnd);
	if(!is_ok) return is_ok;

	GetWindowRect(hwnd, &rect);
	GetWindowRect(GetDesktopWindow(), &desktop_rect);

	wid = rect.right - rect.left;
	hi = rect.bottom - rect.top;
	y = (desktop_rect.bottom - desktop_rect.top)/2 - hi/2;
	x = (desktop_rect.right - desktop_rect.left)/2 - wid/2;

	MoveWindow(hwnd, x, y, wid, hi, TRUE);

	return is_ok;
}


BOOL on_notify(HWND hwnd, int control_id, NMHDR* param)
{
	UNREFERENCED_PARAMETER(control_id);
	UNREFERENCED_PARAMETER(hwnd);

	TREEVIEW_ITEM_DATA_PTR data_ptr = NULL;
	switch(param->code){
		case TVN_SELCHANGED:
			data_ptr = (TREEVIEW_ITEM_DATA_PTR)(((LPNMTREEVIEW)param)->itemNew.lParam);
			if(data_ptr == NULL) return TRUE;
			switch(data_ptr->type_){
				case SEGD_GENERAL_HEADER_1_TYPE:
					break;
				case SEGD_GENERAL_HEADER_2_TYPE:
					break;
				case SEGD_GENERAL_HEADER_3_TYPE:
					break;
				case SEGD_SCAN_TYPE_HEADER_TYPE:
					break;
				case SEGD_EXTENDED_HEADER_TYPE:
					break;
				case SEGD_EXTERNAL_HEADER_TYPE:
					break;
				case SEGD_DEMUX_TRACE_HEADER_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_1_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_2_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_3_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_4_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_5_TYPE:
					break;
				case SEGD_TRACE_EXTENDED_HEADER_6_TYPE:
				case SEGD_TRACE_EXTENDED_HEADER_7_TYPE:
					break;
			}
			return TRUE;
		case LVN_ITEMCHANGED:
			OutputDebugString(__TEXT("TREEVIEW: LVN_SELCHANGED"));
			return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
		HANDLE_MSG(hWnd, WM_CREATE, on_create);
		HANDLE_MSG(hWnd, WM_COMMAND, on_command);
		HANDLE_MSG(hWnd, WM_PAINT, on_paint);
		HANDLE_MSG(hWnd, WM_DESTROY, on_destroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, on_notify);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
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

