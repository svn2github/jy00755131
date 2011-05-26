// TEST1_MakeApplicationDoc.cpp : implementation of the CTEST1_MakeApplicationDoc class
//

#include "stdafx.h"
#include "TEST1_MakeApplication.h"

#include "TEST1_MakeApplicationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTEST1_MakeApplicationDoc

IMPLEMENT_DYNCREATE(CTEST1_MakeApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CTEST1_MakeApplicationDoc, CDocument)
END_MESSAGE_MAP()


// CTEST1_MakeApplicationDoc construction/destruction

CTEST1_MakeApplicationDoc::CTEST1_MakeApplicationDoc()
{
	// TODO: add one-time construction code here

}

CTEST1_MakeApplicationDoc::~CTEST1_MakeApplicationDoc()
{
}

BOOL CTEST1_MakeApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTEST1_MakeApplicationDoc serialization

void CTEST1_MakeApplicationDoc::Serialize(CArchive& ar)
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


// CTEST1_MakeApplicationDoc diagnostics

#ifdef _DEBUG
void CTEST1_MakeApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTEST1_MakeApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTEST1_MakeApplicationDoc commands
