MAX_TIMEOUT_SIZE
MAX_TIMEOUT_SIZE
MAX_TIMEOUT_SIZE
// line.worker.thread.cpp : implementation file
#include "stdafx.h"
#include "line.h"
#include "line.worker.thread.h"
#include "line.socket.impl.h"

// line_worker_thread
extern CLineApp theApp;

IMPLEMENT_DYNCREATE(line_worker_thread, CWinThread)

line_worker_thread::line_worker_thread() : complete_token_event_(new CEvent(FALSE, TRUE)), socket_(NULL)
{
	
}

line_worker_thread::~line_worker_thread()
{
	if(complete_token_event_ != NULL){
		delete complete_token_event_;
		complete_token_event_ = NULL;
	}
}

BOOL line_worker_thread::InitInstance()
{
	AfxSocketInit ();
	socket_ = new line_socket_impl(theApp.command_line_info_.ip_,  theApp.command_line_info_.port_, theApp.command_line_info_.recv_buff_size_);
	return TRUE;
}

int line_worker_thread::ExitInstance()
{
	if(socket_ != NULL){
		delete socket_;
		socket_ = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(line_worker_thread, CWinThread)

	ON_THREAD_MESSAGE(WM_LOGIN_SERVER, login_server_resp)
END_MESSAGE_MAP()

void line_worker_thread::login_server_resp( WPARAM w_param, LPARAM l_param)
{
	if(w_param = true) complete_token_event_->SetEvent();
}

int line_worker_thread::Run()
{
	//显示提示对话框
	theApp.show_dialog_.SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE  | SWP_NOSIZE);
	theApp.show_dialog(IDS_STRING139);

	//连接到服务器
	if(!socket_->login_server() ){
		theApp.show_dialog(IDS_STRING102, SW_HIDE,  line_socket_impl::MAX_TIMEOUT_SIZE );
		Sleep(line_socket_impl::MAX_TIMEOUT_SIZE );
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_QUIT, NULL, NULL);
	}

	DWORD result = WaitForSingleObject(complete_token_event_->m_hObject, line_socket_impl::MAX_TIMEOUT_SIZE * 100000);
	switch(result){
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		theApp.show_dialog(IDS_STRING102, SW_HIDE,  line_socket_impl::MAX_TIMEOUT_SIZE );
		Sleep(line_socket_impl::MAX_TIMEOUT_SIZE );
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_QUIT, NULL, NULL);
		goto GOTO;
	}

	//得到测区大小

GOTO:
	return CWinThread::Run();
}
