/**
@brief	定义netd_pcap_service类
@details	该类主要实现LCI pcap收发功能
@file	netd.pcap.serv.h
@note	
@see	
@version	1.0.2.8.2012.02.17
@author	kuoxin	
@bug
*/
#ifndef __NETD_PCAP_SERVICE_H__
#define __NETD_PCAP_SERVICE_H__

#include "..\core\matrix.object.h"

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	u_int	saddr;		// Source address
	u_int	daddr;		// Destination address
//	u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;

//Ethernet Header
typedef struct ethernet_header
{
	u_char dest[6]; //Total 48 bits
	u_char source[6]; //Total 48 bits
	unsigned short type; //16 bits
}ethernet_header , *ethernet_header_ptr;

class netd_application;
class netd_pcap_recv_thread;
class netd_pcap_snd_thread;

class netd_pcap_service : public matrix_object
{
public:
	netd_pcap_service(netd_application* application_ptr);
	~netd_pcap_service();

public: 
	bool start();
	void stop();
	unsigned short check_sum (unsigned short * addr, int len);
	bool send(pcap_t* handle, unsigned int src_ip, unsigned short src_port, 
			unsigned int dest_ip, unsigned short dest_port, unsigned char* buf, 
			unsigned int buf_len);
public:
	u_char m_cNetMacAddr[6];
	u_char m_cLCIMacAddr[6];
private:
	friend class netd_pcap_recv_thread;
	friend class netd_pcap_snd_thread;

private:
	netd_pcap_snd_thread* snd_thread_ptr_;
	netd_pcap_recv_thread* recv_thread_ptr_;
	netd_application* application_ptr_;
	pcap_t* pcap_handle_ptr_;
};

#endif /*__NETD_PCAP_SERVICE_H__*/