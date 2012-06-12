// E2PROM_TOOLDoc.cpp : implementation of the CE2PROM_TOOLDoc class
//

#include "stdafx.h"
#include "E2PROM_TOOL.h"

#include "E2PROM_TOOLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLDoc

IMPLEMENT_DYNCREATE(CE2PROM_TOOLDoc, CDocument)

BEGIN_MESSAGE_MAP(CE2PROM_TOOLDoc, CDocument)
	//{{AFX_MSG_MAP(CE2PROM_TOOLDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLDoc construction/destruction

CE2PROM_TOOLDoc::CE2PROM_TOOLDoc()
{
	// TODO: add one-time construction code here

	for(int i = 0; i < MAX_PAGE_SIZE; ++i){
		for(int j = 0; j < MAX_NUM_PERPAGE; ++j){
			
			m_data_[i][j].address_ = i * MAX_NUM_PERPAGE + j;
			m_data_[i][j].data_ = 0XFF;
		}
	}

	m_is_progressing_ = false;
}

CE2PROM_TOOLDoc::~CE2PROM_TOOLDoc()
{
}

BOOL CE2PROM_TOOLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLDoc serialization

void CE2PROM_TOOLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLDoc diagnostics

#ifdef _DEBUG
void CE2PROM_TOOLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CE2PROM_TOOLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLDoc commands
