
// SocketSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketSend.h"
#include "SocketSendDlg.h"
#include "afxdialogex.h"
#include <string>
using std::string;

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


// CSocketSendDlg 对话框


CSocketSendDlg::CSocketSendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketSendDlg::IDD, pParent)
	, m_uiDstPort(0)
	, m_bBroadCast(FALSE)
	, m_uiRcvBuf(0)
	, m_uiSndBuf(0)
	, m_SocketSnd(INVALID_SOCKET)
	, m_uiCount(0)
	, m_uiSndNum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DSTPORT, m_uiDstPort);
	DDX_Check(pDX, IDC_CHECK_BROADCAST, m_bBroadCast);
	DDX_Text(pDX, IDC_EDIT_RCVBUF, m_uiRcvBuf);
	DDX_Text(pDX, IDC_EDIT_SNDBUF, m_uiSndBuf);
	DDX_Text(pDX, IDC_EDIT_SNDNUM, m_uiSndNum);
}

BEGIN_MESSAGE_MAP(CSocketSendDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CSocketSendDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CSocketSendDlg 消息处理程序

BOOL CSocketSendDlg::OnInitDialog()
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
	m_uiDstPort = 36666;
	m_bBroadCast = FALSE;
	m_uiRcvBuf = 4000000;
	m_uiSndBuf = 4000000;
	m_uiSndNum = 2000000;
	UpdateData(FALSE);
	GetDlgItem(IDC_IPADDRESS_DST)->SetWindowText(_T("192.168.100.19"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSocketSendDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketSendDlg::OnPaint()
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
HCURSOR CSocketSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketSendDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString str = _T("");
	string strConv = "";
	int ret = 0;
	timeval time_val = {0};
	fd_set write_fds;
	int	iOptval = 0;
	int iOptlen = 0;
	int iCount = 0;
	LARGE_INTEGER liFrq;
	LARGE_INTEGER liCountOld;
	LARGE_INTEGER liCountNow;
	double dbTime = 0;
	// 填充套接字地址结构
	m_SocketSnd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_bBroadCast == TRUE)
	{
		//设置广播模式
		iOptlen = sizeof(int);
		iOptval = 1;
		if (SOCKET_ERROR == setsockopt(m_SocketSnd, SOL_SOCKET, SO_BROADCAST, 
			reinterpret_cast<const char *>(&iOptval), iOptlen))
		{
			AfxMessageBox(_T("广播端口设置失败！"));
		}
	}
	if (SOCKET_ERROR == setsockopt(m_SocketSnd, SOL_SOCKET, SO_RCVBUF,  
		reinterpret_cast<const char *>(&m_uiRcvBuf), sizeof(int)))
	{
		AfxMessageBox(_T("接收缓冲区设置失败！"));
	}
	if (SOCKET_ERROR == setsockopt(m_SocketSnd, SOL_SOCKET, SO_SNDBUF,  
		reinterpret_cast<const char *>(&m_uiSndBuf), sizeof(int)))
	{
		AfxMessageBox(_T("发送缓冲区设置失败！"));
	}

	// 填充套接字地址结构
	m_addrSend.sin_family = AF_INET;
	m_addrSend.sin_port = htons(m_uiDstPort);
	GetDlgItem(IDC_IPADDRESS_DST)->GetWindowText(str);
	strConv = (CStringA)str;
	m_addrSend.sin_addr.S_un.S_addr = inet_addr(strConv.c_str());
	m_uiCount = 0;
	QueryPerformanceFrequency(&liFrq);
	QueryPerformanceCounter(&liCountOld);
	while(m_uiCount < m_uiSndNum)
	{
		FD_ZERO(&write_fds);
		FD_SET(m_SocketSnd, &write_fds);
		ret = select(NULL, NULL, &write_fds, NULL, &time_val);

		if((ret == 0) || (ret == SOCKET_ERROR))
		{
			continue;
		}
		iCount = sendto(m_SocketSnd, m_cFrameData, 256, 0, 
			reinterpret_cast<sockaddr*>(&m_addrSend), sizeof(m_addrSend));
		if (iCount == 256)
		{
			m_uiCount++;
		}
	}
	QueryPerformanceCounter(&liCountNow);
	dbTime = (double)(liCountNow.LowPart - liCountOld.LowPart);
	dbTime /= liFrq.LowPart;
	str.Format(_T("已发送 %d 帧，用时%.5f秒!"), m_uiCount, dbTime);
	AfxMessageBox(str);
}
