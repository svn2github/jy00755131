// 绘图程序Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "绘图程序.h"
#include "绘图程序Dlg.h"
#include <math.h>
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


// C绘图程序Dlg 对话框




C绘图程序Dlg::C绘图程序Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C绘图程序Dlg::IDD, pParent)
	, m_uiShowDataNumCurrent(500)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C绘图程序Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChartViewer, m_ChartViewer);
	DDX_Control(pDX, IDC_PointerPB, m_PointerPB);
	DDX_Control(pDX, IDC_XZoomPB, m_XZoomPB);
	DDX_Control(pDX, IDC_HScrollBar, m_HScrollBar);
	DDX_Control(pDX, IDC_VScrollBar, m_VScrollBar);
}

BEGIN_MESSAGE_MAP(C绘图程序Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PointerPB, &C绘图程序Dlg::OnBnClickedPointerpb)
	ON_BN_CLICKED(IDC_ZoomInPB, &C绘图程序Dlg::OnBnClickedZoominpb)
	ON_BN_CLICKED(IDC_ZoomOutPB, &C绘图程序Dlg::OnBnClickedZoomoutpb)
	ON_BN_CLICKED(IDC_XZoomPB, &C绘图程序Dlg::OnBnClickedXzoompb)
	ON_BN_CLICKED(IDC_XYZoomPB, &C绘图程序Dlg::OnBnClickedXyzoompb)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_CONTROL(CVN_ViewPortChanged, IDC_ChartViewer, OnViewPortChanged)
	ON_BN_CLICKED(IDC_ChartViewer, OnBnClickedChartViewer)
END_MESSAGE_MAP()


// C绘图程序Dlg 消息处理程序

BOOL C绘图程序Dlg::OnInitDialog()
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
	// Load icons to mouse usage buttons
	loadButtonIcon(IDC_PointerPB, IDI_PointerPB, 100, 20);  
	loadButtonIcon(IDC_ZoomInPB, IDI_ZoomInPB, 100, 20);    
	loadButtonIcon(IDC_ZoomOutPB, IDI_ZoomOutPB, 100, 20);

	// Load icons to zoom/scroll direction control buttons
	loadButtonIcon(IDC_XZoomPB, IDI_XZoomPB, 105, 20);
	loadButtonIcon(IDC_XYZoomPB, IDI_XYZoomPB, 105, 20);

	//
	// Initialize member variables
	//
	m_extBgColor = getDefaultBgColor();     // Default background color
	m_minValue = m_maxValue = 0;            // y axes ranges

	setLicenseCode("R5MNGUVV3UXWFT2CC44B1D7E"); //破解chartdirector

	// Set up the ChartViewer to reflect the visible and minimum duration
	m_ChartViewer.setZoomInWidthLimit(ShowDataNumMin / ShowDataNumMax);
	m_ChartViewer.setViewPortWidth(m_uiShowDataNumCurrent / ShowDataNumMax);
	m_ChartViewer.setViewPortLeft(1 - m_ChartViewer.getViewPortWidth());

	// Initially set the mouse to drag to scroll mode in horizontal direction.
	m_PointerPB.SetCheck(1);
	m_XZoomPB.SetCheck(1);
	m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);

	// Can update chart now
	m_ChartViewer.updateViewPort(true, true);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C绘图程序Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C绘图程序Dlg::OnPaint()
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
HCURSOR C绘图程序Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 载入一个图标资源到按钮
void C绘图程序Dlg::loadButtonIcon(int buttonId, int iconId, int width, int height)
{
	GetDlgItem(buttonId)->SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)::LoadImage(
		AfxGetResourceHandle(), MAKEINTRESOURCE(iconId), IMAGE_ICON, width, height, 
		LR_DEFAULTCOLOR));  
}

// 得到默认的背景颜色
int C绘图程序Dlg::getDefaultBgColor(void)
{
	LOGBRUSH LogBrush; 
	HBRUSH hBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, (WPARAM)CClientDC(this).m_hDC, 
		(LPARAM)m_hWnd); 
	::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush); 
	int ret = LogBrush.lbColor;
	return ((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);
}

