// Ex1_04Doc.cpp : implementation of the CEx1_04Doc class
//

#include "stdafx.h"
#include "Ex1_04.h"

#include "Ex1_04Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEx1_04Doc

IMPLEMENT_DYNCREATE(CEx1_04Doc, CDocument)

BEGIN_MESSAGE_MAP(CEx1_04Doc, CDocument)
END_MESSAGE_MAP()


// CEx1_04Doc construction/destruction

CEx1_04Doc::CEx1_04Doc()
{
	// TODO: add one-time construction code here

}

CEx1_04Doc::~CEx1_04Doc()
{
}

BOOL CEx1_04Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CEx1_04Doc serialization

void CEx1_04Doc::Serialize(CArchive& ar)
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


// CEx1_04Doc diagnostics

#ifdef _DEBUG
void CEx1_04Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEx1_04Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEx1_04Doc commands
