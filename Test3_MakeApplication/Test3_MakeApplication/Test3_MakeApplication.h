// Test3_MakeApplication.h : main header file for the Test3_MakeApplication application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTest3_MakeApplicationApp:
// See Test3_MakeApplication.cpp for the implementation of this class
//

class CTest3_MakeApplicationApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CTest3_MakeApplicationApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTest3_MakeApplicationApp theApp;