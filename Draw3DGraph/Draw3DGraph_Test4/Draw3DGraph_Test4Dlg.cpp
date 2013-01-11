// Draw3DGraph_Test4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Draw3DGraph_Test4.h"
#include "Draw3DGraph_Test4Dlg.h"

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


// CDraw3DGraph_Test4Dlg 对话框




CDraw3DGraph_Test4Dlg::CDraw3DGraph_Test4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDraw3DGraph_Test4Dlg::IDD, pParent)
	, m_strFilePath(_T(""))
	, m_uiTraceNume(0)
	, m_iOff(0)
	, m_bOpenFile(false)
	, m_uiTimeCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDraw3DGraph_Test4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH3D1, m_ctrlGraph3D);
	DDX_Control(pDX, IDC_BTN_START, m_ctrlBtnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_ctrlBtnStop);
	DDX_Control(pDX, IDC_BTN_SELECTFILE, m_ctrlBtnSelectFile);
}

BEGIN_MESSAGE_MAP(CDraw3DGraph_Test4Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CDraw3DGraph_Test4Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDraw3DGraph_Test4Dlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SELECTFILE, &CDraw3DGraph_Test4Dlg::OnBnClickedBtnSelectfile)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_COLOR_SINGLE, &CDraw3DGraph_Test4Dlg::OnBnClickedRadioColorSingle)
	ON_BN_CLICKED(IDC_RADIO_COLOR_SPECTRUM, &CDraw3DGraph_Test4Dlg::OnBnClickedRadioColorSpectrum)
	ON_BN_CLICKED(IDC_RADIO_XYVIEW, &CDraw3DGraph_Test4Dlg::OnBnClickedRadioXyview)
	ON_BN_CLICKED(IDC_XZVIEW, &CDraw3DGraph_Test4Dlg::OnBnClickedXzview)
	ON_BN_CLICKED(IDC_RADIO_YZVIEW, &CDraw3DGraph_Test4Dlg::OnBnClickedRadioYzview)
END_MESSAGE_MAP()


// CDraw3DGraph_Test4Dlg 消息处理程序

BOOL CDraw3DGraph_Test4Dlg::OnInitDialog()
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
	rectCtrl.bottom = rectWindow.bottom * 7 / 16;
	m_ctrlBtnSelectFile.MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 1 / 2;
	rectCtrl.bottom = rectWindow.bottom * 17 / 32;
	GetDlgItem(IDC_RADIO_COLOR_SINGLE)->MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 9 / 16;
	rectCtrl.bottom = rectWindow.bottom * 19 / 32;
	GetDlgItem(IDC_RADIO_COLOR_SPECTRUM)->MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 5 / 8;
	rectCtrl.bottom = rectWindow.bottom * 21 / 32;
	GetDlgItem(IDC_RADIO_XYVIEW)->MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 11 / 16;
	rectCtrl.bottom = rectWindow.bottom * 23 / 32;
	GetDlgItem(IDC_RADIO_XZVIEW)->MoveWindow(rectCtrl);

	rectCtrl.top = rectWindow.bottom * 3 / 4;
	rectCtrl.bottom = rectWindow.bottom * 25 / 32;
	GetDlgItem(IDC_RADIO_YZVIEW)->MoveWindow(rectCtrl);

	OnInitGraph3D();
	m_ctrlBtnStart.EnableWindow(FALSE);
	m_ctrlBtnStop.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDraw3DGraph_Test4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDraw3DGraph_Test4Dlg::OnPaint()
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
HCURSOR CDraw3DGraph_Test4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDraw3DGraph_Test4Dlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	SetTimer(TimerID, TimerDelay, NULL);
}

