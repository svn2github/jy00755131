/**
@brief	定义netd_socket_recv_thread类
@details	该类主要实现socket的接受数据功能
@file	matrix.socket.h
@note	
@see	
@version	1.0.0.1.2012.02.14
@author	kuoxin	
@bug
*/
#ifndef __NETD_SOCKET_RECV_THREAD__
#define __NETD_SOCKET_RECV_THREAD__

#include "netd.socket.serv.h"
#include "..\core\matrix.thread.h"

class netd_socket_recv_thread : public matrix_thread
{
public:
	netd_socket_recv_thread(netd_socket_service* socket_service_ptr);
	~netd_socket_recv_thread();
public:
	void stop();

protected:
	void run();

private:
	HANDLE stop_event_;//!<结束该线程的时间通知
	netd_socket_service* socket_service_ptr_;
};

#endif /*__NETD_SOCKET_RECV_THREAD__*/