// SetupGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Test5.h"
#include "SetupGridCtrl.h"


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrlEdit
IMPLEMENT_DYNAMIC(CGridCtrlEdit, CBCGPGridCtrl)

CGridCtrlEdit::CGridCtrlEdit()
{

}

CGridCtrlEdit::~CGridCtrlEdit()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CGridCtrlEdit, CBCGPGridCtrl)

END_MESSAGE_MAP()

void CGridCtrlEdit::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();
	m_nRowHeight += 5;
	//m_nLargeRowHeight = m_nRowHeight;

}
/**
* @brief 创建列表网格
* @note  
* @param  
* @return 
*/
void	CGridCtrlEdit::CreateGrid(CRect& rc,CWnd* pParent)
{
	Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rc, pParent, (UINT)-2);
	// 禁止列排序
	EnableHeader (FALSE, BCGP_GRID_HEADER_HIDE_ITEMS);
	ShowVertScrollBar(FALSE);
	EnableColumnAutoSize(TRUE);
	EnableDragHeaderItems(FALSE);
	//SetReadOnly ();
	//SetSingleSel(TRUE);
	//SetWholeRowSel(TRUE);
	EnableDragSelection(FALSE);

}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrlList

IMPLEMENT_DYNAMIC(CGridCtrlList, CBCGPGridCtrl)

CGridCtrlList::CGridCtrlList()
{

}

CGridCtrlList::~CGridCtrlList()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CGridCtrlList, CBCGPGridCtrl)
	ON_WM_LBUTTONDOWN()	
END_MESSAGE_MAP()

//ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)
void CGridCtrlList::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();
	m_nRowHeight += 5;
	//m_nLargeRowHeight = m_nRowHeight;

}
/**
* @brief 创建列表网格
* @note  
* @param  
* @return 
*/
void	CGridCtrlList::CreateGrid(CRect& rc,CWnd* pParent)
{
	Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER , rc, pParent, (UINT)-1);
	// 禁止列排序
	EnableHeader (FALSE, BCGP_GRID_HEADER_HIDE_ITEMS);		
	EnableColumnAutoSize(TRUE);
	EnableDragHeaderItems(FALSE);
	SetReadOnly ();
	SetSingleSel(TRUE);
	SetWholeRowSel(TRUE);
	EnableDragSelection(FALSE);
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_DEFAULT);
	ShowVertScrollBar();

	BCGP_GRID_COLOR_DATA clrData;
	clrData = GetColorTheme();
	//clrData.m_clrBackground = globalData.clrWindow; //::GetSysColor(COLOR_3DFACE);
	clrData.m_EvenColors.m_clrBackground = RGB(255,255,255);
	clrData.m_OddColors.m_clrBackground = RGB(250,253,253);

	SetColorTheme(clrData);
	AdjustLayout();
}
