// ConfigView.cpp : implementation of the CConfigView class
//

#include "stdafx.h"
#include "Config.h"

#include "ConfigDoc.h"
#include "ConfigView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConfigView

IMPLEMENT_DYNCREATE(CConfigView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CConfigView, CBCGPFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CConfigView construction/destruction

CConfigView::CConfigView()
: CBCGPFormView(CConfigView::IDD)
{
	// TODO: add construction code here
	EnableVisualManagerStyle ();
}

CConfigView::~CConfigView()
{
}

void CConfigView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICACTIVITY, m_ctrlActivity);
}

BOOL CConfigView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPFormView::PreCreateWindow(cs);
}

void CConfigView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// SetBackgroundImage()
}


// CConfigView diagnostics

#ifdef _DEBUG
void CConfigView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CConfigView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CConfigDoc* CConfigView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CConfigDoc)));
	return (CConfigDoc*)m_pDocument;
}
#endif //_DEBUG


// CConfigView message handlers

void CConfigView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// m_bCompactMode = !m_bCompactMode;
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
		pFrm->ShowCompactMode();
	CBCGPFormView::OnLButtonDown(nFlags, point);
}

void CConfigView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	if (m_ctrlActivity.GetSafeHwnd())
	{
		m_ctrlActivity.SetWindowPos (NULL, 1, 1, cx-10, cy-10, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}