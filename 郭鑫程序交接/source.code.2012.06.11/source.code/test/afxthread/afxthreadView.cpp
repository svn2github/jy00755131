// afxthreadView.cpp : implementation of the CafxthreadView class
//

#include "stdafx.h"
#include "afxthread.h"

#include "afxthreadDoc.h"
#include "afxthreadView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CafxthreadView

IMPLEMENT_DYNCREATE(CafxthreadView, CView)

BEGIN_MESSAGE_MAP(CafxthreadView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CafxthreadView construction/destruction

CafxthreadView::CafxthreadView()
{
	// TODO: add construction code here

}

CafxthreadView::~CafxthreadView()
{
}

BOOL CafxthreadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CafxthreadView drawing

void CafxthreadView::OnDraw(CDC* /*pDC*/)
{
	CafxthreadDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CafxthreadView printing

BOOL CafxthreadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CafxthreadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CafxthreadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CafxthreadView diagnostics

#ifdef _DEBUG
void CafxthreadView::AssertValid() const
{
	CView::AssertValid();
}

void CafxthreadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CafxthreadDoc* CafxthreadView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CafxthreadDoc)));
	return (CafxthreadDoc*)m_pDocument;
}
#endif //_DEBUG


// CafxthreadView message handlers
