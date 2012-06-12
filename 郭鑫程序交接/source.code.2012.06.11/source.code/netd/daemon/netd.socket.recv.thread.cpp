/**
@brief	定义netd_socket_recv_thread类
@details	实现socket接受和发送上位机数据
@file	netd.socket.recv.thread.cpp
@note	
@see	
@version	1.0.2.8.2012.02.14
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.socket.recv.thread.h"

netd_socket_recv_thread::netd_socket_recv_thread(netd_socket_service* socket_service_ptr)
												: socket_service_ptr_(socket_service_ptr)
{
	stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

netd_socket_recv_thread::~netd_socket_recv_thread()
{
	if(stop_event_ != NULL)	CloseHandle(stop_event_);
}

void netd_socket_recv_thread::stop()
{
	if(stop_event_ != NULL) SetEvent(stop_event_);
	if(!wait(100))	terminate();
}

void netd_socket_recv_thread::run()
{
	DWORD result = 0x0;

	while(true){

		//recv...
		socket_service_ptr_->recv(socket_service_ptr_);
		//wait...
		result = WaitForSingleObject(stop_event_, socket_service_ptr_->application_ptr_->netd_recv_poll_time_);
		if(result == WAIT_OBJECT_0)	break;
	}
}