void CDraw3DGraph_Test4Dlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bOpenFile == false)
	{
		return;
	}
	KillTimer(TimerID);
	OnInitCursor();
}
// 选择要打开的文件
CString CDraw3DGraph_Test4Dlg::SelectOpenFile(void)
{
	CString strPath = _T("");
	CFileDialog hFileDlg(true,NULL ,
		NULL,
		OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
		TEXT("数据文件 (*.text)|*.text|*.txt|所有文件(*.*)|*.*"),
		NULL);
	if(hFileDlg.DoModal() == IDOK)
	{
		strPath = hFileDlg.GetPathName();
	}
	return strPath;
}
void CDraw3DGraph_Test4Dlg::OnBnClickedBtnSelectfile()
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
	unsigned int uiRowNum = 0;
	unsigned int uiCurveRowNum = 0;
	unsigned int uiCurveColumnNum = 0;
	m_uiTraceNume = 0;
	m_iOff = 0;
	m_bOpenFile = false;
	m_uiTimeCount = 0;

	m_fin.open(m_strFilePath, ios::_Nocreate);
	getline(m_fin, str, '\n');
	getline(m_fin, str, '\n');
	getline(m_fin, str, '\n');
	m_iOff = m_fin.tellg();
	m_fin.seekg(FrameTimeBytesNume + m_iOff, ios::beg);
	while(1)
	{
		if (m_fin.peek() == 0x0a)
		{
			break;
		}
		m_fin.read(data, DataBytesNum);
		m_uiTraceNume++;
	}
	uiRowNum = m_uiTraceNume / ColumnNum;  //22=445/20
	m_xRow.SetSize(uiRowNum);           //X轴数据
	uiCurveRowNum = uiRowNum * (InterpolateRowNum + 1) - InterpolateRowNum;//211=22*(9+1)-9
	m_xCurveRow.SetSize(uiCurveRowNum); //差值计算后x轴数据
	m_dtRow.SetSize(uiRowNum);          //x轴方向衍生数据
	m_zAmpRowTemp.SetSize(uiRowNum);    //X轴方向采样数据临时缓冲
	for (unsigned int i=0; i<uiRowNum; i++)
	{
		m_xRow[i] = i + 1;
	}
	for (unsigned int i=0; i<uiCurveRowNum; i++)
	{
		m_xCurveRow[i] = 1.0 / (InterpolateRowNum + 1) * i + 1;
	}
	m_yColumn.SetSize(ColumnNum);
	uiCurveColumnNum = ColumnNum * (InterpolateColumnNum + 1) - InterpolateColumnNum;
	m_yCurveColumn.SetSize(uiCurveColumnNum);
	m_dtColumn.SetSize(ColumnNum);
	m_zAmpColumnTemp.SetSize(ColumnNum);
	for (unsigned int i=0; i<ColumnNum; i++)
	{
		m_yColumn[i] = i + 1;
	}
	for (unsigned int i=0; i<uiCurveColumnNum; i++)
	{
		m_yCurveColumn[i] = 1.0 / (InterpolateColumnNum + 1) * i + 1;
	}
	m_zAmpData.SetSize(uiRowNum, ColumnNum);                 //采样数据
	m_zCurveAmpData.SetSize(uiCurveRowNum, uiCurveColumnNum);//差值计算后Z轴数据
	for (unsigned int i=0; i< uiRowNum; i++)
	{
		for (unsigned int j = 0; j< ColumnNum; j++)
		{
			m_zAmpData(i, j) = 0.0;
		}
	}
	m_bOpenFile = true;
}  
void CDraw3DGraph_Test4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TimerID)
	{
		CString s = _T("");
		char data[10];
		unsigned int uiCounter = 0;
		int iTemp = 0;
		if (m_uiTimeCount % 72 == 0)
		{
			m_fin.seekg(FrameTimeBytesNume + m_iOff, ios::beg);
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
			if (uiCounter < (m_uiTraceNume / ColumnNum) * ColumnNum)//将数据存入20*22的缓冲中,只取440道数据
			{
				m_zAmpData(uiCounter / ColumnNum, uiCounter % ColumnNum) = iTemp;
			}
			uiCounter ++;
		}
		m_fin.seekg(2, ios::cur);
		m_uiTimeCount++;
		m_iOff = m_fin.tellg();
		// 先按行求差值
		for (unsigned int j=0; j<ColumnNum; j++)
		{
			for (unsigned int i=0; i<m_uiTraceNume / ColumnNum; i++)
			{
				m_zAmpRowTemp[i] = m_zAmpData(i, j);
			}
			CNiMath::Spline(m_xRow, m_zAmpRowTemp, 1, 1, m_dtRow);//求斜率
			for (unsigned int i=0; i<m_xCurveRow.GetSize(); i++)
			{
				CNiMath::InterpolateSpline(m_xRow, m_zAmpRowTemp, m_dtRow, m_xCurveRow[i], m_zCurveAmpData(i, j * (InterpolateColumnNum + 1)));//
			}
		}
 		// 按列求差值
		for (unsigned int i=0; i<m_xCurveRow.GetSize(); i++)
		{
			for (unsigned int j=0; j<ColumnNum; j++)
			{
				m_zAmpColumnTemp[j] = m_zCurveAmpData(i, j * (InterpolateColumnNum + 1));
			}
			CNiMath::Spline(m_yColumn, m_zAmpColumnTemp, 1, 1, m_dtColumn);
			for (unsigned int j=0; j<m_yCurveColumn.GetSize(); j++)
			{
				CNiMath::InterpolateSpline(m_yColumn, m_zAmpColumnTemp, m_dtColumn, m_yCurveColumn[j], m_zCurveAmpData(i, j));
			}
		}

		m_ctrlGraph3D.GetPlots().Item(1).Plot3DSurface(m_xCurveRow, m_yCurveColumn, m_zCurveAmpData);
	}
	CDialog::OnTimer(nIDEvent);
}

void CDraw3DGraph_Test4Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_fin.is_open())
	{
		m_fin.close();
	}
}

