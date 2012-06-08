#pragma once
#include "ClientRecFrame.h"
#include "ClientRecThread.h"
#include "ClientSndFrame.h"
#include "ClientSndThread.h"
#include "ClientSocket.h"
#include "MatrixDllCall.h"
/**
* @class CCommClient
* @brief 与服务器连接的客户端成员类
*/
class CCommClient
{
public:
	CCommClient();
	virtual ~CCommClient();
public:
	/** 接收客户端帧成员类*/
	CClientRecFrame m_oClientRecFrame;
	/** 接收客户端帧的处理线程成员类*/
	CClientRecThread m_oClientRecThread;
	/** 发送客户端帧成员类*/
	CClientSndFrame m_oClientSndFrame;
	/** 发送客户端帧的处理线程成员类*/
	CClientSndThread m_oClientSndThread;
	/** 接收客户端通讯成员类*/
	CClientSocket m_oClientSocket;
	/** DLL函数调用类成员*/
	CMatrixDllCall* m_pMatrixDllCall;
	/** 连接客户端索引指针*/
	hash_map<SOCKET, CCommClient*>* m_pComClientMap;
public:
	// 创建一个客户端连接信息
	void OnInit();
	// 释放一个客户端连接信息
	void OnClose();
};


