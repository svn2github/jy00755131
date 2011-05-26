// Test3_MakeApplicationDoc.cpp : implementation of the CTest3_MakeApplicationDoc class
//

#include "stdafx.h"
#include "Test3_MakeApplication.h"

#include "Test3_MakeApplicationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest3_MakeApplicationDoc

IMPLEMENT_DYNCREATE(CTest3_MakeApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CTest3_MakeApplicationDoc, CDocument)
END_MESSAGE_MAP()


// CTest3_MakeApplicationDoc construction/destruction

CTest3_MakeApplicationDoc::CTest3_MakeApplicationDoc()
{
	// TODO: add one-time construction code here

}

CTest3_MakeApplicationDoc::~CTest3_MakeApplicationDoc()
{
}

BOOL CTest3_MakeApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTest3_MakeApplicationDoc serialization

void CTest3_MakeApplicationDoc::Serialize(CArchive& ar)
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


// CTest3_MakeApplicationDoc diagnostics

#ifdef _DEBUG
void CTest3_MakeApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTest3_MakeApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTest3_MakeApplicationDoc commands
