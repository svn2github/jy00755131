// ConfigDoc.cpp : implementation of the CConfigDoc class
//

#include "stdafx.h"
#include "Config.h"

#include "ConfigDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConfigDoc

IMPLEMENT_DYNCREATE(CConfigDoc, CDocument)

BEGIN_MESSAGE_MAP(CConfigDoc, CDocument)
END_MESSAGE_MAP()


// CConfigDoc construction/destruction

CConfigDoc::CConfigDoc()
{
	// TODO: add one-time construction code here

}

CConfigDoc::~CConfigDoc()
{
}

BOOL CConfigDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle(_T(""));
	return TRUE;
}




// CConfigDoc serialization

void CConfigDoc::Serialize(CArchive& ar)
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


// CConfigDoc diagnostics

#ifdef _DEBUG
void CConfigDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConfigDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CConfigDoc commands
