// TabControlTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TabControlTest.h"
#include "TabControlTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTabControlTestApp

BEGIN_MESSAGE_MAP(CTabControlTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTabControlTestApp construction

CTabControlTestApp::CTabControlTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTabControlTestApp object

CTabControlTestApp theApp;


// CTabControlTestApp initialization

BOOL CTabControlTestApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	globalData.SetDPIAware ();


	CTabControlTestDlg dlg;
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
