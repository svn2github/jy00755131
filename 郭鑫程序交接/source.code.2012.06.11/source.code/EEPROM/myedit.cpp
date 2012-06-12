#include "stdafx.h"
#include "E2PROM_TOOL.h"

#include "myedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(my_data_control, CEdit)

BEGIN_MESSAGE_MAP(my_data_control, CEdit)
	//{{AFX_MSG_MAP(my_data_control)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

my_data_control::my_data_control()
{
	// TODO: add member initialization code here
	
}

my_data_control::~my_data_control()
{
}

/////////////////////////////////////////////////////////////////////////////
// my_data_control diagnostics

#ifdef _DEBUG
void my_data_control::AssertValid() const
{
	my_data_control::AssertValid();
}

void my_data_control::Dump(CDumpContext& dc) const
{
	my_data_control::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
