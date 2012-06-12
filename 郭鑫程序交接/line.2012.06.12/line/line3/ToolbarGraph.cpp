
#include "stdafx.h"
#include "ToolbarGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CToolbarGraph, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolbarGraph::CToolbarGraph (UINT uiID)
{
	m_bText = FALSE;	
	m_bImage = TRUE;
	m_nID = uiID;

	/**宽度*/
	m_iWidth = 200;
	/**模式*/
	m_iMode = 0;
	/**标签*/
	m_strLabel = "";
}

CToolbarGraph::~CToolbarGraph()
{

}

void CToolbarGraph::OnDraw(CDC* pDC, const CRect& rectClient, 
							 CBCGPToolBarImages* /*pImages*/, 
							 BOOL /*bHorz*/, BOOL /*bCustomizeMode*/,						
							 BOOL /*bHighlight*/,						
							 BOOL /*bDrawBorder*/, 
							 BOOL /*bGrayDisabledButtons*/)
{
	CPen oPen;
	CPen* pOldPen;
	CBrush oBrushGreen;
	CBrush oBrushRed;
	CBrush oBrush1;
	CBrush oBrush2;
	CBrush oBrush3;
	CBrush oBrush4;
	CBrush oBrush5;
	CBrush oBrush6;
	CBrush* pOldPenBrush;

	oPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	// 黑色
	oBrushGreen.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
	oBrush1.CreateSolidBrush(RGB(255, 255, 255));	// 色1，白色
	oBrush2.CreateSolidBrush(RGB(255, 255, 200));	// 色2
	oBrush3.CreateSolidBrush(RGB(255, 255, 0));	// 色3，黄色
	oBrush4.CreateSolidBrush(RGB(255, 192, 64));	// 色4
	oBrush5.CreateSolidBrush(RGB(255, 128, 64));	// 色5
	oBrush6.CreateSolidBrush(RGB(255, 128, 0));	// 色6
	oBrushRed.CreateSolidBrush(RGB(255, 0, 0));	// 红色
	pOldPen = pDC->SelectObject(&oPen);
	pOldPenBrush = pDC->SelectObject(&oBrushGreen);

	int iHight = 8;
	CRect oRect;

	if(1 == m_iMode)	// 1：合格-不合格
	{
		pOldPenBrush = pDC->SelectObject(&oBrushGreen);
		oRect.left = rectClient.left;
		oRect.right = rectClient.left + rectClient.Width() / 2;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushGreen);

		pDC->SelectObject(&oBrushRed);
		oRect.left = rectClient.left + rectClient.Width() / 2;
		oRect.right = rectClient.right;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushRed);
	}
	else if(2 == m_iMode)	// 2：不合格-合格-不合格
	{
		pDC->SelectObject(&oBrushRed);
		oRect.left = rectClient.left;
		oRect.right = rectClient.left + rectClient.Width() / 3;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushRed);

		pDC->SelectObject(&oBrushGreen);
		oRect.left = rectClient.left + rectClient.Width() / 3;
		oRect.right = rectClient.left + 2 * rectClient.Width() / 3;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushGreen);

		pDC->SelectObject(&oBrushRed);
		oRect.left = rectClient.left + 2 * rectClient.Width() / 3;
		oRect.right = rectClient.right;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushRed);
	}
	else if(3 == m_iMode)	// 3：噪声监测判据图
	{
		pDC->SelectObject(&oBrushGreen);
		oRect.left = rectClient.left;
		oRect.right = rectClient.left + rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushGreen);

		pDC->SelectObject(&oBrush1);
		oRect.left = rectClient.left + rectClient.Width() / 8;
		oRect.right = rectClient.left + 2 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush1);

		pDC->SelectObject(&oBrush2);
		oRect.left = rectClient.left + 2 * rectClient.Width() / 8;
		oRect.right = rectClient.left + 3 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush2);

		pDC->SelectObject(&oBrush3);
		oRect.left = rectClient.left + 3 * rectClient.Width() / 8;
		oRect.right = rectClient.left + 4 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush3);

		pDC->SelectObject(&oBrush4);
		oRect.left = rectClient.left + 4 * rectClient.Width() / 8;
		oRect.right = rectClient.left + 5 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush4);

		pDC->SelectObject(&oBrush5);
		oRect.left = rectClient.left + 5 * rectClient.Width() / 8;
		oRect.right = rectClient.left + 6 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush5);

		pDC->SelectObject(&oBrush6);
		oRect.left = rectClient.left + 6 * rectClient.Width() / 8;
		oRect.right = rectClient.left + 7 * rectClient.Width() / 8;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrush6);

		pDC->SelectObject(&oBrushRed);
		oRect.left = rectClient.left + 7 * rectClient.Width() / 8;
		oRect.right = rectClient.right;
		oRect.top = rectClient.top;
		oRect.bottom = rectClient.top + iHight;
		pDC->FillRect(&oRect, &oBrushRed);
	}

	oRect.left = rectClient.left;
	oRect.right = rectClient.right;
	oRect.top = rectClient.top + iHight;
	oRect.bottom = rectClient.bottom;

	pDC->DrawText(m_strLabel, &oRect, DT_CENTER | DT_VCENTER);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldPenBrush);
}		 


SIZE CToolbarGraph::OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	CSize size = sizeDefault;
	size.cx = m_iWidth;
	return size;
}