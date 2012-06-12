// InstrumentListDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "InstrumentListDoc.h"


// CInstrumentListDoc

IMPLEMENT_DYNCREATE(CInstrumentListDoc, CDocument)

CInstrumentListDoc::CInstrumentListDoc()
{
}

BOOL CInstrumentListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CInstrumentListDoc::~CInstrumentListDoc()
{
}


BEGIN_MESSAGE_MAP(CInstrumentListDoc, CDocument)
END_MESSAGE_MAP()


// CInstrumentListDoc 诊断

#ifdef _DEBUG
void CInstrumentListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CInstrumentListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CInstrumentListDoc 序列化

void CInstrumentListDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CInstrumentListDoc 命令
