#include <assert.h>
#include "../common/config.h"
#include "pcid.h"
#include "sockimpl.h"

#pragma comment(lib, "Ws2_32")

typedef struct __tagSOCKET_IMPL
{
	SOCKET socket_;
	SOCKET broadcast_socket_;
	unsigned short listen_port_;
	unsigned int recv_buffer_size_;
	unsigned int send_buffer_size_;
	SOCKADDR_IN remote_ip_;
	unsigned short remote_port_;
}SOCKET_IMPL, *SOCKET_IMPL_PTR;

SOCKET_IMPL __socket_impl;

bool init_socket()
{
	bool is_ok = TRUE;
	WSADATA wsaData;

	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return false;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){

		WSACleanup();
		WSASetLastError (WSAVERNOTSUPPORTED);
		return false;
	}

	memset(&__socket_impl, 0x0, sizeof(__socket_impl));

	return is_ok;
}

bool create_udp_socket(const char* listen_ip, unsigned short lisen_port, const char* remote_ip, unsigned short remote_port,
					   unsigned recv_buff_size, unsigned send_buff_size)
{
	bool is_ok = true;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;
	unsigned long arg = 1;
	BOOL is_broadcast = TRUE;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	//create a socket
	assert(__socket_impl.socket_ == NULL);
	__socket_impl.socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(__socket_impl.socket_ != NULL){

		__socket_impl.listen_port_ = lisen_port;
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(listen_ip);//htonl(INADDR_ANY);
		sockAddr.sin_port = htons((u_short)__socket_impl.listen_port_);

		//bind socket
		bind(__socket_impl.socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

		//set receive buff
		__socket_impl.recv_buffer_size_ = recv_buff_size;
		if(__socket_impl.recv_buffer_size_ != 0)
			setsockopt(__socket_impl.socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&__socket_impl.recv_buffer_size_, 
			sizeof(__socket_impl.recv_buffer_size_));

		//set send buff
		__socket_impl.send_buffer_size_ = send_buff_size;
		if(__socket_impl.send_buffer_size_ != 0)
			setsockopt(__socket_impl.socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&__socket_impl.send_buffer_size_, 
			sizeof(__socket_impl.send_buffer_size_));
		//set io control
		ioctlsocket(__socket_impl.socket_, FIONBIO, &arg);

		// disable  new behavior using IOCTL: SIO_UDP_CONNRESET
		WSAIoctl(__socket_impl.socket_, SIO_UDP_CONNRESET,	&new_behavior, sizeof(new_behavior), 
			NULL, 0, &bytes_returned,	NULL, NULL);

	}
	else{
		OUTPUT_DEBUG_STRING(IDS_STRING142);
		return false;
	}

	//create a broadcast socket
	assert(__socket_impl.broadcast_socket_ == NULL);
	__socket_impl.broadcast_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(__socket_impl.broadcast_socket_ != NULL){

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(listen_ip);//htonl(INADDR_ANY);
		sockAddr.sin_port = htons(0);

		setsockopt(__socket_impl.broadcast_socket_, SOL_SOCKET, SO_BROADCAST,
			(char*)&is_broadcast, sizeof(BOOL));

		//bind socket
		bind(__socket_impl.broadcast_socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	}

	//set remote ip
	memset(&__socket_impl.remote_ip_, 0, sizeof(__socket_impl.remote_ip_));

	__socket_impl.remote_ip_.sin_family = AF_INET;

	if (remote_ip == NULL){

		__socket_impl.remote_ip_.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{

		__socket_impl.remote_ip_.sin_addr.s_addr = inet_addr(remote_ip);
		if (__socket_impl.remote_ip_.sin_addr.s_addr == INADDR_NONE){

			LPHOSTENT lphost;
			lphost = gethostbyname(remote_ip);
			if (lphost != NULL){

				__socket_impl.remote_ip_.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			}
			else{

				WSASetLastError(WSAEINVAL);
				return false;
			}
		}
	}

	__socket_impl.remote_ip_.sin_port = htons((u_short)remote_port);
	__socket_impl.remote_port_ = remote_port;

	return is_ok;
}

bool create_udp_socket(unsigned short lisen_port, const char* remote_ip, unsigned short remote_port,
					   unsigned recv_buff_size, unsigned send_buff_size)
{
	bool is_ok = true;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;
	unsigned long arg = 1;
	BOOL is_broadcast = TRUE;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	//create a socket
	assert(__socket_impl.socket_ == NULL);
	__socket_impl.socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(__socket_impl.socket_ != NULL){

		__socket_impl.listen_port_ = lisen_port;
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		sockAddr.sin_port = htons((u_short)__socket_impl.listen_port_);

		//bind socket
		bind(__socket_impl.socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

		//set receive buff
		__socket_impl.recv_buffer_size_ = recv_buff_size;
		if(__socket_impl.recv_buffer_size_ != 0)
			setsockopt(__socket_impl.socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&__socket_impl.recv_buffer_size_, 
						sizeof(__socket_impl.recv_buffer_size_));

		//set send buff
		__socket_impl.send_buffer_size_ = send_buff_size;
		if(__socket_impl.send_buffer_size_ != 0)
			setsockopt(__socket_impl.socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&__socket_impl.send_buffer_size_, 
						sizeof(__socket_impl.send_buffer_size_));
		//set io control
		ioctlsocket(__socket_impl.socket_, FIONBIO, &arg);

		// disable  new behavior using IOCTL: SIO_UDP_CONNRESET
		WSAIoctl(__socket_impl.socket_, SIO_UDP_CONNRESET,	&new_behavior, sizeof(new_behavior), 
				NULL, 0, &bytes_returned,	NULL, NULL);

	}
	else{
		OUTPUT_DEBUG_STRING(IDS_STRING142);
		return false;
	}

	//create a broadcast socket
	assert(__socket_impl.broadcast_socket_ == NULL);
	__socket_impl.broadcast_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(__socket_impl.broadcast_socket_ != NULL){

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		sockAddr.sin_port = htons(0);

		setsockopt(__socket_impl.broadcast_socket_, SOL_SOCKET, SO_BROADCAST,
					(char*)&is_broadcast, sizeof(BOOL));

		//bind socket
		bind(__socket_impl.broadcast_socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	}
	
	//set remote ip
	memset(&__socket_impl.remote_ip_, 0, sizeof(__socket_impl.remote_ip_));

	__socket_impl.remote_ip_.sin_family = AF_INET;

	if (remote_ip == NULL){

		__socket_impl.remote_ip_.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{

		__socket_impl.remote_ip_.sin_addr.s_addr = inet_addr(remote_ip);
		if (__socket_impl.remote_ip_.sin_addr.s_addr == INADDR_NONE){

			LPHOSTENT lphost;
			lphost = gethostbyname(remote_ip);
			if (lphost != NULL){

				__socket_impl.remote_ip_.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			}
			else{

				WSASetLastError(WSAEINVAL);
				return false;
			}
		}
	}

	__socket_impl.remote_ip_.sin_port = htons((u_short)remote_port);
	__socket_impl.remote_port_ = remote_port;
	
	return is_ok;
}

int udp_recv(udp_recv_callback callback, void* args)
{
	char recv_buf[MAX_PACK_NET_SIZE] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	int count = 0;

	while(true){

		if(ioctlsocket(__socket_impl.socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(__socket_impl.socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR){

					OUTPUT_DEBUG_STRING(IDS_STRING143, recv_size);
					break;
				}
				else if(recv_size == 0x0){

					OUTPUT_DEBUG_STRING(IDS_STRING144);
					break;
				}
				else if(recv_size > MAX_PACK_NET_SIZE){

					OUTPUT_DEBUG_STRING(IDS_STRING145);
					break;
				}
				else{

					callback(args, recv_buf, recv_size);
					count += recv_size;
				}
			}
			else
				break;
		}
		else{
			break;
		}
	}

	return count;
}

//int udp_send(const SOCKADDR* remote_ip, const char* send_buf, unsigned int send_buf_size, int flag)
//{
//	int ret = 0;
//	timeval time_val = {0};
//	fd_set write_fds;
//
//	FD_ZERO(&write_fds);
//	FD_SET(__socket_impl.socket_, &write_fds);
//	ret = select(NULL, NULL, &write_fds, NULL, &time_val);
//
//	if(ret == 0) return 0;
//	else if(ret == SOCKET_ERROR) return -1;
//
//	return sendto(__socket_impl.socket_, send_buf, send_buf_size, flag, remote_ip, sizeof(sockaddr_in));
//}

int udp_send(const char* send_buf, unsigned int send_buf_size, const SOCKADDR* remote_ip, int flag)
{
	int ret = 0;
	timeval time_val = {0};
	fd_set write_fds;
	SOCKET s = NULL;

	if(remote_ip == NULL)	remote_ip = (SOCKADDR*)&__socket_impl.remote_ip_;

	if(((SOCKADDR_IN*)remote_ip)->sin_addr.s_addr == htonl(INADDR_BROADCAST)){
		s = __socket_impl.broadcast_socket_;
	}
	else{

		FD_ZERO(&write_fds);
		FD_SET(__socket_impl.socket_, &write_fds);
		ret = select(NULL, NULL, &write_fds, NULL, &time_val);

		if(ret == 0) return 0;
		else if(ret == SOCKET_ERROR) return -1;
		s = __socket_impl.socket_;
	}

	return sendto(s, send_buf, send_buf_size, flag, remote_ip, sizeof(SOCKADDR));
}

void release_socket()
{
	WSACleanup();
}

void close_udp_socket()
{

	if(__socket_impl.socket_){

		closesocket(__socket_impl.socket_);
		__socket_impl.socket_ = NULL;
	}

	if(__socket_impl.broadcast_socket_){

		closesocket(__socket_impl.broadcast_socket_);
		__socket_impl.broadcast_socket_ = NULL;
	}

};





