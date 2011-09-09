// Admin.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Admin.h"
#include "MainFrm.h"

#include "AdminDoc.h"
#include "AdminView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAdminApp
//添加的标识只运行一次的属性名, 自己定义一个属性值,
// 在创建主框架时调用SetProp函数设置窗口属性。
CString	 g_strProgName = _T("管理客户端V2011");
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

BEGIN_MESSAGE_MAP(CAdminApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CAdminApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CAdminApp 构造

CAdminApp::CAdminApp()
: m_strHostIP(_T("192.168.0.10"))
, m_strAdminServerIP(_T("255.255.255.255"))
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAdminApp 对象

CAdminApp theApp;


// CAdminApp 初始化

BOOL CAdminApp::InitInstance()
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

	globalData.SetDPIAware ();

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
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Samples"));

	//	LoadStdProfileSettings();  // 加载标准 INI 文件选项(包括 MRU)
	SetRegistryBase (_T("Settings"));

	InitContextMenuManager();
	InitKeyboardManager();
	InitParam();
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CAdminView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CBCGPDialog
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

// 用于运行对话框的应用程序命令
void CAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CAdminApp 消息处理程序

int CAdminApp::ExitInstance()
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
BOOL  CAdminApp::InitParam()
{
	DWORD  dwTemp;
	// 创建日志管理内存映射区
	if (!m_LogMapping.Open())
	{
		AfxMessageBox(_T("创建日志管理内存映射区失败!"));
		return FALSE;
	}	
	else
	{ 
		dwTemp = m_LogMapping.ReadClientHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strHostIP);
		dwTemp = m_LogMapping.ReadAdminServerHostIP();
		CLogMapping::DWORDIPToStringIP(dwTemp,m_strAdminServerIP);
	}
	
	if(!m_sockUserAdmin.CreateSocket(ADMIN_USERINFOPORT,m_strHostIP))
	{
		AfxMessageBox(_T("创建日志Socket端口失败!"));
		return 0;
	}	
	return TRUE;
}
/**
 * @brief 清除参数对象
 * @note  清除参数对象：关闭内存映射区，关闭通信端口
 * @param  void
 * @return 成功则返回true，失败则返回false
 */
BOOL  CAdminApp::UninitParam()
{	
	m_sockUserAdmin.Close();
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
void    CAdminApp::WriteLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{
	CLogBase	ProgramLog;
	ProgramLog.SetHostAndProgram(m_strHostIP,MATRIX_ADMIN);
	ProgramLog.SetLog(strPosition,strDesc,wType,bShowed);
	// 向服务器发送日志，由服务器记录日志信息
	BYTE    pBuf[SOCKET_LOGFRM_MAXLEN];
	int nSize = ProgramLog.WriteToSocketFrame(pBuf);
	m_sockUserAdmin.SendTo(pBuf,nSize,ADMINSERVER_LOGPORT,m_strAdminServerIP);	
}