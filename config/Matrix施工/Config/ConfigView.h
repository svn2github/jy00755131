// ConfigView.h : interface of the CConfigView class
//
#pragma once
#include "afxwin.h"


class CConfigView : public CBCGPFormView
{
protected: // create from serialization only
	CConfigView();
	DECLARE_DYNCREATE(CConfigView)

public:
	enum{ IDD = IDD_CONFIG_FORM };

	// Attributes
public:
	CConfigDoc* GetDocument() const;
private:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

	// Implementation
public:
	virtual ~CConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CStatic m_ctrlActivity;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in ConfigView.cpp
inline CConfigDoc* CConfigView::GetDocument() const
{ return reinterpret_cast<CConfigDoc*>(m_pDocument); }
#endif

