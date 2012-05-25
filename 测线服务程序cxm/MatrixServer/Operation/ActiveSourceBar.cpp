// ActiveSourceBar.cpp : implementation of the CActiveSourceBar class
//

#include "stdafx.h"
#include "Operation.h"
#include "ActiveSourceBar.h"
#include "ActiveSourceCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveSourceBar

BEGIN_MESSAGE_MAP(CActiveSourceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CActiveSourceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveSourceBar construction/destruction

CActiveSourceBar::CActiveSourceBar()
{
	// TODO: add one-time construction code here
	CRuntimeClass* pFactory = RUNTIME_CLASS(CActiveSourceCtrl);
	m_pActiveSourceCtrl = (CActiveSourceCtrl *)(pFactory->CreateObject());
}

CActiveSourceBar::~CActiveSourceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CActiveSourceBar message handlers

int CActiveSourceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectClient;
	CRect rectGrid;
	CRect rectDlg;
	GetClientRect(rectClient);
	rectGrid = rectClient;
	rectDlg = rectClient;
	rectGrid.bottom -= rectClient.bottom / 3;
	rectDlg.top = rectGrid.bottom;
	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;

	if (!m_GridView.Create(dwViewStyle, rectGrid, this, 1))
	{
		TRACE0("Failed to create Grid View!\n");
		return -1;
	}

	BCGP_GRID_COLOR_DATA clrData;
	clrData = m_GridView.GetColorTheme();
	clrData.m_clrBackground = ::GetSysColor(COLOR_3DFACE);
	clrData.m_EvenColors.m_clrBackground = RGB(255,255,255);	// 奇数行
	clrData.m_OddColors.m_clrBackground = RGB(250,250,250);		// 偶数行

	m_GridView.SetColorTheme(clrData);	
	m_GridView.SetReadOnly();	
	m_GridView.SetSingleSel(TRUE);
	m_GridView.SetWholeRowSel(TRUE);	
	m_GridView.EnableHeader (TRUE, BCGP_GRID_HEADER_HIDE_ITEMS);
	m_GridView.EnableDragSelection(FALSE);


	m_GridView.InsertColumn (0, _T(""), 50);
	m_GridView.InsertColumn (1, _T("Shot#"), 80);
	m_GridView.InsertColumn (2, _T("Breakpoint"), 150);
	m_GridView.InsertColumn (3, _T("SourcePointLine"), 150);
	m_GridView.InsertColumn (4, _T("SourcePointNb"), 150);
	m_GridView.InsertColumn (5, _T("SourcePointIndex"), 150);
	m_GridView.InsertColumn (6, _T("Sfn"), 100);
	m_GridView.InsertColumn (7, _T("Sfl"), 100);
	m_GridView.InsertColumn (8, _T("Spread#"), 100);
	m_GridView.InsertColumn (9, _T("SuperSpread#"), 100);
	m_GridView.InsertColumn (10, _T("ProcessType"), 100);
	m_GridView.InsertColumn (11, _T("Comment"), 100);
	m_GridView.InsertColumn (12, _T("Swath"), 100);

	for (int i=0; i<40; i++)
	{
		CBCGPGridRow* pRow = m_GridView.CreateRow (13);
		// pRow->GetItem (0)->SetValue (pVP->m_VPState);
		pRow->GetItem (0)->SetValue(1);
		pRow->GetItem (1)->SetValue (1);
		pRow->GetItem (2)->SetValue (1);
		pRow->GetItem (3)->SetValue (1);
		pRow->GetItem (4)->SetValue (1);

		pRow->GetItem (5)->SetValue (1);
		pRow->GetItem (6)->SetValue (1);
		pRow->GetItem (7)->SetValue (1);

		pRow->GetItem (8)->SetValue (1);
		pRow->GetItem (9)->SetValue (1);
		pRow->GetItem (10)->SetValue (1);
		pRow->GetItem (11)->SetValue (1);
		pRow->GetItem (12)->SetValue (2);
		m_GridView.AddRow (pRow, FALSE );
	}

	if (m_pActiveSourceCtrl != NULL)
	{
		m_pActiveSourceCtrl->CreateView(this, rectDlg, NULL);
	}
	return 0;
}

void CActiveSourceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	int iBottom = 0;
	if (m_GridView.GetSafeHwnd ())
	{
		iBottom = cy - cy / 3;
		m_GridView.SetWindowPos (NULL, 0, 0, cx, iBottom, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_pActiveSourceCtrl->GetSafeHwnd())
	{
		m_pActiveSourceCtrl->SetWindowPos(NULL, 0, iBottom, cx, cy - iBottom, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

// void CActiveSourceBar::OnPaint() 
// {
// 	CPaintDC dc(this); // device context for painting
// 	
// 	CRect rectTree;
// 	m_wndTree.GetWindowRect (rectTree);
// 	ScreenToClient (rectTree);
// 
// 	rectTree.InflateRect (nBorderSize, nBorderSize);
// 	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
// 								::GetSysColor (COLOR_3DSHADOW));
// }


// 重载右键菜单的WM_CONTEXTMENU消息响应函数
void CActiveSourceBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}


void CActiveSourceBar::OnDestroy()
{
	CBCGPDockingControlBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_GridView.RemoveAll();
}
