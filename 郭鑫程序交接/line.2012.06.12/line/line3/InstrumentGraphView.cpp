// InstrumentGraphView.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "InstrumentGraphView.h"
#include "InstrumentGraphChildFrame.h"
#include "MainFrm.h"
#include "DlgSetInstrument.h"
#include "SensorGraphChildFrame.h"
// CInstrumentGraphView

IMPLEMENT_DYNCREATE(CInstrumentGraphView, CScrollView)

CInstrumentGraphView::CInstrumentGraphView()
{

	//m_iGridX = 0;	// 单元格X方向尺寸
	//m_iGridY = 0;	// 单元格Y方向尺寸

	//m_iGridXPre = 0;	// 单元格X方向尺寸，上一次
	//m_iGridYPre = 0;	// 单元格Y方向尺寸，上一次

	//m_iPaperX = 0;	// 图形区X方向尺寸
	//m_iPaperY = 0;	// 图形区Y方向尺寸

	//m_fZoomRate = 1.0;	// 放大倍数

	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置

	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引

	m_iSelectLinkColumnIndex = -1;	// 选中连接线列索引
	m_iSelectLinkRowIndex = -1;	// 选中连接线行索引
	m_iSelectLinkDirection = -1;	// 选中连接线方向 2：下；4：右
}

CInstrumentGraphView::~CInstrumentGraphView()
{
}


BEGIN_MESSAGE_MAP(CInstrumentGraphView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()	
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_SETMARKER, OnGraphViewInstrumentSetMarker)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_PROPERTIES, OnGraphViewInstrumentProperties)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_SETAUXILIARY, OnGraphViewInstrumentSetAuxiliary)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_SETLOWBOXOFDETOUR, OnGraphViewInstrumentSetLowBoxDetour)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_SETHIGHBOXOFDETOUR, OnGraphViewInstrumentSetHighBoxDetour)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_FORMLINE, OnGraphViewInstrumentFormLine)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_INSTRUMENT_SETINSTRUMENT, OnPopupGraphviewInstrumentSetinstrument)
END_MESSAGE_MAP()

// CInstrumentGraphView 绘图

void CInstrumentGraphView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);
	pDoc->SetTitle (_T("Instrument Graph View"));

	// 绘图前设置绘图参数
	SetupPageSize();
}

void CInstrumentGraphView::OnDraw(CDC* pDC)
{
	int grid_x = 0,
		grid_y = 0;

	((CMainFrame*)AfxGetMainWnd())->get_grid_size(grid_x, grid_y);
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	// 画网格图
	// 画鼠标中间键选中区域
	DrawSelectArea(pDC);
	// 画迂回道标记
	DrawDetourMarker(pDC);
	if(true == pParentFrame->m_bShowLineNbPointNb)
	{
		// 画测线标尺和测点标尺
		((CMainFrame*)AfxGetMainWnd())->draw_mark_line_and_point(pDC, grid_x, grid_y);
		((CMainFrame*)AfxGetMainWnd())->set_scroll_size(this);
	}
	if(true == pParentFrame->m_bShowLinkLine)
	{
		// 画图形单元连接线
		((CMainFrame*)AfxGetMainWnd())->draw_devices_link_line(pDC, grid_x, grid_y);
	}

	((CMainFrame*)AfxGetMainWnd())->draw_devices(pDC,  grid_x, grid_y, pParentFrame->m_uiTestType,
		pParentFrame->m_bShowInstrumentCross, pParentFrame->m_bShowInstrumentPower, pParentFrame->m_bShowInstrumentCollect);
}

// CInstrumentGraphView 诊断

#ifdef _DEBUG
void CInstrumentGraphView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CInstrumentGraphView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CInstrumentGraphView 打印

void CInstrumentGraphView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}

void CInstrumentGraphView::OnFilePrint()
{
	CScrollView::OnFilePrint();
}

BOOL CInstrumentGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CInstrumentGraphView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CInstrumentGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

// CInstrumentGraphView 消息处理程序

int CInstrumentGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 加载现场数据
	OnLoadSiteData();
	// 生成测试值漂浮窗口
	m_wndStaticFloat.Create("", WS_CHILD, CRect(0, 0, 100, 20), this);

	return 0;
}

void CInstrumentGraphView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetWindowOrg(-CMainFrame::paper_head_left_, -CMainFrame::paper_head_top_);
}

void CInstrumentGraphView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect oRect;
	GetWindowRect(oRect);
	CPoint ptClient;
	ptClient = point;
	ptClient.x = ptClient.x - oRect.left;
	ptClient.y = ptClient.y - oRect.top;	
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&ptClient);

	((CMainFrame*)AfxGetMainWnd())->show_context_menu(this, point, ptClient);
}

void CInstrumentGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);

	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	((CMainFrame*)AfxGetMainWnd())->show_device_info(&pParentFrame->m_oLeftViewBarInstrumentBottom, point, CMainFrame::INSTRUMENT_GRAPH_VIEW_TYPE);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CInstrumentGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);

	if(true == m_bDrawSelectArea)
	{
		m_oSelectAreaPoint.left = m_oSelectAreaPoint.left - 2;
		m_oSelectAreaPoint.top = m_oSelectAreaPoint.top - 2;
		m_oSelectAreaPoint.right = m_oSelectAreaPoint.right + 2;
		m_oSelectAreaPoint.bottom = m_oSelectAreaPoint.bottom + 2;
		oDC.LPtoDP(&m_oSelectAreaPoint);
		InvalidateRect(&m_oSelectAreaPoint, TRUE);
	}
	
	oDC.DPtoLP(&point);	
	m_oPointBase = point;	// 鼠标键按下时鼠标点
	m_oPointLast = m_oPointBase;	// 鼠标键释放时鼠标点
	m_bMouseDownLeft = true;	// 鼠标左键是否按下
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置

	CScrollView::OnLButtonDown(nFlags, point);
}

void CInstrumentGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(true == m_bMouseDownLeft)	// 鼠标左键是否按下
	{
		CClientDC oDC(this);
		OnPrepareDC(&oDC, NULL);
		oDC.DPtoLP(&point);
		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);
		m_oPointLast = point;	// 鼠标键释放时鼠标点
		m_bMouseDownLeft = false;	// 鼠标左键是否按下

		// 得到选中的区域
		if(true == GetSelectAreaPoint())
		{
			// 局部放大
			OnZoomInDrag();
			Invalidate(TRUE);
		}
		else
		{
			oRect.left = oRect.left - 2;
			oRect.top = oRect.top - 2;
			oRect.right = oRect.right + 2;
			oRect.bottom = oRect.bottom + 2;
			oDC.LPtoDP(&oRect);
			InvalidateRect(&oRect, TRUE);
		}
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CInstrumentGraphView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);

	if(true == m_bDrawSelectArea)	// 是否画鼠标中间键选中区域位置
	{
		m_oSelectAreaPoint.left = m_oSelectAreaPoint.left - 2;
		m_oSelectAreaPoint.top = m_oSelectAreaPoint.top - 2;
		m_oSelectAreaPoint.right = m_oSelectAreaPoint.right + 2;
		m_oSelectAreaPoint.bottom = m_oSelectAreaPoint.bottom + 2;
		oDC.LPtoDP(&m_oSelectAreaPoint);
		InvalidateRect(&m_oSelectAreaPoint, TRUE);
		m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	}

	oDC.DPtoLP(&point);	
	m_oPointBase = point;	// 鼠标键按下时鼠标点
	m_oPointLast = m_oPointBase;	// 鼠标键释放时鼠标点
	m_bMouseDownMiddle = true;	// 鼠标中间键是否按下

	CScrollView::OnMButtonDown(nFlags, point);
}

void CInstrumentGraphView::OnMButtonUp(UINT nFlags, CPoint point)
{
	if(true == m_bMouseDownMiddle)	// 鼠标中间键是否按下
	{
		CClientDC oDC(this);
		OnPrepareDC(&oDC, NULL);
		oDC.DPtoLP(&point);

		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);
		m_oPointLast = point;	// 鼠标键释放时鼠标点
		m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

		// 得到选中的区域
		if(true == GetSelectAreaPoint())
		{
			// 得到选中区域的行坐标和列坐标区域
			GetSelectAreaIndex();
			// 把选中区域转换为绝对排列字符串
			ConvertSelectAreaToAbsoluteSpreadString();
			m_bDrawSelectArea = true;	// 是否画鼠标中间键选中区域位置
			Invalidate(TRUE);
		}
		else
		{
			oRect.left = oRect.left - 2;
			oRect.top = oRect.top - 2;
			oRect.right = oRect.right + 2;
			oRect.bottom = oRect.bottom + 2;
			oDC.LPtoDP(&oRect);
			InvalidateRect(&oRect, TRUE);
		}
	}

	CScrollView::OnMButtonUp(nFlags, point);
}

void CInstrumentGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pointClient = point;
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);
	if((true == m_bMouseDownLeft) || (true == m_bMouseDownMiddle))	// 鼠标左键是否按下或鼠标中间键是否按下
	{
		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);

		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = point.x;
		oRect.bottom = point.y;
		oRect.NormalizeRect();
		oDC.DrawFocusRect(&oRect);

		m_oPointLast = point;	// 鼠标键释放时鼠标点
	}
	else
	{
		matrix_data::data_base_helper::device_info di;
		// 判断点是否在绘图区域内
		if(((CMainFrame*)AfxGetMainWnd())->get_device_info_from_point(point, di)){
			// 显示单元的测线号和测点号
			OnShowUnitPosition(di.line_index_, di.m_uiPointNb);
			// 在浮动窗口中显示测试值
			OnShowTestValueInFloatWindow(di, pointClient);
		}
		else{
			m_wndStaticFloat.ShowWindow(SW_HIDE);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

/**
* 响应仪器图形视图弹出菜单设置标记点
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetMarker()
{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置标记点
		pMainFrame->OnGraphViewInstrumentSetMarker(3, 1);
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentProperties()
{
	matrix_data::data_base_helper::device_info* di_ptr = ((CMainFrame*)AfxGetMainWnd())->get_device_info_ptr();	
	CInstrumentGraphChildFrame* frame = (CInstrumentGraphChildFrame*)GetParentFrame();
	((CMainFrame*)AfxGetMainWnd())->show_property(&frame->m_oLeftViewBarInstrumentBottom, *di_ptr, CMainFrame::INSTRUMENT_GRAPH_VIEW_TYPE);
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetAuxiliary()
{
	matrix_data::data_base_helper::device_info* di_ptr = NULL;	
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	// 响应仪器图形视图弹出菜单设置辅助道
	di_ptr = pMainFrame->get_device_info_ptr();
	pMainFrame->OnGraphViewInstrumentSetAuxiliary(di_ptr->sn_ & 0x3, di_ptr->sn_);
}

/**
* 响应仪器图形视图弹出菜单设置迂回道
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetLowBoxDetour()
{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置迂回道
		pMainFrame->OnGraphViewInstrumentSetDetour(3, 1, 3, 10);
}

/**
* // 响应仪器图形视图弹出菜单设置迂回道
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetHighBoxDetour()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	// 响应仪器图形视图弹出菜单设置迂回道
	pMainFrame->OnGraphViewInstrumentSetDetour(3, 1, 3, 10);
}

/**
* 响应仪器图形视图弹出菜单FormLine设置
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentFormLine()
{
	matrix_data::data_base_helper::device_info* di_ptr = NULL;	
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	// 响应仪器图形视图弹出菜单FormLine设置
	di_ptr = pMainFrame->get_device_info_ptr();
	pMainFrame->OnGraphViewInstrumentFormLine(di_ptr->sn_ & 0x3, di_ptr->sn_);
}

// 现场数据对象初始化
void CInstrumentGraphView::OnSiteDataInit()
{
//	m_pSiteData = &((CMainFrame*)AfxGetMainWnd())->m_oSiteData;
	//m_iPaperX = m_pSiteData->m_oGraphInstrumentList.m_iPaperX;	// 图形区X方向尺寸
	//m_iPaperY = m_pSiteData->m_oGraphInstrumentList.m_iPaperY;	// 图形区Y方向尺寸

	//m_iGridX = m_pSiteData->m_oGraphInstrumentList.m_iGridX;	// 单元格X方向尺寸
	//m_iGridY = m_pSiteData->m_oGraphInstrumentList.m_iGridY;	// 单元格Y方向尺寸

	//m_iGridXPre = m_iGridX;	// 单元格X方向尺寸，上一次
	//m_iGridYPre = m_iGridY;	// 单元格Y方向尺寸，上一次

	//m_fZoomRate = 1.0;	// 放大倍数

	m_oPointScrollPos.SetPoint(0, 0);	// 滚动条位置
	m_oPointBase.SetPoint(0, 0);	// 鼠标键按下时鼠标点
	m_oPointLast.SetPoint(0, 0);	// 鼠标键释放时鼠标点
	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_oSelectAreaPoint.SetRectEmpty();	// 鼠标选中区域位置，单位像素
	m_oSelectAreaIndex.SetRectEmpty();	// 鼠标中间键选中区域位置，单位单元格

	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引

	m_iSelectLinkColumnIndex = -1;	// 选中连接线列索引
	m_iSelectLinkRowIndex = -1;	// 选中连接线行索引
	m_iSelectLinkDirection = -1;	// 选中连接线方向 2：下；4：右
}

/**
* 响应设置测线数量
* @param void
* @return void
*/
void CInstrumentGraphView::OnSetLineCount()
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	pParentFrame->OnSetLineCount(pParentFrame->data_base_helper_->get_line_number()*2 - 1);
}

// 加载现场数据
void CInstrumentGraphView::OnLoadSiteData()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 响应设置测线数量
	OnSetLineCount();
}

