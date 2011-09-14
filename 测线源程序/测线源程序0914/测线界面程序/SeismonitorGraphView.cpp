// SensorGraphView.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "SeismonitorGraphView.h"
#include "SeismonitorGraphChildFrame.h"

// CSeismonitorGraphView

IMPLEMENT_DYNCREATE(CSeismonitorGraphView, CScrollView)

CSeismonitorGraphView::CSeismonitorGraphView()
{
	m_pSiteData = NULL;;	// 现场数据对象

	m_iGridX = 0;	// 单元格X方向尺寸
	m_iGridY = 0;	// 单元格Y方向尺寸

	m_iGridXPre = 0;	// 单元格X方向尺寸，上一次
	m_iGridYPre = 0;	// 单元格Y方向尺寸，上一次

	m_iPaperX = 0;	// 图形区X方向尺寸
	m_iPaperY = 0;	// 图形区Y方向尺寸

	m_fZoomRate = 1.0;	// 放大倍数

	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列

	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引
}

CSeismonitorGraphView::~CSeismonitorGraphView()
{
}


BEGIN_MESSAGE_MAP(CSeismonitorGraphView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_PROPERTIES, OnGraphViewSensorProperties)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_SETAUXILIARY, OnGraphViewSensorSetAuxiliary)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_MUTE, OnGraphViewSensorSetMute)
END_MESSAGE_MAP()


// CSeismonitorGraphView 绘图

void CSeismonitorGraphView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);
	pDoc->SetTitle (_T("Seismonitor Graph View"));
	
	// 绘图前设置绘图参数
	SetupPageSize();
}

void CSeismonitorGraphView::OnDraw(CDC* pDC)
{
	// 画网格图
//	m_pSiteData->m_oGraphSensorList.DrawGrid(pDC, m_iPaperX, m_iPaperY, m_iGridX, m_iGridY);
	// 画鼠标中间键选中区域
	DrawSelectArea(pDC);
	// 画测线标尺和测点标尺
	m_pSiteData->m_oGraphSensorList.DrawMarkLineNbAndPointNb(pDC, m_iGridX, m_iGridY);
	// 画图形单元
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	m_pSiteData->m_oGraphSensorList.DrawUnitAll(pDC, m_iGridX, m_iGridY, pParentFrame->m_uiTestType, pParentFrame->m_uiSensorNb, pParentFrame->m_oSensorNbMap, pParentFrame->m_fSeismonitorLimit);
}


// CSeismonitorGraphView 诊断
#ifdef _DEBUG
void CSeismonitorGraphView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CSeismonitorGraphView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CSeismonitorGraphView 打印

void CSeismonitorGraphView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}

void CSeismonitorGraphView::OnFilePrint()
{
	CScrollView::OnFilePrint();
}

BOOL CSeismonitorGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSeismonitorGraphView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSeismonitorGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

// CSeismonitorGraphView 消息处理程序

int CSeismonitorGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 加载现场数据
	OnLoadSiteData();
	// 生成测试值漂浮窗口
	m_wndStaticFloat.Create("", WS_CHILD, CRect(0, 0, 100, 20), this);

	return 0;
}

void CSeismonitorGraphView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetWindowOrg(-m_pSiteData->m_oGraphSensorList.m_iPaperHeadLeft, -m_pSiteData->m_oGraphSensorList.m_iPaperHeadTop);
}

