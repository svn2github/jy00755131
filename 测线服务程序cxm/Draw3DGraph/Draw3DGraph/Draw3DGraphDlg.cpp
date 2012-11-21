// Draw3DGraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Draw3DGraph.h"
#include "Draw3DGraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535
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


// CDraw3DGraphDlg 对话框




CDraw3DGraphDlg::CDraw3DGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDraw3DGraphDlg::IDD, pParent)
	, m_iCounter(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDraw3DGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH3D1, m_ctrlGraph3D);
	DDX_Control(pDX, IDC_BTN_START, m_ctrlBtnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_ctrlBtnStop);
}

BEGIN_MESSAGE_MAP(CDraw3DGraphDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CDraw3DGraphDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDraw3DGraphDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_RADIO_SURFACE, &CDraw3DGraphDlg::OnBnClickedRadioSurface)
	ON_BN_CLICKED(IDC_RADIO_SURFACELINE, &CDraw3DGraphDlg::OnBnClickedRadioSurfaceline)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDraw3DGraphDlg 消息处理程序

BOOL CDraw3DGraphDlg::OnInitDialog()
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
	ShowWindow(SW_SHOWMAXIMIZED);
	CRect rectWindow, rectCtrl;
	GetWindowRect(&rectWindow);
	rectCtrl = rectWindow;
	rectCtrl.right = rectWindow.right * 4 / 5;
	m_ctrlGraph3D.MoveWindow(rectCtrl);

	rectCtrl.left = rectWindow.right * 33 / 40;
	rectCtrl.right = rectWindow.right * 39 / 40;
	rectCtrl.top = rectWindow.bottom / 8;
	rectCtrl.bottom = rectWindow.bottom * 3 / 16;
	m_ctrlBtnStart.MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom / 4;
	rectCtrl.bottom = rectWindow.bottom * 5 / 16;
	m_ctrlBtnStop.MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 3 / 8;
	rectCtrl.bottom = rectWindow.bottom * 13 / 32;
	GetDlgItem(IDC_RADIO_SURFACE)->MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 7 / 16;
	rectCtrl.bottom = rectWindow.bottom * 15 / 32;
	GetDlgItem(IDC_RADIO_SURFACELINE)->MoveWindow(rectCtrl);

	m_xTimeData.SetSize(SampleTime);
	m_yTraceData.SetSize(TraceNum);
	m_zAmpData.SetSize(SampleTime, TraceNum);
	for (int i = 0; i < TraceNum; i++)
	{
		m_yTraceData[i] = i;
	}
	for (int i = 0; i < SampleTime; i++)
	{
		m_xTimeData[i] = i;
	}
// 	// Plot the data
// 	m_ctrlGraph3D.GetPlots().Item(1).Plot3DSurface(xData, yData, zData);
// 	m_ctrlGraph3D.GetPlots().Add();
// 	m_ctrlGraph3D.GetPlots().Item(2).Plot3DSurface(xData, yData, zData2);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDraw3DGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDraw3DGraphDlg::OnPaint()
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
HCURSOR CDraw3DGraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDraw3DGraphDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(TimerID, TimerDelay, NULL);
}

void CDraw3DGraphDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(TimerID);
}

void CDraw3DGraphDlg::OnBnClickedRadioSurface()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::Surface);
}

void CDraw3DGraphDlg::OnBnClickedRadioSurfaceline()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::SurfaceLine);
}

void CDraw3DGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TimerID)
	{
		if (m_iCounter < SampleTime)
		{
			for (int i = 0; i < m_iCounter; i++)
			{
				for (int j = 0; j < TraceNum; j++)
				{
					if (j < (TraceNum - 1) / 2)
					{
						m_zAmpData(i, j) = SampleAmpMax * sin((m_xTimeData[i] + m_iCounter) / 20 * PI + j * PI / (TraceNum - 1));
					}
					else
					{
						m_zAmpData(i, j) = SampleAmpMax * sin((m_xTimeData[i] + m_iCounter) / 20 * PI + (TraceNum - 1 - j) * PI / (TraceNum - 1));
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < SampleTime; i++)
			{
				for (int j = 0; j < TraceNum; j++)
				{
					if (j < (TraceNum - 1) / 2)
					{
						m_zAmpData(i, j) = SampleAmpMax * sin((m_xTimeData[i] + m_iCounter) / 20 * PI + j * PI / (TraceNum - 1));
					}
					else
					{
						m_zAmpData(i, j) = SampleAmpMax * sin((m_xTimeData[i] + m_iCounter) / 20 * PI + (TraceNum - 1 - j) * PI / (TraceNum - 1));
					}
				}
			}
		}
		m_ctrlGraph3D.GetPlots().Item(1).Plot3DSurface(m_xTimeData, m_yTraceData, m_zAmpData);
		m_iCounter++;
	}
	CDialog::OnTimer(nIDEvent);
}
