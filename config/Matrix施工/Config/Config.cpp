// Config.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Config.h"
#include "MainFrm.h"

#include "ConfigDoc.h"
#include "ConfigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConfigApp

BEGIN_MESSAGE_MAP(CConfigApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CConfigApp construction

CConfigApp::CConfigApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_strHostIP = _T("127.0.0.1");
	m_strAdminServerIP= _T("255.255.255.255");
	m_strFTPServerIP= _T("255.255.255.255");
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile += CONFIG_XMLFILE;
}


// The one and only CConfigApp object

CConfigApp theApp;


// CConfigApp initialization

BOOL CConfigApp::InitInstance()
{
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	globalData.SetDPIAware ();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization

	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	InitContextMenuManager();
	InitKeyboardManager();
	InitShellManager();
	InitParam();
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CConfigDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CConfigView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	LoadCustomState ();
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CConfigApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CConfigApp message handlers


int CConfigApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	UninitParam();
	BCGCBProCleanUp();
	return __super::ExitInstance();
}
/**
 * @brief 初始化参数
 * @note  初始化参数：日志类、通信端口
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
BOOL  CConfigApp::InitParam()
{
	// 创建日志管理内存映射区
	if (!m_LogMapping.Open())
	{
		AfxMessageBox(_T("创建日志管理内存映射区失败!"));
		return FALSE;
	}	
	else
	{
		// 本机IP
		DWORD dwTemp = m_LogMapping.ReadClientHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strHostIP);
		// AdminServer管理服务器IP
		dwTemp = m_LogMapping.ReadAdminServerHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strAdminServerIP);
		// FTP服务器IP地址
		dwTemp = m_LogMapping.ReadFTPServerHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strFTPServerIP);
	}
	// 从FTP服务器上下载XML配置文件
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile += CONFIG_XMLFILE;
	// 创建显示数据流进程的Socket端口
	if(!m_sockConfig.CreateSocket(CONFIG_SYSPROGRESS,m_strHostIP))
	{
		AfxMessageBox(_T("创建显示施工状态的Socket端口失败!"));
		return FALSE;
	}
	return TRUE;
}
/**
 * @brief 清除参数对象
 * @note  清除参数对象：关闭内存映射区，关闭通信端口
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
BOOL  CConfigApp::UninitParam()
{	
	m_sockConfig.Close();
	m_LogMapping.Destroy();
	return TRUE;
}
/**
 * @brief 日志管理类
 * @note  程序的所有运行记录通过该函数发送出去，由AdminServer程序接收后再通知客户端。
 * @param  LPCTSTR strPosition,日志发生的位置描述
 * @param LPCTSTR strDesc,日志描述
 * @param WORD wType,日志类型：正常、异常、警告、调试等
 * @param BOOL bShowed，是否显示给用户
 * @return 成功则返回true，失败则返回false
 */
void    CConfigApp::WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{
	CLogBase	ProgramLog;
	ProgramLog.SetHostAndProgram(m_strHostIP,MATRIX_CONFIG);
	ProgramLog.SetLog(strPosition,strDesc,wType,bShowed);
	// 向服务器发送日志，由服务器记录日志信息
	BYTE    pBuf[SOCKET_LOGFRM_MAXLEN];
	int nSize = ProgramLog.WriteToSocketFrame(pBuf);
	m_sockConfig.SendTo(pBuf,nSize,ADMINSERVER_LOGPORT,m_strAdminServerIP);	
}
/**
 * @brief 将配置文件保存到FTP服务器上，并发生消息通知其他程序
 * @note  将配置文件保存到FTP服务器上，并发生消息通知其他程序
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
bool	CConfigApp::SaveXMLToFTPServer()
{	// 将XML文件传送到服务器上
	PutMatrixXMLToFTPServer(m_strFTPServerIP,CONFIG_XMLFILE,m_strLocalXMLFile);
	// 发送消息通知本地的其他程序
	SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_CONFIG,MATRIX_CONFIG_ALL);
	return true;
}