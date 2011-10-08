// Ex1_04.h : main header file for the Ex1_04 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEx1_04App:
// See Ex1_04.cpp for the implementation of this class
//

class CEx1_04App : public CWinApp
{
public:
	CEx1_04App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEx1_04App theApp;