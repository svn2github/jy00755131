/**
@brief	定义netd_pcap_recv_thread类
@details	该类主要实现pcap的接受数据功能
@file	netd.pcap.recv.thread.cpp
@note	
@see	
@version	1.0.2.8.2012.02.17
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.pcap.serv.h"
#include "netd.pcap.recv.thread.h"

netd_pcap_recv_thread::netd_pcap_recv_thread(netd_pcap_service* pcap_service_ptr)
												: pcap_service_ptr_(pcap_service_ptr)
{
	stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

netd_pcap_recv_thread::~netd_pcap_recv_thread()
{
	if(stop_event_ != NULL)	CloseHandle(stop_event_);
}

void netd_pcap_recv_thread::stop()
{
	if(stop_event_ != NULL) SetEvent(stop_event_);
	if(!wait(100))	terminate();
}

void netd_pcap_recv_thread::run()
{
	int status = 0x0;
	ip_header* ih = NULL;
	unsigned int ip_len = 0x0;
	udp_header* uh = NULL;
	struct pcap_pkthdr* pkt_header = NULL;
	const u_char* pkt_data = NULL;
	unsigned short sport = 0x0;
	unsigned short dport = 0x0;
	inp_data data;

	do{

		status = pcap_next_ex(pcap_service_ptr_->pcap_handle_ptr_, &pkt_header, &pkt_data);
		if(status == 1){

			ih = (ip_header *)(pkt_data + sizeof(ethernet_header));
			ip_len = (ih->ver_ihl & 0xf) * 4;
			uh = (udp_header *)((u_char*)ih + ip_len);

			if(uh->dport == pcap_service_ptr_->application_ptr_->lci_inp_port_ && 
				pcap_service_ptr_->application_ptr_->lci_ip_ == ih->saddr &&
				pcap_service_ptr_->application_ptr_->netd_ip_ == ih->daddr){

				InterlockedIncrement(&pcap_service_ptr_->application_ptr_->pcap_data_inp_num_);
				data.copy(pkt_data + 42, inp_data::MAX_IN_BUFFER_SIZE);
				pcap_service_ptr_->application_ptr_->inp_queue_->push(data);
			}
		}

		//wait...
		if(WaitForSingleObject(stop_event_, 0) == WAIT_OBJECT_0)	break;
	}while(status >= 0);
}
