// workspacebar3.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkSpaceBar3 : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar3();

	// Attributes
protected:
	CTreeCtrl	m_wndTree;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CWorkSpaceBar3();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar3)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
