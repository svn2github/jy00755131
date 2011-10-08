// Ex1_04View.h : interface of the CEx1_04View class
//


#pragma once


class CEx1_04View : public CView
{
protected: // create from serialization only
	CEx1_04View();
	DECLARE_DYNCREATE(CEx1_04View)

// Attributes
public:
	CEx1_04Doc* GetDocument() const;

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
	virtual ~CEx1_04View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Ex1_04View.cpp
inline CEx1_04Doc* CEx1_04View::GetDocument() const
   { return reinterpret_cast<CEx1_04Doc*>(m_pDocument); }
#endif

