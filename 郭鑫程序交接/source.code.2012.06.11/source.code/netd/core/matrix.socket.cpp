/**
@brief	定义matrix_object基类
@details	该类主要实现基本机制
@file	matrix.object.cpp
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include <Mswsock.h>
#include "matrix.socket.h"

#define SOCKET_BOARDCAST_ADDRESS "255.255.255.255"


matrix_socket::matrix_socket(matrix_socket_type socket_type, matrix_object* parent_object_ptr)
							:matrix_object(parent_object_ptr), socket_type_(socket_type),
							socket_(INVALID_SOCKET), socket_state_(matrix_socket_unconnected_State),
							socket_option_(matrix_socket_normal_option)
{
	listen_ip_ = 0x0;
	listen_port_ = 0x0;
	remote_ip_ = 0x0;
	remote_port_ = 0x0;
	recv_buffer_size_ = 0x0;
	send_buffer_size_ = 0x0;

}

matrix_socket::~matrix_socket(){}

bool matrix_socket::create(char const* listen_ip, unsigned short listen_port, 
						   char const* remote_ip, unsigned short remote_port,
						   unsigned int recv_buffer_size, unsigned int send_buffer_size)
{

	bool is_ok = true;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;
	unsigned long arg = 1;
	BOOL is_boardcast = FALSE;
	SOCKADDR_IN sockAddr;

	if(socket_state_ != matrix_socket_unconnected_State) return matrix_socket_connected_error;

	memset(&sockAddr,0,sizeof(sockAddr));

	//create a socket
	if(socket_type_ == matrix_udp_socket_type)	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	else										socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(socket_ != NULL){

		sockAddr.sin_family = AF_INET;
		listen_ip_ = sockAddr.sin_addr.s_addr = inet_addr(listen_ip);
		listen_port_ = sockAddr.sin_port = htons(listen_port);

		//bind socket
		bind(socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

		if(remote_ip != NULL){

			if(stricmp(SOCKET_BOARDCAST_ADDRESS, remote_ip) == 0){

				is_boardcast = TRUE;
				socket_option_ |= matrix_socket_boardcast_option; 
				setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, 
					(const char*)&is_boardcast, sizeof(is_boardcast));
			}
		}

		//set receive buff
		if(recv_buffer_size != 0){
			recv_buffer_size_ = recv_buffer_size;
			setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&recv_buffer_size_, 
				sizeof(recv_buffer_size_));
		}

		//set send buff
		if(send_buffer_size != 0){
			send_buffer_size_ = send_buffer_size;
			setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&send_buffer_size_, 
				sizeof(send_buffer_size_));
		}

		//set io control
		ioctlsocket(socket_, FIONBIO, &arg);

		// disable  new behavior using IOCTL: SIO_UDP_CONNRESET
		WSAIoctl(socket_, SIO_UDP_CONNRESET,	&new_behavior, sizeof(new_behavior), 
			NULL, 0, &bytes_returned,	NULL, NULL);
	}
	else{
		return false;
	}

	//set remote ip
	if(remote_ip == NULL){
		remote_ip_ = htonl(INADDR_ANY);
	}
	else{

		remote_ip_ = is_boardcast ? htonl(INADDR_BROADCAST) : inet_addr(remote_ip);

		if(!is_boardcast){

			LPHOSTENT lphost;
			lphost = gethostbyname(remote_ip);

			if(lphost != NULL)	remote_ip_ = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else				return false;
		}
	}

	remote_port_ = htons(remote_port);

	return is_ok;
}

int matrix_socket::recv(void* args, int flags)
{
	char recv_buf[MAX_PACK_NET_SIZE] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	int count = 0;

	while(true){

		if(ioctlsocket(socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = ::recv(socket_, recv_buf, sizeof(recv_buf), flags);

				if(recv_size == SOCKET_ERROR)			break;
				else if(recv_size == 0x0)				break;
				else if(recv_size > MAX_PACK_NET_SIZE)	break;
				else{

					recv_private(args, recv_buf, recv_size);
					count += recv_size;
				}
			}
			else	break;
		}
		else		break;
	}

	return count;
}

int matrix_socket::send(const char* send_buf, unsigned int send_buf_size, const SOCKADDR* remote_ip, int flag)
{
	int ret = 0;
	timeval time_val = {0};
	fd_set write_fds;
	SOCKADDR_IN tmp_remote_ip;

	if(remote_ip == NULL){
		
		SecureZeroMemory(&tmp_remote_ip, sizeof(tmp_remote_ip));
		tmp_remote_ip.sin_family = AF_INET;
		tmp_remote_ip.sin_port = htons(remote_port_);
		tmp_remote_ip.sin_addr.s_addr = remote_ip_;
		remote_ip = (SOCKADDR*)&tmp_remote_ip;
	}

	FD_ZERO(&write_fds);
	FD_SET(socket_, &write_fds);
	ret = select(NULL, NULL, &write_fds, NULL, &time_val);

	if(ret == 0) return 0;
	else if(ret == SOCKET_ERROR) return -1;

	return sendto(socket_, send_buf, send_buf_size, flag, remote_ip, sizeof(SOCKADDR));
}

void matrix_socket::release()
{
	WSACleanup();
}

void matrix_socket::close()
{
	if(socket_){
		closesocket(socket_);
		socket_ = NULL;
	}
};

bool matrix_socket::recv_private(void* args, const char* buf, unsigned int buf_size)
{
	args = args;
	buf = buf;
	buf_size = buf_size;
	return true;
}
