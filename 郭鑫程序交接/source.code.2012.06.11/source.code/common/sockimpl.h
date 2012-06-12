#if !defined(__SOCKET_IMPL_H_)
#define __SOCKET_IMPL_H_

#include <Windows.h>
#include <WinSock2.h>
#include <Mswsock.h>

#define SOCKET_BOARDCAST_ADDRESS __TEXT("255.255.255.255")

enum __enuSOCKET_IMPL_ERROR
{
	CREATE_SOCKET_ERROR, //创建socket失败！
	REMOTE_CLOSE_CONNECTION_GRACEFULLY,//由于远程主机完美断开连接，SOCKET接受数据失败!
	SOCKET_RECEIVE_BUFFER_TOO_SMALL, //接受数据大于所设缓冲, 丢弃该数据包...
	SOCKET_IMPL_PTR_IS_NULL, //传入__tagSOCKET_IMPL参数为空
	SOCKET_IMPL_SOCKET_VAL_IS_INVALID, //传入__tagSOCKET_IMPL的socket参数无效
};

enum __enuSOCKET_IO_TYPE
{
	SOCKET_IO_TYPE_IS_NULL = 0x0,
	SOCKET_IO_TYPE_IS_SELECT = 0x1,
	SOCKET_IO_TYPE_IS_ASYNC_SELECT = 0x2,
};

typedef struct __tagSOCKET_IMPL
{
	SOCKET socket_;
	unsigned short listen_port_;
	unsigned int recv_buffer_size_;
	unsigned int send_buffer_size_;
	SOCKADDR_IN remote_ip_;
	unsigned short remote_port_;

	HWND window_handle_;
	unsigned int window_message_;
	long network_event_;
	enum __enuSOCKET_IO_TYPE type_;//reference __enuSOCKET_IO_TYPE
}SOCKET_IMPL, *SOCKET_IMPL_PTR;

typedef bool (*udp_recv_callback)(void* args, const char*, unsigned);
bool init_socket();
bool create_udp_socket(SOCKET_IMPL_PTR socket_impl, const char_t* remote_ip);
int udp_recv(SOCKET_IMPL_PTR socket_impl, udp_recv_callback callback, void* args);//MAX PACK SIZE: MAX_PACK_NET_SIZE
int udp_send(SOCKET_IMPL_PTR socket_impl, const char* send_buf, unsigned int send_buf_size, const SOCKADDR* remote_ip = NULL, int flag = 0); // return value: 0: write status is not ready, 
void close_udp_socket(SOCKET_IMPL_PTR socket_impl);
void release_socket();
bool async_select(SOCKET_IMPL_PTR socket_impl, HWND window_handle, unsigned int message, long network_event);

#endif //__SOCKET_IMPL_H_

