// Operation.h : Operation 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// COperationApp:
// 有关此类的实现，请参阅 Operation.cpp
//

class COperationApp : public CWinApp,public CBCGPWorkspace
{
	// 属性
public:
	COperationApp();


	HINSTANCE    m_hinstBCGCBRes;
//	DWORD		 m_dwHostIP;
	CString		 m_strHostIP;
	
	
	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern COperationApp theApp;