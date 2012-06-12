/**
@brief	该文件为测线socket操作类的声明文件
@details	
@file	line.socket.h
@note	继承：CSocket  
@see	
@version	
@author	kuoxin	
@bug
*/


#ifndef __LINE_SOCKET__
#define __LINE_SOCKET__

#include "net.protocol.h"
#include <list>

#define __in__
#define __out__
#define __in_out__

#define SOCKET_CLOSED 0xff10 //!< 提示用户,该客户端网络连接已经中断
#define SOCKET_RESEND_ERROR	0xff20//!< 重发多次数据包,该数据包将被丢弃
//#define WM_LINE_SOCKET_NOTIFY WM_USER + 0x100 //!< 自定义socket通知消息

/**
@struct	__tagSOCKET_PACKAGE_SENT
@brief 存放发送的数据包信息
@note	
*/
typedef struct __tagSOCKET_PACKAGE_SENT
{
	unsigned short cmd_;//!< 命令字
	unsigned char cmd_type_; //!< 命令字类型1为命令, 3为应答
	unsigned int server_timestep_;//!< 服务端时间戳
	unsigned int client_timestep_;//!< 客户端时间戳
	unsigned int serial_id_;//!< 包流水号
	unsigned int cmd_index_;//!< // 命令流水号
	unsigned int num_; //!< 该命令字数据包总数
	unsigned int idx_; //!< 当前数据包是该命令字的第几包,以0开始
	char pack_[FrameSizeLimit]; //!< 发送数据包
	unsigned int size_; //!< 发送数据包大小 必须小于FrameSizeLimit
	unsigned int counter_sent_;//!< 发送计数器,当该参数超过line_socket::MAX_RESEND_COUNTER次数后,该数据包将被抛弃,并且提示用户
	unsigned int time_counter_;//!< 发送超时定时器
	char result_;//!< 应答设置和查询命令是否成功
	char tail_;//!< 帧尾
	int flag_;//!< 发送标志
}SOCKET_PACKAGE_SENT, *SOCKET_PACKAGE_SENT_PTR;

enum __enuPACKET_RECV_STATUS
{	HEAD_RECV = 0x0, LENGTH_RECV, DATA_RECV,};

/**
@struct	__tagSOCKET_PACKAGE_RECV
@brief 存放接受数据包信息
@note	
	帧命令字、时间戳、包流水号均相同，则认为是同一帧
*/
typedef struct __tagSOCKET_PACKAGE_RECV
{
	enum __enuPACKET_RECV_STATUS status_; //!< 接受状态
	char head_[4];//!< 帧头标志
	unsigned short length_;//!< 帧内容总长度，小于帧长限制
	unsigned short cmd_;//!< 帧命令字，表明帧的功能
	char cmd_type_;//!< 帧命令，为1则为命令,为3则为应答命令
	unsigned int server_timestep_;//!< 服务端时间戳
	unsigned int client_timestep_;//!< 客户端时间戳
	unsigned int serial_id_;//!< 包流水号
	unsigned int cmd_index_;//!<  命令流水号
	unsigned int num_;//!< 总帧数
	unsigned int index_;//!< 帧序号
	char result_;//!< 应答设置和查询命令是否成功
	char tail_;//!< 帧尾
	unsigned int size_;//!< 接受数据结构大小
	char pack_[FrameSizeLimit];//!< 接受数据包大小
}SOCKET_PACKAGE_RECV, *SOCKET_PACKAGE_RECV_PTR;

/**
@class line_socket
@brief 主要实现socket功能
@note 该类继承自CSocket
*/
class line_socket : public CAsyncSocket
{
public:
	line_socket();
	~line_socket();

public:
	static const int CURRENT_RECVBUF_SIZE = 10*1024*1024;//!< 发送缓冲
	static const int MAX_RESEND_COUNTER = 3;//!< 重发次数
	static const int HIT_TEST_TIME = 500;//!< 心跳测试时间以毫秒为单位
	static const int WAITING_FOR_RESP_TIME = 100000;//!< 等待超时时间
	static const int MAX_TIME_COUNTER = 3;//!< 最大等待发送响应超时次数
	static const int RECV_PROCESSING_LOOP_TIME = 100;//!< 接受处理循环等待时间,以毫秒为单位

public:
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnConnect(int nErrorCode);
	BOOL Create(UINT nSocketPort = 0,int nSocketType = SOCK_STREAM,LPCTSTR lpszSocketAddress = NULL);

public:
	bool Send(__in__ SOCKET_PACKAGE_SENT_PTR pack, __in__ int pack_num);
	void SetPackageCmd(__in_out__ SOCKET_PACKAGE_SENT& frame, __in__ unsigned short cmd, __in__ char cmd_type);
	void SetPackageNum(__in_out__ SOCKET_PACKAGE_SENT& frame, __in__ unsigned int num, __in__ unsigned int index);
	void SetPackageInfoSize(__in_out__ SOCKET_PACKAGE_SENT& frame, __in__ unsigned short size);
	void SetPackage(__in_out__ SOCKET_PACKAGE_SENT& frame, __in__ const void* buf, __in__ unsigned int size);
	SOCKET_PACKAGE_SENT_PTR GetFreePackSent(){return pop_free_pack();};

protected:
	virtual void send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
protected:

	SOCKET_PACKAGE_SENT_PTR pop_free_pack();
	void push_free_pack(SOCKET_PACKAGE_SENT_PTR pack);

	SOCKET_PACKAGE_SENT_PTR pop_sent_pack();
	void push_sent_pack(SOCKET_PACKAGE_SENT_PTR pack);

	void push_back_wait_sending_queue(SOCKET_PACKAGE_SENT_PTR pack);
	void push_front_wait_sending_queue(SOCKET_PACKAGE_SENT_PTR pack);
	SOCKET_PACKAGE_SENT_PTR pop_wait_sending_queue();

	void send_private(SOCKET_PACKAGE_SENT_PTR pack_ptr);

	SOCKET_PACKAGE_RECV_PTR pop_free_processing_pack();
	void push_free_processing_pack(SOCKET_PACKAGE_RECV_PTR pack);

	void push_wait_processing_queue_non_repeat(SOCKET_PACKAGE_RECV_PTR pack);
	void push_wait_processing_queue(SOCKET_PACKAGE_RECV_PTR pack);
	SOCKET_PACKAGE_RECV_PTR pop_wait_processing_queue();

	static UINT __cdecl recv_processing_thread_proc(LPVOID pParam);
	static UINT __cdecl check_timeout_thread_proc(LPVOID pParam);
	static UINT __cdecl send_processing_thread_proc(LPVOID pParam);

	void throw_socket_error(int error);

	void analyze_package_and_push_recv_queue(SOCKET_PACKAGE_RECV_PTR package);
	void normalize_package_and_push_recv_queue(SOCKET_PACKAGE_RECV_PTR package);
	void check_timeout_and_resend_data();
	void process_resp_pack(line_socket* ls, SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void process_pack_recvd(line_socket* ls, SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);

	template<typename T> void clear(std::list<T> queue, CCriticalSection& lock);

	LONG get_client_timestamp(){	return InterlockedIncrement(&timestep_);};
	LONG get_client_pack_id(){	return InterlockedIncrement(&packet_id_);};

protected:
	CString ip_; //!< 服务器ip地址
	unsigned short port_; //!< 服务器端口
	unsigned int recv_buff_size_; //!< 接受缓冲大小
private:
	CCriticalSection sent_pack_queue_lock_; //!< 发送完成等待应答数据包锁
	CCriticalSection wait_sending_pack_queue_lock_; //!< 等待发送数据包锁
	CCriticalSection free_pack_queue_lock_; //!< 闲置发送数据包锁
	std::list<SOCKET_PACKAGE_SENT_PTR> wait_sending_pack_queue_; //!< 等待发送数据包
	std::list<SOCKET_PACKAGE_SENT_PTR> free_pack_queue_; //!< 闲置发送数据包,可以在需要时取出用于发送
	std::list<SOCKET_PACKAGE_SENT_PTR> sent_pack_queue_; //!< 发送完成等待应答数据包

	CCriticalSection wait_processing_pack_queue_lock_; //!< 等待处理数据包锁
	CCriticalSection free_processing_pack_queue_lock_; //!< 闲置处理数据包锁
	std::list<SOCKET_PACKAGE_RECV_PTR> wait_processing_pack_queue_; //!< 等待处理数据包
	std::list<SOCKET_PACKAGE_RECV_PTR> free_processing_pack_queue_; //!< 闲置处理数据包

	CWinThread* recv_proc_thread_; //!< 处理接受数据线程,该线程将用于分发接受数据包同时提供应答数据包比较
	CWinThread* check_timeout_thread_;//!< 检查超时重发线程
	CWinThread* send_proc_thread_; //!<由于OnSend只能在WSAEWOULDBLOCK错误,Accept和Connect时,发送消息,因此需要使用发送线程
	
	CEvent stop_event_; //!< 停止循环通知事件

	SOCKET_PACKAGE_RECV recv_package_; //!< 当前接受数据包存放位置

	volatile unsigned int timestep_;// 客户端时间戳
	volatile unsigned int packet_id_;// 包流水号
public:
	virtual void OnClose(int nErrorCode);
};

#endif /*__LINE_SOCKET__*/