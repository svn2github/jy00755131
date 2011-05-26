// Test3_MakeApplicationView.h : interface of the CTest3_MakeApplicationView class
//


#pragma once


class CTest3_MakeApplicationView : public CView
{
protected: // create from serialization only
	CTest3_MakeApplicationView();
	DECLARE_DYNCREATE(CTest3_MakeApplicationView)

// Attributes
public:
	CTest3_MakeApplicationDoc* GetDocument() const;

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
	virtual ~CTest3_MakeApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Test3_MakeApplicationView.cpp
inline CTest3_MakeApplicationDoc* CTest3_MakeApplicationView::GetDocument() const
   { return reinterpret_cast<CTest3_MakeApplicationDoc*>(m_pDocument); }
#endif

