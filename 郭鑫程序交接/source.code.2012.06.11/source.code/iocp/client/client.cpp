#include "stdafx.h"
#include "client.h"
#include <WinSock2.h>
#include <Mswsock.h>

#include "resource.h"

#pragma comment(lib, "ws2_32")

#define MAIN_TITLE "client.1.0.1.10.2012.02.24"
#define HANDLE_DIALOG_MESSAGE(hwnd, message, function_proc)	\
	case (message): return (SetDlgMsgResult(hwnd, message, HANDLE_##message((hwnd), (wParam), (lParam), (function_proc))))

enum 
{
	MAX_STRING_SIZE	= 260,
	MAX_PACK_NET_SIZE = 1460,	
	INPUT_PACKAGE_SIZE = 128,
	OUTPUT_PACKAGE_SIZE = 128,
	MAX_THREAD_SIZE = 256
};

typedef struct __tagIP_HEADER
{
	int size;//数据包大小
	char* data_; //数据包
}IP_HEADER;

HINSTANCE instance_;// current instance
HWND main_window_;

//sock
char listen_ip_[MAX_STRING_SIZE] = "192.168.0.25"; 
char remote_ip_[MAX_STRING_SIZE] = "192.168.0.19";
unsigned short listen_port_ = 0x9002;
unsigned short remote_port_ = 0x9999;
unsigned recv_buff_size_ = 1024 * 1024;
unsigned send_buff_size_ = 1024 * 1024;
unsigned recv_poll_time_ = 10;
unsigned snd_waiting_time_ = 1000;
unsigned snd_pack_num_per_loop_ = 1;

//UDP & PCI counter
LONG volatile upd_send_num_ = 0;
LONG volatile upd_recv_num_ = 0;

SOCKET __socket = NULL;

unsigned char input_data_[INPUT_PACKAGE_SIZE + 4] = {0};
unsigned char output_data_[OUTPUT_PACKAGE_SIZE] = {0};
unsigned int snd_thread_num_ = 5;

HANDLE dispath_thread_handle_ = NULL;
HANDLE snd_thread_handle_[MAX_THREAD_SIZE] = {NULL};
HANDLE stop_sending_event_ = NULL;
HANDLE	dispatchEvent_ = NULL;		/*	the event for dispatching message*/
HANDLE	socketEvent_ = NULL;

bool be_socket_initialize_ = false;
bool be_starting_recv_ = false;
bool be_starting_snd_ = false;

BOOL set_window_control(bool);

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	int len = 128;

	memset(input_data_, 0x0, sizeof(input_data_));
	memcpy(input_data_, &len, sizeof(int));

	input_data_[4] = 0x11;
	input_data_[5] = 0x22;
	input_data_[6] = 0x33;
	input_data_[7] = 0x44;

	// Initialize global strings
	instance_ = hInstance;

	main_window_ = CreateDialogParam(instance_, MAKEINTRESOURCE(IDD_MAIN_DIALOG),
		NULL, main_Wnd_proc, NULL);

	if(!main_window_){	return FALSE;}

	SetWindowText(main_window_, MAIN_TITLE);
	ShowWindow(main_window_, nCmdShow);
	UpdateWindow(main_window_);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)){

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

BOOL WINAPI main_Wnd_proc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
		HANDLE_DIALOG_MESSAGE(window_handle, WM_INITDIALOG, init_dialog);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_CLOSE, close_dialog);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_COMMAND, command_message);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_DESTROY, destroy_dialog);
	}

	return 0;
}

BOOL init_dialog(HWND window_handle, HWND default_window_handle, LPARAM param)
{
	bool is_ok = TRUE;
	WSADATA wsaData;

	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return false;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){

		WSACleanup();
		WSASetLastError (WSAVERNOTSUPPORTED);
		return false;
	}

	main_window_ = window_handle;

	set_window_control(false);

	return TRUE;
}

BOOL destroy_dialog(HWND window_handle)
{
	PostQuitMessage(0);
	return 0;
}

BOOL close_dialog(HWND window_handle)
{
	return DestroyWindow(window_handle);
}

BOOL command_message(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	BOOL is_ok = TRUE;
	is_ok = do_control(window_handle, resource_id, window_control_handle, notification_code);
	return is_ok;
}

BOOL do_control(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	BOOL is_ok = TRUE;

	switch(resource_id){
		case ID_START_BUTTON:
			start_socket_button_down();
			break;
		case ID_SND_BUTTON:
			snd_button_down();
			break;
		//case ID_RECV_BUTTON:
		//	recv_button_down();
		//	break;
	}

	return is_ok;
}

unsigned __stdcall udp_snd_thread_proc(void* args)
{
	int i  = 0;
	int result = 0x0;

	while(true){

		for(i = 0; i < snd_pack_num_per_loop_; ++i){

			send(__socket, (char*)input_data_, sizeof(input_data_), 0);
			InterlockedIncrement(&upd_send_num_);
		}

		if(WaitForSingleObject(stop_sending_event_, snd_waiting_time_) == WAIT_OBJECT_0) 
			break;
	}

	_endthreadex(0x0);
	return result;
}