void CSeismonitorGraphView::OnContextMenu(CWnd* pWnd, CPoint point)
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

	// 判断点是否在绘图区域内
	if(true == JudgeIfPointInPaper(&ptClient))
	{	
		// 得到点坐标对应的单元
		GetUnitUnderPoint(&ptClient);
		if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
		{
			CChannelSetupData* pChannelSetupData = NULL;
			pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);
			if(NULL != pChannelSetupData)
			{
				CMenu menu;
				menu.LoadMenu (IDR_POPUP_GRAPHVIEW_SENSOR);
				CMenu* pSumMenu = menu.GetSubMenu(0);
				// 判断：
				if(0 == pChannelSetupData->m_uiSN)
				{
					pSumMenu->DeleteMenu(IDR_POPUP_GRAPHVIEW_SENSOR_SETAUXILIARY, MF_BYCOMMAND);
				}
				if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd)))
				{
					CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
					if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
						return;
					((CBCGPMDIFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
					UpdateDialogControls(this, FALSE);
				}
			}
		}
	}
}

void CSeismonitorGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);
	// 判断点是否在绘图区域内
	if(true == JudgeIfPointInPaper(&point))
	{	
		// 得到点坐标对应的单元
		GetUnitUnderPoint(&point);
		if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
		{
			CChannelSetupData* pChannelSetupData = NULL;
			pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);
			if(NULL != pChannelSetupData)
			{
				// 响应显示属性值
				OnShowProperty(pChannelSetupData->m_uiIndex);
			}
		}
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CSeismonitorGraphView::OnLButtonDown(UINT nFlags, CPoint point)
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
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列

	CScrollView::OnLButtonDown(nFlags, point);
}

void CSeismonitorGraphView::OnLButtonUp(UINT nFlags, CPoint point)
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

void CSeismonitorGraphView::OnMButtonDown(UINT nFlags, CPoint point)
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

void CSeismonitorGraphView::OnMButtonUp(UINT nFlags, CPoint point)
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
			m_strAbsoluteSpread = "all";
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

void CSeismonitorGraphView::OnMouseMove(UINT nFlags, CPoint point)
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
		// 判断点是否在绘图区域内
		if(true == JudgeIfPointInPaper(&point))
		{	
			// 得到点坐标对应的单元
			GetUnitUnderPoint(&point);
			if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
			{
				CChannelSetupData* pChannelSetupData = NULL;
				pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);
				if(NULL != pChannelSetupData)
				{
					// 显示单元的测线号和测点号
					OnShowUnitPosition(pChannelSetupData->m_uiNbLine, pChannelSetupData->m_uiNbPoint);					
					// 在浮动窗口中显示测试值
					OnShowTestValueInFloatWindow(pChannelSetupData, pointClient);
				}
			}
			else
			{
				m_wndStaticFloat.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			m_wndStaticFloat.ShowWindow(SW_HIDE);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorProperties()
{
	CChannelSetupData* pChannelSetupData = NULL;
	pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);
	if(NULL != pChannelSetupData)
	{
		// 响应显示属性值
		OnShowProperty(pChannelSetupData->m_uiIndex);
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorSetAuxiliary()
{
	CChannelSetupData* pChannelSetupData = NULL;
	pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);

	if(NULL != pChannelSetupData)
	{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置辅助道
		pMainFrame->OnGraphViewInstrumentSetAuxiliary(3, pChannelSetupData->m_uiSN);
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorSetMute()
{
	CChannelSetupData* pChannelSetupData = NULL;
	pChannelSetupData = m_pSiteData->m_oGraphSensorList.GetChannelSetupData(m_iSelectUnitRowIndex, m_iSelectUnitColumnIndex);

	if(NULL != pChannelSetupData)
	{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应检波器图形视图弹出菜单Mute设置
		pMainFrame->OnGraphViewSensorSetMute(pChannelSetupData->m_uiNbLine, pChannelSetupData->m_uiNbPoint);
	}
}

// 现场数据对象初始化
void CSeismonitorGraphView::OnSiteDataInit()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pSiteData = &pMainFrame->m_oSiteData;

	m_iPaperX = m_pSiteData->m_oGraphSensorList.m_iPaperX;	// 图形区X方向尺寸
	m_iPaperY = m_pSiteData->m_oGraphSensorList.m_iPaperY;	// 图形区Y方向尺寸

	m_iGridX = m_pSiteData->m_oGraphSensorList.m_iGridX;	// 单元格X方向尺寸
	m_iGridY = m_pSiteData->m_oGraphSensorList.m_iGridY;	// 单元格Y方向尺寸

	m_iGridXPre = m_iGridX;	// 单元格X方向尺寸，上一次
	m_iGridYPre = m_iGridY;	// 单元格Y方向尺寸，上一次

	m_fZoomRate = 1.0;	// 放大倍数

	m_oPointScrollPos.SetPoint(0, 0);	// 滚动条位置
	m_oPointBase.SetPoint(0, 0);	// 鼠标键按下时鼠标点
	m_oPointLast.SetPoint(0, 0);	// 鼠标键释放时鼠标点
	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列
	m_oSelectAreaPoint.SetRectEmpty();	// 鼠标选中区域位置，单位像素
	m_oSelectAreaIndex.SetRectEmpty();	// 鼠标中间键选中区域位置，单位单元格

	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引
}

/**
* 响应设置测线数量
* @param void
* @return void
*/
void CSeismonitorGraphView::OnSetLineCount()
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnSetLineCount(m_pSiteData->m_oGraphSensorList.m_uiRowCount);
}

// 加载现场数据
void CSeismonitorGraphView::OnLoadSiteData()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 响应设置测线数量
	OnSetLineCount();
}

