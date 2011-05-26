// InstrumentGraphView.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "InstrumentGraphView.h"
#include "InstrumentGraphChildFrame.h"
#include "MainFrm.h"

// CInstrumentGraphView

IMPLEMENT_DYNCREATE(CInstrumentGraphView, CScrollView)

CInstrumentGraphView::CInstrumentGraphView()
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

	m_iSelectLinkColumnIndex = -1;	// 选中连接线列索引
	m_iSelectLinkRowIndex = -1;	// 选中连接线行索引
	m_iSelectLinkDirection = -1;	// 选中连接线方向 2：下；4：右

	m_pGraphInstrumentDataDetourLow = NULL;	// 迂回道低端仪器
	m_pGraphInstrumentDataDetourHigh = NULL;	// // 迂回道高端仪器
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
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	// 画网格图
//	m_pSiteData->m_oGraphInstrumentList.DrawGrid(pDC, m_iPaperX, m_iPaperY, m_iGridX, m_iGridY);
	// 画鼠标中间键选中区域
	DrawSelectArea(pDC);
	// 画迂回道标记
	DrawDetourMarker(pDC);
	if(true == pParentFrame->m_bShowLineNbPointNb)
	{
		// 画测线标尺和测点标尺
		m_pSiteData->m_oGraphInstrumentList.DrawMarkLineNbAndPointNb(pDC, m_iGridX, m_iGridY);
	}
	if(true == pParentFrame->m_bShowLinkLine)
	{
		// 画图形单元连接线
		m_pSiteData->m_oGraphInstrumentList.DrawUnitLinkLineAll(pDC, m_iGridX, m_iGridY);
	}
	// 画图形单元
	m_pSiteData->m_oGraphInstrumentList.DrawUnitAll(pDC, m_iGridX, m_iGridY, pParentFrame->m_uiTestType,
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
	pDC->SetWindowOrg(-m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft, -m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop);
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

	// 判断点是否在绘图区域内
	if(true == JudgeIfPointInPaper(&ptClient))
	{	
		// 得到点坐标对应的单元
		GetUnitUnderPoint(&ptClient);
		if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
		{
			CGraphInstrumentData* pGraphInstrumentData = NULL;
			pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
			if(NULL != pGraphInstrumentData)
			{
				CMenu menu;
				menu.LoadMenu (IDR_POPUP_GRAPHVIEW_INSTRUMENT);
				CMenu* pSumMenu = menu.GetSubMenu(0);
				// 判断：不是交叉站
				if(1 != pGraphInstrumentData->m_pInstrument->m_uiInstrumentType)	// 仪器类型 1-交叉站；2-电源站；3-采集站
				{
					pSumMenu->DeleteMenu(IDR_POPUP_GRAPHVIEW_INSTRUMENT_FORMLINE, MF_BYCOMMAND);
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

void CInstrumentGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);
	// 判断点是否在绘图区域内
	if(true == JudgeIfPointInPaper(&point))
	{	
		// 得到点坐标对应的单元
		GetUnitUnderPoint(&point);
		if((-1 != m_iSelectLinkColumnIndex) && (-1 != m_iSelectLinkRowIndex))
		{
			CGraphInstrumentData* pGraphInstrumentData = NULL;
			pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectLinkColumnIndex, m_iSelectLinkRowIndex);
			if(NULL != pGraphInstrumentData)
			{
				if(true == pGraphInstrumentData->m_pInstrument->m_bDetourMarkerLow)
				{
					// 响应显示迂回道图形视图
					OnShowDetourGraphView(pGraphInstrumentData->m_pInstrument->m_uiIndex);
				}
			}
		}
		if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
		{
			CGraphInstrumentData* pGraphInstrumentData = NULL;
			pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
			if(NULL != pGraphInstrumentData)
			{
				// 响应显示属性值
				OnShowProperty(pGraphInstrumentData->m_pInstrument->m_uiIndex);
			}
		}
	}

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
	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列

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
		// 判断点是否在绘图区域内
		if(true == JudgeIfPointInPaper(&point))
		{	
			// 得到点坐标对应的单元
			GetUnitUnderPoint(&point);
			if((-1 != m_iSelectUnitRowIndex) && (-1 != m_iSelectUnitColumnIndex))
			{
				CGraphInstrumentData* pGraphInstrumentData = NULL;
				pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
				if(NULL != pGraphInstrumentData)
				{
					// 显示单元的测线号和测点号
					OnShowUnitPosition(pGraphInstrumentData->m_pInstrument->m_uiLineNb, pGraphInstrumentData->m_pInstrument->m_uiPointNb);					
					// 在浮动窗口中显示测试值
					OnShowTestValueInFloatWindow(pGraphInstrumentData->m_pInstrument, pointClient);
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
* 响应仪器图形视图弹出菜单设置标记点
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetMarker()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置标记点
		pMainFrame->OnGraphViewInstrumentSetMarker(pGraphInstrumentData->m_pInstrument->m_uiInstrumentType, pGraphInstrumentData->m_pInstrument->m_uiSN);
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentProperties()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		// 响应显示属性值
		OnShowProperty(pGraphInstrumentData->m_pInstrument->m_uiIndex);
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetAuxiliary()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置辅助道
		pMainFrame->OnGraphViewInstrumentSetAuxiliary(pGraphInstrumentData->m_pInstrument->m_uiInstrumentType, pGraphInstrumentData->m_pInstrument->m_uiSN);
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetLowBoxDetour()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		if(NULL != m_pGraphInstrumentDataDetourLow)
		{
			// 清除迂回道标记
			ClearDetourMarker(m_pGraphInstrumentDataDetourLow);
		}
		m_pGraphInstrumentDataDetourLow = pGraphInstrumentData;	// 迂回道低端仪器
		// 清除迂回道标记
		ClearDetourMarker(m_pGraphInstrumentDataDetourLow);
		if((NULL != m_pGraphInstrumentDataDetourLow) && (NULL != m_pGraphInstrumentDataDetourHigh))
		{
			bool bData = m_pSiteData->m_oInstrumentList.JudgeInstrumentAAtInstrumentBLeft(m_pGraphInstrumentDataDetourLow->m_pInstrument,
				m_pGraphInstrumentDataDetourHigh->m_pInstrument);
			if(true == bData)
			{
				CMainFrame* pMainFrame = NULL;
				pMainFrame = (CMainFrame*)AfxGetMainWnd();
				// 响应仪器图形视图弹出菜单设置迂回道
				pMainFrame->OnGraphViewInstrumentSetDetour(m_pGraphInstrumentDataDetourLow->m_pInstrument->m_uiInstrumentType,
					m_pGraphInstrumentDataDetourLow->m_pInstrument->m_uiSN,
					m_pGraphInstrumentDataDetourHigh->m_pInstrument->m_uiInstrumentType,
					m_pGraphInstrumentDataDetourHigh->m_pInstrument->m_uiSN);
			}
		}
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentSetHighBoxDetour()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		if(NULL != m_pGraphInstrumentDataDetourHigh)
		{
			// 清除迂回道标记
			ClearDetourMarker(m_pGraphInstrumentDataDetourHigh);
		}
		m_pGraphInstrumentDataDetourHigh = pGraphInstrumentData;	// 迂回道高端仪器
		// 清除迂回道标记
		ClearDetourMarker(m_pGraphInstrumentDataDetourHigh);
		if((NULL != m_pGraphInstrumentDataDetourLow) && (NULL != m_pGraphInstrumentDataDetourHigh))
		{
			bool bData = m_pSiteData->m_oInstrumentList.JudgeInstrumentAAtInstrumentBLeft(m_pGraphInstrumentDataDetourLow->m_pInstrument,
				m_pGraphInstrumentDataDetourHigh->m_pInstrument);
			if(true == bData)
			{
				CMainFrame* pMainFrame = NULL;
				pMainFrame = (CMainFrame*)AfxGetMainWnd();
				// 响应仪器图形视图弹出菜单设置迂回道
				pMainFrame->OnGraphViewInstrumentSetDetour(m_pGraphInstrumentDataDetourLow->m_pInstrument->m_uiInstrumentType,
					m_pGraphInstrumentDataDetourLow->m_pInstrument->m_uiSN,
					m_pGraphInstrumentDataDetourHigh->m_pInstrument->m_uiInstrumentType,
					m_pGraphInstrumentDataDetourHigh->m_pInstrument->m_uiSN);
			}
		}
	}
}

/**
* 函数说明
* @param void
* @return void
*/
void CInstrumentGraphView::OnGraphViewInstrumentFormLine()
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	pGraphInstrumentData = m_pSiteData->m_oGraphInstrumentList.GetGraphInstrumentDataByPosIndex(m_iSelectUnitColumnIndex, m_iSelectUnitRowIndex);
	if(NULL != pGraphInstrumentData)
	{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单FormLine设置
		pMainFrame->OnGraphViewInstrumentFormLine(pGraphInstrumentData->m_pInstrument->m_uiInstrumentType, pGraphInstrumentData->m_pInstrument->m_uiSN);
	}
}

// 现场数据对象初始化
void CInstrumentGraphView::OnSiteDataInit()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pSiteData = &pMainFrame->m_oSiteData;

	m_iPaperX = m_pSiteData->m_oGraphInstrumentList.m_iPaperX;	// 图形区X方向尺寸
	m_iPaperY = m_pSiteData->m_oGraphInstrumentList.m_iPaperY;	// 图形区Y方向尺寸

	m_iGridX = m_pSiteData->m_oGraphInstrumentList.m_iGridX;	// 单元格X方向尺寸
	m_iGridY = m_pSiteData->m_oGraphInstrumentList.m_iGridY;	// 单元格Y方向尺寸

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
	pParentFrame->OnSetLineCount(m_pSiteData->m_oGraphInstrumentList.m_uiRowCount);
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
	m_pGraphInstrumentDataDetourLow = NULL;	// 迂回道低端仪器
	m_pGraphInstrumentDataDetourHigh = NULL;	// // 迂回道高端仪器
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
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = m_iPaperX + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft * 2;
	sizeTotal.cy = m_iPaperY + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/**
* 放大1倍
* @param void
* @return void
*/
void CInstrumentGraphView::OnZoomIn() 
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
	m_oPointScrollPos.x = (int)((float)(m_oPointScrollPos.x - m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft) * fZoomRateX) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft;
	m_oPointScrollPos.y = (int)((float)(m_oPointScrollPos.y - m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop) * fZoomRateY) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop;
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
void CInstrumentGraphView::OnZoomInDrag() 
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
	m_oPointScrollPos.x = (int)((float)m_oPointScrollPos.x * fZoomRateX) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadLeft;
	m_oPointScrollPos.y = (int)((float)m_oPointScrollPos.y * fZoomRateY) + m_pSiteData->m_oGraphInstrumentList.m_iPaperHeadTop;
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

/**
* 缩小1倍
* @param void
* @return void
*/
void CInstrumentGraphView::OnZoomOut()
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
void CInstrumentGraphView::OnZoomGraph()
{
	m_iGridX = (int)((float)m_iGridX * m_fZoomRate);	// 单元格X方向尺寸
	m_iGridY = (int)((float)m_iGridY * m_fZoomRate);	// 单元格Y方向尺寸
	if(m_iGridX > m_pSiteData->m_oGraphInstrumentList.m_iGridXMax)
	{
		m_iGridX = m_pSiteData->m_oGraphInstrumentList.m_iGridXMax;
	}
	if(m_iGridY > m_pSiteData->m_oGraphInstrumentList.m_iGridYMax)
	{
		m_iGridY = m_pSiteData->m_oGraphInstrumentList.m_iGridYMax;
	}
	if(m_iGridX < m_pSiteData->m_oGraphInstrumentList.m_iGridXMin)
	{
		m_iGridX = m_pSiteData->m_oGraphInstrumentList.m_iGridXMin;
	}
	if(m_iGridY < m_pSiteData->m_oGraphInstrumentList.m_iGridYMin)
	{
		m_iGridY = m_pSiteData->m_oGraphInstrumentList.m_iGridYMin;
	}
	if(m_pSiteData->m_oGraphInstrumentList.m_uiColumnCount > 0)
	{
		m_iPaperX = (m_pSiteData->m_oGraphInstrumentList.m_uiColumnCount - 1) * m_iGridX;	// X方向尺寸
	}
	if(m_pSiteData->m_oGraphInstrumentList.m_uiRowCount > 0)
	{	
		m_iPaperY = (m_pSiteData->m_oGraphInstrumentList.m_uiRowCount - 1) * m_iGridY;	// Y方向尺寸
	}

	m_iGridXPre = m_iGridX;	// 单元格X方向尺寸，上一次
	m_iGridYPre = m_iGridY;	// 单元格Y方向尺寸，上一次
}

/**
* 得到点坐标对应的单元
* @param CPoint* pPoint 当前鼠标点
* @return void
*/
void CInstrumentGraphView::GetUnitUnderPoint(CPoint* pPoint)
{
	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引

	m_iSelectLinkColumnIndex = -1;	// 选中连接线列索引
	m_iSelectLinkRowIndex = -1;	// 选中连接线行索引
	m_iSelectLinkDirection = -1;	// 选中连接线方向 2：下；4：右

	int iColumnIndex;	// 鼠标点靠近的列索引
	int iRowIndex;	// 鼠标点靠近的行索引
	int iAreaColumnndex;	// 鼠标点所在区域的列索引
	int iAreaRowIndex;	// 鼠标点所在区域的行索引

	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphInstrumentList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);

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
			else if(pPoint->x <= iColumnIndex * m_iGridX + iUnitWidth / 2)	// 连接线，下
			{
				m_iSelectLinkColumnIndex = iColumnIndex;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 2;	// 选中连接线方向 2：下；4：右
			}
			else if(pPoint->y <= iRowIndex * m_iGridY + iUnitHight / 2)	// 连接线，右
			{
				m_iSelectLinkColumnIndex = iColumnIndex;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 4;	// 选中连接线方向 2：下；4：右
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
			else if(pPoint->x <= iColumnIndex * m_iGridX + iUnitWidth / 2)	// 连接线，下
			{
				m_iSelectLinkColumnIndex = iColumnIndex;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex - 1;	// 选中连接线行索引
				m_iSelectLinkDirection = 2;	// 选中连接线方向 2：下；4：右
			}
			else if(pPoint->y >= iRowIndex * m_iGridY - iUnitHight / 2)	// 连接线，右
			{
				m_iSelectLinkColumnIndex = iColumnIndex;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 4;	// 选中连接线方向 2：下；4：右
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
			else if(pPoint->x >= iColumnIndex * m_iGridX - iUnitWidth / 2)	// 连接线，下
			{
				m_iSelectLinkColumnIndex = iColumnIndex - 1;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 2;	// 选中连接线方向 2：下；4：右
			}
			else if(pPoint->y <= iRowIndex * m_iGridY + iUnitHight / 2)	// 连接线，右
			{
				m_iSelectLinkColumnIndex = iColumnIndex - 1;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 4;	// 选中连接线方向 2：下；4：右
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
			else if(pPoint->x >= iColumnIndex * m_iGridX - iUnitWidth / 2)	// 连接线，下
			{
				m_iSelectLinkColumnIndex = iColumnIndex;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex - 1;	// 选中连接线行索引
				m_iSelectLinkDirection = 2;	// 选中连接线方向 2：下；4：右
			}
			else if(pPoint->y >= iRowIndex * m_iGridY - iUnitHight / 2)	// 连接线，右
			{
				m_iSelectLinkColumnIndex = iColumnIndex - 1;	// 选中连接线列索引
				m_iSelectLinkRowIndex = iRowIndex;	// 选中连接线行索引
				m_iSelectLinkDirection = 4;	// 选中连接线方向 2：下；4：右
			}
		}
	}
}

/**
* 得到选中的区域
* @param void
* @return void
*/
bool CInstrumentGraphView::GetSelectAreaPoint()
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
void CInstrumentGraphView::GetSelectAreaIndex()
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
void CInstrumentGraphView::ConvertSelectAreaToAbsoluteSpreadString()
{
	// 把选中区域转换为绝对排列字符串
	m_strAbsoluteSpread = m_pSiteData->m_oGraphInstrumentList.ConvertSelectAreaToAbsoluteSpreadString(&m_oSelectAreaIndex);
}

/**
* 判断点是否在绘图区域内
* @param CPoint* pPoint 鼠标点
* @return void
*/
bool CInstrumentGraphView::JudgeIfPointInPaper(CPoint* pPoint)
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphInstrumentList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);
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
void CInstrumentGraphView::OnShowTestValueInFloatWindow(CInstrument* pInstrument, CPoint pointClient)
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	CString strTestValue = pInstrument->GetTestResultString(pParentFrame->m_uiTestType, 0);
	if(("" != strTestValue) && (6 != pParentFrame->m_uiTestType))	// 仪器增益/ 相位。 Gain/Phase 误差
	{
		m_wndStaticFloat.SetWindowText(strTestValue);
		m_wndStaticFloat.MoveWindow(pointClient.x + 20, pointClient.y, 100, 20);
		m_wndStaticFloat.ShowWindow(SW_SHOW);
	}
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
	unsigned int uiCount = m_pSiteData->m_oInstrumentList.GetDetourInstrumentCount(uiIndex);
	if(uiCount > 1)
	{
		CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
		pParentFrame->OnShowDetourGraphView(uiCount);
	}
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
	// 判断：迂回道低端仪器存在
	if(NULL != m_pGraphInstrumentDataDetourLow)
	{
		DrawDetourMarker(pDC, m_pGraphInstrumentDataDetourLow);
	}
	// 判断：迂回道高端仪器存在
	if(NULL != m_pGraphInstrumentDataDetourHigh)
	{
		DrawDetourMarker(pDC, m_pGraphInstrumentDataDetourHigh);
	}
}

// 画迂回道标记
void CInstrumentGraphView::DrawDetourMarker(CDC* pDC, CGraphInstrumentData* m_pGraphInstrumentData)
{
	CPen oPen;
	CPen* pOldPen;
	CBrush oBrush;
	CBrush* pOldBrush;;

	oPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	pOldPen = pDC->SelectObject(&oPen);
	oBrush.CreateSolidBrush(RGB(255, 0, 255));
	pOldBrush = pDC->SelectObject(&oBrush);

	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphInstrumentList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);
	int iCenterX = m_pGraphInstrumentData->m_iPosX * m_iGridX;
	int iCenterY = m_pGraphInstrumentData->m_iPosY * m_iGridY;
	int iWidth = iUnitWidth / 2;
	int iHightMax = m_pSiteData->m_oGraphInstrumentList.m_iGridYMin / 2;
	int iHightMin = iUnitHight / 2;

	CPoint pts[3];
	// 判断：迂回道低端
	if(m_pGraphInstrumentDataDetourLow == m_pGraphInstrumentData)
	{
		pts[0].x = iCenterX;
		pts[0].y = iCenterY - iHightMin;

		pts[1].x = iCenterX;
		pts[1].y = iCenterY - iHightMax;

		pts[2].x = iCenterX - iWidth;
		pts[2].y = iCenterY - iHightMin;
	}
	// 判断：迂回道高端
	else if(m_pGraphInstrumentDataDetourHigh == m_pGraphInstrumentData)
	{
		pts[0].x = iCenterX;
		pts[0].y = iCenterY - iHightMin;

		pts[1].x = iCenterX;
		pts[1].y = iCenterY - iHightMax;

		pts[2].x = iCenterX + iWidth;
		pts[2].y = iCenterY - iHightMin;
	}
	pDC->Polygon(pts, 3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

// 清除迂回道标记
void CInstrumentGraphView::ClearDetourMarker()
{
	// 判断：迂回道低端仪器存在
	if(NULL != m_pGraphInstrumentDataDetourLow)
	{
		ClearDetourMarker(m_pGraphInstrumentDataDetourLow);
	}
	// 判断：迂回道高端仪器存在
	if(NULL != m_pGraphInstrumentDataDetourHigh)
	{
		ClearDetourMarker(m_pGraphInstrumentDataDetourHigh);
	}
}

// 清除迂回道标记
void CInstrumentGraphView::ClearDetourMarker(CGraphInstrumentData* m_pGraphInstrumentData)
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	m_pSiteData->m_oGraphInstrumentList.GetUnitSize(m_iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);
	int iCenterX = m_pGraphInstrumentData->m_iPosX * m_iGridX;
	int iCenterY = m_pGraphInstrumentData->m_iPosY * m_iGridY;
	int iWidth = iUnitWidth / 2;
	int iHightMax = m_pSiteData->m_oGraphInstrumentList.m_iGridYMin / 2;
	int iHightMin = iUnitHight / 2;

	CRect oRect;
	oRect.left = iCenterX - iWidth;
	oRect.right = iCenterX + iWidth;
	oRect.top = iCenterY - iHightMax;
	oRect.bottom = iCenterY + iHightMin;
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.LPtoDP(&oRect);
	InvalidateRect(&oRect, TRUE);
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