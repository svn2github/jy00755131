// Operation.h : main header file for the Operation application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// COperationApp:
// See Operation.cpp for the implementation of this class
//

class COperationApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	COperationApp();

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

extern COperationApp theApp;