// 重新加载现场数据
void CSeismonitorGraphView::OnReloadSiteDataCMD()
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

// 绘图前设置绘图参数
void CSeismonitorGraphView::SetupPageSize()
{
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = m_iPaperX + m_pSiteData->m_oGraphSensorList.m_iPaperHeadLeft * 2;
	sizeTotal.cy = m_iPaperY + m_pSiteData->m_oGraphSensorList.m_iPaperHeadTop * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// 放大
void CSeismonitorGraphView::OnZoomIn() 
{
	float fGridXPre = (float)m_iGridXPre;
	float fGridYPre = (float)m_iGridYPre;

	m_fZoomRate = 2.0;	// 放大倍数
	// 缩放图形
	OnZoomGraph();
	// 得到滚动条位置
	m_oPointScrollPos = GetScrollPosition();
	float fZoomRateX = m_iGridX / fGridXPre;
	float fZoomRateY = m_iGridY / fGridYPre;
	m_oPointScrollPos.x = (int)((float)(m_oPointScrollPos.x - m_pSiteData->m_oGraphSensorList.m_iPaperHeadLeft) * fZoomRateX) + m_pSiteData->m_oGraphSensorList.m_iPaperHeadLeft;
	m_oPointScrollPos.y = (int)((float)(m_oPointScrollPos.y - m_pSiteData->m_oGraphSensorList.m_iPaperHeadTop) * fZoomRateY) + m_pSiteData->m_oGraphSensorList.m_iPaperHeadTop;
	// 绘图前设置绘图参数
	SetupPageSize();	
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);	
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列
	Invalidate(TRUE);
}

