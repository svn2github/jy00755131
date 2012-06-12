/**
@brief	定义netd_pcap_snd_thread类
@details	该类主要实现pcap的发送数据功能
@file	netd.pcap.snd.thread.cpp
@note	
@see	
@version	1.0.2.8.2012.02.19
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.pcap.serv.h"
#include "netd.pcap.snd.thread.h"

netd_pcap_snd_thread::netd_pcap_snd_thread(netd_pcap_service* pcap_service_ptr)
											: pcap_service_ptr_(pcap_service_ptr)
{
	stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

netd_pcap_snd_thread::~netd_pcap_snd_thread()
{
	if(stop_event_ != NULL)	CloseHandle(stop_event_);
}

void netd_pcap_snd_thread::stop()
{
	if(stop_event_ != NULL) SetEvent(stop_event_);
	if(!wait(100))	terminate();
}

void netd_pcap_snd_thread::run()
{
	unsigned i = 0, size = 0;
	outp_data data;

	while(true){

		size = pcap_service_ptr_->application_ptr_->outp_queue_->size();
		for(i = 0; i < size; ++i){

			if(!pcap_service_ptr_->application_ptr_->outp_queue_->pop(data)) break;
			pcap_service_ptr_->send(pcap_service_ptr_->pcap_handle_ptr_, 
									pcap_service_ptr_->application_ptr_->netd_ip_,
									pcap_service_ptr_->application_ptr_->netd_outp_port_,
									pcap_service_ptr_->application_ptr_->lci_ip_,
									pcap_service_ptr_->application_ptr_->lci_outp_port_,
									data.buf_,outp_data::MAX_OUT_BUFFER_SIZE);
			InterlockedIncrement(&pcap_service_ptr_->application_ptr_->pcap_data_outp_num_);
		}

		//wait...
		if(WaitForSingleObject(stop_event_, pcap_service_ptr_->application_ptr_->pcap_outp_poll_time_) == WAIT_OBJECT_0)	break;
	}
}
