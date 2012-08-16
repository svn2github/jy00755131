#pragma once
#include "..\MatrixCommDll\MatrixCommDll.h"
#include "..\MatrixServer\CommOptDll.h"
class CCommDll
{
public:
	CCommDll(void);
	~CCommDll(void);
public:
	/** 通讯类指针*/
	CMatrixCommDll*	m_pMatrixCommDll;
	/** 客户端通讯类指针*/
	CCommClient* m_pCommClient;
	/** Dll句柄*/
	HINSTANCE m_hCommDll;
	/** 施工客户端参数类成员*/
	CCommOptDll m_oXMLDllOpt;
public:
	// 载入MatrixCommDll动态链接库
	void LoadMatrixCommDll(CString strPath);
	// 释放MatrixCommDll动态链接库
	void FreeMatrixCommDll(void);
	// 创建客户端通讯
	void OnCreateClientComm(unsigned int uiPort, CString strIP, HWND hWnd = NULL);
	// 释放客户端通讯
	void OnDeleteClientComm();
	// 初始化
	void OnInit(CString strPath, unsigned int uiPort = 0, CString strIP = _T(""), HWND hWnd = NULL);
	// 关闭
	void OnClose(void);
	/** 接收帧命令字处理*/
	void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread);
};