// 载入数据
void C绘图程序Dlg::LoadData(void)
{
	// The data for the line chart
	double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56,
		56, 60, 70, 76, 63, 67, 75, 64, 51};
	double data1[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67,
		58, 59, 73, 77, 84, 82, 80, 84, 98};
	double data2[] = {36, 28, 25, 33, 38, 20, 22, 30, 25, 33, 30, 24, 28, 15, 21, 26,
		46, 42, 48, 45, 43, 52, 64, 60, 70};

	// The labels for the line chart
	const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
		"24"};

	// Create an XYChart object of size 600 x 300 pixels, with a light blue (EEEEFF)
	// background, black border, 1 pxiel 3D border effect and rounded corners
	XYChart *c = new XYChart(300, 650, m_extBgColor);
	c->setRoundedFrame();

	// Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white
	// background. Turn on both horizontal and vertical grid lines with light grey
	// color (0xcccccc)
	c->setPlotArea(30, 45, 195, 520, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	// Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 9
	// pts Arial Bold font. Set the background and border color to Transparent.
	c->addLegend(225, 45, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	// Add a title box to the chart using 15 pts Times Bold Italic font, on a light
	// blue (CCCCFF) background with glass effect. white (0xffffff) on a dark red
	// (0x800000) background, with a 1 pixel 3D border.
	c->addTitle("Hitech Matrix428", "timesbi.ttf", 12)->setBackground(
		0xccccff, 0x000000, Chart::glassEffect());

	// Swap the x and y axis to become a rotated chart
	c->swapXY();
// 
	// Set the y axis on the top side (right + rotated = top)
	c->setYAxisOnRight(false);

// 	// Add a title to the y axis
// 	c->yAxis()->setTitle("Denoised by DTCWT");

	// Reverse the x axis so it is pointing downwards
	c->xAxis()->setReverse();

	// Set the labels on the x axis.
	c->xAxis()->setLabels(StringArray(labels, sizeof(labels)/sizeof(labels[0])));

	// Display 1 out of 3 labels on the x-axis.
	c->xAxis()->setLabelStep(3);

// 	// Add a title to the x axis
// 	c->xAxis()->setTitle("Jun 12, 2006");

	// Add a line layer to the chart
	LineLayer *layer = c->addLineLayer();

	// Set the default line width to 2 pixels
	layer->setLineWidth(2);

	// Add the three data sets to the line layer. For demo purpose, we use a dash
	// line color for the last line
	layer->addDataSet(DoubleArray(data0, sizeof(data0)/sizeof(data0[0])), 0xff0000, "FDU #1");
	layer->addDataSet(DoubleArray(data1, sizeof(data1)/sizeof(data1[0])), 0x008800, "FDU #2");
	layer->addDataSet(DoubleArray(data2, sizeof(data2)/sizeof(data2[0])), 0x3333ff, "FDU #3");

	// Set the chart image to the WinChartViewer
	delete m_ChartViewer.getChart();
	m_ChartViewer.setChart(c);
// 	// Output the chart
// 	c->makeChart("Matrix428.bmp");
// 
// 	//free up resources
// 	delete c;
}

void C绘图程序Dlg::OnBnClickedPointerpb()
{
	// TODO: 在此添加控件通知处理程序代码
	 m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
}

void C绘图程序Dlg::OnBnClickedZoominpb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ChartViewer.setMouseUsage(Chart::MouseUsageZoomIn);
}

void C绘图程序Dlg::OnBnClickedZoomoutpb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ChartViewer.setMouseUsage(Chart::MouseUsageZoomOut);
}

void C绘图程序Dlg::OnBnClickedXzoompb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ChartViewer.setZoomDirection(Chart::DirectionHorizontal); 
	m_ChartViewer.setScrollDirection(Chart::DirectionHorizontal);

	// Viewport is always unzoomed as y-axis is auto-scaled
	m_ChartViewer.setViewPortTop(0);
	m_ChartViewer.setViewPortHeight(1);

	// Update chart to auto-scale axis
	m_ChartViewer.updateViewPort(true, true);
}

void C绘图程序Dlg::OnBnClickedXyzoompb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ChartViewer.setZoomDirection(Chart::DirectionHorizontalVertical); 
	m_ChartViewer.setScrollDirection(Chart::DirectionHorizontalVertical);  
}

void C绘图程序Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nSBCode != SB_ENDSCROLL)
	{
		// User is still scrolling

		// Set the view port based on the scroll bar
		m_ChartViewer.setViewPortLeft(moveScrollBar(nSBCode, nPos, pScrollBar));
		// Update the chart image only, but no need to update the image map.
		m_ChartViewer.updateViewPort(true, false);
	}
	else
		// Scroll bar has stoped moving. Can update image map now.
		m_ChartViewer.updateViewPort(false, true);
}

