/**
@brief	该文件主要实现接受SEG-D写文件过程中,收到进度通知消息
@details 收到segd.conv.exe发出的udp数据包,显示进度信息显示在对话框中
@file	segd.api.dialog.cpp
@note	使用wchar_t
@see	
@version	1.0.11.42.2011.10.14
@author	kuoxin	
@bug
	1.0.3.24.2011.09.21		修改弹出窗体进度条无效问题和修正其中错误
	1.0.10.40.2011.09.21	改正测试驱动程序不能退出错误,实现了2种方式对于该问题的修改方法
							1. 使用线程
							2. 使用peekmessage方式
	1.0.11.42.2011.10.14	静态代码分析,修改错误
	
*/
#include "../../common/config.h"
#include "segd.api.h"
#include "segd.api.dialog.h"
#include <shlwapi.h>
#include <commctrl.h>
#include "resource.h"
#include "..\..\common\sockimpl.h"
#include "windef.h"


#define WM_SOCKET_MESSAGE	(WM_USER + 10) //!< 定义socket消息

extern HINSTANCE __instance;
//HWND	__progress_window = NULL; //!< 显示进度对话框句柄
SOCKET_IMPL __socket_impl;	//!< 接受UDP通知socket数据结构

bool init_dialog_private(HWND, SOCKET_IMPL_PTR);
INT_PTR CALLBACK dialog_proc_private(HWND, UINT, WPARAM, LPARAM);

#if defined(__THREAD_METHOD__)

typedef struct __tagTHREAD_PROC_PARAM
{
	unsigned int listen_port_; 
	HWND parent_window_handler_; 
	char_t* window_title_;
}THREAD_PROC_PARAM, *THREAD_PROC_PARAM_PTR;

THREAD_PROC_PARAM __param;

