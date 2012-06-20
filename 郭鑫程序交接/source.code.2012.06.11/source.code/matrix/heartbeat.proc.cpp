
#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/code.conv.h"
#include "../common/sockimpl.h"
#include "matrix.h"
#include "common.functions.h"
#include "heartbeat.proc.h"
#include "equipment.comm.order.h"

#define __MATRIX_DEBUG__

enum
{
	MATRIX_HEARTBEAT_PORT = DESTINATION_PORT,
	MATRIX_HEARTBEAT_LISTEN_PORT = 0x9400,
};

extern THE_HEART_BEAT_PACKAGE_INFO __heartbeat_data;
extern APPLICATION_ENVIRONMENT __application_environment;

SOCKET_IMPL __heartbeat_socket;
#ifdef __MATRIX_DEBUG__
unsigned int __snd_counter = 0x0;
unsigned int __recv_counter = 0x0;

unsigned int __fdu_snd_counter = 0x0;
unsigned int __fdu_recv_counter = 0x0;

#endif //__MATRIX_DEBUG_1_

void make_heart_beat_package(unsigned char* buf, unsigned int ip)
{
	unsigned iPos = 0;
	unsigned i = 0;
	iPos = make_package_head(ip, MATRIX_HEARTBEAT_LISTEN_PORT, CMD_SEARCH, buf);
	i = CMD_SYSTEM_TIME;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;

	i = 0x19;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;

	i = 0x00;
	memcpy(buf + iPos, &i, 1);
}

bool init_heartbeat_proc(TCHAR const* remote_ip)
{
	bool is_ok = true;

	if(__heartbeat_socket.socket_)	close_udp_socket(&__heartbeat_socket);

	SecureZeroMemory(&__heartbeat_socket, sizeof(__heartbeat_socket)); 

	__heartbeat_socket.remote_port_ = MATRIX_HEARTBEAT_PORT;
	__heartbeat_socket.listen_port_ = MATRIX_HEARTBEAT_LISTEN_PORT;

	is_ok = create_udp_socket(&__heartbeat_socket, remote_ip);

	if(!is_ok) return is_ok;

	SecureZeroMemory(&__heartbeat_data, sizeof(__heartbeat_data));

	is_ok = async_select(&__heartbeat_socket, __application_environment.main_window_,
						WM_HEARTBEAT_PACKAGE_MESSAGE, FD_READ);

#ifdef __MATRIX_DEBUG__
	__snd_counter = 0x0;
	__recv_counter = 0x0;

	__fdu_snd_counter = 0x0;
	__fdu_recv_counter = 0x0;

#endif //__MATRIX_DEBUG__

	return is_ok;
}

void release_heartbeat_proc()
{
	close_udp_socket(&__heartbeat_socket);
}