void C绘图程序Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nSBCode != SB_ENDSCROLL)
	{
		// User is still scrolling

		// Set the view port based on the scroll bar
		m_ChartViewer.setViewPortTop(moveScrollBar(nSBCode, nPos, pScrollBar));
		// Update the chart image only, but no need to update the image map.
		m_ChartViewer.updateViewPort(true, false);
	}
	else
		// Scroll bar has stoped moving. Can update image map now.
		m_ChartViewer.updateViewPort(false, true);
}
//
// CChartViewer ViewPortChanged event
//
void C绘图程序Dlg::OnViewPortChanged()
{
	//
	// Set up the scroll bars to reflect the current position and size of the view port
	//
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = 0x1fffffff;

	m_HScrollBar.EnableWindow(m_ChartViewer.getViewPortWidth() < 1);
	if (m_ChartViewer.getViewPortWidth() < 1)
	{
		info.nPage = (int)ceil(m_ChartViewer.getViewPortWidth() * (info.nMax - info.nMin));
		info.nPos = (int)(0.5 + m_ChartViewer.getViewPortLeft() * (info.nMax - info.nMin))
			+ info.nMin;
		m_HScrollBar.SetScrollInfo(&info);
	}

	m_VScrollBar.EnableWindow(m_ChartViewer.getViewPortHeight() < 1);
	if (m_ChartViewer.getViewPortHeight() < 1)
	{
		info.nPage = (int)ceil(m_ChartViewer.getViewPortHeight() * (info.nMax - info.nMin));
		info.nPos = (int)(0.5 + m_ChartViewer.getViewPortTop() * (info.nMax - info.nMin))
			+ info.nMin;
		m_VScrollBar.SetScrollInfo(&info);
	}

	//
	// Update chart and image map if necessary
	//
	if (m_ChartViewer.needUpdateChart())
		drawChart(&m_ChartViewer);
	if (m_ChartViewer.needUpdateImageMap())
		updateImageMap(&m_ChartViewer);
}
//
// User clicks on the CChartViewer
//
void C绘图程序Dlg::OnBnClickedChartViewer() 
{
	ImageMapHandler *handler = m_ChartViewer.getImageMapHandler();
	if (0 != handler)
	{
		//
		// Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We 
		// consider the hot spot as clickable if its href ("path") parameter is not empty.
		//
		const char *path = handler->getValue("path");
		if ((0 != path) && (0 != *path))
		{
			// In this sample code, we just show all hot spot parameters.
			// @@@@@@@@
// 			CHotSpotDlg hs;
// 			hs.SetData(handler);
// 			hs.DoModal();
		}
	}
}

//
// Handle scroll bar events
//
double C绘图程序Dlg::moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//
	// Get current scroll bar position
	//
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	pScrollBar->GetScrollInfo(&info);

	//
	// Compute new position based on the type of scroll bar events
	//
	int newPos = info.nPos;
	switch (nSBCode)
	{
	case SB_LEFT:
		newPos = info.nMin;
		break;
	case SB_RIGHT:
		newPos = info.nMax;
		break;
	case SB_LINELEFT:
		newPos -= (info.nPage > 10) ? info.nPage / 10 : 1;
		break;
	case SB_LINERIGHT:
		newPos += (info.nPage > 10) ? info.nPage / 10 : 1;
		break;
	case SB_PAGELEFT:
		newPos -= info.nPage;
		break;
	case SB_PAGERIGHT:
		newPos += info.nPage;
		break;
	case SB_THUMBTRACK:
		newPos = info.nTrackPos;
		break;
	}
	if (newPos < info.nMin) newPos = info.nMin;
	if (newPos > info.nMax) newPos = info.nMax;

	// Update the scroll bar with the new position
	pScrollBar->SetScrollPos(newPos);

	// Returns the position of the scroll bar as a ratio of its total length
	return ((double)(newPos - info.nMin)) / (info.nMax - info.nMin);
}
//
// Draw the chart and display it in the given viewer
//
void C绘图程序Dlg::drawChart(CChartViewer *viewer)
{
	// @@@@@@
	LoadData();
}
//
// Update the image map
//
void C绘图程序Dlg::updateImageMap(CChartViewer *viewer)
{
	if (0 == viewer->getImageMapHandler())
	{
		// no existing image map - creates a new one
		viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
			"title='[{dataSetName}] {x|1}: Volt {value|9}'"));
	}
}