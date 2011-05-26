// Test5View.cpp : implementation of the CTest5View class
//

#include "stdafx.h"
#include "Test5.h"

#include "Test5Doc.h"
#include "Test5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest5View

IMPLEMENT_DYNCREATE(CTest5View, CView)

BEGIN_MESSAGE_MAP(CTest5View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CTest5View construction/destruction

CTest5View::CTest5View()
{
	// TODO: add construction code here

}

CTest5View::~CTest5View()
{
}

BOOL CTest5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTest5View drawing

void CTest5View::OnDraw(CDC* /*pDC*/)
{
	CTest5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CTest5View printing

void CTest5View::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CTest5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTest5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTest5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTest5View diagnostics

#ifdef _DEBUG
void CTest5View::AssertValid() const
{
	CView::AssertValid();
}

void CTest5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTest5Doc* CTest5View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest5Doc)));
	return (CTest5Doc*)m_pDocument;
}
#endif //_DEBUG


// CTest5View message handlers
