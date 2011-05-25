// 网络压力测试Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "网络压力测试.h"
#include "网络压力测试Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// C网络压力测试Dlg 对话框




C网络压力测试Dlg::C网络压力测试Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C网络压力测试Dlg::IDD, pParent)
	, m_uiTimeDelay(0)
	, m_uiSendFrameNum(0)
	, m_csIPAim(_T(""))
	, m_uiAimPort(0)
	, m_uiRealTimeDelay(0)
	, m_uiRealSendFrameNum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C网络压力测试Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C网络压力测试Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &C网络压力测试Dlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &C网络压力测试Dlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C网络压力测试Dlg 消息处理程序

BOOL C网络压力测试Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	CString str;
	GetDlgItem(IDC_EDIT_TIMEDELAY)->SetWindowText(_T("10"));
	GetDlgItem(IDC_EDIT_SENDFRAME)->SetWindowText(_T("20"));
	GetDlgItem(IDC_EDIT_REALTIMEDELAY)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT_REALSENDFRAME)->SetWindowText(_T("0"));
	GetDlgItem(IDC_IPADDRESS_AIM)->SetWindowText(_T("192.168.0.11"));
	str.Format(_T("%d"), SendPort);
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(str);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_ucSendFrame[0] = 0x11;
	m_ucSendFrame[1] = 0x22;
	m_ucSendFrame[2] = 0x33;
	m_ucSendFrame[3] = 0x44;

	SetTimer(2, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C网络压力测试Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C网络压力测试Dlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C网络压力测试Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C网络压力测试Dlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_TIMEDELAY)->GetWindowText(str);
	m_uiTimeDelay = _ttoi(str);
	GetDlgItem(IDC_EDIT_SENDFRAME)->GetWindowText(str);
	m_uiSendFrameNum = _ttoi(str);
	GetDlgItem(IDC_IPADDRESS_AIM)->GetWindowText(m_csIPAim);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(str);
	
	m_uiAimPort = _ttoi(str);
	
	m_uiRealSendFrameNum = 0;
	m_uiRealTimeDelay = 0;

	m_hSocket.Close();
	if (FALSE == m_hSocket.Create(RcvPort, SOCK_DGRAM))
	{
		AfxMessageBox(_T("端口创建失败！"));
	}
	if (FALSE == m_hSocket.SetSockOpt(SO_SNDBUF,&m_hSocket,SndBufSize,SOL_SOCKET))
	{
		AfxMessageBox(_T("设置发送缓冲区大小失败！"));
	}

	KillTimer(1);
	SetTimer(1, m_uiTimeDelay, NULL);
}

void C网络压力测试Dlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	m_hSocket.Close();
	KillTimer(1);
}

void C网络压力测试Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int icount = 0;
	DWORD dwTimeDelay1 = 0;
	DWORD dwTimeDelay2 = 0;
	if (1 == nIDEvent)
	{
		KillTimer(1);
		dwTimeDelay1 = GetTickCount();
		if (m_uiSendFrameNum > 0)
		{
			for (unsigned int i=0; i<m_uiSendFrameNum; i++)
			{
				icount = m_hSocket.SendTo(m_ucSendFrame, 256, m_uiAimPort, m_csIPAim);
				if (icount == 256)
				{
					m_uiRealSendFrameNum++;
				}
				else
				{
					AfxMessageBox(_T("发送数据帧失败！"));
				}
			}
		}
		dwTimeDelay2 = GetTickCount();
		m_uiRealTimeDelay = dwTimeDelay2 - dwTimeDelay1;
		SetTimer(1, m_uiTimeDelay, NULL);
	}
	if (2 == nIDEvent)
	{
		CString str = _T("");
		str.Format(_T("%u"), m_uiRealTimeDelay);
		GetDlgItem(IDC_EDIT_REALTIMEDELAY)->SetWindowText(str);
		str.Format(_T("%u"), m_uiRealSendFrameNum);
		GetDlgItem(IDC_EDIT_REALSENDFRAME)->SetWindowText(str);
	}
	CDialog::OnTimer(nIDEvent);
}
