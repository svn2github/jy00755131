// AdminServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdminServer.h"
#include "AdminServerDlg.h"
#include "..\common\AdminParseXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAdminServerApp
//添加的标识只运行一次的属性名, 自己定义一个属性值,
// 在创建主框架时调用SetProp函数设置窗口属性。
CString	 g_strProgName = _T("管理服务器");
HANDLE   g_hProgValue = (HANDLE)1;               
// 被EnumWindows函数调用的回调函数，依据属性查找窗口
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
	HANDLE h = GetProp(hwnd,g_strProgName);
	if( h == g_hProgValue)
	{
		*(HWND*)lParam = hwnd;
		return false;
	}
	return true;
}

BEGIN_MESSAGE_MAP(CAdminServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAdminServerApp construction

CAdminServerApp::CAdminServerApp()
: m_strHostIP(_T("192.168.0.12"))
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_strLocalXMLFile = SERVERDIR_XMLFILE;
	m_strLocalXMLFile +=ADMINSERVER_XMLFILE;
}


// The one and only CAdminServerApp object

CAdminServerApp theApp;


// CAdminServerApp initialization

BOOL CAdminServerApp::InitInstance()
{
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);    //枚举所有运行的窗口
	if(oldHWnd != NULL)
	{		
		::ShowWindow(oldHWnd,SW_SHOWNORMAL);          //激活找到的前一个程序
		::SetForegroundWindow(oldHWnd);                //把它设为前景窗口
		return false;                                  //退出本次运行
	}
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Samples"));
	
	//--------------------------------------------//
	// 从XML文件读用户信息
	CAdminParseXML		ParseXML;
	m_strLocalXMLFile = SERVERDIR_XMLFILE;
	m_strLocalXMLFile +=ADMINSERVER_XMLFILE;
	ParseXML.ParseHostConfig(m_strLocalXMLFile,m_strHostIP);
	ParseXML.ParseAllUserInfo(m_strLocalXMLFile,&m_AllUserInfo);
	if(!m_sockLogInfo.CreateSocket(ADMINSERVER_LOGPORT,m_strHostIP))
	{
		AfxMessageBox(_T("创建日志Socket端口失败!"));
		return FALSE;
	}
	//--------------------------------------------//
	CAdminServerDlg dlg;
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CAdminServerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_sockLogInfo.Close();
	m_LogFile.CloseLogFile();
	return CWinApp::ExitInstance();
}
