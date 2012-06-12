//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2010 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPSliderButton.cpp: implementation of the CBCGPSliderButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPSliderButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBCGPSliderButton, CBCGPToolbarButton, 1)

static const int iDefaultWidth = 150;
static const int iDefaultHeight = 25;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4355)

CBCGPSliderButton::CBCGPSliderButton() :
	m_wndSlider (*this)
{
	m_dwStyle = WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS;
	m_iWidth = iDefaultWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
}
//**************************************************************************************
CBCGPSliderButton::CBCGPSliderButton (UINT uiId,
			int iImage,
			DWORD dwStyle,
			int iWidth) :
	CBCGPToolbarButton (uiId, iImage),
	m_wndSlider (*this)
{
	m_dwStyle = dwStyle | WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS;
	m_iWidth = (iWidth == 0) ? iDefaultWidth : iWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
}

#pragma warning (default : 4355)

//**************************************************************************************
CBCGPSliderButton::~CBCGPSliderButton()
{
	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.DestroyWindow ();
	}
}
//**************************************************************************************
SIZE CBCGPSliderButton::OnCalculateSize (CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL bHorz)
{
	if (!IsVisible ())
	{
		if (m_wndSlider.GetSafeHwnd () != NULL)
		{
			m_wndSlider.ShowWindow (SW_HIDE);
		}

		return CSize (0,0);
	}

	if (bHorz)
	{
		if (m_wndSlider.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_wndSlider.ShowWindow (SW_SHOWNOACTIVATE);
			m_wndSlider.ModifyStyle (TBS_VERT, TBS_HORZ);
		}

		return CSize (m_iWidth, iDefaultHeight);
	}
	else
	{
		if (m_wndSlider.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_wndSlider.ShowWindow (SW_SHOWNOACTIVATE);
			m_wndSlider.ModifyStyle (TBS_HORZ, TBS_VERT);
		}

		return CSize (iDefaultHeight, m_iWidth);
	}
}
//**************************************************************************************
void CBCGPSliderButton::Serialize(CArchive& ar)
{
	CBCGPToolbarButton::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_iWidth;
		ar >> m_dwStyle;

		int		nMin;
		int		nMax;
		int		nValue;

		ar >> nMin;
		ar >> nMax;
		ar >> nValue;

		SetRange (nMin, nMax);
		SetValue (nValue, FALSE);
	}
	else
	{
		ar << m_iWidth;
		ar << m_dwStyle;
		ar << m_nMin;
		ar << m_nMax;
		ar << m_nValue;
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnMove ()
{
	if (m_wndSlider.GetSafeHwnd () != NULL &&
		(m_wndSlider.GetStyle () & WS_VISIBLE))
	{
		m_wndSlider.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnSize (int iSize)
{
	m_iWidth = iSize;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_wndSlider.GetSafeHwnd () != NULL &&
		(m_wndSlider.GetStyle () & WS_VISIBLE))
	{
		m_wndSlider.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CBCGPSliderButton::OnChangeParentWnd (CWnd* pWndParent)
{
	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		CWnd* pWndParentCurr = m_wndSlider.GetParent ();
		ASSERT (pWndParentCurr != NULL);

		if (pWndParent != NULL &&
			pWndParentCurr->GetSafeHwnd () == pWndParent->GetSafeHwnd ())
		{
			return;
		}

		m_wndSlider.DestroyWindow ();
	}

	if (pWndParent == NULL || pWndParent->GetSafeHwnd () == NULL)
	{
		return;
	}

	if (!m_wndSlider.Create (m_dwStyle, m_rect, pWndParent, m_nID))
	{
		ASSERT (FALSE);
		return;
	}

	m_wndSlider.SetRange (m_nMin, m_nMax);
	m_wndSlider.SetPos (m_nValue);
	m_wndSlider.SetOwner (pWndParent->GetTopLevelFrame ());
}
//**************************************************************************************
void CBCGPSliderButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarButton::CopyFrom (s);

	const CBCGPSliderButton& src = (const CBCGPSliderButton&) s;

	m_dwStyle = src.m_dwStyle;
	m_iWidth = src.m_iWidth;
	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nValue = src.m_nValue;
}
//************************************************************************************
void CBCGPSliderButton::SetRange (int iMin, int iMax)
{
	m_nMin = iMin;
	m_nMax = iMax;

	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.SetRange (iMin, iMax, TRUE);
	}

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nMin = iMin;
				pSlider->m_nMax = iMax;

				if (pSlider->m_wndSlider.GetSafeHwnd () != NULL)
				{
					pSlider->m_wndSlider.SetRange (iMin, iMax, TRUE);
				}
			}
		}
	}
}
//************************************************************************************
void CBCGPSliderButton::SetValue (int iValue, BOOL bNotify)
{
	m_nValue = iValue;
	if (GetHwnd () != NULL)
	{
		m_wndSlider.SetPos (iValue);
	}

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nValue = iValue;

				if (pSlider->GetHwnd () != NULL)
				{
					pSlider->m_wndSlider.SetPos (iValue);
				}
			}
		}
	}

	if (bNotify && m_wndSlider.GetSafeHwnd () != NULL)
	{
		m_wndSlider.GetOwner ()->SendMessage (WM_COMMAND, m_nID);
	}
}
//************************************************************************************
int CBCGPSliderButton::GetPos (UINT uiCmd)
{
	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPSliderButton* pSlider = DYNAMIC_DOWNCAST (CBCGPSliderButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider->GetHwnd () != NULL)
			{
				return pSlider->m_wndSlider.GetPos ();
			}
		}
	}

	return -1;
}
//****************************************************************************************
void CBCGPSliderButton::OnShow (BOOL bShow)
{
	if (m_wndSlider.GetSafeHwnd () != NULL)
	{
		if (bShow)
		{
			m_wndSlider.ShowWindow (SW_SHOWNOACTIVATE);
			OnMove ();
		}
		else
		{
			m_wndSlider.ShowWindow (SW_HIDE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarSliderCtrl

CToolbarSliderCtrl::CToolbarSliderCtrl(CBCGPSliderButton& btnSlider) :
	m_btnSlider (btnSlider)
{
	m_bVisualManagerStyle = TRUE;
}

CToolbarSliderCtrl::~CToolbarSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CToolbarSliderCtrl, CBCGPSliderCtrl)
	//{{AFX_MSG_MAP(CToolbarSliderCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarSliderCtrl message handlers

void CToolbarSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos = GetPos ();

	CBCGPSliderCtrl::OnMouseMove(nFlags, point);

	if ((nFlags & MK_LBUTTON) && GetPos () != nPos)
	{
		m_btnSlider.SetValue (GetPos (), TRUE);
	}
}

void CToolbarSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nPos = GetPos ();

	CBCGPSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (GetPos () != nPos)
	{
		m_btnSlider.SetValue (GetPos (), TRUE);
	}
}

void CToolbarSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CBCGPSliderCtrl::OnLButtonUp(nFlags, point);
	if (IsWindowVisible ())
	{
		m_btnSlider.SetValue (GetPos (), TRUE);
	}
}

