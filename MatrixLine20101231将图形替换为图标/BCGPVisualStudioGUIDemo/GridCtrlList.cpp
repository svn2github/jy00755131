// GridCtrlList.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "GridCtrlList.h"


// CGridCtrlList

IMPLEMENT_DYNAMIC(CGridCtrlList, CBCGPGridCtrl)

CGridCtrlList::CGridCtrlList()
{
	m_iRowHeightAdd = 0;	// 行附加高度
}

CGridCtrlList::~CGridCtrlList()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlList, CBCGPGridCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGridCtrlList 消息处理程序

void CGridCtrlList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd* pWnd = GetParent();
	pWnd->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GRID_LIST_DBLCLK, BN_CLICKED));

	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);
}

void CGridCtrlList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_GRID_LIST_LBBUTTONDOWN, BN_CLICKED));

	CBCGPGridCtrl::OnLButtonDown(nFlags, point);
}

void CGridCtrlList::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();

	m_nRowHeight = m_nRowHeight + m_iRowHeightAdd;
	m_nLargeRowHeight = m_nRowHeight;
}