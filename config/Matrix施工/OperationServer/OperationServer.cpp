// OperationServer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OperationServer.h"
#include "OperationServerDlg.h"
#include "..\\common\LineStruct.h"
#include "OperationServerParseXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//添加的标识只运行一次的属性名, 自己定义一个属性值
CString	 g_strProgName = _T("施工服务");
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

// COperationServerApp

BEGIN_MESSAGE_MAP(COperationServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COperationServerApp 构造

COperationServerApp::COperationServerApp()
: m_dwFileNumber(SEGDFILENUMBER_INIT)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_dwFDUTime=0;
	m_dwHostIP =0x0100007F;	// 127.0.0.1
	m_dwMainCrossIP = 0xFFFFFFFF;	// 255.255.255.255
	m_wMainCrossPort = 0x5010;
	// 从FTP服务器上下载XML配置文件
	m_strLocalXMLFile = SERVERDIR_XMLFILE;
	m_strLocalXMLFile += OPERATIONSERVER_XMLFILE;
}


// 唯一的一个 COperationServerApp 对象

COperationServerApp theApp;


// COperationServerApp 初始化

BOOL COperationServerApp::InitInstance()
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

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("Windows OLE library initialization failed!"));
		return FALSE;
	}

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
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
	SetRegistryKey(_T("Setting"));

	ParseXMLFile();
	COperationServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int COperationServerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类 
	// 保存全局的SEGD文件编号
	COperationServerParseXML    ParseXML;	
	ParseXML.WriteLastUsedSEGDFileNumber(m_strLocalXMLFile,GetGlobalFileNumber(FALSE));
	return CWinApp::ExitInstance();
}

BOOL COperationServerApp::ParseXMLFile()
{
	// ---------读配置文件------------------------------------------
	COperationServerParseXML    ParseXML;
	m_strLocalXMLFile = SERVERDIR_XMLFILE;
	m_strLocalXMLFile += OPERATIONSERVER_XMLFILE;
	if(!ParseXML.ParseServerSetup(m_strLocalXMLFile))
	{		
		return FALSE;
	}	
	// 从配置文件中读取最后使用（上次使用）的文件编号，作为当前开始的SEGD文件标号
	DWORD dwFileNumber = ParseXML.ParseLastUsedSEGDFileNumber(m_strLocalXMLFile);
	SetGlobalFileNumber(dwFileNumber);
	return TRUE;
}