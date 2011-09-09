// Install.h : Install 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CInstallApp:
// 有关此类的实现，请参阅 Install.cpp
//
#include "..\common\SocketParam.h"
#include "..\common\XMLFileNameParam.h"
#include "..\common\LogMapping.h"
#include "SocketInstall.h"
class CInstallApp : public CWinApp,
					public CBCGPWorkspace
{
public:
	CInstallApp();
public:
	/** 本机IP地址*/
	CString			m_strHostIP;
	/** Admin Server主机IP地址*/
	CString		m_strAdminServerIP;
	/** FTP服务器地址*/
	CString     m_strFTPServerIP;
	/** 本地XML配置文件（含路径） */
	CString     m_strLocalXMLFile;
	/** 发送日志的Socket端口 */
	CSocketInstall	m_sockInstall;
	/** 日志内存映射区*/
	CLogMapping		m_LogMapping;

// 重写
public:
	/// 初始化参数
	BOOL		InitParam();
	/// 释放参数
	BOOL		UninitParam();
	/// 将XML配置文件写入到FTP服务器上
	bool		SaveXMLToFTPServer();
	/// 写日志
	void		WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType=LOGTYPE_NORMAL,BOOL bShowed=FALSE);
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CInstallApp theApp;