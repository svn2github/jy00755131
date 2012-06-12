// InstrumentGraphDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "InstrumentGraphDoc.h"


// CInstrumentGraphDoc

IMPLEMENT_DYNCREATE(CInstrumentGraphDoc, CDocument)

CInstrumentGraphDoc::CInstrumentGraphDoc()
{
}

BOOL CInstrumentGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CInstrumentGraphDoc::~CInstrumentGraphDoc()
{
}


BEGIN_MESSAGE_MAP(CInstrumentGraphDoc, CDocument)
END_MESSAGE_MAP()


// CInstrumentGraphDoc 诊断

#ifdef _DEBUG
void CInstrumentGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CInstrumentGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CInstrumentGraphDoc 序列化

void CInstrumentGraphDoc::Serialize(CArchive& ar)
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


// CInstrumentGraphDoc 命令
