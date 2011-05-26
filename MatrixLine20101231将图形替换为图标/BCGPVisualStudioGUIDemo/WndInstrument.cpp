// WndInstrument.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "WndInstrument.h"

// CWndInstrument

IMPLEMENT_DYNAMIC(CWndInstrument, CBCGPStatic)

CWndInstrument::CWndInstrument()
{
	/** 仪器总数*/
	m_uiInstrumentCountAll = 0;
	/** 图形单元间隔X*/
	m_iGridX = 50;
	/** 图形单元间隔Y*/
	m_iGridY = 50;
	/** 图形单元宽度*/
	m_iUnitWidth = 32;
	/** 图形单元高度*/
	m_iUnitHight = 32;
	/** 图形高度X*/
	m_iPaperHight = 100;
}

CWndInstrument::~CWndInstrument()
{
}


BEGIN_MESSAGE_MAP(CWndInstrument, CBCGPStatic)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CWndInstrument 消息处理程序



void CWndInstrument::OnSize(UINT nType, int cx, int cy)
{
	CBCGPStatic::OnSize(nType, cx, cy);
	RedrawWindow();
}

void CWndInstrument::OnPaint()
{
	CPaintDC dc(this);
	GetClientRect(&m_oRect);
	m_iPaperHight = m_oRect.bottom - m_iGridY;

	// 画背景
	DrawBackground(&dc);
	// 画连接线
	DrawLinkLine(&dc);

	// 画全部单元图
	if(1 < m_uiInstrumentCountAll)
	{		
		for(unsigned int i = 0; i < m_uiInstrumentCountAll; i++)
		{
			DrawUnit(&dc, i);
		}
	}
}

/**
* 画背景
* @param CDC* pDC 画图句柄
* @return void
*/
void CWndInstrument::DrawBackground(CDC* pDC)
{
	CBrush oBrush;
	CBrush* pOldPenBrush;
	oBrush.CreateSolidBrush(RGB(255, 255, 255));
	pOldPenBrush = pDC->SelectObject(&oBrush);
	pDC->FillRect(&m_oRect, &oBrush);
	pDC->SelectObject(pOldPenBrush);
}

/**
* 画连接线
* @param CDC* pDC 画图句柄
* @return void
*/
void CWndInstrument::DrawLinkLine(CDC* pDC)
{
	CPen oPenLine;
	CPen* pOldPen;

	oPenLine.CreatePen(PS_SOLID, 3, RGB(0, 128, 0));
	pOldPen = pDC->SelectObject(&oPenLine);

	CPoint ptStart, ptEnd;

	ptStart.x = m_iGridX;
	ptStart.y = m_iPaperHight;

	ptEnd.x = (m_uiInstrumentCountAll) * m_iGridX;
	ptEnd.y = m_iPaperHight;

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

/**
* 函数说明
* @param CDC* pDC 画图句柄
* @param int iIndex 图形单元索引
* @return void
*/
// 画单元图
void CWndInstrument::DrawUnit(CDC* pDC, int iIndex)
{
	CPen oPenUnit;
	CPen* pOldPen;
	CBrush oBrushUnit;
	CBrush* pOldPenBrush;

	int iCenterX = m_iGridX + iIndex * m_iGridX;
	int iCenterY = m_iPaperHight;
	CRect oRect;
	oRect.left = iCenterX - m_iUnitWidth / 2;
	oRect.top = iCenterY - m_iUnitHight / 2;
	oRect.right = oRect.left + m_iUnitWidth;
	oRect.bottom = oRect.top + m_iUnitHight;

	oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色

	pOldPen = pDC->SelectObject(&oPenUnit);
	pOldPenBrush = pDC->SelectObject(&oBrushUnit);

	pDC->Ellipse(&oRect);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldPenBrush);
}

/**
* 响应显示迂回道图形视图
* @param unsigned int uiCount 迂回道中仪器数量
* @return void
*/
void CWndInstrument::OnShowDetourGraphView(unsigned int uiCount)
{
	m_uiInstrumentCountAll = uiCount;
	RedrawWindow();
}
