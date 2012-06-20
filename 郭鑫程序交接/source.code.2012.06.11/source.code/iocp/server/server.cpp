#include "stdafx.h"
#include "server.h"
#include <WinSock2.h>
#include <Mswsock.h>
#include <strsafe.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock")

#define MAIN_TITLE "server.1.0.1.10.2012.02.24"

#define HANDLE_DIALOG_MESSAGE(hwnd, message, function_proc)	\
	case (message): return (SetDlgMsgResult(hwnd, message, HANDLE_##message((hwnd), (wParam), (lParam), (function_proc))))

enum 
{
	MAX_STRING_SIZE	= 260,
	MAX_PACK_NET_SIZE = 1460,	
	INPUT_PACKAGE_SIZE = 128,
	OUTPUT_PACKAGE_SIZE = 128,
	MAX_RECV_THREAD_SIZE = 256,
	MAX_SEND_THREAD_SIZE = 256
};

typedef enum _IO_OPERATION {
	ClientIoAccept,
	ClientIoRead,
	ClientIoWrite
} IO_OPERATION, *PIO_OPERATION;

typedef struct __tagIO_CONTEXT 
{
	WSAOVERLAPPED               overlapped_;
	char                        buffer_[8192];
	WSABUF                      wsa_buf_;
	int                         total_bytes_;
	int                         sent_bytes_;
	IO_OPERATION                io_operation_;
	SOCKET                      socket_;
	struct __tagIO_CONTEXT      *forward_;
} IO_CONTEXT, *IO_CONTEXT_PTR;

typedef struct __tagSOCKET_CONTEXT
{
	SOCKET                      socket_;
	IO_CONTEXT_PTR             io_context_ptr_;
	struct __tagSOCKET_CONTEXT  *back_; 
	struct __tagSOCKET_CONTEXT  *forward_;
} SOCKET_CONTEXT, *SOCKET_CONTEXT_PTR;

HINSTANCE instance_;// current instance
HWND main_window_;

SOCKET_CONTEXT_PTR __head = NULL;
SOCKET_CONTEXT_PTR __current = NULL;

//sock
char listen_ip_[MAX_STRING_SIZE] = "192.168.0.25"; 
char remote_ip_[MAX_STRING_SIZE] = "192.168.0.19";
unsigned short listen_port_ = 0x9002;
unsigned short remote_port_ = 0x9999;

unsigned recv_buff_size_ = 1024 * 1024 * 10;
unsigned send_buff_size_ = 1024 * 1024 * 10;

unsigned recv_poll_time_ = 10;
unsigned snd_waiting_time_ = 1000;

unsigned snd_pack_num_per_loop_ = 1;

//UDP & PCI counter
LONG volatile upd_send_num_ = 0;
LONG volatile upd_recv_num_ = 0;

SOCKET __socket = NULL;

unsigned char input_data_[INPUT_PACKAGE_SIZE] = {0};
unsigned char output_data_[OUTPUT_PACKAGE_SIZE] = {0};

HANDLE recv_thread_handle_[MAX_RECV_THREAD_SIZE] = {NULL};
HANDLE stop_receiving_event_ = NULL;
HANDLE __iocp = NULL;
HANDLE start_thread_handle_ = NULL;
HANDLE stop_event_ = NULL;

bool be_socket_initialize_ = false;
bool be_starting_recv_ = false;
bool __be_end_server = false;

unsigned int recv_thread_num_ = 0x8;//线程数量不能超过MAX_RECV_THREAD_SIZE
CRITICAL_SECTION __cs;

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
//		case ID_RECV_BUTTON:
//			recv_button_down();
//			break;
	}

	return is_ok;
}

SOCKET_CONTEXT_PTR allocate_context(SOCKET sd, IO_OPERATION client_io)
{
	SOCKET_CONTEXT_PTR context_ptr = NULL;

	EnterCriticalSection(&__cs);

	context_ptr = (SOCKET_CONTEXT_PTR)malloc(sizeof(SOCKET_CONTEXT));

	ZeroMemory(context_ptr, sizeof(SOCKET_CONTEXT));
	context_ptr->io_context_ptr_ = (IO_CONTEXT_PTR)malloc(sizeof(IO_CONTEXT));
	ZeroMemory(context_ptr->io_context_ptr_, sizeof(IO_CONTEXT));
	
	context_ptr->socket_ = sd;
	context_ptr->io_context_ptr_->socket_ = INVALID_SOCKET;
	context_ptr->io_context_ptr_->wsa_buf_.buf = context_ptr->io_context_ptr_->buffer_;
	context_ptr->io_context_ptr_->wsa_buf_.len = sizeof(context_ptr->io_context_ptr_->buffer_);
	context_ptr->io_context_ptr_->io_operation_ = client_io; 	

	LeaveCriticalSection(&__cs);
	return context_ptr;
}

