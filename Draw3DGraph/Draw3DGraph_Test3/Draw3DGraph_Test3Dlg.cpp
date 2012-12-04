// Draw3DGraph_Test3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Draw3DGraph_Test3.h"
#include "Draw3DGraph_Test3Dlg.h"

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


// CDraw3DGraph_Test3Dlg 对话框




CDraw3DGraph_Test3Dlg::CDraw3DGraph_Test3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDraw3DGraph_Test3Dlg::IDD, pParent)
	, m_strFilePath(_T(""))
	, m_uiTraceNume(0)
	, m_iOff(0)
	, m_bOpenFile(false)
	, m_uiTimeCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDraw3DGraph_Test3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SELECTFILE, m_ctrlBtnSelectFile);
	DDX_Control(pDX, IDC_CWGRAPH3D1, m_ctrlGraph3D);
	DDX_Control(pDX, IDC_BTN_START, m_ctrlBtnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_ctrlBtnStop);
}

BEGIN_MESSAGE_MAP(CDraw3DGraph_Test3Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SELECTFILE, &CDraw3DGraph_Test3Dlg::OnBnClickedBtnSelectfile)
	ON_BN_CLICKED(IDC_BTN_START, &CDraw3DGraph_Test3Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDraw3DGraph_Test3Dlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_RADIO_SURFACE, &CDraw3DGraph_Test3Dlg::OnBnClickedRadioSurface)
	ON_BN_CLICKED(IDC_RADIO_SURFACELINE, &CDraw3DGraph_Test3Dlg::OnBnClickedRadioSurfaceline)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDraw3DGraph_Test3Dlg 消息处理程序

BOOL CDraw3DGraph_Test3Dlg::OnInitDialog()
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

	rectCtrl.top = rectWindow.bottom * 1 / 2;
	rectCtrl.bottom = rectWindow.bottom * 9 / 16;
	m_ctrlBtnSelectFile.MoveWindow(rectCtrl);

	m_xTimeData.SetSize(SampleTime);

	m_ctrlGraph3D.SetPlotAreaColor(RGB(128, 128, 128));
	/*m_ctrlGraph3D.GetPlots().Item(1).SetProjectionYZ(TRUE);*/
	m_ctrlGraph3D.GetPlots().Item(1).SetFillColor(RGB(0, 0, 0));
	m_ctrlGraph3D.Plots.Item(1).ColorMapStyle = CNiPlot3D::None;
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(1);
	m_Axis3D.SetCaption(_T("Time"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(2);
	m_Axis3D.SetCaption(_T("Trace"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
	m_Axis3D.GetLabels().SetOpposite(true);
	m_Axis3D.GetLabels().SetNormal(false);
	m_Axis3D.SetCaptionOpposite(true);
	m_Axis3D.SetCaptionNormal(false);
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(3);
	m_Axis3D.SetMinMax(SampleAmpMin, SampleAmpMax);
	m_Axis3D.SetCaption(_T("Amp"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
	m_ctrlGraph3D.SetCaptionColor(RGB(213, 43, 213));
	m_ctrlGraph3D.SetGridFrameColor(RGB(128, 0, 0));
	//	m_ctrlGraph3D.SetPlotAreaColor(RGB(0, 128, 0));


	m_ctrlBtnStart.EnableWindow(FALSE);
	m_ctrlBtnStop.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDraw3DGraph_Test3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDraw3DGraph_Test3Dlg::OnPaint()
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
HCURSOR CDraw3DGraph_Test3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 选择要打开的文件
CString CDraw3DGraph_Test3Dlg::SelectOpenFile(void)
{
	CString strPath = _T("");
	CFileDialog hFileDlg(true,NULL ,
		NULL,
		OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
		TEXT("数据文件 (*.text)|*.text|*.txt|所有文件(*.*)|*.*|"),
		NULL);
	if(hFileDlg.DoModal() == IDOK)
	{
		strPath = hFileDlg.GetPathName();
	}
	return strPath;
}
void CDraw3DGraph_Test3Dlg::OnBnClickedBtnSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlBtnStart.EnableWindow(FALSE);
	m_ctrlBtnStop.EnableWindow(FALSE);
	m_strFilePath = SelectOpenFile();
	if (m_strFilePath == _T(""))
	{
		AfxMessageBox(_T("请选择数据文件！"));
		return;
	}
	m_ctrlBtnStart.EnableWindow(TRUE);
	m_ctrlBtnStop.EnableWindow(TRUE);
	if (m_fin.is_open())
	{
		m_fin.close();
	}
	string str = "";
	char data[10];
	m_uiTraceNume = 0;
	m_iOff = 0;
	m_bOpenFile = false;
	m_uiTimeCount = 0;
	char temp = 9;
	m_fin.open(m_strFilePath, ios::_Nocreate);
	getline(m_fin, str, '\n');
	getline(m_fin, str, '\n');
	getline(m_fin, str, '\n');
	m_iOff = m_fin.tellg();
	
	m_fin.read(data, 3);
	while(temp != m_fin.peek())
	{
		m_fin.seekg(1, ios::cur);
	}
	m_fin.seekg(1, ios::cur);
	while(1)
	{
		if (m_fin.peek() == 0x0a)
		{
			break;
		}
		m_fin.read(data, DataBytesNum);
		m_uiTraceNume++;
	}
	m_yTraceData.SetSize(m_uiTraceNume);
	for (unsigned int i=0; i<m_uiTraceNume; i++)
	{
		m_yTraceData[i] = i;
	}
	m_zAmpData.SetSize(SampleTime, m_uiTraceNume);
	for (unsigned int i=0; i< SampleTime; i++)
	{
		for (unsigned int j = 0; j< m_uiTraceNume; j++)
		{
			m_zAmpData(i, j) = 0.0;
		}
	}
	m_bOpenFile = true;

// 	while(1)
// 	{
// 		if ((iCount % 445 == 0) && (iCount > 0))
// 		{
// 			m_fin.seekg(2, ios::cur);
// 		}
// 		if (iCount % (445 * 72) == 0)
// 		{
// 			m_fin.seekg(13, ios::cur);
// 		}
// 		m_fin.read(data, 10);
// 		if (m_fin.fail() == true)
// 		{
// 			break;
// 		}
// 		s = data;
// 		sscanf_s(s, _T("%8d /t"), &i);
// 		iCount ++;
// 	}
}

void CDraw3DGraph_Test3Dlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	SetTimer(TimerID, TimerDelay, NULL);
}

void CDraw3DGraph_Test3Dlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	KillTimer(TimerID);
}

void CDraw3DGraph_Test3Dlg::OnBnClickedRadioSurface()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::Surface);
}

void CDraw3DGraph_Test3Dlg::OnBnClickedRadioSurfaceline()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	m_ctrlGraph3D.GetPlots().Item(1).SetLineColor(RGB(0, 255, 0));
	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::SurfaceLine);
}

