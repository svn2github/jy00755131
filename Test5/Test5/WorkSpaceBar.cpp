// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "Test5.h"
#include "WorkSpaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*const int nBorderSize = 1;*/
char member1[] = "张路 40 北京 北京 群众 北京 13888888888";
char member2[] = "贺玲 22 天津 天津 群众 天津 13666666666";
char member3[] = "程显明 28 辽宁 天津 党员 天津 13777777777";
char* members[3] = {member1, member2, member3};

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
: m_pGridView(NULL)
{
	// TODO: add one-time construction code here

}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
// 	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
// 								TVS_LINESATROOT | TVS_HASBUTTONS;
// 	
// 	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
// 	{
// 		TRACE0("Failed to create workspace view\n");
// 		return -1;      // fail to create
// 	}
// 
// 	// Setup trees content:
// 	HTREEITEM hRoot1 = m_wndTree.InsertItem (_T("Root 1"));
// 	m_wndTree.InsertItem (_T("Item 1"), hRoot1);
// 	m_wndTree.InsertItem (_T("Item 2"), hRoot1);
	m_pGridView = new CBCGPGridCtrl;
	if(!m_pGridView)
		return -1;
	CRect rect(0,0,300,300);
	if (!m_pGridView->Create(WS_CHILD|WS_VISIBLE,rect,this,1))
	{
		TRACE0("Warning: couldn't create Grid View!\n");
		return FALSE;
	}
	BCGP_GRID_COLOR_DATA clrData;
	clrData = m_pGridView->GetColorTheme();
	clrData.m_clrBackground = ::GetSysColor(COLOR_3DFACE);
	clrData.m_EvenColors.m_clrBackground = RGB(255,192,203);	// 奇数行
	clrData.m_OddColors.m_clrBackground = RGB(175,238,238);		// 偶数行

	m_pGridView->SetColorTheme(clrData);	
	m_pGridView->SetReadOnly();	
	m_pGridView->SetSingleSel(TRUE);
	m_pGridView->SetWholeRowSel(TRUE);	
	m_pGridView->EnableHeader (TRUE, BCGP_GRID_HEADER_HIDE_ITEMS);
	m_pGridView->EnableDragSelection(FALSE);

	SetGridHead();
	LoadMembers(members);
	LoadState (_T("WorkSpaceBar"));

	return 0;
}

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
// 	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
// 		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
// 		SWP_NOACTIVATE | SWP_NOZORDER);
	if (m_pGridView->GetSafeHwnd ())
	{
		m_pGridView->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CWorkSpaceBar::OnDestroy() 
{
	SaveState (_T("WorkSpaceBar"));
	CBCGPDockingControlBar::OnDestroy();

	if(m_pGridView)
	{			
		m_pGridView->RemoveAll();		
		delete m_pGridView;
	}
	m_pGridView = NULL;

}

bool CWorkSpaceBar::SetGridHead()
{	if(m_pGridView==NULL)
	return false;

	m_pGridView->InsertColumn (0, _T(" 姓名 "), 80);
	m_pGridView->InsertColumn (1, _T(" 年龄 "), 80);
	m_pGridView->InsertColumn (2, _T(" 籍贯 "), 80);
	m_pGridView->InsertColumn (3, _T(" 出生地 "), 120);
	m_pGridView->InsertColumn (4, _T(" 政治面貌 "), 160);
	m_pGridView->InsertColumn (5, _T(" 家庭住址 "), 160);
	m_pGridView->InsertColumn (6, _T(" 联系电话 "), 160);

	// pGridCtrl->AddRow();

	return true;

}

void CWorkSpaceBar::LoadMembers(char* csmembers[])
{
	int membernum = sizeof(csmembers) - 1;
	for (int i=0; i<membernum; i++)
	{
		LoadMember(csmembers[i]);
	}

}

void CWorkSpaceBar::LoadMember(char csmember[])
{
	char name[256], age[256], homeplace[256], birthplace[256], political[256], homeaddr[256], telephonenum[256];
	sscanf(csmember, "%s%s%s%s%s%s%s", name, age, homeplace, birthplace, political, homeaddr, telephonenum);
	CBCGPGridRow* pRow = m_pGridView->CreateRow(m_pGridView->GetColumnCount());
	pRow->GetItem (0)->SetValue(name);
	pRow->GetItem (1)->SetValue (age);
	pRow->GetItem (2)->SetValue (homeplace);
	pRow->GetItem (3)->SetValue (birthplace);
	pRow->GetItem (4)->SetValue (political);
	pRow->GetItem (5)->SetValue (homeaddr);
	pRow->GetItem (6)->SetValue (telephonenum);
	m_pGridView->AddRow (pRow, FALSE );
}
