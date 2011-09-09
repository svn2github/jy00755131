// Config.h : main header file for the Config application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CConfigApp:
// See Config.cpp for the implementation of this class
//
#include "..\common\SocketParam.h"
#include "..\common\XMLFileNameParam.h"
#include "..\common\LogMapping.h"
#include "SocketConfig.h"
class CConfigApp : public CWinApp,
					public CBCGPWorkspace
{
public:
	CConfigApp();
	/** 本机IP地址*/
	CString			m_strHostIP;
	/** Admin Server主机IP地址*/
	CString		m_strAdminServerIP;
	/** FTP服务器地址*/
	CString     m_strFTPServerIP;
	/** 本地XML配置文件（含路径） */
	CString     m_strLocalXMLFile;
	/** 发送日志的Socket端口 */
	CSocketConfig	m_sockConfig;
	/** 日志内存映射区*/
	CLogMapping		m_LogMapping;
// 方法
public:	
	/// 初始化参数
	BOOL		InitParam();
	/// 释放参数
	BOOL		UninitParam();
	/// 将XML配置文件写入到FTP服务器上
	bool	SaveXMLToFTPServer();
	/// 写日志
	void		WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType=LOGTYPE_NORMAL,BOOL bShowed=FALSE);
// Overrides
public:
// Implementation
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
};

extern CConfigApp theApp;