/**
* 左键拖放出选择框，框内区域局部放大
* @param void
* @return void
*/
void CSeismonitorGraphView::OnZoomInDrag() 
{
	float fWidthDrag= (float)m_oSelectAreaPoint.Width();

	float fGridXPre = (float)m_iGridXPre;
	float fGridYPre = (float)m_iGridYPre;

	CRect oRect;
	GetClientRect(&oRect);
	float fWidthClient = (float)oRect.Width();
	m_fZoomRate = fWidthClient / fWidthDrag;
	// 缩放图形
	OnZoomGraph();

	// 得到滚动条位置
	m_oPointScrollPos.x = m_oSelectAreaPoint.left;
	m_oPointScrollPos.y = m_oSelectAreaPoint.top;

	float fZoomRateX = m_iGridX / fGridXPre;
	float fZoomRateY = m_iGridY / fGridYPre;
	m_oPointScrollPos.x = (int)((float)m_oPointScrollPos.x * fZoomRateX) + m_pSiteData->m_oGraphSensorList.m_iPaperHeadLeft;
	m_oPointScrollPos.y = (int)((float)m_oPointScrollPos.y * fZoomRateY) + m_pSiteData->m_oGraphSensorList.m_iPaperHeadTop;
	if((m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) <= oRect.Height())
	{
		m_oPointScrollPos.y = 0;
	}
	else
	{
		if(m_oPointScrollPos.y > ((m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) - oRect.Height()))
		{
			m_oPointScrollPos.y = (m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) - oRect.Height();
		}
	}
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
void CSeismonitorGraphView::OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY) 
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
		m_fZoomRate = fZoomRate;
		// 缩放图形
		OnZoomGraph();

		m_oPointScrollPos.x = (int)((float)m_iPaperX * fRateX) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft;
		m_oPointScrollPos.y = (int)((float)m_iPaperX * fRateY) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop;
		if((m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) <= oRect.Height())
		{
			m_oPointScrollPos.y = 0;
		}
		else
		{
			if(m_oPointScrollPos.y > ((m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) - oRect.Height()))
			{
				m_oPointScrollPos.y = (m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2) - oRect.Height();
			}
		}
		// 绘图前设置绘图参数
		SetupPageSize();
		// 设置滚动条新位置
		ScrollToPosition(m_oPointScrollPos);
	}
	Invalidate(TRUE);	
}

// 缩小
void CSeismonitorGraphView::OnZoomOut()
{
	m_fZoomRate = 0.5;	// 放大倍数
	// 缩放图形
	OnZoomGraph();
	// 绘图前设置绘图参数
	SetupPageSize();	
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列
	Invalidate(TRUE);
}

/**
* 缩放图形
* @param void
* @return void
*/
void CSeismonitorGraphView::OnZoomGraph()
{
	m_iGridX = (int)((float)m_iGridX * m_fZoomRate);	// 单元格X方向尺寸
	m_iGridY = (int)((float)m_iGridY * m_fZoomRate);	// 单元格Y方向尺寸
	if(m_iGridX > m_pSiteData->m_oGraphSensorList.m_iGridXMax)
	{
		m_iGridX = m_pSiteData->m_oGraphSensorList.m_iGridXMax;
	}
	if(m_iGridY > m_pSiteData->m_oGraphSensorList.m_iGridYMax)
	{
		m_iGridY = m_pSiteData->m_oGraphSensorList.m_iGridYMax;
	}
	if(m_iGridX < m_pSiteData->m_oGraphSensorList.m_iGridXMin)
	{
		m_iGridX = m_pSiteData->m_oGraphSensorList.m_iGridXMin;
	}
	if(m_iGridY < m_pSiteData->m_oGraphSensorList.m_iGridYMin)
	{
		m_iGridY = m_pSiteData->m_oGraphSensorList.m_iGridYMin;
	}
	m_iPaperX = (m_pSiteData->m_oGraphSensorList.m_uiColumnCount - 1) * m_iGridX;	// X方向尺寸
	m_iPaperY = (m_pSiteData->m_oGraphSensorList.m_uiRowCount - 1) * m_iGridY;	// Y方向尺寸

	m_iGridXPre = m_iGridX;	// 单元格X方向尺寸，上一次
	m_iGridYPre = m_iGridY;	// 单元格Y方向尺寸，上一次
}