void CDraw3DGraph_Test3Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TimerID)
	{
		CString s = _T("");
		char data[10];
		unsigned int uiCounter = 0;
		unsigned int x = 0;
		// 横向制表符为9
		char temp = 9;
		int iTemp = 0;
		if (m_uiTimeCount < SampleTime)
		{
			x = m_uiTimeCount;
			for (int i = 0; i < SampleTime; i++)
			{
				m_xTimeData[i] = i;
			}
		}
		else
		{
			x = SampleTime - 1;
			for (unsigned int i = 0; i < SampleTime; i++)
			{
				m_xTimeData[i] = i + m_uiTimeCount - SampleTime + 1;
			}
			for (unsigned int i = 1; i < SampleTime; i++)
			{
				for (unsigned int j = 0; j < m_uiTraceNume; j++)
				{
					m_zAmpData(i-1, j) = m_zAmpData(i, j);
				}
			}
 			m_Axis3D = m_ctrlGraph3D.GetAxes().Item(1);
 			m_Axis3D.SetMinMax(m_xTimeData[0], m_xTimeData[SampleTime - 1]);
		}
		if (m_uiTimeCount % 72 == 0)
		{
			m_fin.read(data, 3);
			if (m_fin.fail() == true)
			{
				CDialog::OnTimer(nIDEvent);
				return;
			}
			while(temp != m_fin.peek())
			{
				m_fin.seekg(1, ios::cur);
			}
			m_fin.seekg(1, ios::cur);
		}
		while(1)
		{
			if (m_fin.peek() == 0x0a)
			{
				break;
			}
			m_fin.read(data, DataBytesNum);
			if (m_fin.fail() == true)
			{
				break;
			}
			s = data;
			sscanf_s(s, _T("%8d /t"), &iTemp);
			if (iTemp > 10000)
			{
				iTemp = 0;
			}
			else if (iTemp < -10000)
			{
				iTemp = 0;
			}
			m_zAmpData(x, uiCounter) = iTemp;
			uiCounter ++;
		}
		m_fin.seekg(2, ios::cur);
		m_uiTimeCount++;
		m_iOff = m_fin.tellg();
		m_ctrlGraph3D.GetPlots().Item(1).Plot3DSurface(m_xTimeData, m_yTraceData, m_zAmpData);
	}
	CDialog::OnTimer(nIDEvent);
}

void CDraw3DGraph_Test3Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_fin.is_open())
	{
		m_fin.close();
	}
}