// 初始化光标
void CDraw3DGraph_Test4Dlg::OnInitCursor(void)
{
	m_ctrlGraph3D.Cursors.RemoveAll();
	m_Cursor = m_ctrlGraph3D.Cursors.Add();
	m_Cursor.Name = _T("Cursor 1");
	m_Cursor.Enabled = true;
	m_Cursor.Visible = true;
	m_Cursor.NameVisible = true;
	m_Cursor.TextColor = CNiColor(0, 0, 0);

	m_Cursor.XPosition = 5.0;
	m_Cursor.YPosition = 5.0;
	m_Cursor.ZPosition = 5.0;
	m_Cursor.PositionVisible = true;
	m_Cursor.SnapMode = CNiCursor3D::SnapNearestPlot;

	m_Cursor.PointSize = 10.0;
	m_Cursor.PointColor = CNiColor(0, 0, 128);
	m_Cursor.PointStyle = CNiCursor3D::Asterisk;

	m_Cursor.LineWidth = 1.0;
	m_Cursor.LineColor = CNiColor(255, 0, 0);
	m_Cursor.LineStyle = CNiCursor3D::LineSolid;

	m_Cursor.Transparency = 75;
	m_Cursor.PlaneColor = CNiColor(0, 128, 0);
	m_Cursor.XYPlaneVisible = false;
	m_Cursor.XZPlaneVisible = false;
	m_Cursor.YZPlaneVisible = false;
}

// 初始化绘图控件
void CDraw3DGraph_Test4Dlg::OnInitGraph3D(void)
{
	m_ctrlGraph3D.FastDraw = true;
	m_ctrlGraph3D.SetPlotAreaColor(RGB(128, 128, 128));
	// 	m_ctrlGraph3D.SetCaptionColor(RGB(213, 43, 213));
	// 	m_ctrlGraph3D.SetGridFrameColor(RGB(128, 0, 0));
	//	m_ctrlGraph3D.SetPlotAreaColor(RGB(0, 128, 0));
	/*m_ctrlGraph3D.GetPlots().Item(1).SetProjectionYZ(TRUE);*/

	m_ctrlGraph3D.GetPlots().Item(1).SetFillColor(RGB(0, 0, 0));
	//	m_ctrlGraph3D.Plots.Item(1).ColorMapStyle = CNiPlot3D::None;
	m_ctrlGraph3D.Plots.Item(1).ColorMapStyle = CNiPlot3D::ColorSpectrum;
	m_ctrlGraph3D.GetPlots().Item(1).SetLineColor(RGB(0, 191, 255));
	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::SurfaceLine);
	/*	m_ctrlGraph3D.GetPlots().Item(1).SetStyle(CNiPlot3D::SurfaceContour);*/

//	m_ctrlGraph3D.SetViewMode(CNiGraph3D::ViewXYPlane);
	OnInitAxis3D();
}

// 初始化3D坐标轴
void CDraw3DGraph_Test4Dlg::OnInitAxis3D(void)
{
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(1);
	m_Axis3D.SetCaption(_T("Row"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(2);
	m_Axis3D.SetCaption(_T("Column"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
// 	m_Axis3D.GetLabels().SetOpposite(true);
// 	m_Axis3D.GetLabels().SetNormal(false);
// 	m_Axis3D.SetCaptionOpposite(true);
// 	m_Axis3D.SetCaptionNormal(false);
	m_Axis3D = m_ctrlGraph3D.GetAxes().Item(3);
	m_Axis3D.SetMinMax(SampleAmpMin, SampleAmpMax);
	m_Axis3D.SetCaption(_T("Amp"));
	m_Axis3D.SetCaptionColor(RGB(255, 0, 0));
}

void CDraw3DGraph_Test4Dlg::OnBnClickedRadioColorSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlGraph3D.Plots.Item(1).ColorMapStyle = CNiPlot3D::None;
	m_ctrlGraph3D.GetPlots().Item(1).SetLineColor(RGB(0, 255, 0));
	((CButton*)GetDlgItem(IDC_RADIO_COLOR_SINGLE))->SetCheck(1); 
	((CButton*)GetDlgItem(IDC_RADIO_COLOR_SPECTRUM))->SetCheck(0); 
}

void CDraw3DGraph_Test4Dlg::OnBnClickedRadioColorSpectrum()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlGraph3D.Plots.Item(1).ColorMapStyle = CNiPlot3D::ColorSpectrum;
	m_ctrlGraph3D.GetPlots().Item(1).SetLineColor(RGB(0, 191, 255));
	((CButton*)GetDlgItem(IDC_RADIO_COLOR_SINGLE))->SetCheck(0); 
	((CButton*)GetDlgItem(IDC_RADIO_COLOR_SPECTRUM))->SetCheck(1); 
}

void CDraw3DGraph_Test4Dlg::OnBnClickedRadioXyview()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_RADIO_XYVIEW))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_YZVIEW))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_XZVIEW))->SetCheck(0);
	m_ctrlGraph3D.SetViewMode(CNiGraph3D::ViewXYPlane);
}

void CDraw3DGraph_Test4Dlg::OnBnClickedXzview()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_RADIO_XYVIEW))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_YZVIEW))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_XZVIEW))->SetCheck(1);
	m_ctrlGraph3D.SetViewMode(CNiGraph3D::ViewXZPlane);
}

void CDraw3DGraph_Test4Dlg::OnBnClickedRadioYzview()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_RADIO_XYVIEW))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_YZVIEW))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_XZVIEW))->SetCheck(0);
	m_ctrlGraph3D.SetViewMode(CNiGraph3D::ViewYZPlane);
}
