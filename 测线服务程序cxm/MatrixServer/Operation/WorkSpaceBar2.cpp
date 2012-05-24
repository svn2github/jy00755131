// workspace2.cpp : implementation of the CWorkSpaceBar2 class
//

#include "stdafx.h"
#include "Operation.h"
#include "WorkSpaceBar2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2

BEGIN_MESSAGE_MAP(CWorkSpaceBar2, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar2)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 construction/destruction

CWorkSpaceBar2::CWorkSpaceBar2()
{
	// TODO: add one-time construction code here
}

CWorkSpaceBar2::~CWorkSpaceBar2()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 message handlers

int CWorkSpaceBar2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();
	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup trees content:
	HTREEITEM hRoot1 = m_wndTree.InsertItem (_T("Root 2"));
	m_wndTree.InsertItem (_T("Item 1"), hRoot1);
	m_wndTree.InsertItem (_T("Item 2"), hRoot1);

	return 0;
}

void CWorkSpaceBar2::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}
