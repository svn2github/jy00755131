#include <assert.h>
#include "../common/config.h"
#include "sockimpl.h"

#pragma comment(lib, "Ws2_32")

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

	return is_ok;
}

bool create_udp_socket(SOCKET_IMPL_PTR socket_impl, const char_t* remote_ip)
{
	bool is_ok = true;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;
	unsigned long arg = 1;
	BOOL is_boardcast = FALSE;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

#if defined(UNICODE) || defined(_UNICODE)
	char buf[MAX_STRING_SIZE] = {NULL};
	size_t size = 0x0;
#endif //UNICODE) || _UNICODE

	//create a socket
	assert(socket_impl->socket_ == NULL);
	socket_impl->socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_impl->socket_ != NULL){

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		sockAddr.sin_port = htons((u_short)socket_impl->listen_port_);

		//bind socket
		bind(socket_impl->socket_, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

		if(remote_ip != NULL){

			if(_tcsicmp(SOCKET_BOARDCAST_ADDRESS, remote_ip) == 0){

				is_boardcast = TRUE;
				setsockopt(socket_impl->socket_, SOL_SOCKET, SO_BROADCAST, 
					(const char*)&is_boardcast, sizeof(is_boardcast));
			}
		}

		//set receive buff
		if(socket_impl->recv_buffer_size_ != 0)
			setsockopt(socket_impl->socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&socket_impl->recv_buffer_size_, 
						sizeof(socket_impl->recv_buffer_size_));

		//set send buff
		if(socket_impl->send_buffer_size_ != 0)
			setsockopt(socket_impl->socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&socket_impl->send_buffer_size_, 
						sizeof(socket_impl->send_buffer_size_));
		//set io control
		ioctlsocket(socket_impl->socket_, FIONBIO, &arg);

		// disable  new behavior using IOCTL: SIO_UDP_CONNRESET
		WSAIoctl(socket_impl->socket_, SIO_UDP_CONNRESET,	&new_behavior, sizeof(new_behavior), 
				NULL, 0, &bytes_returned,	NULL, NULL);

	}
	else{
		WSASetLastError(CREATE_SOCKET_ERROR);
		return false;
	}
	
	//set remote ip
	memset(&socket_impl->remote_ip_, 0, sizeof(socket_impl->remote_ip_));
	socket_impl->remote_ip_.sin_family = AF_INET;

	if (remote_ip == NULL){

		socket_impl->remote_ip_.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{

#if defined(UNICODE) || defined(_UNICODE)
		wcstombs_s(&size, buf, sizeof(buf), remote_ip, sizeof(buf));
		buf[size] = 0x0;
		socket_impl->remote_ip_.sin_addr.s_addr = is_boardcast ? htonl(INADDR_BROADCAST) : inet_addr(buf);
#else
		socket_impl->remote_ip_.sin_addr.s_addr = is_boardcast ? htonl(INADDR_BROADCAST) : inet_addr(remote_ip);
#endif //UNICODE) || _UNICODE

		if(socket_impl->remote_ip_.sin_addr.s_addr == INADDR_NONE && !is_boardcast){

			LPHOSTENT lphost;
#if defined(UNICODE) || defined(_UNICODE)
			wcstombs_s(&size, buf, sizeof(buf), remote_ip, sizeof(buf));
			buf[size] = 0x0;
			lphost = gethostbyname(buf);
#else
			lphost = gethostbyname(remote_ip);
#endif

			if (lphost != NULL){

				socket_impl->remote_ip_.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			}
			else{

				WSASetLastError(WSAEINVAL);
				return false;
			}
		}
	}

	socket_impl->remote_ip_.sin_port = htons((u_short)socket_impl->remote_port_);
	
	return is_ok;
}

int udp_recv(SOCKET_IMPL_PTR socket_impl, udp_recv_callback callback, void* args)
{
	char recv_buf[MAX_PACK_NET_SIZE] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	int count = 0;

	while(true){

		if(ioctlsocket(socket_impl->socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(socket_impl->socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR)	break;
				else if(recv_size == 0x0){

					WSASetLastError(REMOTE_CLOSE_CONNECTION_GRACEFULLY);
					break;
				}
				else if(recv_size > MAX_PACK_NET_SIZE){

					WSASetLastError(SOCKET_RECEIVE_BUFFER_TOO_SMALL);
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

int udp_send(SOCKET_IMPL_PTR socket_impl, const char* send_buf, unsigned int send_buf_size, const SOCKADDR* remote_ip, int flag)
{
	int ret = 0;
	timeval time_val = {0};
	fd_set write_fds;

	if(remote_ip == NULL)	remote_ip = (SOCKADDR*)&socket_impl->remote_ip_;

	FD_ZERO(&write_fds);
	FD_SET(socket_impl->socket_, &write_fds);
	ret = select(NULL, NULL, &write_fds, NULL, &time_val);

	if(ret == 0) return 0;
	else if(ret == SOCKET_ERROR) return -1;

	return sendto(socket_impl->socket_, send_buf, send_buf_size, flag, remote_ip, sizeof(SOCKADDR));
}

void release_socket()
{
	WSACleanup();
}

void close_udp_socket(SOCKET_IMPL_PTR socket_impl)
{

	if(socket_impl->socket_){
		closesocket(socket_impl->socket_);
		socket_impl->socket_ = NULL;
	}

};

bool async_select(SOCKET_IMPL_PTR socket_impl, HWND window_handle, unsigned int message, long network_event)
{
	bool is_ok = true;

	if(socket_impl == NULL){

		WSASetLastError(SOCKET_IMPL_PTR_IS_NULL);
		return is_ok = false;
	}
	else if(socket_impl->socket_ == INVALID_SOCKET){

		WSASetLastError(SOCKET_IMPL_SOCKET_VAL_IS_INVALID);
		return is_ok = false;
	}

	socket_impl->type_ = SOCKET_IO_TYPE_IS_ASYNC_SELECT;
	socket_impl->window_handle_ = window_handle;
	socket_impl->window_message_ = message;
	socket_impl->network_event_ = network_event;	

	return is_ok = WSAAsyncSelect(socket_impl->socket_, window_handle, message, network_event) == SOCKET_ERROR ? false : true;
}



