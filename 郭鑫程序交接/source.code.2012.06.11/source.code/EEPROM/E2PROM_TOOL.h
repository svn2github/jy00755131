// E2PROM_TOOL.h : main header file for the E2PROM_TOOL application
//

#if !defined(AFX_E2PROM_TOOL_H__013B6A92_3A8E_43B6_A727_30E1B85588C5__INCLUDED_)
#define AFX_E2PROM_TOOL_H__013B6A92_3A8E_43B6_A727_30E1B85588C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLApp:
// See E2PROM_TOOL.cpp for the implementation of this class
//

class CE2PROM_TOOLApp : public CWinApp
{
public:
	CE2PROM_TOOLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE2PROM_TOOLApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CE2PROM_TOOLApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

unsigned short str_to_hex2(char* buf, int buf_size);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E2PROM_TOOL_H__013B6A92_3A8E_43B6_A727_30E1B85588C5__INCLUDED_)
