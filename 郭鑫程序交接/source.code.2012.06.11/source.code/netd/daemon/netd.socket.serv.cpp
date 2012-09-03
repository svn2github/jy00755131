/**
@brief	定义netd_socket_service类
@details	实现socket接受和发送上位机数据
@file	netd.socket.serv.cpp
@note	
@see	
@version	1.0.2.8.2012.02.14
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.socket.serv.h"

netd_socket_service::netd_socket_service(netd_application* application_ptr)
:application_ptr_(application_ptr)
{
	snd_thread_ptr_ = new netd_socket_snd_thread(this);
	recv_thread_ptr_ = new netd_socket_recv_thread(this);
}

netd_socket_service::~netd_socket_service()
{
	if(snd_thread_ptr_){
		delete snd_thread_ptr_;
		snd_thread_ptr_ = NULL;
	}

	if(recv_thread_ptr_){
		delete recv_thread_ptr_;
		recv_thread_ptr_ = NULL;
	}
}

bool netd_socket_service::start()
{
	bool is_ok = true;
	struct in_addr listen_ip;
	struct in_addr remote_ip;
	char l_ip[MAX_STRING_SIZE] = {0};
	char r_ip[MAX_STRING_SIZE] = {0};

	listen_ip.S_un.S_addr = application_ptr_->netd_ip_;
	remote_ip.S_un.S_addr = application_ptr_->matrix_service_ip_;

	strcpy(l_ip, inet_ntoa(listen_ip));
	strcpy(r_ip, inet_ntoa(remote_ip));

	is_ok = create(l_ip, ntohs(application_ptr_->netd_listen_port_),
				r_ip, ntohs(application_ptr_->matrix_service_listen_port_),
				application_ptr_->netd_recv_buffer_size_, application_ptr_->netd_snd_buffer_size_);
	if(!is_ok){
		application_ptr_->output_log("创建socket失败!");
		return is_ok;
	}

	snd_thread_ptr_->start();
	recv_thread_ptr_->start();

	return is_ok;
}

void netd_socket_service::stop()
{
	if(snd_thread_ptr_)		snd_thread_ptr_->stop();
	if(recv_thread_ptr_)	recv_thread_ptr_->stop();
	close();
}

bool netd_socket_service::recv_private(void* args, const char* buf, unsigned int buf_size)
{
	bool is_ok = true;
	netd_socket_service* nss = reinterpret_cast<netd_socket_service*>(args);

	outp_data data((unsigned char*)buf, buf_size);
	nss->application_ptr_->outp_queue_->push(data);
	InterlockedIncrement(&nss->application_ptr_->socket_data_received_num_);

	return is_ok;
}


