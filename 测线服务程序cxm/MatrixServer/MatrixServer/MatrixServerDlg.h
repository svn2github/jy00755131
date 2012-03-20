
// MatrixServerDlg.h : 头文件
//

#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
#include "ServerSocketEvent.h"
#define ServerListenPort 5150

// CMatrixServerDlg 对话框
class CMatrixServerDlg : public CDialogEx
{
// 构造
public:
	CMatrixServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MATRIXSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HMODULE m_hDllMod;
	m_oEnvironmentStruct* m_pEnv;
	SOCKET m_SocketListen;
	SOCKADDR_IN m_ServerInternetAddr;
	DWORD m_dwEventTotal;
	WSAEVENT m_WSAEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	CServerSocketEvent* m_ServerSocketEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	BOOL CreateSocketInformation(SOCKET s);
	void FreeSocketInformation(DWORD Event);
public:
	afx_msg void OnBnClickedBnStart();
	afx_msg void OnBnClickedBnStop();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonStartsample();
	afx_msg void OnBnClickedButtonStopsample();
	// DLL创建实例
	void Dll_Create_Instance(void);
	// DLL初始化
	void Dll_Init_Instance(void);
	// 初始化套接字库
	void OnInitSocketLib(void);
	// 释放套接字库
	void OnCloseSocketLib(void);
	// 创建Server端口监听
	void CreateSocketListen(void);
	// DLL关闭实例
	void Dll_Close_Instance(void);
	// DLL释放实例
	void Dll_Free_Instance(void);
	// 创建Server与客户端通讯线程
	void OnCreateCommThread(void);
	// 线程句柄
	HANDLE m_hThread;
	// 与客户端通讯线程函数
	DWORD CommThreadRun(void);
	// DLL开始AD数据采集
	void Dll_StartSample(void);
	// DLL停止AD数据采集
	void Dll_StopSample(void);
	// DLL开始工作
	void Dll_Work(void);
	// DLL停止工作
	void Dll_Stop(void);
};
