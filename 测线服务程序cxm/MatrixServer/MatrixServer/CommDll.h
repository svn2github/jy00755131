#pragma once
#include "MatrixDllCall.h"
#include "CommServerDll.h"
#include "CommLineDll.h"
#include "CommOptDll.h"
class CCommDll
{
public:
	CCommDll(void);
	~CCommDll(void);
public:
	/** 通讯类指针*/
	CMatrixCommDll*	m_pMatrixCommDll;
	/** 服务端通讯类指针*/
	CCommServer* m_pCommServer;
	/** Dll句柄*/
	HINSTANCE m_hCommDll;
	/** 服务端程序命令响应类成员*/
	CCommServerDll m_oCommServerDll;
	/** 测线客户端程序命令响应类成员*/
	CCommLineDll m_oCommLineDll;
	/** 施工客户端程序命令响应类成员*/
	CCommOptDll m_oCommOptDll;
public:
	// 载入MatrixCommDll动态链接库
	void LoadMatrixCommDll(CString strPath);
	// 释放MatrixCommDll动态链接库
	void FreeMatrixCommDll(void);
	// 创建服务端通讯
	void OnCreateServerComm();
	// 释放服务端通讯
	void OnDeleteServerComm();
	// 初始化
	void OnInit(CString strPath);
	// 关闭
	void OnClose(void);
	/** 命令字解析*/
	void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread);
};


