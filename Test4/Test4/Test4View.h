// Test4View.h : interface of the CTest4View class
//


#pragma once


class CTest4View : public CView
{
protected: // create from serialization only
	CTest4View();
	DECLARE_DYNCREATE(CTest4View)

// Attributes
public:
	CTest4Doc* GetDocument() const;

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
	virtual ~CTest4View();
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

#ifndef _DEBUG  // debug version in Test4View.cpp
inline CTest4Doc* CTest4View::GetDocument() const
   { return reinterpret_cast<CTest4Doc*>(m_pDocument); }
#endif

