// line3View.h : interface of the Cline3View class
//


#pragma once


class CLineView : public CView
{
protected: // create from serialization only
	CLineView();
	DECLARE_DYNCREATE(CLineView)

// Attributes
public:
	CLineDoc* GetDocument() const;

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
	virtual ~CLineView();
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

#ifndef _DEBUG  // debug version in line3View.cpp
inline CLineDoc* CLineView::GetDocument() const
   { return reinterpret_cast<CLineDoc*>(m_pDocument); }
#endif

