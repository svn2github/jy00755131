#ifndef __NetFrameHeader_H__
#define __NetFrameHeader_H__

/* 4 bytes IP address */
typedef struct IP_Address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}IP_Address;

/* IPv4 header */
typedef struct IP_Header
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
}IP_Header;

/* UDP header*/
typedef struct UDP_Header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}UDP_Header;

//Ethernet Header
typedef struct Ethernet_Header
{
	u_char dest[6]; //Total 48 bits
	u_char source[6]; //Total 48 bits
	unsigned short type; //16 bits
}Ethernet_Header , *Ethernet_Header_ptr;

#endif