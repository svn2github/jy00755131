// Matrix428.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Matrix428.h"
#include "Matrix428Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMatrix428App

BEGIN_MESSAGE_MAP(CMatrix428App, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMatrix428App construction

CMatrix428App::CMatrix428App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMatrix428App object

CMatrix428App theApp;


// CMatrix428App initialization

BOOL CMatrix428App::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	globalData.SetDPIAware ();


	CMatrix428Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	BCGCBProCleanUp ();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