// 重新加载现场数据
void CInstrumentGraphView::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 响应设置测线数量
	OnSetLineCount();
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);
	Invalidate(TRUE);
}

/**
* 绘图前设置绘图参数
* @param void
* @return void
*/
void CInstrumentGraphView::SetupPageSize()
{
	((CMainFrame*)AfxGetMainWnd())->set_scroll_size(this);
}

/**
* 放大1倍
* @param void
* @return void
*/
void CInstrumentGraphView::OnZoomIn() 
{
	((CMainFrame*)AfxGetMainWnd())->zoom_in(this, m_oPointScrollPos);
	Invalidate(TRUE);
}

/**
* 左键拖放出选择框，框内区域局部放大
* @param void
* @return void
*/
void CInstrumentGraphView::OnZoomInDrag() 
{
	CRect oRect;
	GetClientRect(&oRect);
	((CMainFrame*)AfxGetMainWnd())->zoom_in_drag(oRect, m_oSelectAreaPoint, m_oPointScrollPos);
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);
	Invalidate(TRUE);
}

/**
* 导航局部放大
* @param float fZoomRate 放大倍数
* @param float fRateX 基准点X
* @param float fRateY 基准点Y
* @return void
*/
void CInstrumentGraphView::OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY) 
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);

	// 判断：放大系数大于1.0
	if((fZoomRate > 1.01) || (fRateY > 0.01))
	{
		CRect oRect;
		GetClientRect(&oRect);
		((CMainFrame*)AfxGetMainWnd())->zoom_in_navigator(oRect, m_oPointScrollPos, fZoomRate,fRateX, fRateY);
		// 绘图前设置绘图参数
		SetupPageSize();
		// 设置滚动条新位置
		ScrollToPosition(m_oPointScrollPos);
	}
	Invalidate(TRUE);	
}

/**
* 缩小1倍
* @param void
* @return void
*/
void CInstrumentGraphView::OnZoomOut()
{
	((CMainFrame*)AfxGetMainWnd())->zoom_out(this);
	Invalidate(TRUE);
}

bool CInstrumentGraphView::GetSelectAreaPoint()
{
	SIZE paper_size = ((CMainFrame*)AfxGetMainWnd())->get_paper_size();

	m_oSelectAreaPoint.left = m_oPointBase.x;
	m_oSelectAreaPoint.top = m_oPointBase.y;
	m_oSelectAreaPoint.right = m_oPointLast.x;
	m_oSelectAreaPoint.bottom = m_oPointLast.y;
	m_oSelectAreaPoint.NormalizeRect();

	float fWidthDrag= (float)m_oSelectAreaPoint.Width();

	if(fWidthDrag < 50.0)
	{
		return false;
	}

	if(m_oSelectAreaPoint.left < 0)
	{
		m_oSelectAreaPoint.left = 0;
	}
	if(m_oSelectAreaPoint.top < 0)
	{
		m_oSelectAreaPoint.top = 0;
	}
	if(m_oSelectAreaPoint.right > paper_size.cx)
	{
		m_oSelectAreaPoint.right = paper_size.cx;
	}
	if(m_oSelectAreaPoint.bottom > paper_size.cy)
	{
		m_oSelectAreaPoint.bottom = paper_size.cy;
	}
	return true;
}

/**
* 得到选中区域的行坐标和列坐标区域
* @param void
* @return void
*/
void CInstrumentGraphView::GetSelectAreaIndex()
{
	SIZE grid_size = ((CMainFrame*)AfxGetMainWnd())->get_grid_size();
	int iAreaColumnIndex;	// 鼠标点所在区域的列索引
	int iAreaRowIndex;	// 鼠标点所在区域的行索引

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.left / (float)grid_size.cx);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.top / (float)grid_size.cy);	// 鼠标点所在区域的行索引
	m_oSelectAreaIndex.left = iAreaColumnIndex;	// 鼠标中间键选中区域位置
	m_oSelectAreaIndex.top = iAreaRowIndex;	// 鼠标中间键选中区域位置
	if(m_oSelectAreaPoint.left > 0)
	{
		m_oSelectAreaIndex.left = iAreaColumnIndex + 1;	// 鼠标中间键选中区域位置
	}	
	if(m_oSelectAreaPoint.top > 0)
	{
		m_oSelectAreaIndex.top = iAreaRowIndex + 1;	// 鼠标中间键选中区域位置
	}

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.right / (float)grid_size.cx);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.bottom / (float)grid_size.cy);	// 鼠标点所在区域的行索引
	m_oSelectAreaIndex.right = iAreaColumnIndex;	// 鼠标中间键选中区域位置
	m_oSelectAreaIndex.bottom = iAreaRowIndex;	// 鼠标中间键选中区域位置
}

