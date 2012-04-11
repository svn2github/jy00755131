#pragma once
#include "CommProtocol.h"
#include <hash_map>
using stdext::hash_map;

// CClientSocket 命令目标
class CComClient;
class CClientSocket : public CAsyncSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	// 连接客户端索引指针
	hash_map<SOCKET, CComClient*>* m_pComClientMap;
	// 接收缓冲区大小
	char m_cRecBuf[ServerRecBufSize];
	// 接收点坐标
	int m_iPosRec;
	// 处理点坐标
	int m_iPosProc;
	// 帧内容长度
	unsigned short m_usFrameInfoSize;
	// 连接客户端类指针
	CComClient* m_pComClient;
	// 初始化
	void OnInit(CComClient* pComClient, int iSndBufferSize, int iRcvBufferSize);
	// 关闭
	void OnClose(void);
};


