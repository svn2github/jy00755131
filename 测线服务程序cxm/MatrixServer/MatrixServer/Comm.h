#pragma once
#include "CommServer.h"
#include "MatrixDllCall.h"
/**
* @brief 与客户端连接的通讯类
*/
class CComm
{
public:
	CComm(void);
	~CComm(void);
public:
	/** 服务端CAsyncSocket*/
	CCommServer m_oServerAsyncSocket;
	/** DLL函数调用类成员*/
	CMatrixDllCall* m_pMatrixDllCall;
public:
	/** 初始化*/
	void OnInit(void);
	/** 关闭*/
	void OnClose(void);
	/** 初始化套接字库*/
	void OnInitSocketLib(void);
	/** 释放套接字库*/
	void OnCloseSocketLib(void);
};

