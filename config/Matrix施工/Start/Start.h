// Start.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CStartApp:
// See Start.cpp for the implementation of this class
//
#include "..\common\LogMapping.h"
#include "..\common\UserInfo.h"
#include "..\common\StartParseXML.h"
#include "SocketAdminServer.h"
class CStartApp : public CWinApp,
				  public CBCGPWorkspace
{
public:
	CStartApp();
	// 属性
public:
	//////////////////////////////////////////////////////////////////////////
	/** 本机IP地址 */
	CString		m_strHostIP;
	/** Admin Server主机IP地址*/
	CString		m_strAdminServerIP;
	/** 与管理服务器通信socket句柄：用户管理、日志管理 */
	CSocketAdminServer	m_sockAdminServer;
	/** FTP服务器地址*/
	CString     m_strFTPServerIP;
	/** 本地XML配置文件（含路径） */
	CString     m_strLocalXMLFile;
	//////////////////////////////////////////////////////////////////////////
	/** 接收发送日志Socket */
	// CSocketLogInfo		m_sockLogInfo;
	/** 日志内存映射区*/
	CLogMapping			m_LogMapping;
	/** 最新日志对象 */
	CLogBase			m_Log;	
	//////////////////////////////////////////////////////////////////////////
	/** 当前用户信息 */
	CUserInfo			m_UserInfo;
	/** 登录成功*/
	BOOL				m_bUserLogin;
// Overrides
public:
	virtual BOOL InitInstance();
	
	BOOL  InitParam();
	BOOL  UninitParam();
// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	// 设置界面风格
	// void SetVisualManager(int nVisualIndex);
};

extern CStartApp theApp;