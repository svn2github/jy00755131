// InstrumentListView.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "InstrumentListView.h"

// CInstrumentListView

IMPLEMENT_DYNCREATE(CInstrumentListView, CBCGPGridView)

CInstrumentListView::CInstrumentListView()
{

}

CInstrumentListView::~CInstrumentListView()
{

}

void CInstrumentListView::OnInitialUpdate() 
{
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);

	pDoc->SetTitle (_T("Instrument List View"));
}

BEGIN_MESSAGE_MAP(CInstrumentListView, CBCGPGridView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()	
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CInstrumentListView 绘图

void CInstrumentListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CInstrumentListView 诊断

#ifdef _DEBUG
void CInstrumentListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CInstrumentListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CInstrumentListView 打印

void CInstrumentListView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}

void CInstrumentListView::OnFilePrint()
{
	CBCGPGridView::OnFilePrint();
}

BOOL CInstrumentListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return CBCGPGridView::OnPreparePrinting (pInfo);
}

void CInstrumentListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnBeginPrinting (pDC, pInfo);
}

void CInstrumentListView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnEndPrinting (pDC, pInfo);
}

// CInstrumentListView 消息处理程序

int CInstrumentListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pInstrumentListCtrl = new CInstrumentListCtrl;
	CRect rectClient;
	GetClientRect (rectClient);
	if (FALSE == m_pInstrumentListCtrl->Create (WS_CHILD, rectClient, this, 1))
	{
		return -1;
	}
	m_pInstrumentListCtrl->SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	m_pInstrumentListCtrl->ShowWindow (SW_SHOW);

	m_pWndGridCtrl = m_pInstrumentListCtrl;
	return 0;
}

void CInstrumentListView::OnDestroy() 
{
	if (m_pInstrumentListCtrl->GetSafeHwnd () != NULL)
	{
		m_pInstrumentListCtrl->DestroyWindow ();
		delete m_pInstrumentListCtrl;
	}

	m_pInstrumentListCtrl = NULL;
	m_pWndGridCtrl = NULL;

	CBCGPGridView::OnDestroy();
}

void CInstrumentListView::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPGridView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	if (m_pInstrumentListCtrl->GetSafeHwnd () != NULL)
	{
		m_pInstrumentListCtrl->SetFocus ();
	}
}

void CInstrumentListView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}

// 布局
void CInstrumentListView::AdjustLayout()
{
	if (m_pInstrumentListCtrl->GetSafeHwnd () == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect (rectClient);
	m_pInstrumentListCtrl->SetWindowPos (NULL, 0, 0, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

// 重新加载现场仪器数据
void CInstrumentListView::OnReloadSiteDataCMD()
{
	m_pInstrumentListCtrl->OnReloadSiteDataCMD();
}

// 为新的测试数据刷新显示
void CInstrumentListView::OnRefreshForNewTestValue(int iTestType)
{
	m_pInstrumentListCtrl->OnRefreshForNewTestValue(iTestType);
}