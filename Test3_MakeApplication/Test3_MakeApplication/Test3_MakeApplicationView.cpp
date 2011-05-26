// Test3_MakeApplicationView.cpp : implementation of the CTest3_MakeApplicationView class
//

#include "stdafx.h"
#include "Test3_MakeApplication.h"

#include "Test3_MakeApplicationDoc.h"
#include "Test3_MakeApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest3_MakeApplicationView

IMPLEMENT_DYNCREATE(CTest3_MakeApplicationView, CView)

BEGIN_MESSAGE_MAP(CTest3_MakeApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CTest3_MakeApplicationView construction/destruction

CTest3_MakeApplicationView::CTest3_MakeApplicationView()
{
	// TODO: add construction code here

}

CTest3_MakeApplicationView::~CTest3_MakeApplicationView()
{
}

BOOL CTest3_MakeApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTest3_MakeApplicationView drawing

void CTest3_MakeApplicationView::OnDraw(CDC* /*pDC*/)
{
	CTest3_MakeApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CTest3_MakeApplicationView printing

void CTest3_MakeApplicationView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CTest3_MakeApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTest3_MakeApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTest3_MakeApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTest3_MakeApplicationView diagnostics

#ifdef _DEBUG
void CTest3_MakeApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CTest3_MakeApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTest3_MakeApplicationDoc* CTest3_MakeApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest3_MakeApplicationDoc)));
	return (CTest3_MakeApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CTest3_MakeApplicationView message handlers