/**
* 得到点坐标对应的单元
* @param CPoint* pPoint 当前鼠标点
* @return void
*/
void CSeismonitorGraphView::GetUnitUnderPoint(CPoint* pPoint)
{
	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引

	int iColumnIndex;	// 鼠标点靠近的列索引
	int iRowIndex;	// 鼠标点靠近的行索引
	int iAreaColumnndex;	// 鼠标点所在区域的列索引
	int iAreaRowIndex;	// 鼠标点所在区域的行索引

	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphSensorList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);

	iColumnIndex = (int)((float)pPoint->x / (float)m_iGridX + 0.5);	// 鼠标点靠近的列索引
	iRowIndex = (int)((float)pPoint->y / (float)m_iGridY + 0.5);	// 鼠标点靠近的列索引

	iAreaColumnndex = (int)((float)pPoint->x / (float)m_iGridX);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)pPoint->y / (float)m_iGridY);	// 鼠标点所在区域的行索引

	if(iColumnIndex == iAreaColumnndex)	// 区域左
	{
		if(iRowIndex == iAreaRowIndex)	// 区域左上
		{
			// 在单元右下角范围内
			if((pPoint->x <= iColumnIndex * m_iGridX + iUnitWidth / 2) && (pPoint->y <= iRowIndex * m_iGridY + iUnitHight / 2))
			{
				m_iSelectUnitColumnIndex = iColumnIndex;	// 选中单元列索引
				m_iSelectUnitRowIndex = iRowIndex;	// 选中单元行索引
			}
		}
		else if(iRowIndex == iAreaRowIndex + 1)	// 区域左下
		{
			// 在单元右上角范围内
			if((pPoint->x <= iColumnIndex * m_iGridX + iUnitWidth / 2) && (pPoint->y >= iRowIndex * m_iGridY - iUnitHight / 2))
			{
				m_iSelectUnitColumnIndex = iColumnIndex;	// 选中单元列索引
				m_iSelectUnitRowIndex = iRowIndex;	// 选中单元行索引
			}
		}
	}
	else if(iColumnIndex == iAreaColumnndex + 1)	// 区域右
	{
		if(iRowIndex == iAreaRowIndex)	// 区域右上
		{
			// 在单元左下角范围内
			if((pPoint->x >= iColumnIndex * m_iGridX - iUnitWidth / 2) && (pPoint->y <= iRowIndex * m_iGridY + iUnitHight / 2))
			{
				m_iSelectUnitColumnIndex = iColumnIndex;	// 选中单元列索引
				m_iSelectUnitRowIndex = iRowIndex;	// 选中单元行索引
			}
		}
		else if(iRowIndex == iAreaRowIndex + 1)	// 区域右下
		{
			// 在单元左上角范围内
			if((pPoint->x >= iColumnIndex * m_iGridX - iUnitWidth / 2) && (pPoint->y >= iRowIndex * m_iGridY - iUnitHight / 2))
			{
				m_iSelectUnitColumnIndex = iColumnIndex;	// 选中单元列索引
				m_iSelectUnitRowIndex = iRowIndex;	// 选中单元行索引
			}
		}
	}
}

/**
* 得到选中的区域
* @param void
* @return void
*/
bool CSeismonitorGraphView::GetSelectAreaPoint()
{
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
	if(m_oSelectAreaPoint.right > m_iPaperX)
	{
		m_oSelectAreaPoint.right = m_iPaperX;
	}
	if(m_oSelectAreaPoint.bottom > m_iPaperY)
	{
		m_oSelectAreaPoint.bottom = m_iPaperY;
	}
	return true;
}

/**
* 得到选中区域的行坐标和列坐标区域
* @param void
* @return void
*/
void CSeismonitorGraphView::GetSelectAreaIndex()
{
	int iAreaColumnIndex;	// 鼠标点所在区域的列索引
	int iAreaRowIndex;	// 鼠标点所在区域的行索引

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.left / (float)m_iGridX);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.top / (float)m_iGridY);	// 鼠标点所在区域的行索引
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

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.right / (float)m_iGridX);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.bottom / (float)m_iGridY);	// 鼠标点所在区域的行索引
	m_oSelectAreaIndex.right = iAreaColumnIndex;	// 鼠标中间键选中区域位置
	m_oSelectAreaIndex.bottom = iAreaRowIndex;	// 鼠标中间键选中区域位置
}

