// line3View.cpp : implementation of the CLineView class
//

#include "stdafx.h"
#include "line.h"

#include "lineDoc.h"
#include "lineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLineView

IMPLEMENT_DYNCREATE(CLineView, CView)

BEGIN_MESSAGE_MAP(CLineView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CLineView construction/destruction

CLineView::CLineView()
{
	// TODO: add construction code here

}

CLineView::~CLineView()
{
}

BOOL CLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLineView drawing

void CLineView::OnDraw(CDC* /*pDC*/)
{
	CLineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CLineView printing

void CLineView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLineView diagnostics

#ifdef _DEBUG
void CLineView::AssertValid() const
{
	CView::AssertValid();
}

void CLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLineDoc* CLineView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLineDoc)));
	return (CLineDoc*)m_pDocument;
}
#endif //_DEBUG


// CLineView message handlers
