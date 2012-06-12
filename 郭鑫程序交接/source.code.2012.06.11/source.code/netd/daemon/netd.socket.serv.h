/**
@brief	定义netd_socket_service类
@details	该类主要实现上位机socket收发功能
@file	matrix.socket.h
@note	
@see	
@version	1.0.0.1.2012.02.14
@author	kuoxin	
@bug
*/
#ifndef __NETD_SOCKET_SERVICE_H__
#define __NETD_SOCKET_SERVICE_H__

#include "..\core\matrix.socket.h"

class netd_socket_service;

#include "netd.application.h"
#include "netd.socket.recv.thread.h"
#include "netd.socket.snd.thread.h"

class netd_socket_service : public matrix_socket
{
public:
	netd_socket_service(netd_application* application_ptr);
	~netd_socket_service();

public: 
	bool start();
	void stop();

protected:
	bool recv_private(void* args, const char* buf, unsigned int buf_size);

private:
	friend class netd_socket_recv_thread;
	friend class netd_socket_snd_thread;

private:

	netd_socket_snd_thread* snd_thread_ptr_;
	netd_socket_recv_thread* recv_thread_ptr_;
	netd_application* application_ptr_;
};

#endif /*__NETD_SOCKET_SERVICE_H__*/