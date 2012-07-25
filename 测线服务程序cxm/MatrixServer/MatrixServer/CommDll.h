#pragma once
#include "MatrixDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommDll
{
public:
	CCommDll(void);
	~CCommDll(void);
public:
	CMatrixDllCall* m_pMatrixDllCall;
	// 通讯类指针
	CMatrixCommDll*	m_pMatrixCommDll;
	// 服务端通讯类指针
	CCommServer* m_pCommServer;
	// Dll句柄
	HINSTANCE m_hCommDll;
public:
	// 载入MatrixServerDll动态链接库
	void LoadMatrixCommDll(CString strPath);
	// 释放MatrixServerDll动态链接库
	void FreeMatrixCommDll(void);
	// 创建服务端通讯
	void OnCreateServerComm();
	// 释放服务端通讯
	void OnDeleteServerComm();
	// 初始化
	void OnInit(CString strPath);
	// 关闭
	void OnClose(void);
};

