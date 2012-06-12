// LeftViewBar.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "LeftViewBarInstrumentTop.h"
#include "InstrumentGraphChildFrame.h"

IMPLEMENT_DYNAMIC(CLeftViewBarInstrumentTop, CBCGPDockingControlBar)

CLeftViewBarInstrumentTop::CLeftViewBarInstrumentTop()
{
	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bDrawSelectArea = false;	// 是否画鼠标选中区域位置
	m_fZoomRate = 1.0;	// 放大倍数
	m_fRateX = 0.0;	// 基准点X
	m_fRateY = 0.0;	// 基准点Y	
	
	m_iGridY = 0;	// 单元格Y方向尺寸
	m_iGridYMax = 20;	// 单元格Y方向尺寸最大值
	m_iLineHight = 0;	// 测线高度
	m_iPaperHeadX = 20;	// 图形区X坐标预留尺寸	
	m_iPaperHeadTop = 25;	// 图形区左边预留尺寸
	m_bCanDrawGraph = false;	// 是否可以画图

	m_uiLineCount = 0;	//测线数量
}

CLeftViewBarInstrumentTop::~CLeftViewBarInstrumentTop()
{
}


BEGIN_MESSAGE_MAP(CLeftViewBarInstrumentTop, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CLeftViewBarInstrumentTop)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLeftViewBarInstrumentTop message handlers

int CLeftViewBarInstrumentTop::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	RemoveCaptionButtons();
	return 0;
}

void CLeftViewBarInstrumentTop::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	if (CanAdjustLayout ())
	{
		m_bDrawSelectArea = false;	// 是否画鼠标选中区域位置
		if((cx == 0) || (cx == 1))
		{
			CRect oRect;
			GetWindowRect(&oRect);
			oRect.right = m_oRect.left + 250;
			MoveWindow(oRect);
		}
		GetClientRect(&m_oRect);
		// 判断当前图形大小是否可以画图
		CheckIfCanDraw();
		RedrawWindow();
	}
}

void CLeftViewBarInstrumentTop::OnPaint() 
{
	CPaintDC dc(this);
	GetClientRect(&m_oRect);

	// 画背景
	DrawBackground(&dc);
	// 判断：可以画图
	if(true == m_bCanDrawGraph)
	{
		// 画测线
		DrawLines(&dc);
		// 画鼠标选中区域
		DrawSelectArea(&dc);
	}
}

void CLeftViewBarInstrumentTop::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 判断：可以画图
	if(true == m_bCanDrawGraph)
	{
		CClientDC oDC(this);

		oDC.DPtoLP(&point);	
		m_oPointBase = point;	// 鼠标键按下时鼠标点
		m_oPointLast = m_oPointBase;	// 鼠标键释放时鼠标点
		m_bMouseDownLeft = true;	// 鼠标左键是否按下
	}

	CBCGPDockingControlBar::OnLButtonDown(nFlags, point);
}

void CLeftViewBarInstrumentTop::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 判断：鼠标左键是否按下 && 可以画图
	if((true == m_bMouseDownLeft) && (true == m_bCanDrawGraph))
	{
		CClientDC oDC(this);
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
			m_bDrawSelectArea = true;	// 是否画鼠标选中区域位置
			Invalidate(TRUE);
		}
		else
		{
			m_bDrawSelectArea = false;	// 是否画鼠标选中区域位置
			Invalidate(TRUE);
		}
		// 导航局部放大
		OnZoomInNavigater();
	}

	CBCGPDockingControlBar::OnLButtonUp(nFlags, point);
}