unsigned __stdcall dispatch_worker_thread_proc(void* args)
{
	unsigned int result = 0;
	int recvSize = 0, curSize = 0, size = 0;
	char buf[1024] = { 0 }, *data = NULL;
	WSANETWORKEVENTS networkEvents = { 0 };

	// 等待事件队列
	HANDLE events[] = { socketEvent_, dispatchEvent_, 0x0 };

	while(true){

		// 等待SOCKET事件和退出事件
		result = WaitForMultipleObjects(2, events, FALSE, 30000);

		// 没有超时、判断网络事件和退出事件
		if(result != WAIT_TIMEOUT){

			// 得到等待结果
			result -= WAIT_OBJECT_0;
			switch(result)
			{
			case 0:
				{
					result = WSAEnumNetworkEvents(__socket, events[0], &networkEvents);

					if(result == SOCKET_ERROR) return 0x0;

					if(networkEvents.lNetworkEvents & FD_CLOSE){ return 0x0;}
					else if(networkEvents.lNetworkEvents & FD_READ){

						if(networkEvents.iErrorCode[FD_READ_BIT] == WSAENETDOWN) return 0x0;

						// 清空字节流计数器
						curSize = 0, size = 0;

						// 得到可接收字节数
						if(SOCKET_ERROR == ioctlsocket(__socket, FIONREAD, (u_long*)&size)) return 0x0;

						if(size < 1024)	data = buf;
						else			data = new char[size + 1];

						while(curSize < size){
							recvSize = recv(__socket, &data[curSize], size - curSize, 0);
							if(recvSize == SOCKET_ERROR || recvSize == 0){
								if(data != buf && 0 != data){
									delete []data; 
									data = NULL;
								}
								return 0x0;
							}
							curSize += recvSize;
						}

						bool receive_normal = true;
						receive_normal &= (curSize != SOCKET_ERROR);
						receive_normal &= (curSize != 0);

						if(receive_normal){
							if(!dispath_message(data, size)){
								if(data != buf && 0 != data){
									delete []data;
									data = NULL;
								}
								return 0x0;
							}
						}
						else{
							if(data != buf && 0 != data){
								delete []data;
								data = NULL;
							}
							return 0x0;
						}
					}/* end if(networkEvents.lNetworkEvents & FD_READ) */
				}
				break;
			case 1:
				goto GOTO;
			default:
				break;
			}/* end switch(result) */
		}/* end if(result != WAIT_TIMEOUT) */
	}/* end while(1) */

GOTO:
	_endthreadex(0x0);
	return WSAEventSelect(__socket, socketEvent_, NULL);
}

void start_socket_button_down()
{
	u_long arg = 1;
	sockaddr_in addr;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;

	if(!be_socket_initialize_){

		//reset the counter
		InterlockedExchange(&upd_recv_num_, 0);
		InterlockedExchange(&upd_send_num_, 0);

		//save the option-info
		set_window_control(true);

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(listen_ip_);
		addr.sin_port = htons(listen_port_);

		__socket = socket(AF_INET, SOCK_STREAM, 0);
		bind(__socket, (SOCKADDR*)&addr, sizeof(addr));

		setsockopt(__socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recv_poll_time_, sizeof(recv_poll_time_));
		setsockopt(__socket, SOL_SOCKET, SO_RCVBUF, (const char*)&recv_buff_size_, sizeof(recv_buff_size_));
		setsockopt(__socket, SOL_SOCKET, SO_SNDBUF, (const char*)&send_buff_size_, sizeof(send_buff_size_));

		addr.sin_addr.s_addr = inet_addr(remote_ip_);
		addr.sin_port = htons(remote_port_);
		connect(__socket, (sockaddr*)&addr, sizeof(addr));

		arg = 1;
		ioctlsocket(__socket, FIONBIO, &arg);

		dispatchEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
		socketEvent_ = WSACreateEvent();
		WSAEventSelect(__socket, socketEvent_, FD_READ | FD_CLOSE);

		dispath_thread_handle_ = (HANDLE)_beginthreadex(NULL, 0x0, dispatch_worker_thread_proc, NULL, 0x0, NULL);

		be_socket_initialize_ = true;
		SetWindowText(GetDlgItem(main_window_, ID_START_BUTTON), "释放SOCKET");
		SetTimer(main_window_, IDT_GET_COUNTER, 1000, timer_proc);
	}
	else{

		KillTimer(main_window_, IDT_GET_COUNTER);

		if(dispatchEvent_){

			SetEvent(dispatchEvent_);
			WaitForSingleObject(dispath_thread_handle_, 100);
			CloseHandle(dispatchEvent_);
			dispatchEvent_ = NULL;

			CloseHandle(dispath_thread_handle_);
			dispath_thread_handle_ = NULL;
		}

		if(socketEvent_){
			WSACloseEvent(socketEvent_);
			socketEvent_ = NULL;
		}

		if(__socket != INVALID_SOCKET){

			LINGER lg = { 1, 0 };
			setsockopt(__socket, SOL_SOCKET, SO_LINGER, (char*)&lg, sizeof(LINGER));
			closesocket(__socket);
			__socket = INVALID_SOCKET;
		}	

		be_socket_initialize_ = false;
		SetWindowText(GetDlgItem(main_window_, ID_START_BUTTON), "初始化SOCKET");
	}
}

