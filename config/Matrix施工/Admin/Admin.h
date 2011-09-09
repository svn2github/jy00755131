// Admin.h : Admin 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


#include "..\common\UserInfo.h"
#include "..\common\SocketParam.h"
#include "..\common\LogMapping.h"
#include "SocketUserAdmin.h"
// 自定义消息
// 收到服务器发送全部用户信息后，发送该消息
#define		WM_GETALLUSERINFO		WM_USER+200

// CAdminApp:
// 有关此类的实现，请参阅 Admin.cpp
//
class CAdminApp : public CWinApp,
			      public CBCGPWorkspace
{
public:
	CAdminApp();

	//////////////////////////////////////////////////////////////////////////
	/** 主机IP */ 
	CString			m_strHostIP;
	/** Admin Server主机IP地址*/
	CString		m_strAdminServerIP;
	////////////////////////////用户管理//////////////////////////////////////
	/** 所有用户信息*/
	CUserInfos		m_AllUserInfo;
	/** 管理用户端口*/
	CSocketUserAdmin	m_sockUserAdmin;
	/** 日志内存映射区*/
	CLogMapping		m_LogMapping;
	// 方法
public:	
	/// 初始化参数
	BOOL		InitParam();
	/// 释放参数
	BOOL		UninitParam();
	/// 写日志
	void		WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType=LOGTYPE_NORMAL,BOOL bShowed=FALSE);
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual int ExitInstance();
};

extern CAdminApp theApp;