/**
* 把选中区域转换为绝对排列字符串
* @param void
* @return void
*/
void CSeismonitorGraphView::ConvertSelectAreaToAbsoluteSpreadString()
{
	// 把选中区域转换为绝对排列字符串
	m_strAbsoluteSpread = m_pSiteData->m_oGraphSensorList.ConvertSelectAreaToAbsoluteSpreadString(&m_oSelectAreaIndex);
}

/**
* 判断点是否在绘图区域内
* @param CPoint* pPoint 鼠标点
* @return void
*/
bool CSeismonitorGraphView::JudgeIfPointInPaper(CPoint* pPoint)
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphSensorList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);
	if((pPoint->x >= (-iUnitWidth / 2)) && (pPoint->x <= (m_iPaperX + (iUnitWidth / 2))) && (pPoint->y >= (-iUnitHight / 2)) && (pPoint->y <= (m_iPaperY + (iUnitHight / 2))))
	{
		return true;
	}
	return false;
}

/**
* 显示单元的测线号和测点号
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return void
*/
void CSeismonitorGraphView::OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowUnitPosition(uiNbLine, uiNbPoint);
}

/**
* 在浮动窗口中显示测试值
* @param CChannelSetupData* pChannelSetupData 检波器
* @param CPoint pointClient 鼠标点
* @return void
*/
void CSeismonitorGraphView::OnShowTestValueInFloatWindow(CChannelSetupData* pChannelSetupData, CPoint pointClient)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	CString strTestUnit = "";
	CString strTestValue = "";
	CString strValue = "";

	strTestUnit = m_pSiteData->GetSensorTestTypeUnitStringByID(pParentFrame->m_uiTestType);
	strTestValue = pChannelSetupData->GetTestResultStringByTestType(pParentFrame->m_uiTestType, strTestUnit);	
	strValue.Format("%d  %s", pChannelSetupData->m_uiSN, strTestValue);
	if("" != strValue)
	{
		m_wndStaticFloat.SetWindowText(strValue);
		m_wndStaticFloat.MoveWindow(pointClient.x + 20, pointClient.y, 200, 20);
		m_wndStaticFloat.ShowWindow(SW_SHOW);
	}
}

/**
* 响应显示属性值
* @param unsigned int uiIndex 检波器索引号
* @return void
*/
void CSeismonitorGraphView::OnShowProperty(unsigned int uiIndex)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowProperty(uiIndex);
}

// 画鼠标中间键选中区域
void CSeismonitorGraphView::DrawSelectArea(CDC* pDC)
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

// 为新的测试数据刷新显示
void CSeismonitorGraphView::OnRefreshForNewTestValue()
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphSensorList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);

	unsigned int uiIP = 0;
	CChannelSetupData* pChannelSetupData = NULL;
	CGraphSensorData* m_pGraphSensor = NULL;
	CRect oRect;
	int iCenterX;
	int iCenterY;
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);

	while(m_pSiteData->m_olsIPSeismonitor.GetCount() > 0)
	{	
		uiIP = m_pSiteData->m_olsIPSeismonitor.RemoveHead();
		if(TRUE == m_pSiteData->m_oChannelList.GetChannelFromMap(uiIP, pChannelSetupData))
		{
			m_pGraphSensor = &m_pSiteData->m_oGraphSensorList.m_pArrayGraphSensor[pChannelSetupData->m_uiIndex];

			iCenterX = m_pGraphSensor->m_iPosX * m_iGridX;
			iCenterY = m_pGraphSensor->m_iPosY * m_iGridY;
			
			oRect.left = iCenterX - iUnitWidth / 2;
			oRect.top = iCenterY - iUnitHight / 2;
			oRect.right = oRect.left + iUnitWidth;
			oRect.bottom = oRect.top + iUnitHight;

			oDC.LPtoDP(&oRect);
			InvalidateRect(&oRect, TRUE);
		}		
	}
}