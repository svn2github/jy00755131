// outputbar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:

	CListCtrl	m_wndList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// »’÷æ ‰≥ˆ
	void AppendLog(LPCTSTR strLog);
};

/////////////////////////////////////////////////////////////////////////////
