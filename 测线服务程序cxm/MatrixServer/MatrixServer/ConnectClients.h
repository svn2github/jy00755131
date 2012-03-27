#pragma once
#include "ClientInfo.h"
// 线程函数的等待时间
#define WaitForCommThreadSleepTimes	100
class CConnectClients
{
public:
	CConnectClients(void);
	~CConnectClients(void);
public:
	// 监听Socket
	SOCKET m_SocketListen;
	// 服务程序的Sockaddr
	SOCKADDR_IN m_ServerInternetAddr;
	// 事件触发总数
	DWORD m_dwEventTotal;
	// WSAEvent事件数组
	WSAEVENT m_WSAEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	// 连接的客户端结构体数组
	CClientInfo* m_ClientInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
public:
	// 得到服务器连接Socket
	BOOL OnGetListenSocket(void);
	// 选择触发事件的Socket消息
	BOOL OnSocketEventSelect(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);
	// 将Socket同接收客户端IP地址和端口绑定
	BOOL OnSocketBind(u_long hostlong, u_short hostshort);
	// 设定监听客户端个数
	BOOL OnSocketListen(int backlog);
	// 创建Server端口监听
	BOOL OnCreateSocketListen(u_long hostlong, u_short hostshort, int backlog);
	// 创建一个客户端连接信息
	BOOL CreateSocketInformation(SOCKET s);
	// 释放一个客户端连接信息
	void FreeSocketInformation(DWORD Event);
	// 处理函数
	BOOL OnProcComm(void);
	// 处理TCP/IP连接事件
	int OnProcAcceptEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event);
	// 处理TCP/IP接收事件
	int OnProcRevEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event);
	// 处理TCP/IP发送事件
	int OnProcSendEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event);
	// 处理TCP/IP关闭事件
	int OnProcCloseEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event);
};

