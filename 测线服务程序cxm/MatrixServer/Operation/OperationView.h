// OperationView.h : interface of the COperationView class
//


#pragma once


class COperationView : public CView
{
protected: // create from serialization only
	COperationView();
	DECLARE_DYNCREATE(COperationView)

// Attributes
public:
	COperationDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetupSource();
	afx_msg void OnSetupOperation();
	afx_msg void OnSetupProcesstype();
	afx_msg void OnSetupNoiseediting();
	afx_msg void OnSetupComments();
	afx_msg void OnSetupOptions();
	afx_msg void OnSetupDelaysetup();
	afx_msg void OnSetupPreferences();
	afx_msg void OnViewAllVPBar();
	afx_msg void OnUpdataViewAllVPBar(CCmdUI *pCmdUI);
	afx_msg void OnViewVPToDoBar();
	afx_msg void OnUpdataViewVPToDoBar(CCmdUI *pCmdUI);
	afx_msg void OnViewVPDoneBar();
	afx_msg void OnUpdataViewVPDoneBar(CCmdUI *pCmdUI);
	afx_msg void OnViewActiveSourceBar();
	afx_msg void OnUpdataViewActiveSourceBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdataViewVPSourceBar(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in OperationView.cpp
inline COperationDoc* COperationView::GetDocument() const
   { return reinterpret_cast<COperationDoc*>(m_pDocument); }
#endif

