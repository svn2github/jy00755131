/**
@brief	定义netd_pcap_recv_thread类
@details	该类主要实现pcap的接受数据功能
@file	netd.pcap.recv.thread.h
@note	
@see	
@version	1.0.2.8.2012.02.17
@author	kuoxin	
@bug
*/
#ifndef __NETD_PCAP_RECV_THREAD__
#define __NETD_PCAP_RECV_THREAD__

#include "..\core\matrix.thread.h"

class netd_pcap_service;
class netd_pcap_recv_thread : public matrix_thread
{
public:
	netd_pcap_recv_thread(netd_pcap_service* pcap_service_ptr);
	~netd_pcap_recv_thread();
public:
	void stop();

protected:
	void run();
private:
	HANDLE stop_event_;
	netd_pcap_service* pcap_service_ptr_;
};

#endif /*__NETD_PCAP_RECV_THREAD__*/