// Matrix428.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CMatrix428App:
// See Matrix428.cpp for the implementation of this class
//

class CMatrix428App : public CWinApp
{
public:
	CMatrix428App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMatrix428App theApp;