/**
* 把选中区域转换为绝对排列字符串
* @param void
* @return void
*/
void CInstrumentGraphView::ConvertSelectAreaToAbsoluteSpreadString()
{
}

/**
* 显示单元的测线号和测点号
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return void
*/
void CInstrumentGraphView::OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowUnitPosition(uiNbLine, uiNbPoint);
}

/**
* 在浮动窗口中显示测试值
* @param CInstrument* pInstrument 仪器
* @param CPoint pointClient 鼠标点
* @return void
*/
void CInstrumentGraphView::OnShowTestValueInFloatWindow(matrix_data::data_base_helper::const_device_info& di, CPoint pointClient)
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	CString strInstrumentType = "";
	CString strValue = "";

	strInstrumentType = matrix_data::data_base_helper::get_device_type_string(di.sn_);
	strValue.Format("%s%d  %s", strInstrumentType, di.sn_, matrix_data::data_base_helper::get_device_test_result_by_test_type(di, pParentFrame->m_uiTestType));

	m_wndStaticFloat.SetWindowText(strValue);
	m_wndStaticFloat.MoveWindow(pointClient.x + 20, pointClient.y, 250, 20);
	m_wndStaticFloat.ShowWindow(SW_SHOW);
}

/**
* 响应显示属性值
* @param unsigned int uiIndex 仪器索引号
* @return void
*/
void CInstrumentGraphView::OnShowProperty(unsigned int uiIndex)
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowProperty(uiIndex);
}

/**
* 响应显示迂回道图形视图
* @param unsigned int uiIndex 仪器索引号
* @return void
*/
void CInstrumentGraphView::OnShowDetourGraphView(unsigned int uiIndex)
{
	// 得到迂回道中仪器数量
		CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
		pParentFrame->OnShowDetourGraphView(1);
}

// 画鼠标中间键选中区域
void CInstrumentGraphView::DrawSelectArea(CDC* pDC)
{
	if(true == m_bDrawSelectArea)	// 是否画鼠标中间键选中区域位置
	{
		CPen oPen;
		CPen* pOldPen;
		CBrush oBrush;
		CBrush* pOldPenBrush;

		oPen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
		oBrush.CreateSolidBrush(RGB(255, 0, 0));
		pOldPen = pDC->SelectObject(&oPen);
		pOldPenBrush = pDC->SelectObject(&oBrush);

		pDC->FillRect(&m_oSelectAreaPoint, &oBrush);
		pDC->Rectangle(&m_oSelectAreaPoint);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldPenBrush);
	}
}

// 画迂回道标记
void CInstrumentGraphView::DrawDetourMarker(CDC* pDC)
{
}

// 清除迂回道标记
void CInstrumentGraphView::ClearDetourMarker()
{
}

// 为新的测试数据刷新显示
void CInstrumentGraphView::OnRefreshForNewTestValue(int iTestType)
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	if(((unsigned int)iTestType == pParentFrame->m_uiTestType) || (30 == pParentFrame->m_uiTestType))
	{
		RedrawWindow();
	}
}
// cxm 2011-09-19
// 点击GO按钮则清空选中设备的测试结果  by heling 2011-11-16
void CInstrumentGraphView::OnClearCheckInstrumentTestResult(unsigned int uiTestType)
{
}
void CInstrumentGraphView::OnPopupGraphviewInstrumentSetinstrument()
{
	matrix_data::data_base_helper::device_info* di_ptr = NULL;	
	di_ptr = ((CMainFrame*)AfxGetMainWnd())->get_device_info_ptr();

	// TODO: 在此添加命令处理程序代码
	CDlgSetInstrument dlg;
	dlg.m_iSelectUnitColumnIndex = di_ptr->m_uiPointNb;			// 从0开始计数
	dlg.m_iSelectUnitRowIndex = di_ptr->line_index_ - 1;						// 从0开始计数
	dlg.m_uiColumnCount =  ((CLineApp*)AfxGetApp())->get_section_number();
	dlg.m_uiRowCount=  ((CLineApp*)AfxGetApp())->get_line_number();
	dlg.m_uiInstrumentType = di_ptr->sn_ & 0x3;
	dlg.DoModal();
}
