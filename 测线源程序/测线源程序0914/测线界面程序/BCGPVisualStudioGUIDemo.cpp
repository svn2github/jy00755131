//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2010 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPVisualStudioGUIDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "BCGPVisualManagerXP.h"
#include "AppLookDlg.h"

#include "InstrumentListDoc.h"
#include "InstrumentListView.h"

#include "SensorListDoc.h"
#include "SensorListView.h"

#include "InstrumentGraphDoc.h"
#include "InstrumentGraphChildFrame.h"
#include "InstrumentGraphView.h"

#include "SensorGraphDoc.h"
#include "SensorGraphChildFrame.h"
#include "SensorGraphView.h"

#include "SeismonitorGraphDoc.h"
#include "SeismonitorGraphChildFrame.h"
#include "SeismonitorGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPVisualStudioGUIDemoApp

BEGIN_MESSAGE_MAP(CBCGPVisualStudioGUIDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CBCGPVisualStudioGUIDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_APP_LOOK, OnViewAppLook)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPVisualStudioGUIDemoApp construction

CBCGPVisualStudioGUIDemoApp::CBCGPVisualStudioGUIDemoApp() :
	CBCGPWorkspace (TRUE)
{
	m_pDocTemplateInstrumentList = NULL;	// 仪器列表文档模板
	m_pDocTemplateSensorList = NULL;	// 检波器列表文档模板
	m_pDocTemplateInstrumentGraph = NULL;	// 仪器图形文档模板
	m_pDocTemplateSensorGraph = NULL;	// 检波器图形文档模板
	m_pDocTemplateSeismonitorGraph = NULL;	// 噪声监测图形文档模板

	m_bHiColorIcons = FALSE;
}

CBCGPVisualStudioGUIDemoApp::~CBCGPVisualStudioGUIDemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBCGPVisualStudioGUIDemoApp object

CBCGPVisualStudioGUIDemoApp theApp;

/**
* App初始化
* @param void
* @return void
*/
BOOL CBCGPVisualStudioGUIDemoApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	globalData.SetDPIAware ();

 	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1400
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Examples"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager ();

	EnableUserTools (ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10, 
					RUNTIME_CLASS (CBCGPUserTool), IDR_MENU_ARGS, IDR_MENU_DIRS);

	// 创建仪器列表文档模板
	m_pDocTemplateInstrumentList = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInstrumentListDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CInstrumentListView));
	AddDocTemplate (m_pDocTemplateInstrumentList);

	// 创建检波器列表文档模板
	m_pDocTemplateSensorList = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSensorListDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSensorListView));
	AddDocTemplate (m_pDocTemplateSensorList);

	// 创建仪器图形文档模板
	m_pDocTemplateInstrumentGraph = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInstrumentGraphDoc),
		RUNTIME_CLASS(CInstrumentGraphChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CInstrumentGraphView));
	AddDocTemplate (m_pDocTemplateInstrumentGraph);

	// 创建检波器图形文档模板
	m_pDocTemplateSensorGraph = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSensorGraphDoc),
		RUNTIME_CLASS(CSensorGraphChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSensorGraphView));
	AddDocTemplate (m_pDocTemplateSensorGraph);

	// 创建噪声监测图形文档模板
	m_pDocTemplateSeismonitorGraph = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSeismonitorGraphDoc),
		RUNTIME_CLASS(CSeismonitorGraphChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSeismonitorGraphView));
	AddDocTemplate (m_pDocTemplateSeismonitorGraph);

	// 生成主框架对象
	CMainFrame* pMainFrame = new CMainFrame;

	EnableLoadWindowPlacement (FALSE);

	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 重置工具条
	pMainFrame->OnResetToolBarMatrix();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		if (!pMainFrame->LoadMDIState (GetRegSectionPath ()))
		{
		}
	}
	else
	{
		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	if (!ReloadWindowPlacement (pMainFrame))
	{
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
	}

	// 现场数据对象初始化
	pMainFrame->OnSiteDataInit();
	// 加载现场数据
	pMainFrame->LoadSiteData();

	CAppLookDlg dlg (TRUE, m_pMainWnd);
	dlg.DoModal();

	return TRUE;
}

// App command to run the dialog
void CBCGPVisualStudioGUIDemoApp::OnAppAbout()
{
	BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}

/////////////////////////////////////////////////////////////////////////////
// Customization load/save methods

void CBCGPVisualStudioGUIDemoApp::PreLoadState ()
{
	GetMouseManager()->AddView (IDR_BCGDEVTYPE0, _T("My view"), IDR_BCGDEVTYPE0);
}

void CBCGPVisualStudioGUIDemoApp::LoadCustomState ()
{
}

void CBCGPVisualStudioGUIDemoApp::SaveCustomState ()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPVisualStudioGUIDemoApp message handlers

int CBCGPVisualStudioGUIDemoApp::ExitInstance() 
{
	BCGCBProCleanUp();
	return CWinApp::ExitInstance();
}

void CBCGPVisualStudioGUIDemoApp::OnViewAppLook() 
{
	CAppLookDlg dlg (FALSE, m_pMainWnd);
	dlg.DoModal ();
}

BOOL CBCGPVisualStudioGUIDemoApp::SaveAllModified() 
{
	if (!CWinApp::SaveAllModified ())
	{
		return FALSE;
	}

	CBCGPMDIFrameWnd* pMainFrame = 
		DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, m_pMainWnd);
	if (pMainFrame != NULL)
	{
		pMainFrame->SaveMDIState (GetRegSectionPath ());
	}

	return TRUE;
}
