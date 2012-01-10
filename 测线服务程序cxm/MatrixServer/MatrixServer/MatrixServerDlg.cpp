
// MatrixServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "MatrixServerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMatrixServerDlg 对话框




CMatrixServerDlg::CMatrixServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMatrixServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMatrixServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMatrixServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_START, &CMatrixServerDlg::OnBnClickedBnStart)
	ON_BN_CLICKED(IDC_BN_STOP, &CMatrixServerDlg::OnBnClickedBnStop)
	ON_BN_CLICKED(IDC_BN_CLOSE, &CMatrixServerDlg::OnBnClickedBnClose)
	ON_BN_CLICKED(IDC_BN_REFRESH, &CMatrixServerDlg::OnBnClickedBnRefresh)
	ON_BN_CLICKED(IDC_BN_SAVE, &CMatrixServerDlg::OnBnClickedBnSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CMatrixServerDlg::OnBnClickedButtonInit)
END_MESSAGE_MAP()


// CMatrixServerDlg 消息处理程序

BOOL CMatrixServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	hMod = LoadLibrary(_T("MatrixServerDll"));
	Create_Instance a = NULL;
	if (hMod)
	{
		a = (Create_Instance)GetProcAddress(hMod, "OnCreateInstance");
		pEnv = (*a)();
	}
	// 现场管理对象初始化
//	m_oSiteManage.OnInit();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMatrixServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMatrixServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMatrixServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMatrixServerDlg::OnBnClickedBnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	// 现场管理对象
//	m_oSiteManage.OnWork();
	On_Work a = NULL;
	if (hMod)
	{
		a = (On_Work)GetProcAddress(hMod, "OnWork");
		(*a)(pEnv);
	}
}


void CMatrixServerDlg::OnBnClickedBnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	// 现场管理对象
//	m_oSiteManage.OnStop();
	On_Stop a = NULL;
	if (hMod)
	{
		a = (On_Stop)GetProcAddress(hMod, "OnStop");
		(*a)(pEnv);
	}
}


void CMatrixServerDlg::OnBnClickedBnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	// 现场管理对象
//	m_oSiteManage.OnClose();
	On_Close a = NULL;
	if (hMod)
	{
		a = (On_Close)GetProcAddress(hMod, "OnClose");
		(*a)(pEnv);
	}
}


void CMatrixServerDlg::OnBnClickedBnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CMatrixServerDlg::OnBnClickedBnSave()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CMatrixServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	Free_Instance b = NULL;
	if (hMod)
	{
		b = (Free_Instance)GetProcAddress(hMod, "OnFreeInstance");
		(*b)(pEnv);
		FreeLibrary(hMod);
	}
}


void CMatrixServerDlg::OnBnClickedButtonInit()
{
	// TODO: 在此添加控件通知处理程序代码
	On_Init b = NULL;
	if (hMod)
	{
		b = (On_Init)GetProcAddress(hMod, "OnInit");
		(*b)(pEnv);
	}
}
