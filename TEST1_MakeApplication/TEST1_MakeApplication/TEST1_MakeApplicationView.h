// TEST1_MakeApplicationView.h : interface of the CTEST1_MakeApplicationView class
//


#pragma once


class CTEST1_MakeApplicationView : public CView
{
protected: // create from serialization only
	CTEST1_MakeApplicationView();
	DECLARE_DYNCREATE(CTEST1_MakeApplicationView)

// Attributes
public:
	CTEST1_MakeApplicationDoc* GetDocument() const;

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
	virtual ~CTEST1_MakeApplicationView();
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

#ifndef _DEBUG  // debug version in TEST1_MakeApplicationView.cpp
inline CTEST1_MakeApplicationDoc* CTEST1_MakeApplicationView::GetDocument() const
   { return reinterpret_cast<CTEST1_MakeApplicationDoc*>(m_pDocument); }
#endif

