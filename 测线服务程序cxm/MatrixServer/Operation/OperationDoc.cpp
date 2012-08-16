// OperationDoc.cpp : implementation of the COperationDoc class
//

#include "stdafx.h"
#include "Operation.h"

#include "OperationDoc.h"
#include "..\MatrixCommDll\Parameter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationDoc

IMPLEMENT_DYNCREATE(COperationDoc, CDocument)

BEGIN_MESSAGE_MAP(COperationDoc, CDocument)
END_MESSAGE_MAP()


// COperationDoc construction/destruction

COperationDoc::COperationDoc()
{
	// TODO: add one-time construction code here

}

COperationDoc::~COperationDoc()
{
}

BOOL COperationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// COperationDoc serialization

void COperationDoc::Serialize(CArchive& ar)
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


// COperationDoc diagnostics

#ifdef _DEBUG
void COperationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COperationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COperationDoc commands


void COperationDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDocument::SetTitle(ClientTitle);
}
