// Test5View.h : interface of the CTest5View class
//


#pragma once


class CTest5View : public CView
{
protected: // create from serialization only
	CTest5View();
	DECLARE_DYNCREATE(CTest5View)

// Attributes
public:
	CTest5Doc* GetDocument() const;

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
	virtual ~CTest5View();
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

#ifndef _DEBUG  // debug version in Test5View.cpp
inline CTest5Doc* CTest5View::GetDocument() const
   { return reinterpret_cast<CTest5Doc*>(m_pDocument); }
#endif

