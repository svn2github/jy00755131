// Operation.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Operation.h"
#include "MainFrm.h"
#include "OperationDoc.h"
#include "OperationView.h"
#include "..\\common\LineStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationApp
//添加的标识只运行一次的属性名, 自己定义一个属性值,
// 在创建主框架时调用SetProp函数设置窗口属性。
CString	 g_strProgName = _T("施工客户端2011");
HANDLE   g_hProgValue = (HANDLE)2;               
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

BEGIN_MESSAGE_MAP(COperationApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COperationApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COperationApp 构造
COperationApp::COperationApp():
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_strHostIP = _T("127.0.0.1");
	m_strAdminServerIP=_T("255.255.255.255");
	m_strFTPServerIP= _T("255.255.255.255");
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile += OPERATION_XMLFILE;
}


// 唯一的一个 COperationApp 对象

COperationApp theApp;


// COperationApp 初始化

BOOL COperationApp::InitInstance()
{
	
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);    //枚举所有运行的窗口
	if(oldHWnd != NULL)
	{		
		::ShowWindow(oldHWnd,SW_SHOWNORMAL);          //激活找到的前一个程序
		::SetForegroundWindow(oldHWnd);                //把它设为前景窗口
		return false;                                  //退出本次运行
	}
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

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
	CWinApp::InitInstance();

	
	globalData.SetDPIAware ();
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名	
	m_bSaveState=FALSE;
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Samples"));

	//LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	SetRegistryBase (_T("Settings"));

// 	SetRegistryBase (_T("SettingsOperation"));
// 	m_hinstBCGCBRes = LoadLibrary (_T("BCGCBProResCHS.dll"));    // *** - language
// 	BCGCBProSetResourceHandle (m_hinstBCGCBRes);

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();
	InitParam();

	
	//m_dwHostIP = CIPList::GetLocalIP();
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COperationDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(COperationView));
	//RUNTIME_CLASS(CActiveSourceView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;	
	ParseCommandLine(cmdInfo);
    // cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void COperationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

// 	// BCGPShowAboutDlg(AFX_IDS_APP_TITLE);
}


// COperationApp 消息处理程序
int COperationApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	UninitParam();
	BCGCBProCleanUp();
	/*FreeLibrary (m_hinstBCGCBRes);*/
	return __super::ExitInstance();
}

void COperationApp::PreLoadState ()
{

//	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}
/**
 * @brief 初始化参数
 * @note  初始化参数：日志类、通信端口
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
BOOL  COperationApp::InitParam()
{
	// 创建日志管理内存映射区
	if (!m_LogMapping.Open())
	{
		AfxMessageBox(_T("创建日志管理内存映射区失败!"));
		return FALSE;
	}	
	else
	{ 
		DWORD dwTemp = m_LogMapping.ReadClientHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strHostIP);
		dwTemp = m_LogMapping.ReadAdminServerHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strAdminServerIP);
		dwTemp = m_LogMapping.ReadFTPServerHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strFTPServerIP);
	}
	// 从FTP服务器上下载XML配置文件
	m_strLocalXMLFile = CLIENTDIR_XMLFILE;
	m_strLocalXMLFile += OPERATION_XMLFILE;
	if(!m_sockLogInfo.CreateSocket(OPERATION_LOGPORT,m_strHostIP))
	{
		AfxMessageBox(_T("创建日志Socket端口失败!"));
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
BOOL  COperationApp::UninitParam()
{
	m_sockLogInfo.Close();
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
void    COperationApp::WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{
	CLogBase	ProgramLog;
	ProgramLog.SetHostAndProgram(m_strHostIP,MATRIX_OPERATION);
	ProgramLog.SetLog(strPosition,strDesc,wType,bShowed);
	// 向服务器发送日志，由服务器记录日志信息
	BYTE    pBuf[SOCKET_LOGFRM_MAXLEN];
	int nSize = ProgramLog.WriteToSocketFrame(pBuf);
	m_sockLogInfo.SendTo(pBuf,nSize,ADMINSERVER_LOGPORT,m_strAdminServerIP);	
}

/**
 * @brief 将配置文件保存到FTP服务器上，并发生消息通知其他程序
 * @note  将配置文件保存到FTP服务器上，并发生消息通知其他程序
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
bool	COperationApp::SaveXMLToFTPServer( )
{	// 将XML文件传送到服务器上
	PutMatrixXMLToFTPServer(m_strFTPServerIP,OPERATION_XMLFILE,m_strLocalXMLFile);
	// 发送消息通知本地的其他程序
	SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_OPERATION,MATRIX_OPERATION_ALL);
	return true;
}