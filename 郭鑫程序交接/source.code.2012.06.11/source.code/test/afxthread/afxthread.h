// afxthread.h : main header file for the afxthread application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CafxthreadApp:
// See afxthread.cpp for the implementation of this class
//

class CafxthreadApp : public CWinApp
{
public:
	CafxthreadApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTestAfxbeginthread();
	afx_msg void OnTestAfxendthread();
};

extern CafxthreadApp theApp;