/**
@brief	定义netd_socket_snd_thread类
@details	实现socket发送上位机数据
@file	netd.socket.snd.thread.cpp
@note	
@see	
@version	1.0.2.8.2012.02.15
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.socket.snd.thread.h"

netd_socket_snd_thread::netd_socket_snd_thread(netd_socket_service* socket_service_ptr)
												: socket_service_ptr_(socket_service_ptr)
{
	stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

netd_socket_snd_thread::~netd_socket_snd_thread()
{
	if(stop_event_ != NULL)	CloseHandle(stop_event_);
}

void netd_socket_snd_thread::stop()
{
	if(stop_event_ != NULL) SetEvent(stop_event_);
	if(!wait(100))	terminate();
}

void netd_socket_snd_thread::run()
{
	DWORD result = 0x0;
	int i = 0;
	unsigned int size = 0;
	inp_data i_d;
	SOCKADDR_IN remote_ip;

	SecureZeroMemory(&remote_ip, sizeof(remote_ip));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_addr.s_addr = socket_service_ptr_->application_ptr_->matrix_service_ip_;

	while(true){

		size = socket_service_ptr_->application_ptr_->inp_queue_->size();
		for(i = 0x0; i < size; ++i){
			if(socket_service_ptr_->application_ptr_->inp_queue_->pop(i_d)){

			//	memcpy(&remote_ip.sin_addr.s_addr, i_d.buf_ + netd_application::DEST_IP_BEGIN_POS, sizeof(unsigned int));
				memcpy(&remote_ip.sin_port, i_d.buf_ + netd_application::DEST_PORT_BEGIN_POS, sizeof(unsigned short));
				remote_ip.sin_port = htons(remote_ip.sin_port + netd_application::ADDITION_PORT_VALUE);
				socket_service_ptr_->send((char*)i_d.buf_, sizeof(i_d.buf_), (SOCKADDR*)&remote_ip);
				InterlockedIncrement(&socket_service_ptr_->application_ptr_->socket_data_sent_num_);
			}
			else break;
		}

		//wait...
		result = WaitForSingleObject(stop_event_, socket_service_ptr_->application_ptr_->netd_snd_poll_time_);
		if(result == WAIT_OBJECT_0)	break;
	}
}