unsigned __stdcall thread_proc(void* param_ptr)
{
	THREAD_PROC_PARAM_PTR ptr = (THREAD_PROC_PARAM_PTR)param_ptr;
	BOOL is_ok = TRUE;
	int size = 0;
	int i_ret = APP_SUCCESS;
	char_t buf[MAX_STRING_SIZE] = {0};
	MSG msg;
	HWND	progress_window = NULL;

	progress_window = FindWindow(__TEXT("#32770"), ptr->window_title_);
	if(progress_window != NULL){
		if(IsWindow(progress_window)){

			size = GetWindowText(progress_window, buf, sizeof(buf)/sizeof(buf[0]));
			buf[size] = 0x0;
			set_last_error(IDS_STRING5015);
			output_log(IDS_STRING5015, buf);
			ShowWindow(progress_window, SW_SHOW);
			i_ret = APP_FAULT;
			goto GOTO;
		}
	}

	__socket_impl.listen_port_ = ptr->listen_port_;
	progress_window = CreateDialogParam(__instance, MAKEINTRESOURCE(IDD_DIALOG1), ptr->parent_window_handler_, 
										dialog_proc_private, (LPARAM)&__socket_impl);
	if(progress_window == NULL){
		set_last_error(IDS_STRING5021);
		output_log(IDS_STRING5021);
		return i_ret = APP_FAULT;
	}

	SetWindowText(progress_window, ptr->window_title_);
	ShowWindow(progress_window, SW_SHOW);
	UpdateWindow(progress_window);

	while((is_ok = GetMessage(&msg, progress_window, 0, 0)) != 0){

		if(is_ok == -1){	break;}
		else{ 
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
GOTO:
	_endthreadex(i_ret);
	return i_ret;
}

#endif //__THREAD_METHOD__

int show_writing_progress_private(unsigned int listen_port, HWND parent_window_handler, wchar_t const* window_title)
{
#if defined(__THREAD_METHOD__)
	__param.listen_port_ = listen_port;
	__param.parent_window_handler_ = parent_window_handler;
	__param.window_title_ = const_cast<char_t*>(window_title);
	CloseHandle((HANDLE)_beginthreadex(NULL, 0, thread_proc, &__param, 0, NULL));

#else
	BOOL is_ok = TRUE;
	int size = 0;
	char_t buf[MAX_STRING_SIZE] = {0};
	MSG msg;
	HWND	progress_window = NULL;

	progress_window = FindWindow(__TEXT("#32770"), window_title);
	if(progress_window != NULL){
		if(IsWindow(progress_window)){

			size = GetWindowText(progress_window, buf, sizeof(buf)/sizeof(buf[0]));
			buf[size] = 0x0;
			set_last_error(IDS_STRING5015);
			output_log(IDS_STRING5015, buf);
			ShowWindow(progress_window, SW_SHOW);
			return APP_FAULT;
		}
	}

	__socket_impl.listen_port_ = listen_port;
	progress_window = CreateDialogParam(__instance, MAKEINTRESOURCE(IDD_DIALOG1), parent_window_handler, 
		dialog_proc_private, (LPARAM)&__socket_impl);
	if(progress_window == NULL){
		set_last_error(IDS_STRING5021);
		output_log(IDS_STRING5021);
		return APP_FAULT;
	}

	SetWindowText(progress_window, window_title);
	ShowWindow(progress_window, SW_SHOW);
	UpdateWindow(progress_window);

	while(PeekMessage(&msg, (HWND)-1, 0, 0, PM_REMOVE)){

		if(!IsDialogMessage(progress_window, &msg)){
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
		else{
			PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		}
	}
#endif //__THREAD_METHOD__
	return APP_SUCCESS;
}

bool init_dialog_private(HWND dialog_handler, SOCKET_IMPL_PTR socket_impl_ptr)
{
	bool is_ok = true;
	if(socket_impl_ptr->socket_ != NULL)	close_udp_socket(socket_impl_ptr);
	
	if(!create_udp_socket(socket_impl_ptr, NULL)){	
		set_last_error(IDS_STRING5019);
		output_log(IDS_STRING5019, WSAGetLastError());
		return is_ok = false;
	}

	async_select(socket_impl_ptr, dialog_handler, WM_SOCKET_MESSAGE, FD_READ);

	return is_ok;
}

BOOL close_dialog_private(HWND dialog_handler)
{
	async_select(&__socket_impl, dialog_handler, 0, 0);
	close_udp_socket(&__socket_impl);
	memset(&__socket_impl, 0x0, sizeof(__socket_impl));
	return DestroyWindow(dialog_handler);
}

void show_progress_info_private(HWND dialog_handler, char const* recv_buf, int recv_size)
{
	int step_postion = 0x0;
	int range = 0x0;
	static int prev_range = 0x0;
	int descrip_size = 0x0;
	char_t descrip[MAX_STRING_SIZE] = {NULL};
	int size = sizeof(descrip) + sizeof(int) * 3;

	if(recv_size > size){ set_last_error(IDS_STRING5023); output_log(IDS_STRING5023);	return; }


	CopyMemory(&step_postion, recv_buf, sizeof(step_postion));
	step_postion = ntohl(step_postion);

	CopyMemory(&range, recv_buf + sizeof(step_postion), sizeof(range));
	range = ntohl(range);

	CopyMemory(&descrip_size, recv_buf + sizeof(step_postion) * 2, sizeof(descrip_size));
	descrip_size = ntohl(descrip_size);

	if(recv_size < descrip_size){

		set_last_error(IDS_STRING5022);
		output_log(IDS_STRING5022);
		return;
	}

	descrip_size = descrip_size > sizeof(descrip) ? sizeof(descrip): descrip_size;
	CopyMemory(descrip, recv_buf + sizeof(step_postion) * 3, descrip_size * sizeof(char_t));
	descrip[descrip_size] = 0x0;

	SetWindowText(GetDlgItem(dialog_handler, IDC_PROGRESS_INFO_STATIC), descrip);
	if(prev_range != range){
		SendMessage(GetDlgItem(dialog_handler, IDC_PROGRESS1), PBM_SETRANGE, 0, MAKELPARAM(0, range));
		prev_range = range;
	}

	SendMessage(GetDlgItem(dialog_handler, IDC_PROGRESS1), PBM_SETPOS, step_postion, 0);
}

void on_receive_package_private(HWND dialog_handler, SOCKET_IMPL_PTR sock_impl_ptr, LPARAM lparam)
{
	int recv_size = 0x0;
	char recv_buf[MAX_PACK_NET_SIZE] = {0};
	int error_code = WSAGETSELECTERROR(lparam);
	fd_set fds;
	int ready;
	timeval timeout = {0};

	if(WSAGETSELECTEVENT(lparam) == FD_READ){

		FD_ZERO(&fds);
		FD_SET(sock_impl_ptr->socket_, &fds);
		ready = select(0, &fds, NULL, NULL, &timeout);
		if(ready == SOCKET_ERROR)	error_code = WSAGetLastError();

		if(ready == 1){
			memset(recv_buf, 0x0, sizeof(recv_buf));
			recv_size = recv(sock_impl_ptr->socket_, recv_buf, sizeof(recv_buf), 0);
			if(recv_size > 0)	show_progress_info_private(dialog_handler, recv_buf, recv_size);
		}
	}	
}

INT_PTR CALLBACK dialog_proc_private(HWND dialog_handler, UINT window_message, WPARAM w_param, LPARAM l_param)
{
	int size = 0;
	char_t buf[MAX_STRING_SIZE] = {0};

	switch(window_message){
		case WM_INITDIALOG: 
			if(!init_dialog_private(dialog_handler, (SOCKET_IMPL_PTR)l_param)){
				size = GetWindowText(dialog_handler, buf, sizeof(buf)/sizeof(buf[0]));
				buf[size] = 0x0;
				set_last_error(IDS_STRING5020); 
				output_log(IDS_STRING5020, buf);
			}
			return TRUE; 
		case WM_COMMAND: 
			if(LOWORD(w_param) == IDOK) PostMessage(dialog_handler, WM_CLOSE, NULL, NULL); 
			return TRUE;
		case WM_SOCKET_MESSAGE:
			on_receive_package_private(dialog_handler, &__socket_impl, l_param);
			return TRUE;
		case WM_CLOSE:
			return close_dialog_private(dialog_handler);
	} 
	return FALSE; 
}

