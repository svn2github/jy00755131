#pragma once
#include "..\MatrixCommDll\MatrixCommDll.h"
#include "..\MatrixServer\CommLineDll.h"
/**
* @class CLineCommDll
* @detail 测线客户端通讯类
*/
class CLineCommDll
{
public:
	CLineCommDll(void);
	~CLineCommDll(void);
public:
	/** 通讯类指针*/
	CMatrixCommDll*	m_pMatrixCommDll;
	/** 客户端通讯类指针*/
	CCommClient* m_pCommClient;
	/** Dll句柄*/
	HINSTANCE m_hCommDll;
 	/** 测线客户端参数类成员*/
 	CCommLineDll m_oXMLDllLine;
	/** 客户端界面句柄*/
	HWND m_hWnd;
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

