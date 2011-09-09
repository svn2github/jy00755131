// AdminServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "..\common\UserInfo.h"
#include "..\common\SocketParam.h"
#include "..\common\XMLFileNameParam.h"
#include "..\common\CLogBase.h"
#include "LogFile.h"
#include "SocketLogInfo.h"
// CAdminServerApp:
// See AdminServer.cpp for the implementation of this class
//

class CAdminServerApp : public CWinApp
{
public:
	CAdminServerApp();
// 属性
public:
	//////////////////////////////////////////////////////////////////////////
	/** 主机IP */
	CString			m_strHostIP;
	/** 本地配置文件*/
	CString			m_strLocalXMLFile;
	////////////////////////////用户管理//////////////////////////////////////
	/** 所有用户信息*/
	CUserInfos		m_AllUserInfo;
	////////////////////////////日志管理////////////////////////////////
	/** 日志记录*/
	CLogBase		m_Log;
	/** 写日志文件 */
	CLogFile		m_LogFile;
	/** 接收日志信息 */
	CSocketLogInfo	m_sockLogInfo;
	//////////////////////////////////////////////////////////////////////////
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAdminServerApp theApp;