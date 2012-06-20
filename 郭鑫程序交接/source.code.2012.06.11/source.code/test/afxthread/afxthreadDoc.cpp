// afxthreadDoc.cpp : implementation of the CafxthreadDoc class
//

#include "stdafx.h"
#include "afxthread.h"

#include "afxthreadDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CafxthreadDoc

IMPLEMENT_DYNCREATE(CafxthreadDoc, CDocument)

BEGIN_MESSAGE_MAP(CafxthreadDoc, CDocument)
END_MESSAGE_MAP()


// CafxthreadDoc construction/destruction

CafxthreadDoc::CafxthreadDoc()
{
	// TODO: add one-time construction code here

}

CafxthreadDoc::~CafxthreadDoc()
{
}

BOOL CafxthreadDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CafxthreadDoc serialization

void CafxthreadDoc::Serialize(CArchive& ar)
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


// CafxthreadDoc diagnostics

#ifdef _DEBUG
void CafxthreadDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CafxthreadDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CafxthreadDoc commands
