// Test4View.cpp : implementation of the CTest4View class
//

#include "stdafx.h"
#include "Test4.h"

#include "Test4Doc.h"
#include "Test4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest4View

IMPLEMENT_DYNCREATE(CTest4View, CView)

BEGIN_MESSAGE_MAP(CTest4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CTest4View construction/destruction

CTest4View::CTest4View()
{
	// TODO: add construction code here

}

CTest4View::~CTest4View()
{
}

BOOL CTest4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTest4View drawing

void CTest4View::OnDraw(CDC* /*pDC*/)
{
	CTest4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CTest4View printing

void CTest4View::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CTest4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTest4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTest4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTest4View diagnostics

#ifdef _DEBUG
void CTest4View::AssertValid() const
{
	CView::AssertValid();
}

void CTest4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTest4Doc* CTest4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest4Doc)));
	return (CTest4Doc*)m_pDocument;
}
#endif //_DEBUG


// CTest4View message handlers
