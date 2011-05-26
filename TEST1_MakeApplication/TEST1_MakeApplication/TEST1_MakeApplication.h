// TEST1_MakeApplication.h : main header file for the TEST1_MakeApplication application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTEST1_MakeApplicationApp:
// See TEST1_MakeApplication.cpp for the implementation of this class
//

class CTEST1_MakeApplicationApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CTEST1_MakeApplicationApp();

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

extern CTEST1_MakeApplicationApp theApp;