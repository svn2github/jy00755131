
// MatrixServerDlg.h : 头文件
//

#pragma once
#include "ServerSocketEvent.h"
#include "MatrixDllCall.h"
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
	CMatrixDllCall m_oMatrixDllCall;
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
	afx_msg void OnBnClickedButtonStartsampleAll();
	afx_msg void OnBnClickedButtonStopsampleAll();

	// 初始化套接字库
	void OnInitSocketLib(void);
	// 释放套接字库
	void OnCloseSocketLib(void);
	// 创建Server端口监听
	void CreateSocketListen(void);
	// 创建Server与客户端通讯线程
	void OnCreateCommThread(void);
	// 线程句柄
	HANDLE m_hThread;
	// 与客户端通讯线程函数
	DWORD CommThreadRun(void);
	afx_msg void OnBnClickedBnAdcsetAll();
	afx_msg void OnBnClickedBnAdcsetByrout();
	afx_msg void OnBnClickedBnStartsampleByrout();
	afx_msg void OnBnClickedBnStopsampleByrout();
};
