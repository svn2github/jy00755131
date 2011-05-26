// Test4.h : main header file for the Test4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTest4App:
// See Test4.cpp for the implementation of this class
//

class CTest4App : public CWinApp,
							public CBCGPWorkspace
{
public:
	CTest4App();

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

extern CTest4App theApp;