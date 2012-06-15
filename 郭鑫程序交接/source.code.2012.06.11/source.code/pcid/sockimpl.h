#if !defined(__SOCKET_IMPL_H_)
#define __SOCKET_IMPL_H_

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <Mswsock.h>

typedef bool (*udp_recv_callback)(void* args, const char*, unsigned);

bool init_socket();
bool create_udp_socket(unsigned short lisen_port, const char* remote_ip, unsigned short remote_port, 
					   unsigned recv_buff_size, unsigned send_buff_size);

int udp_recv(udp_recv_callback callback, void* args);//MAX PACK SIZE: MAX_PACK_NET_SIZE

int udp_send(const char* send_buf, unsigned int send_buf_size, const SOCKADDR* remote_ip = NULL, int flag = 0); // return value: 0: write status is not ready, 

void close_udp_socket();
void release_socket();

#endif //__SOCKET_IMPL_H_

