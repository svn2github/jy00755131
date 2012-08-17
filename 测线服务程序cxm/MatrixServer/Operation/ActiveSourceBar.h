// activesourcebar.h : interface of the CActiveSourceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OptParam.h"
class CActiveSourceCtrl;
class CActiveSourceBar : public CBCGPDockingControlBar
{
public:
	CActiveSourceBar();

// Attributes
protected:
	
	CActiveSourceCtrl* m_pActiveSourceCtrl;
private:
	CImageList	m_ImageList;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveSourceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CActiveSourceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CActiveSourceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
public:
	// 设置表头
	bool SetGridHead(void);
	// 载入全部震源信息
	void LoadActiveSources(void);
	// 载入单条震源信息
	void LoadActiveSource(int iNb);
	// 按键ID
	int m_iActiveSourceBtnID[ActiveSourceNumLimit];
public:
	CBCGPGridCtrl   m_GridView;
	afx_msg void OnActivesourceDisassociatevp();
	afx_msg void OnActivesourceAssociatevp();
	afx_msg void OnActivesourceAssociatesplspn();
	afx_msg void OnActivesourceCleanready();
	afx_msg void OnProcRdyBtn(UINT id);
};

/////////////////////////////////////////////////////////////////////////////