void add_to_list(SOCKET_CONTEXT_PTR context_ptr)
{
	EnterCriticalSection(&__cs);

	if(__current){

		context_ptr->back_ = NULL;
		context_ptr->forward_ = __current;
		__current->back_ = context_ptr;
		__current = context_ptr;
	}
	else{
		__current = context_ptr;
		context_ptr->forward_ = NULL;
		context_ptr->back_ = NULL;
	}

	LeaveCriticalSection(&__cs);
}

SOCKET_CONTEXT_PTR update_completion_port(SOCKET sd, IO_OPERATION ClientIo, bool be_add_to_list)	
{
	SOCKET_CONTEXT_PTR context_ptr;

	context_ptr = allocate_context(sd, ClientIo);
	__iocp = CreateIoCompletionPort((HANDLE)sd, __iocp, (DWORD_PTR)context_ptr, 0);
	if( be_add_to_list ) add_to_list(context_ptr);

	return context_ptr;
}

SOCKET create_socket()
{
	SOCKET sd;
	int val = 0;
	sd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0x0, WSA_FLAG_OVERLAPPED);
	setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char*)&val, sizeof(val));
	return sd;
}

bool create_accept_socket(bool be_updated) 
{
	int nRet = 0;
	DWORD dwRecvNumBytes = 0;

	if(be_updated)
		__head = update_completion_port(__socket, ClientIoAccept, FALSE);

	__head->io_context_ptr_->socket_ = create_socket();
	AcceptEx(__socket, __head->io_context_ptr_->socket_, __head->io_context_ptr_->buffer_, 
		sizeof(__head->io_context_ptr_->buffer_) - (2 * (sizeof(SOCKADDR_STORAGE) + 16)),
		sizeof(SOCKADDR_STORAGE) + 16, sizeof(SOCKADDR_STORAGE) + 16, 
		&dwRecvNumBytes, (LPOVERLAPPED)&__head->io_context_ptr_->overlapped_);
	return true;
}

void delete_from_list(SOCKET_CONTEXT_PTR lpPerSocketContext)	{

	SOCKET_CONTEXT_PTR pBack;
	SOCKET_CONTEXT_PTR pForward;
	IO_CONTEXT_PTR     pNextIO     = NULL;
	IO_CONTEXT_PTR     pTempIO     = NULL;

	EnterCriticalSection(&__cs);

	if( lpPerSocketContext ) {
		pBack       = lpPerSocketContext->back_;
		pForward    = lpPerSocketContext->forward_;

		if( pBack == NULL && pForward == NULL ) {

			//
			// This is the only node in the list to delete
			//
			__current = NULL;
		} else if( pBack != NULL && pForward == NULL ) {

			//
			// This is the start node in the list to delete
			//
			pForward->back_ = NULL;
			__current = pForward;
		} else if( pBack == NULL && pForward != NULL ) {

			//
			// This is the end node in the list to delete
			//
			pBack->forward_ = NULL;
		} else if( pBack && pForward ) {

			//
			// Neither start node nor end node in the list
			//
			pBack->forward_ = pForward;
			pForward->back_ = pBack;
		}

		//
		// Free all i/o context structures per socket
		//
		pTempIO = (IO_CONTEXT_PTR)(lpPerSocketContext->io_context_ptr_);
		do {
			pNextIO = (IO_CONTEXT_PTR)(pTempIO->forward_);
			if( pTempIO ) {

				//
				//The overlapped structure is safe to free when only the posted i/o has
				//completed. Here we only need to test those posted but not yet received 
				//by PQCS in the shutdown process.
				//
				if( __be_end_server )
					while( !HasOverlappedIoCompleted((LPOVERLAPPED)pTempIO) ) Sleep(0);
				delete pTempIO;
				pTempIO = NULL;
			}
			pTempIO = pNextIO;
		} while( pNextIO );

		delete lpPerSocketContext;
		lpPerSocketContext = NULL;
	} 

	LeaveCriticalSection(&__cs);

	return;
}


