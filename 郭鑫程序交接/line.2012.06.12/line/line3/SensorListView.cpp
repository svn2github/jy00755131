// SensorListView.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "SensorListView.h"


// CSensorListView

IMPLEMENT_DYNCREATE(CSensorListView, CBCGPGridView)

CSensorListView::CSensorListView()
{

}

CSensorListView::~CSensorListView()
{

}

void CSensorListView::OnInitialUpdate() 
{
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);

	pDoc->SetTitle (_T("Sensor List View"));
}

BEGIN_MESSAGE_MAP(CSensorListView, CBCGPGridView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CSensorListView 绘图

void CSensorListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CSensorListView 诊断

#ifdef _DEBUG
void CSensorListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSensorListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CSensorListView 打印

void CSensorListView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}

void CSensorListView::OnFilePrint()
{
	CBCGPGridView::OnFilePrint();
}

BOOL CSensorListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return CBCGPGridView::OnPreparePrinting (pInfo);
}

void CSensorListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnBeginPrinting (pDC, pInfo);
}

void CSensorListView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridView::OnEndPrinting (pDC, pInfo);
}

// CSensorListView 消息处理程序

int CSensorListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pSensorListCtrl = new CSensorListCtrl;
	CRect rectClient;
	GetClientRect (rectClient);
	if (FALSE == m_pSensorListCtrl->Create (WS_CHILD, rectClient, this, 1))
	{
		return -1;
	}
	m_pSensorListCtrl->SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	m_pSensorListCtrl->ShowWindow (SW_SHOW);

	m_pWndGridCtrl = m_pSensorListCtrl;
	return 0;
}

void CSensorListView::OnDestroy() 
{
	if (m_pSensorListCtrl->GetSafeHwnd () != NULL)
	{
		m_pSensorListCtrl->DestroyWindow ();
		delete m_pSensorListCtrl;
	}

	m_pSensorListCtrl = NULL;
	m_pWndGridCtrl = NULL;

	CBCGPGridView::OnDestroy();
}

void CSensorListView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}

void CSensorListView::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPGridView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	if (m_pSensorListCtrl->GetSafeHwnd () != NULL)
	{
		m_pSensorListCtrl->SetFocus ();
	}
}

// 布局
void CSensorListView::AdjustLayout()
{
	if (m_pSensorListCtrl->GetSafeHwnd () == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect (rectClient);
	m_pSensorListCtrl->SetWindowPos (NULL, 0, 0, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

// 重新加载现场仪器数据
void CSensorListView::OnReloadSiteDataCMD()
{
	m_pSensorListCtrl->OnReloadSiteDataCMD();
}

// 为新的测试数据刷新显示
void CSensorListView::OnRefreshForNewTestValue(int iTestType)
{
	m_pSensorListCtrl->OnRefreshForNewTestValue(iTestType);
}