/**
@brief	定义netd_pcap_service类
@details	实现pcap接受和发送上位机数据
@file	netd.pcap.serv.cpp
@note	
@see	
@version	1.0.2.8.2012.02.19
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.pcap.recv.thread.h"
#include "netd.pcap.snd.thread.h"
#include "netd.pcap.serv.h"

netd_pcap_service::netd_pcap_service(netd_application* application_ptr)
:application_ptr_(application_ptr),pcap_handle_ptr_(NULL)
{
	snd_thread_ptr_ = new netd_pcap_snd_thread(this);
	recv_thread_ptr_ = new netd_pcap_recv_thread(this);
}

netd_pcap_service::~netd_pcap_service()
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

unsigned short netd_pcap_service::check_sum (unsigned short * addr, int len)
{
	int nleft = len;
	unsigned sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	/*
	* Our algorithm is simple, using a 32 bit accumulator (sum), we add
	* sequential 16 bit words to it, and at the end, fold back all the
	* carry bits from the top 16 bits into the lower 16 bits.
	*/
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	/* mop up an odd byte, if necessary */
	if (nleft == 1) {
		* (unsigned char *) (&answer) = * (unsigned char *) w;
		sum += answer;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
	sum += (sum >> 16);     /* add carry */
	answer = ~sum;     /* truncate to 16 bits */
	return (answer);
}

bool netd_pcap_service::send(pcap_t* handle, unsigned int src_ip, unsigned short src_port, 
							 unsigned int dest_ip, unsigned short dest_port, 
							 unsigned char* buf, unsigned int buf_len)
{
	unsigned char tmp_buf[512] = {0};
	ip_header ip;
	ethernet_header eh = {0};
	udp_header dh = {0};
	unsigned int tmp = 0x0;

	eh.type = 0x08;
	memcpy(tmp_buf, &eh, sizeof(eh));

	ip.ver_ihl = 0x45;		// Version (4 bits) + Internet header length (4 bits)
	ip.tos = 0x0;			// Type of service 
	tmp = buf_len + sizeof(ip_header) + sizeof(udp_header);
	ip.tlen = htons(tmp);			// Total length 
	ip.identification = htons(3347); // Identification
	ip.flags_fo = 0x40;		// Flags (3 bits) + Fragment offset (13 bits)
	ip.ttl = 0x80;			// Time to live
	ip.proto = 17;			// Protocol
	ip.saddr = src_ip;// Source address
	ip.daddr = dest_ip;
	ip.crc = 0x0;
	ip.crc = check_sum((unsigned short*)&ip, sizeof(ip));// Header checksum
 
	memcpy(tmp_buf + sizeof(eh), &ip, sizeof(ip));

	dh.crc =0x0;
	dh.dport = dest_port;
	dh.sport = src_port;
	tmp = buf_len + sizeof(udp_header);
	dh.len = htons(tmp);
	
	tmp = sizeof(eh) + sizeof(ip);
	memcpy(tmp_buf + tmp, &dh, sizeof(dh));
	tmp += sizeof(dh);
	memcpy(tmp_buf + tmp, buf, buf_len);
	tmp += buf_len;

	return pcap_sendpacket(handle, tmp_buf, tmp) == 0 ? true : false;
}

bool netd_pcap_service::start()
{
	bool is_ok = true;

	pcap_if_t *alldevs;
	pcap_if_t *d;
	char errbuf[PCAP_ERRBUF_SIZE+1];
	unsigned int netmask;
	struct bpf_program fcode;

	/* Retrieve the interfaces list */
	pcap_findalldevs(&alldevs, errbuf);

	for(unsigned i = 0; i < application_ptr_->netcard_id_; ++i)	d = alldevs->next;

	pcap_handle_ptr_  = pcap_open_live(d->name, application_ptr_->pcap_max_package_size_, 
										1, application_ptr_->pcap_timeout_, errbuf);
	pcap_setbuff(pcap_handle_ptr_, application_ptr_->pcap_buff_size_);

	if(d->addresses != NULL)
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		netmask=0xffffff; 

	//compile the filter
	pcap_compile(pcap_handle_ptr_, &fcode, application_ptr_->pcap_filter_, 1, netmask);
	pcap_setfilter(pcap_handle_ptr_, &fcode);
	pcap_freealldevs(alldevs);

	snd_thread_ptr_->start();
	recv_thread_ptr_->start();

	return is_ok;
}

void netd_pcap_service::stop()
{
	snd_thread_ptr_->stop();
	recv_thread_ptr_->stop();
}

