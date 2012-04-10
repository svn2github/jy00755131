#pragma once
#include "ClientRecFrame.h"
#include <hash_map>
using stdext::hash_map;
#define RecBufSize	819200

// CComClient 命令目标

class CComClient : public CAsyncSocket
{
public:
	CComClient();
	virtual ~CComClient();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	// 客户端连接索引指针
	hash_map<SOCKET, CComClient*>* m_pComClientMap;
	// 客户端接收帧成员类
	CClientRecFrame m_oClientRecFrame;
	// 接收缓冲区大小
	char m_cRecBuf[RecBufSize];
	// 接收点坐标
	int m_iPosRec;
	// 处理点坐标
	int m_iPosProc;
	// 帧内容长度
	unsigned short m_usFrameInfoSize;
public:
	// 创建一个客户端连接信息
	void CreateSocketInformation();
	// 释放一个客户端连接信息
	void FreeSocketInformation();
};


