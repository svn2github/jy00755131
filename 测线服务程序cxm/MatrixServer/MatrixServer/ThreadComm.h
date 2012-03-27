#pragma once
#include "ConnectClients.h"
// 服务器监听端口
#define ServerListenPort		5150
// 服务器允许连接的客户端个数
#define ConnectClientNumLimit	10
// 线程关闭的等待时间
#define WaitForCloseCommThreadTimes	500
class CThreadComm
{
public:
	CThreadComm(void);
	~CThreadComm(void);
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 线程句柄
	HANDLE m_hThread;
	// 线程ID
	DWORD m_dwThreadID;
	// 线程函数
	DWORD ThreadRunFunc(void);
	// 线程关闭事件
	HANDLE m_hThreadClose;
	// 关闭标志位
	bool m_bClose;
	// 处理函数
	BOOL OnProcComm(void);
	// 连接的客户端对象
	CConnectClients m_oConnectClients;
};

