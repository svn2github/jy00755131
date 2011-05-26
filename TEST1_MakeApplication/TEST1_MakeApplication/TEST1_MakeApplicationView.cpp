// TEST1_MakeApplicationView.cpp : implementation of the CTEST1_MakeApplicationView class
//

#include "stdafx.h"
#include "TEST1_MakeApplication.h"

#include "TEST1_MakeApplicationDoc.h"
#include "TEST1_MakeApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTEST1_MakeApplicationView

IMPLEMENT_DYNCREATE(CTEST1_MakeApplicationView, CView)

BEGIN_MESSAGE_MAP(CTEST1_MakeApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CTEST1_MakeApplicationView construction/destruction

CTEST1_MakeApplicationView::CTEST1_MakeApplicationView()
{
	// TODO: add construction code here

}

CTEST1_MakeApplicationView::~CTEST1_MakeApplicationView()
{
}

BOOL CTEST1_MakeApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTEST1_MakeApplicationView drawing

void CTEST1_MakeApplicationView::OnDraw(CDC* /*pDC*/)
{
	CTEST1_MakeApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CTEST1_MakeApplicationView printing

void CTEST1_MakeApplicationView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CTEST1_MakeApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTEST1_MakeApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTEST1_MakeApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTEST1_MakeApplicationView diagnostics

#ifdef _DEBUG
void CTEST1_MakeApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CTEST1_MakeApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTEST1_MakeApplicationDoc* CTEST1_MakeApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTEST1_MakeApplicationDoc)));
	return (CTEST1_MakeApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CTEST1_MakeApplicationView message handlers
