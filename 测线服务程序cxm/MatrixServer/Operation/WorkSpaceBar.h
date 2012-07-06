// workspace2.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Parameter.h"
class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();

// Attributes
protected:
	CBCGPGridCtrl   m_GridView;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
public:
	// 载入全部炮点信息
	virtual void LoadShotPoints(void);
protected:
	// 设置表头
	virtual bool SetGridHead(void);
private:
	CImageList	m_ImageList;
	// 载入单条炮点信息
	virtual void LoadShotPoint(void);
public:
	afx_msg void OnDestroy();
public:
	// 震源ID数组
	int m_iActiveSourceID[ActiveSourceNumLimit];
/*	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);*/
};

/////////////////////////////////////////////////////////////////////////////
