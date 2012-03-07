// Operation.h : Operation 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "..\common\SocketParam.h"
#include "..\common\XMLFileNameParam.h"
#include "..\common\ConfigParseXML.h"
#include "..\common\LogMapping.h"
#include "SocketLogInfo.h"
// COperationApp:
// 有关此类的实现，请参阅 Operation.cpp
//

class COperationApp : public CWinApp,public CBCGPWorkspace
{
	// 属性
public:
	COperationApp();
	/** 本机IP地址*/
	CString		 m_strHostIP;
	/** Admin Server主机IP地址*/
	CString		m_strAdminServerIP;
	/** FTP服务器地址*/
	CString     m_strFTPServerIP;
	/** 本地XML配置文件（含路径） */
	CString     m_strLocalXMLFile;
	/** 发送日志的Socket端口 */
	CSocketLogInfo	m_sockLogInfo;
	/** 日志内存映射区*/
	CLogMapping		m_LogMapping;
protected:
	HINSTANCE    m_hinstBCGCBRes;

public:	
	// cxm 2012.3.7
// 	BOOL  InitParam();
// 	BOOL  UninitParam();
// 	bool  SaveXMLToFTPServer();
// 	void  WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType=LOGTYPE_NORMAL,BOOL bShowed=FALSE);	
// 重写
public:
	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
	virtual BOOL InitInstance();
	
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern COperationApp theApp;