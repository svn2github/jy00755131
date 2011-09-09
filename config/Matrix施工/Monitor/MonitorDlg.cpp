// MonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "MonitorDlg.h"
#include "CiPlotChannelX.h"
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


// CMonitorDlg 对话框




CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorDlg::IDD, pParent)
	,m_dwXValue(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPLOTX1, m_iPlotX);
}

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMonitorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMonitorDlg 消息处理程序

BOOL CMonitorDlg::OnInitDialog()
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
	if(!m_SockSample.CreateSocket(0xDF00,this))
	{
		 AfxMessageBox(_T("打开m_SocketData的端口失败！"));
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorDlg::OnPaint()
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
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonitorDlg::AnalysisProtocolOfSampleSock(void)
{
	static unsigned char	pBuf[46100];	// 数据缓冲区最大长度是发送采集排列的数据帧
	WORD			wFrmLen = 46100;		// 帧长: 初始化为缓冲区的大小，被ReceiveFrom函数使用
	WORD			wFDUCount;			
	DWORD			dwFrmIndex;				// 帧计数	
	int				i,nReadSize;
	DWORD			dwTemp;
	CString         strTemp;
	DWORD			dwFDUIP;		// FDU IP地址
	DWORD			dwSampIndex;	// 采样数据帧计数

	// 接收来自客户端的数据
	nReadSize = m_SockSample.Receive( pBuf, wFrmLen);
	switch (nReadSize)
	{
	case 0:		// 没有收到数据 
		return;
		break;
	case SOCKET_ERROR:	// 发生错误，记录日志
		if (GetLastError() != WSAEWOULDBLOCK) 
		{  //发生socket错误
			strTemp=_T(" socket接收数据错误！");
			AfxMessageBox(strTemp);
			return;
		}
		break;
	default:
		break;
	}
	// -----------解析帧格式---------------------------------------------------------
	// 判断帧头是否0xEB, 0x90
	if (pBuf[0]!=0xEB || pBuf[1]!=0x90)
	{
		strTemp.Format(_T(" Frame head(%d,%d)is wrong！"),pBuf[0],pBuf[1]);
		AfxMessageBox(strTemp);
		return;
	}
	// 计算帧长度
	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);
	// 	if(wFrmLen>24576)
	// 	{	// 帧长度超长，发生错误记录日志		
	// 		strTemp.Format(_T(" Frame length(%d)is wrong！"),wFrmLen);
	// 		AfxMessageBox(strTemp);		
	// 		return;
	// 	}
	// 计算帧计数
	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4 , 4);
	// 帧尾
	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
	{
		// 帧尾错误，需要重新定位帧头
		strTemp.Format(_T(" Frame end(%d,%d)is wrong！"),pBuf[wFrmLen-2],pBuf[wFrmLen-1]);
		AfxMessageBox(strTemp);
		return;
	}
	// -------------------解析命令码，按照命令码进行相应操作-------------------------------
	// 采集道
	wFDUCount=MAKEWORD(pBuf[11],pBuf[12]);
	// 
	CiPlotChannelX iPlotChn;
	iPlotChn = m_iPlotX.get_Channel(0);
	// 先显示第一道数据
	memcpy_s(&dwFDUIP,sizeof(dwFDUIP),pBuf+13 , 4);
	memcpy_s(&dwSampIndex,sizeof(dwSampIndex),pBuf+17 , 4);
	for (i=0;i<74;i++)
	{
		memcpy_s(&dwTemp,sizeof(dwTemp),pBuf+21+i*3,4);
		dwTemp = dwTemp & 0x00FFFFFF;
		m_dwXValue++;		
		iPlotChn.AddXY(m_dwXValue,dwTemp);
		// m_OScopeCtrl.AppendPoint(dwTemp);
	}
}
void CMonitorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_SockSample.Close();
}

void CMonitorDlg::OnBnClickedButton1()
{
	static unsigned char	pBuf[250];
	//static unsigned long  dwData=1;
	static unsigned char  byData=0;
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;

	// 帧长
	pBuf[2] = 246;
	pBuf[3] = 0;
	// 帧计数
	// memcpy_s(pBuf+4,4,&dwFrmIndex,4);
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  =0;
	pBuf[10] = 0;
	// 数据个数
	pBuf[11] =1;
	pBuf[12] =0;
	// 空闲线程索引号
	memset(pBuf+13,0,4);
	memset(pBuf+17,0,4);

	for(int i=0;i<74;i++)
	{
		// memcpy_s(pBuf+21+i*3,4,&dwData,4);
		memset(pBuf+21,byData,222);
	}
	byData++;
	//dwData+=10;
	//if(dwData>0xFFFFF0)
	//	dwData = 1;
	// 帧校验
	pBuf[243]=0;
	// 帧尾
	pBuf[244] = 0x14;
	pBuf[245] = 0x6F;

	m_SockSample.SendTo(pBuf,246,0xDF00,_T("192.168.0.12"));
}
