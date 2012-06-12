// GridCtrlEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "GridCtrlEdit.h"


// CGridCtrlEdit

IMPLEMENT_DYNAMIC(CGridCtrlEdit, CBCGPGridCtrl)

CGridCtrlEdit::CGridCtrlEdit()
{
	m_iRowHeightAdd = 0;	// 行附加高度
}

CGridCtrlEdit::~CGridCtrlEdit()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlEdit, CBCGPGridCtrl)
END_MESSAGE_MAP()

void CGridCtrlEdit::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();

	m_nRowHeight = m_nRowHeight + m_iRowHeightAdd;
	m_nLargeRowHeight = m_nRowHeight;
}

// CGridCtrlEdit 消息处理程序