void close_client (SOCKET_CONTEXT_PTR lpPerSocketContext, BOOL bGraceful)	
{

	EnterCriticalSection(&__cs);

	if( lpPerSocketContext ) {
		if( !bGraceful ) {

			//
			// force the subsequent closesocket to be abortative.
			//
			LINGER  lingerStruct;

			lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 0;
			setsockopt(lpPerSocketContext->socket_, SOL_SOCKET, SO_LINGER,
				(char *)&lingerStruct, sizeof(lingerStruct) );
		}

		if( lpPerSocketContext->io_context_ptr_->socket_ != INVALID_SOCKET ) {
			closesocket(lpPerSocketContext->io_context_ptr_->socket_);
			lpPerSocketContext->io_context_ptr_->socket_ = INVALID_SOCKET;
		};

		closesocket(lpPerSocketContext->socket_);
		lpPerSocketContext->socket_ = INVALID_SOCKET;
		delete_from_list(lpPerSocketContext);
		lpPerSocketContext = NULL;
	} 

	LeaveCriticalSection(&__cs);

	return;    
} 

VOID free_list() 
{
	SOCKET_CONTEXT_PTR pTemp1, pTemp2;


	EnterCriticalSection(&__cs);

	pTemp1 = __current; 
	while( pTemp1 ) {
		pTemp2 = pTemp1->back_;
		close_client(pTemp1, FALSE);
		pTemp1 = pTemp2;
	}

	LeaveCriticalSection(&__cs);

	return;
}