VOID CALLBACK timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	char buf[MAX_STRING_SIZE] = {0};

	sprintf_s(buf, sizeof(buf), "%d", InterlockedCompareExchange(&upd_send_num_, 0, 0));
	SendMessage(GetDlgItem(hwnd, IDC_STATIC_UDP_SEND_NUM), WM_SETTEXT, NULL, (LPARAM)buf);

	sprintf_s(buf, sizeof(buf), "%d", InterlockedCompareExchange(&upd_recv_num_, 0, 0));
	SendMessage(GetDlgItem(hwnd, IDC_STATIC_UDP_RECV_NUM), WM_SETTEXT, NULL, (LPARAM)buf);
}

void snd_button_down()
{
	int i = 0x0;

	if(!be_starting_snd_){

		//save the option-info
		set_window_control(true);

		InterlockedExchange(&upd_send_num_, 0);

		stop_sending_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
		for(i = 0; i < snd_thread_num_; ++i)
			snd_thread_handle_[i] = (HANDLE)_beginthreadex(NULL, 0, udp_snd_thread_proc, NULL, 0x0, NULL);

		be_starting_snd_ = true;
		SetWindowText(GetDlgItem(main_window_, ID_SND_BUTTON), "结束发送数据");
	}
	else{

		SetEvent(stop_sending_event_);
		WaitForMultipleObjects(snd_thread_num_, snd_thread_handle_, TRUE, 100);

		CloseHandle(stop_sending_event_);
		stop_sending_event_ = NULL;

		for(i = 0; i < snd_thread_num_; ++i){
			CloseHandle(snd_thread_handle_[i]);
			snd_thread_handle_[i] = NULL;
		}

		be_starting_snd_ = false;
		SetWindowText(GetDlgItem(main_window_, ID_SND_BUTTON), "开始发送数据");
	}
}

BOOL set_window_control(bool is_save)
{
	char buf[MAX_STRING_SIZE] = {0};

	if(is_save){

		SendDlgItemMessage(main_window_, IDC_EDIT_LISTEN_PORT, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		listen_port_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_REMOTE_IP, WM_GETTEXT, 
			sizeof(remote_ip_), (LPARAM)remote_ip_);

		SendDlgItemMessage(main_window_, IDC_EDIT_LISTEN_IP, WM_GETTEXT, 
			sizeof(listen_ip_), (LPARAM)listen_ip_);

		SendDlgItemMessage(main_window_, IDC_EDIT_REMOTE_PORT, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		remote_port_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_RECV_NUM, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		recv_buff_size_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SEND_NUM, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		send_buff_size_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_POLL_TIME, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		recv_poll_time_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SND_NUM_PER_HALF_HUNDRAND, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		snd_pack_num_per_loop_ = atoi(buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SND_POLL_TIME, WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		snd_waiting_time_ = atoi(buf);

	}
	else{

		sprintf_s(buf, sizeof(buf), "%d", listen_port_);
		SendDlgItemMessage(main_window_, IDC_EDIT_LISTEN_PORT, WM_SETTEXT, NULL, (LPARAM)buf);

		SendDlgItemMessage(main_window_, IDC_EDIT_REMOTE_IP, WM_SETTEXT, NULL, (LPARAM)remote_ip_);

		SendDlgItemMessage(main_window_, IDC_EDIT_LISTEN_IP, WM_SETTEXT, NULL, (LPARAM)listen_ip_);

		sprintf_s(buf, sizeof(buf), "%d", remote_port_);
		SendDlgItemMessage(main_window_, IDC_EDIT_REMOTE_PORT, WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", recv_buff_size_);
		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_RECV_NUM, WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", send_buff_size_);
		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SEND_NUM, WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", recv_poll_time_);
		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_POLL_TIME, WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", snd_pack_num_per_loop_);
		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SND_NUM_PER_HALF_HUNDRAND, WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", snd_waiting_time_);
		SendDlgItemMessage(main_window_, IDC_EDIT_SOCK_SND_POLL_TIME, WM_SETTEXT, NULL, (LPARAM)buf);
	}

	return TRUE;
}

bool dispath_message(char const* ptr, int size)
{
	return true;
}