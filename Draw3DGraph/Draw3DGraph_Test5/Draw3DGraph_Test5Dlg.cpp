// Draw3DGraph_Test5Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Draw3DGraph_Test5.h"
#include "Draw3DGraph_Test5Dlg.h"

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


// CDraw3DGraph_Test5Dlg 对话框




CDraw3DGraph_Test5Dlg::CDraw3DGraph_Test5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDraw3DGraph_Test5Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDraw3DGraph_Test5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH3D1, m_ctrlGraph3D);
}

BEGIN_MESSAGE_MAP(CDraw3DGraph_Test5Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDraw3DGraph_Test5Dlg 消息处理程序

BOOL CDraw3DGraph_Test5Dlg::OnInitDialog()
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
	m_ctrlGraph3D.SetProjectionStyle(m_ctrlGraph3D.Perspective);//投射模式——近大远小

	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(1);
	m_Axis3D.SetCaption(_T("Y-axis"));
	m_Axis3D.SetCaptionColor(RGB(255, 255, 255));
	m_Axis3D.GetLabels().SetColor(RGB(255, 255, 255));
	m_Axis3D.GetLabels().SetOpposite(true);
	m_Axis3D.GetLabels().SetNormal(false);
	m_Axis3D.SetCaptionOpposite(true);
	m_Axis3D.SetCaptionNormal(false);
	m_Axis3D.SetInverted(true);
/*	m_Axis3D.SetCaptionOrientationStyle(m_Axis3D.FaceCamera);*/
	m_Axis3D.GetTicks().SetMajorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMajorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMinorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorGrid(true);
	m_Axis3D.GetTicks().SetMajorDivisions(5);
	m_Axis3D.GetTicks().SetMinorDivisions(5);
	m_Axis3D.GetTicks().SetOpposite(true);
	m_Axis3D.GetTicks().SetNormal(false);
 	m_Axis3D.GetTicks().SetOutside(true);
	m_Axis3D.GetTicks().SetInside(false);
	m_Axis3D.SetMinMax(0, 100);

	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(2);
	m_Axis3D.SetCaption(_T("X-axis"));
	m_Axis3D.SetCaptionColor(RGB(255, 255, 255));
	m_Axis3D.GetLabels().SetColor(RGB(255, 255, 255));
	m_Axis3D.GetLabels().SetOpposite(true);
	m_Axis3D.GetLabels().SetNormal(true);
	m_Axis3D.SetCaptionOpposite(true);
	m_Axis3D.SetCaptionNormal(true);
	/*	m_Axis3D.SetCaptionOrientationStyle(m_Axis3D.FaceCamera);*/
	m_Axis3D.GetTicks().SetMajorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMajorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMinorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorGrid(true);
	m_Axis3D.GetTicks().SetMajorDivisions(5);
	m_Axis3D.GetTicks().SetMinorDivisions(5);
	m_Axis3D.GetTicks().SetOpposite(true);
	m_Axis3D.GetTicks().SetNormal(false);
	m_Axis3D.GetTicks().SetOutside(true);
	m_Axis3D.GetTicks().SetInside(false);
	m_Axis3D.SetMinMax(0, 100);

 	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(3);
 	m_Axis3D.GetLabels().SetColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMajorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMajorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorTickColor(RGB(255, 255, 255));
	m_Axis3D.GetTicks().SetMinorGridColor(RGB(234, 234, 234));
	m_Axis3D.GetTicks().SetMinorGrid(true);
	m_Axis3D.GetTicks().SetMajorDivisions(5);
	m_Axis3D.GetTicks().SetMinorDivisions(5);
	m_Axis3D.GetTicks().SetOpposite(false);
	m_Axis3D.GetTicks().SetNormal(false);
	m_Axis3D.GetTicks().SetOutside(false);
	m_Axis3D.GetTicks().SetInside(false);
	m_Axis3D.SetMinMax(25, 35);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDraw3DGraph_Test5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDraw3DGraph_Test5Dlg::OnPaint()
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
HCURSOR CDraw3DGraph_Test5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

