// IC_TEST.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CIC_TESTApp:
// See IC_TEST.cpp for the implementation of this class
//

class CIC_TESTApp : public CWinApp
{
public:
	CIC_TESTApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CIC_TESTApp theApp;