unsigned __stdcall recv_thread_proc(void* args)
{
	int result = 0x0;

	HANDLE hIOCP = __iocp;
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	SOCKET_CONTEXT_PTR lpPerSocketContext = NULL;
	SOCKET_CONTEXT_PTR lpAcceptSocketContext = NULL;
	IO_CONTEXT_PTR lpIOContext = NULL; 
	WSABUF buffRecv;
	WSABUF buffSend;
	DWORD dwRecvNumBytes = 0;
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwIoSize = 0;
	HRESULT hRet;

	while( TRUE ) {

		//
		// continually loop to service io completion packets
		//
		bSuccess = GetQueuedCompletionStatus(
			hIOCP,
			&dwIoSize,
			(PDWORD_PTR)&lpPerSocketContext,
			(LPOVERLAPPED *)&lpOverlapped,
			INFINITE 
			);

		if( lpPerSocketContext == NULL )	return(0);
		if( __be_end_server ) 	return(0);

		lpIOContext = (IO_CONTEXT_PTR)lpOverlapped;

		//
		//We should never skip the loop and not post another AcceptEx if the current
		//completion packet is for previous AcceptEx
		//
		if( lpIOContext->io_operation_ != ClientIoAccept ) {
			if( !bSuccess || (bSuccess && (0 == dwIoSize)) ) {

				//
				// client connection dropped, continue to service remaining (and possibly 
				// new) client connections
				//
				close_client(lpPerSocketContext, FALSE); 
				continue;
			}
		}

		//
		// determine what type of IO packet has completed by checking the PER_IO_CONTEXT 
		// associated with this socket.  This will determine what action to take.
		//
		switch( lpIOContext->io_operation_ ) {
		case ClientIoAccept:

			//
			// When the AcceptEx function returns, the socket sAcceptSocket is 
			// in the default state for a connected socket. The socket sAcceptSocket 
			// does not inherit the properties of the socket associated with 
			// sListenSocket parameter until SO_UPDATE_ACCEPT_CONTEXT is set on 
			// the socket. Use the setsockopt function to set the SO_UPDATE_ACCEPT_CONTEXT 
			// option, specifying sAcceptSocket as the socket handle and sListenSocket 
			// as the option value. 
			//
			nRet = setsockopt(
				lpPerSocketContext->io_context_ptr_->socket_, 
				SOL_SOCKET,
				SO_UPDATE_ACCEPT_CONTEXT,
				(char *)&__socket,
				sizeof(__socket)
				);

			if( nRet == SOCKET_ERROR ) {
				WSASetEvent(stop_event_);
				return(0);
			}

			lpAcceptSocketContext = update_completion_port(
				lpPerSocketContext->io_context_ptr_->socket_, 
				ClientIoAccept, TRUE);

			if( lpAcceptSocketContext == NULL ) {

				WSASetEvent(stop_event_);
				return(0);
			}

			if( dwIoSize ) {
				lpAcceptSocketContext->io_context_ptr_->io_operation_ = ClientIoWrite;
				lpAcceptSocketContext->io_context_ptr_->total_bytes_  = dwIoSize;
				lpAcceptSocketContext->io_context_ptr_->sent_bytes_   = 0;
				lpAcceptSocketContext->io_context_ptr_->wsa_buf_.len   = dwIoSize;
				hRet = StringCbCopyN(lpAcceptSocketContext->io_context_ptr_->buffer_,
					sizeof(lpAcceptSocketContext->io_context_ptr_->buffer_),
					lpPerSocketContext->io_context_ptr_->buffer_,
					sizeof(lpPerSocketContext->io_context_ptr_->buffer_)
					);
				lpAcceptSocketContext->io_context_ptr_->wsa_buf_.buf = lpAcceptSocketContext->io_context_ptr_->buffer_;

				nRet = WSASend(
					lpPerSocketContext->io_context_ptr_->socket_,
					&lpAcceptSocketContext->io_context_ptr_->wsa_buf_, 1,
					&dwSendNumBytes,
					0,
					&(lpAcceptSocketContext->io_context_ptr_->overlapped_), NULL);

				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {
					close_client(lpAcceptSocketContext, FALSE);
				}
			} 
			else {

				//
				// AcceptEx completes but doesn't read any data so we need to post
				// an outstanding overlapped read.
				//
				lpAcceptSocketContext->io_context_ptr_->io_operation_ = ClientIoRead;
				dwRecvNumBytes = 0;
				dwFlags = 0;
				buffRecv.buf = lpAcceptSocketContext->io_context_ptr_->buffer_,
					buffRecv.len = sizeof(lpAcceptSocketContext->io_context_ptr_->buffer_);
				nRet = WSARecv(
					lpAcceptSocketContext->socket_,
					&buffRecv, 1,
					&dwRecvNumBytes,
					&dwFlags,
					&lpAcceptSocketContext->io_context_ptr_->overlapped_, NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {
					close_client(lpAcceptSocketContext, FALSE);
				}
			}

			//
			//Time to post another outstanding AcceptEx
			//
			if( !create_accept_socket(FALSE) ) {
				WSASetEvent(stop_event_);
				return(0);
			}
			break;


		case ClientIoRead:

			//
			// a read operation has completed, post a write operation to echo the
			// data back to the client using the same data buffer.
			//
			lpIOContext->io_operation_ = ClientIoWrite;
			lpIOContext->total_bytes_ = dwIoSize;
			lpIOContext->sent_bytes_  = 0;
			lpIOContext->wsa_buf_.len  = dwIoSize;
			dwFlags = 0;
			nRet = WSASend(
				lpPerSocketContext->socket_,
				&lpIOContext->wsa_buf_, 1, &dwSendNumBytes,
				dwFlags,
				&(lpIOContext->overlapped_), NULL);
			if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {
				close_client(lpPerSocketContext, FALSE);
			}
			break;

		case ClientIoWrite:

			//
			// a write operation has completed, determine if all the data intended to be
			// sent actually was sent.
			//
			lpIOContext->io_operation_ = ClientIoWrite;
			lpIOContext->sent_bytes_  += dwIoSize;
			dwFlags = 0;
			if( lpIOContext->sent_bytes_ < lpIOContext->total_bytes_ ) {

				//
				// the previous write operation didn't send all the data,
				// post another send to complete the operation
				//
				buffSend.buf = lpIOContext->buffer_ + lpIOContext->sent_bytes_;
				buffSend.len = lpIOContext->total_bytes_ - lpIOContext->sent_bytes_;
				nRet = WSASend (
					lpPerSocketContext->socket_,
					&buffSend, 1, &dwSendNumBytes,
					dwFlags,
					&(lpIOContext->overlapped_), NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {

					close_client(lpPerSocketContext, FALSE);
				}
			} else {

				//
				// previous write operation completed for this socket, post another recv
				//
				lpIOContext->io_operation_ = ClientIoRead; 
				dwRecvNumBytes = 0;
				dwFlags = 0;
				buffRecv.buf = lpIOContext->buffer_,
					buffRecv.len = sizeof(lpIOContext->buffer_);
				nRet = WSARecv(
					lpPerSocketContext->socket_,
					&buffRecv, 1, &dwRecvNumBytes,
					&dwFlags,
					&lpIOContext->overlapped_, NULL);
			}
			break;

		} //switch
	} //while

	_endthreadex(0x0);
	return result;
}

unsigned __stdcall start_thread_proc(void* args)
{
	int result = 0x0;
	LINGER lingerStruct;
	struct sockaddr_in local_addr = {0};
	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	int i = 0;

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = listen_port_;
	local_addr.sin_addr.s_addr = inet_addr(listen_ip_);

	InitializeCriticalSection(&__cs);
	__be_end_server = false;

	for(i = 0; i < recv_thread_num_; ++i)
		recv_thread_handle_[i] = (HANDLE)_beginthreadex(NULL, 0x0, recv_thread_proc, NULL, 0x0, NULL);

	__socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
	setsockopt(__socket, SOL_SOCKET, SO_SNDBUF, (char *)&result, sizeof(result));
	bind(__socket, (sockaddr*)&local_addr, sizeof(local_addr));
	listen(__socket, 5);

	create_accept_socket(true);
	
	WaitForSingleObject(stop_event_, INFINITE);
	__be_end_server = true;

	for(i = 0; i < recv_thread_num_; ++i)
		PostQueuedCompletionStatus(__iocp, 0, 0, NULL);

	WaitForMultipleObjects(recv_thread_num_, recv_thread_handle_, TRUE, 100);
	for(i = 0; i < recv_thread_num_; ++i){

		CloseHandle(recv_thread_handle_[i]);
		recv_thread_handle_[i] = NULL;
	}

	DeleteCriticalSection(&__cs);
	_endthreadex(0x0);
	return result;
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

		__iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
		start_thread_handle_ = (HANDLE)_beginthreadex(NULL, 0x0, start_thread_proc, NULL, 0x0, NULL);

		be_socket_initialize_ = true;
		SetWindowText(GetDlgItem(main_window_, ID_START_BUTTON), "停止接收");
		SetTimer(main_window_, IDT_GET_COUNTER, 1000, timer_proc);
	}
	else{

		if(stop_event_){

			SetEvent(stop_event_);
			WaitForSingleObject(start_thread_handle_, 100);
			CloseHandle(start_thread_handle_);
			stop_event_ = NULL;
			start_thread_handle_ = NULL;
		}

		if(__iocp){	
			CloseHandle(__iocp);
			__iocp = NULL;
		}

		KillTimer(main_window_, IDT_GET_COUNTER);
		be_socket_initialize_ = false;
		SetWindowText(GetDlgItem(main_window_, ID_START_BUTTON), "开始接收");
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

//void recv_button_down()
//{
//	int i= 0;
//
//	if(!be_starting_recv_){
//
//		InterlockedExchange(&upd_recv_num_, 0);
//
//		//save the option-info
//		set_window_control(true);
//
//		stop_receiving_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//		for(i = 0; i < recv_thread_num_; ++i)
//			recv_thread_handle_[i] = (HANDLE)_beginthreadex(NULL, 0, udp_recv_thread_proc, NULL, 0x0, NULL);
//
//		be_starting_recv_ = true;
//
//		//set timer
//		//SetTimer(main_window_, IDT_RECV_TIMER, recv_poll_time_, recv_timer_proc);
//	//	SetWindowText(GetDlgItem(main_window_, ID_RECV_BUTTON), "停止接受数据");
//	}
//	else{
//
//		SetEvent(stop_receiving_event_);
//		WaitForMultipleObjects(recv_thread_num_, recv_thread_handle_, TRUE, 100);
//
//		CloseHandle(stop_receiving_event_);
//		stop_receiving_event_ = NULL;
//
//		for(i = 0; i < recv_thread_num_; ++i){
//
//			CloseHandle(recv_thread_handle_[i]);
//			recv_thread_handle_[i] = NULL;
//		}
//
//		be_starting_recv_ = false;
//		//KillTimer(main_window_, IDT_RECV_TIMER);
//		//SetWindowText(GetDlgItem(main_window_, ID_RECV_BUTTON), "开始接受数据");
//	}
//}

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
