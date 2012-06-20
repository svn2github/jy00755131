// afxthreadView.h : interface of the CafxthreadView class
//


#pragma once


class CafxthreadView : public CView
{
protected: // create from serialization only
	CafxthreadView();
	DECLARE_DYNCREATE(CafxthreadView)

// Attributes
public:
	CafxthreadDoc* GetDocument() const;

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
	virtual ~CafxthreadView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in afxthreadView.cpp
inline CafxthreadDoc* CafxthreadView::GetDocument() const
   { return reinterpret_cast<CafxthreadDoc*>(m_pDocument); }
#endif

