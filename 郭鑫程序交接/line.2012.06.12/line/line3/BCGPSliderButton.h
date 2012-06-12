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
// BCGPSliderButton.h: interface for the CBCGPSliderButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_)
#define AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CToolbarSliderCtrl window

class CBCGPSliderButton;

class CToolbarSliderCtrl : public CBCGPSliderCtrl
{
// Construction
public:
	CToolbarSliderCtrl(CBCGPSliderButton& btnSlider);

// Attributes
public:
	CBCGPSliderButton& m_btnSlider;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolbarSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarSliderCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CBCGPSliderButton : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CBCGPSliderButton)

public:
	CBCGPSliderButton();
	CBCGPSliderButton (UINT uiId,
			int iImage = -1,
			DWORD dwStyle = 0,
			int iWidth = 0);

	virtual ~CBCGPSliderButton();

	void SetRange (int iMin, int iMax);
	void SetValue (int iValue, BOOL bNotify = TRUE);

	static int GetPos (UINT uiCmd);

// Overrides:
	virtual void Serialize(CArchive& ar);
	virtual void CopyFrom (const CBCGPToolbarButton& src);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnShow (BOOL bShow);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual void OnMove ();
	virtual void OnSize (int iSize);
	virtual HWND GetHwnd ()
	{	
		return m_wndSlider.GetSafeHwnd ();
	}

	virtual BOOL CanBeStretched () const
	{	
		return TRUE;	
	}

	virtual BOOL HaveHotBorder () const
	{
		return FALSE;
	}

	virtual void OnDraw (CDC* /*pDC*/, const CRect& /*rect*/, CBCGPToolBarImages* /*pImages*/,
						BOOL /*bHorz*/ = TRUE, BOOL /*bCustomizeMode*/ = FALSE,
						BOOL /*bHighlight*/ = FALSE,
						BOOL /*bDrawBorder*/ = TRUE,
						BOOL /*bGrayDisabledButtons*/ = TRUE) {}
// Attributes:
public:
	CToolbarSliderCtrl	m_wndSlider;
	int				m_iWidth;
	DWORD			m_dwStyle;
	int				m_nMin;
	int				m_nMax;
	int				m_nValue;
};

#endif // !defined(AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_)
