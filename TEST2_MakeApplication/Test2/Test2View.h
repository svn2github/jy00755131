// Test2View.h : interface of the CTest2View class
//


#pragma once


class CTest2View : public CView
{
protected: // create from serialization only
	CTest2View();
	DECLARE_DYNCREATE(CTest2View)

// Attributes
public:
	CTest2Doc* GetDocument() const;

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
	virtual ~CTest2View();
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

#ifndef _DEBUG  // debug version in Test2View.cpp
inline CTest2Doc* CTest2View::GetDocument() const
   { return reinterpret_cast<CTest2Doc*>(m_pDocument); }
#endif