void heartbeat_proc()
{
	int ret = 0;
	int count = 0;
	unsigned char buf[MAX_OUTPUT_PACKAGE_SIZE] = {0};

	do{
		make_heart_beat_package(buf, IP_MAIN_LAUX);
		ret = udp_send(&__heartbeat_socket, (const char*)buf, sizeof(buf));
#ifndef __MATRIX_DEBUG__
		if(ret != sizeof(buf)) ++count;
		else break;
#endif __MATRIX_DEBUG__

#ifdef __MATRIX_DEBUG__
		make_heart_beat_package(buf, 0x01c000c0);
		ret = udp_send(&__heartbeat_socket, (const char*)buf, sizeof(buf));
		if(ret != sizeof(buf)) ++count;
		else break;
#endif //__MATRIX_DEBUG__

	}while(count < 3);

	if(count == 3){	output_log(IDS_STRING106, count);	}
#ifdef __MATRIX_DEBUG__
	else{
	
		sprintf_s((char*)buf, sizeof(buf), "%d", ++__snd_counter);
		SendMessage(GET_WINDOW_HANDLE(IDC_SND_COUNTER_STATIC), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s((char*)buf, sizeof(buf), "%d", ++__fdu_snd_counter);
		SendMessage(GET_WINDOW_HANDLE(IDC_FDU_SND_COUNTER_STATIC), WM_SETTEXT, NULL, (LPARAM)buf);
	}			
#endif //__MATRIX_DEBUG_1_ 
}

bool check_heart_beat_rec_package(unsigned char* buf, int buf_size, THE_HEART_BEAT_PACKAGE_INFO_PTR heart_beat_pack_info)
{
	bool is_ok = true;
	buf_size = buf_size;
	unsigned system_time = 0;

	memset(heart_beat_pack_info, 0x0, sizeof(THE_HEART_BEAT_PACKAGE_INFO));

	memcpy(&heart_beat_pack_info->ip_, &buf[0x10], 4);
	memcpy(&heart_beat_pack_info->cmd_word_, &buf[0x1a], 2);
	memcpy(&system_time, &buf[0x1c], 1);
	if (heart_beat_pack_info->cmd_word_ == CMD_SEARCH && system_time == CMD_SYSTEM_TIME)
	{
		memcpy(&heart_beat_pack_info->time_, &buf[0x1d], 4);
		
		memcpy(&system_time, &buf[0x1d + 4], 1);
		if(system_time == 0x19)
			memcpy(&heart_beat_pack_info->net_time_, &buf[0x1d + 5], 4);
	}
	else 
		is_ok = false;

	return is_ok;
}

BOOL on_receive_heartbeat_package(WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(wparam);
	int recv_size = 0x0;
	char recv_buf[MAX_INPUT_PACKAGE_SIZE] = {0};

	int error_code = WSAGETSELECTERROR(lparam);
	fd_set fds;
	int ready;
	timeval timeout = {0};

	char bufxx[1024] = {0};
	int size = 0x0;

#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE] = {0};
#endif //__MATRIX_DEBUG__

	if(WSAGETSELECTEVENT(lparam) == FD_READ){

		FD_ZERO(&fds);
		FD_SET(__heartbeat_socket.socket_, &fds);
		ready = select(0, &fds, NULL, NULL, &timeout);
		if(ready == SOCKET_ERROR)	error_code = WSAGetLastError();

		if(ready == 1){	

			SecureZeroMemory(recv_buf, sizeof(recv_buf));
			SecureZeroMemory(&__heartbeat_data, sizeof(__heartbeat_data));
			recv_size = recv(__heartbeat_socket.socket_, recv_buf, sizeof(recv_buf), 0);
			if(recv_size > 0){

				check_heart_beat_rec_package((unsigned char*)recv_buf, recv_size, &__heartbeat_data);
#ifdef __MATRIX_DEBUG__

				if(__heartbeat_data.ip_ == IP_MAIN_LAUX){

					if(__recv_counter == 0)	__snd_counter = 1;
					sprintf_s(bufxx, sizeof(bufxx), "%d", ++__recv_counter);
					SendMessage(GET_WINDOW_HANDLE(IDC_RECV_COUNTER_STATIC), WM_SETTEXT, NULL, (LPARAM)bufxx);

					sprintf_s(bufxx, sizeof(bufxx), "%d", __snd_counter - (++__recv_counter));
					SendMessage(GET_WINDOW_HANDLE(IDC_RECV_SND_MINUE_STATIC), WM_SETTEXT, NULL, (LPARAM)bufxx);
				}

				if(__heartbeat_data.ip_ == 0x01c000c0){

					if(__fdu_recv_counter == 0)	__fdu_snd_counter = 1;
					sprintf_s(bufxx, sizeof(bufxx), "%d", ++__fdu_recv_counter);
					SendMessage(GET_WINDOW_HANDLE(IDC_FDU_RECV_COUNTER_STATIC), WM_SETTEXT, NULL, (LPARAM)bufxx);

					sprintf_s(bufxx, sizeof(bufxx), "%d", __fdu_snd_counter - (++__fdu_recv_counter));
					SendMessage(GET_WINDOW_HANDLE(IDC_FDU_RECV_SND_MINUE_STATIC), WM_SETTEXT, NULL, (LPARAM)bufxx);
				}
#endif __MATRIX_DEBUG__

#ifdef __MATRIX_DEBUG__
				sprintf_s(dbuf, sizeof(dbuf), __MATRIX_TEXT("ip %08X; time %X; net_time %X"), __heartbeat_data.ip_, 
							__heartbeat_data.time_, __heartbeat_data.net_time_);
				output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("接收心跳查询响应"), dbuf);
#endif //__MATRIX_DEBUG__
			}
		}
		else			output_log(IDS_STRING133, error_code);
	}

	return TRUE;	
}
