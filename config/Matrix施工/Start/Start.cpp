// Start.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Start.h"
#include "StartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStartApp
//添加的标识只运行一次的属性名, 自己定义一个属性值,
// 在创建主框架时调用SetProp函数设置窗口属性。
CString	 g_strProgName = _T("启动工具条");
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
BEGIN_MESSAGE_MAP(CStartApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CStartApp construction

CStartApp::CStartApp()
: m_strHostIP(_T("192.168.0.12"))
, m_strAdminServerIP(_T("192.168.0.12"))
, m_strFTPServerIP(_T("192.168.0.12"))
, m_bUserLogin(FALSE)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile +=START_XMLFILE;
}


// The one and only CStartApp object

CStartApp theApp;


// CStartApp initialization

BOOL CStartApp::InitInstance()
{
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);    //枚举所有运行的窗口
	if(oldHWnd != NULL)
	{		
		::ShowWindow(oldHWnd,SW_SHOWNORMAL);          //激活找到的前一个程序
		::SetForegroundWindow(oldHWnd);                //把它设为前景窗口
		return false;                                  //退出本次运行
	}

	globalData.SetDPIAware ();
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
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
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
	SetRegistryBase (_T("Settings"));

	InitContextMenuManager();
	InitKeyboardManager();
	InitParam();
	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));


	CStartDlg dlg;
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

int CStartApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	UninitParam();
	BCGCBProCleanUp();
	return __super::ExitInstance();
}
/**
 * @brief 初始化程序参数
 * @note  读配置文件，打开网络通信端口，创建共享内存。
 * @param  void
 * @return BOOL，成功返回TRUE，失败返回FALSE
 */
BOOL  CStartApp::InitParam()
{
	// 读配置文件
	CStartParseXML		ParseXMLFile;
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile +=START_XMLFILE;
	if(!ParseXMLFile.ParseClientComputer(m_strLocalXMLFile,m_strHostIP,m_strFTPServerIP))
	{
		AfxMessageBox(_T("读取配置文件失败！"));
		return FALSE;
	}
	// 创建日志管理内存映射区
	if (!m_LogMapping.Create())
	{
		AfxMessageBox(_T("创建日志管理内存映射区失败!"));
		return FALSE;
	}
	m_LogMapping.WriteClientHostIP(m_strHostIP);
	m_LogMapping.WriteFTPServerHostIP(m_strFTPServerIP);
	return TRUE;
}
/**
 * @brief 释放程序参数
 * @note  关闭网络通信端口、释放共享内存。
 * @param  void
 * @return BOOL，成功返回TRUE，失败返回FALSE
 */
BOOL  CStartApp::UninitParam()
{
	m_LogMapping.Destroy();
	return TRUE;
}
/*
void CStartApp::SetVisualManager(int nVisualIndex) 
{	
	switch (nVisualIndex)
	{
	case 0:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		break;

	case 1:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case 2:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 3:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 4:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 5:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		break;

	case 6:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerCarbon));
		break;

	case 7:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2010));
		break;

	case 8:
		CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Blue);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
		break;

	case 9:
		CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Silver);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
		break;

	case 10:
		CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Black);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
		break;

	}
	 CBCGPVisualManager::GetInstance()->RedrawAll();
}*/