// Test2Doc.cpp : implementation of the CTest2Doc class
//

#include "stdafx.h"
#include "Test2.h"

#include "Test2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest2Doc

IMPLEMENT_DYNCREATE(CTest2Doc, CDocument)

BEGIN_MESSAGE_MAP(CTest2Doc, CDocument)
END_MESSAGE_MAP()


// CTest2Doc construction/destruction

CTest2Doc::CTest2Doc()
{
	// TODO: add one-time construction code here

}

CTest2Doc::~CTest2Doc()
{
}

BOOL CTest2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTest2Doc serialization

void CTest2Doc::Serialize(CArchive& ar)
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


// CTest2Doc diagnostics

#ifdef _DEBUG
void CTest2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTest2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTest2Doc commands
