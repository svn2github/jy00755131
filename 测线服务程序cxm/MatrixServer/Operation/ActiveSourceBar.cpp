// ActiveSourceBar.cpp : implementation of the CActiveSourceBar class
//

#include "stdafx.h"
#include "Operation.h"
#include "ActiveSourceBar.h"
#include "ActiveSourceCtrl.h"
#include "ButtonItem.h"
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
	ON_COMMAND(ID_ACTIVESOURCE_DISASSOCIATEVP, &CActiveSourceBar::OnActivesourceDisassociatevp)
	ON_COMMAND(ID_ACTIVESOURCE_ASSOCIATEVP, &CActiveSourceBar::OnActivesourceAssociatevp)
	ON_COMMAND(ID_ACTIVESOURCE_ASSOCIATESPLSPN, &CActiveSourceBar::OnActivesourceAssociatesplspn)
	ON_COMMAND(ID_ACTIVESOURCE_CLEANREADY, &CActiveSourceBar::OnActivesourceCleanready)
	ON_COMMAND_RANGE(IDC_ACTIVESOURCE_BUTTON, IDC_ACTIVESOURCE_BUTTON + ActiveSourceNumLimit - 1, &CActiveSourceBar::OnProcRdyBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveSourceBar construction/destruction

CActiveSourceBar::CActiveSourceBar()
{
	// TODO: add one-time construction code here
	CRuntimeClass* pFactory = RUNTIME_CLASS(CActiveSourceCtrl);
	m_pActiveSourceCtrl = (CActiveSourceCtrl *)(pFactory->CreateObject());
	for (int i=0; i<ActiveSourceNumLimit; i++)
	{
		m_iActiveSourceBtnID[i] = IDC_ACTIVESOURCE_BUTTON + i;
	}
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

	// 设置表头
	SetGridHead();
	m_ImageList.Create (IDB_BITMAP_ACTIVESOURCE, 16, 0, RGB (255, 0, 255));
	m_GridView.SetImageList (&m_ImageList);
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
	iBottom = cy - cy / 3;
	if (m_GridView.GetSafeHwnd ())
	{
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
void CActiveSourceBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	// 如果没有选择震源或者网格中某一行，则不弹出菜单
	CBCGPGridRow* pRow= m_GridView.GetCurSel();
	if(NULL==pRow)
		return;
	CMenu menu;
	// 加载右击菜单
	VERIFY(menu.LoadMenu (IDR_POPUP_ACTIVESOURCE));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);	
	CString  strTemp;
	// 每一行的数据等于震源的编号
	strTemp.Format(_T("Source %d"),pRow->GetData());

	// 更改第一个菜单的标题，显示为“Source”+震源编号
	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO);		// must fill up this field
	info.fMask = MIIM_STRING;					// get the state of the menu item
	info.dwTypeData = strTemp.GetBuffer();
	info.cch = strTemp.GetLength();
	VERIFY(pPopup->SetMenuItemInfo(ID_ACTIVESOURCE_SOURCE, &info,FALSE));
	strTemp.ReleaseBuffer();
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create(this, point.x, point.y,pPopup->Detach(),FALSE,TRUE);
}


void CActiveSourceBar::OnDestroy()
{
	CBCGPDockingControlBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_GridView.RemoveAll();
}

// 设置表头
bool CActiveSourceBar::SetGridHead(void)
{
	if(!m_GridView.GetSafeHwnd())		
		return false;
	m_GridView.InsertColumn (0, _T(""), 40);
	m_GridView.InsertColumn (1, _T("Rdy"), 40);
	m_GridView.InsertColumn (2, _T("Rdy VP"), 60);
	m_GridView.InsertColumn (3, _T("Source Name"), 120);
	m_GridView.InsertColumn (4, _T("Source Index"), 120);
	m_GridView.InsertColumn (5, _T("Shot#"), 80);
	m_GridView.InsertColumn (6, _T("Dist min"), 80);
	m_GridView.InsertColumn (7, _T("Current Stack"), 120);
	m_GridView.InsertColumn (8, _T("SourcePoint Line"), 150);
	m_GridView.InsertColumn (9, _T("SourcePoint Number"), 160);
	m_GridView.InsertColumn (10, _T("SourcePoint Index"), 160);
	m_GridView.InsertColumn (11, _T("Sfl"), 60);
	m_GridView.InsertColumn (12, _T("Sfn"), 60);
	m_GridView.InsertColumn (13, _T("Spread#"), 80);
	m_GridView.InsertColumn (14, _T("SuperSpread"), 100);
	m_GridView.InsertColumn (15, _T("ProcessType"), 100);
	m_GridView.InsertColumn (16, _T("Swath"), 60);
	return true;
}

// 载入全部震源信息
void CActiveSourceBar::LoadActiveSources(void)
{
	m_GridView.RemoveAll();
	for (int i =0; i<5; i++)
	{
		LoadActiveSource(i);
	}
}


// 载入单条震源信息
void CActiveSourceBar::LoadActiveSource(int iNb)
{
	CBCGPGridRow* pRow = m_GridView.CreateRow (17);
	pRow->GetItem (0)->SetImage(1);
	pRow->ReplaceItem (1, new CButtonItem (_T(""), m_iActiveSourceBtnID[iNb]));
	if (iNb == 4)
	{
		pRow->GetItem(1)->SetValue(_T("R"));
	}
	pRow->GetItem (2)->SetValue (_T("-"));
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
	pRow->SetData(1);
}

void CActiveSourceBar::OnActivesourceDisassociatevp()
{
	// TODO: 在此添加命令处理程序代码
}


void CActiveSourceBar::OnActivesourceAssociatevp()
{
	// TODO: 在此添加命令处理程序代码
}


void CActiveSourceBar::OnActivesourceAssociatesplspn()
{
	// TODO: 在此添加命令处理程序代码
}


void CActiveSourceBar::OnActivesourceCleanready()
{
	// TODO: 在此添加命令处理程序代码
}

void CActiveSourceBar::OnProcRdyBtn(UINT id)
{
	int itest = 0;
}