void CLeftViewBarInstrumentTop::OnMouseMove(UINT nFlags, CPoint point)
{
	// 判断：鼠标左键是否按下 && 可以画图
	if((true == m_bMouseDownLeft) && (true == m_bCanDrawGraph))
	{
		CClientDC oDC(this);
		oDC.DPtoLP(&point);
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

	CBCGPDockingControlBar::OnMouseMove(nFlags, point);
}

/**
* 导航局部放大
* @param void
* @return void
*/
void CLeftViewBarInstrumentTop::OnZoomInNavigater()
{
	CInstrumentGraphChildFrame* pParentFrame = (CInstrumentGraphChildFrame*)GetParentFrame();
	// 导航局部放大
	pParentFrame->OnZoomInNavigater(m_fZoomRate, m_fRateX, m_fRateY);
}

/**
* 响应设置测线数量
* @param unsigned int uiLineCount 测线数量
* @return void
*/
void CLeftViewBarInstrumentTop::OnSetLineCount(unsigned int uiLineCount)
{
	m_bDrawSelectArea = false;	// 是否画鼠标选中区域位置
	m_uiLineCount = uiLineCount;
	if(TRUE == ::IsWindow(this->m_hWnd))
	{
		// 判断当前图形大小是否可以画图
		CheckIfCanDraw();
		RedrawWindow();
	}	
}

/**
* 得到选中的区域
* @param void
* @return bool true：成功；false：失败
*/
bool CLeftViewBarInstrumentTop::GetSelectAreaPoint()
{
	m_oSelectArea.left = m_oPointBase.x;
	m_oSelectArea.top = m_oPointBase.y;
	m_oSelectArea.right = m_oPointLast.x;
	m_oSelectArea.bottom = m_oPointLast.y;
	m_oSelectArea.NormalizeRect();
	if(m_oSelectArea.left < m_iPaperHeadX)
	{
		m_oSelectArea.left = m_iPaperHeadX;
	}
	if(m_oSelectArea.right > m_oRect.right - m_iPaperHeadX)
	{
		m_oSelectArea.right = m_oRect.right - m_iPaperHeadX;
	}
	if(m_oSelectArea.top < m_iPaperHeadTop)
	{
		m_oSelectArea.top = m_iPaperHeadTop;
	}
	if(m_oSelectArea.bottom > m_iGridY * (m_uiLineCount - 1)  + m_iPaperHeadTop)
	{
		m_oSelectArea.bottom = m_iGridY * (m_uiLineCount - 1)  + m_iPaperHeadTop;
	}

	float fWidthDrag = (float)m_oSelectArea.Width();
	if(fWidthDrag < 10.0)
	{
		m_fZoomRate = 1.0;	// 放大倍数
		m_fRateX = 0.0;	// 基准点X
		m_fRateY = 0.0;	// 基准点Y
		return false;
	}
	float fWidthClient = (float)(m_oRect.Width() - 2 * m_iPaperHeadX);
	m_fZoomRate = fWidthClient / fWidthDrag;
	m_fRateX = ((float)(m_oSelectArea.left - m_iPaperHeadX)) / ((float)(m_oRect.Width() - 2 * m_iPaperHeadX));	// 基准点X
	if(m_uiLineCount > 1)
	{
		m_fRateY = ((float)(m_oSelectArea.top - m_iPaperHeadTop)) / ((float)(m_iGridY * (m_uiLineCount - 1)));	// 基准点Y
	}
	else
	{
		m_fRateY = 0.0;
	}

	return true;
}

/**
* 画背景
* @param CDC* pDC 画图句柄
* @return void
*/
void CLeftViewBarInstrumentTop::DrawBackground(CDC* pDC)
{
	CBrush oBrush;
	CBrush* pOldPenBrush;
	oBrush.CreateSolidBrush(RGB(255, 255, 255));
	pOldPenBrush = pDC->SelectObject(&oBrush);
	pDC->FillRect(&m_oRect, &oBrush);
	pDC->SelectObject(pOldPenBrush);
}

/**
* 判断当前图形大小是否可以画图
* @param void
* @return bool true：可以；false：不可以
*/
bool CLeftViewBarInstrumentTop::CheckIfCanDraw()
{
	if(0 == m_uiLineCount)
	{
		m_bCanDrawGraph = false;
		return false;
	}
	if(m_oRect.Width() <= m_iPaperHeadX * 2)
	{
		m_bCanDrawGraph = false;
		return false;
	}
	if(m_oRect.Height() <= (m_iPaperHeadTop + m_uiLineCount))
	{
		m_bCanDrawGraph = false;
		return false;
	}
	m_iGridY = (m_oRect.Height() - m_iPaperHeadTop) / m_uiLineCount;
	if(m_iGridY > m_iGridYMax)
	{
		m_iGridY = m_iGridYMax;
	}
	m_iLineHight = m_iGridY / 2;
	if(0 == m_iLineHight)
	{
		m_iLineHight = 1;
	}

	m_bCanDrawGraph = true;
	return true;
}

/**
* 画测线
* @param CDC* pDC
* @return void
*/
void CLeftViewBarInstrumentTop::DrawLines(CDC* pDC)
{
	CPen oPenLine;
	CPen* pOldPen;

	oPenLine.CreatePen(PS_SOLID, m_iLineHight, RGB(128, 128, 128));
	pOldPen = pDC->SelectObject(&oPenLine);

	for(int i = 0; i < m_uiLineCount; i++)
	{
		pDC->MoveTo(m_iPaperHeadX, m_iPaperHeadTop + m_iGridY * i);
		pDC->LineTo(m_oRect.right - m_iPaperHeadX, m_iPaperHeadTop + m_iGridY * i);
	}
	pDC->SelectObject(pOldPen);
}

/**
* 画鼠标选中区域
* @param CDC* pDC
* @return void
*/
void CLeftViewBarInstrumentTop::DrawSelectArea(CDC* pDC)
{
	if(true == m_bDrawSelectArea)	// 是否画鼠标选中区域位置
	{
		CPen oPen;
		CPen* pOldPen;

		oPen.CreatePen(PS_SOLID, 3, RGB(0, 128, 0));
		pOldPen = pDC->SelectObject(&oPen);

		pDC->MoveTo(CPoint(m_oSelectArea.left, m_oSelectArea.top));
		pDC->LineTo(CPoint(m_oSelectArea.right, m_oSelectArea.top));
		pDC->MoveTo(CPoint(m_oSelectArea.left, m_oSelectArea.bottom));
		pDC->LineTo(CPoint(m_oSelectArea.right, m_oSelectArea.bottom));

		pDC->MoveTo(CPoint(m_oSelectArea.left, m_oSelectArea.top));
		pDC->LineTo(CPoint(m_oSelectArea.left, m_oSelectArea.bottom));
		pDC->MoveTo(CPoint(m_oSelectArea.right, m_oSelectArea.top));
		pDC->LineTo(CPoint(m_oSelectArea.right, m_oSelectArea.bottom));

		pDC->SelectObject(pOldPen);
	}
}

