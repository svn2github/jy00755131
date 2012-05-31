// OperationView.cpp : implementation of the COperationView class
//

#include "stdafx.h"
#include "Operation.h"

#include "OperationDoc.h"
#include "OperationView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationView

IMPLEMENT_DYNCREATE(COperationView, CView)

BEGIN_MESSAGE_MAP(COperationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_SETUP_SOURCE, &COperationView::OnSetupSource)
	ON_COMMAND(ID_SETUP_OPERATION, &COperationView::OnSetupOperation)
	ON_COMMAND(ID_SETUP_PROCESSTYPE, &COperationView::OnSetupProcesstype)
	ON_COMMAND(ID_SETUP_NOISEEDITING, &COperationView::OnSetupNoiseediting)
	ON_COMMAND(ID_SETUP_COMMENTS, &COperationView::OnSetupComments)
	ON_COMMAND(ID_SETUP_OPTIONS, &COperationView::OnSetupOptions)
	ON_COMMAND(ID_SETUP_DELAYSETUP, &COperationView::OnSetupDelaysetup)
	ON_COMMAND(ID_SETUP_PREFERENCES, &COperationView::OnSetupPreferences)
END_MESSAGE_MAP()

// COperationView construction/destruction

COperationView::COperationView()
{
	// TODO: add construction code here

}

COperationView::~COperationView()
{
}

BOOL COperationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COperationView drawing

void COperationView::OnDraw(CDC* /*pDC*/)
{
	COperationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// COperationView printing

void COperationView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL COperationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COperationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COperationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COperationView diagnostics

#ifdef _DEBUG
void COperationView::AssertValid() const
{
	CView::AssertValid();
}

void COperationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COperationDoc* COperationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationDoc)));
	return (COperationDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationView message handlers


void COperationView::OnSetupSource()
{
	// TODO: 在此添加命令处理程序代码
	int itest = 0;
}


void COperationView::OnSetupOperation()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupProcesstype()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupNoiseediting()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupComments()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupOptions()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupDelaysetup()
{
	// TODO: 在此添加命令处理程序代码
}


void COperationView::OnSetupPreferences()
{
	// TODO: 在此添加命令处理程序代码
}
