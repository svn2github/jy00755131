// SensorGraphDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "SeismonitorGraphDoc.h"


// CSeismonitorGraphDoc

IMPLEMENT_DYNCREATE(CSeismonitorGraphDoc, CDocument)

CSeismonitorGraphDoc::CSeismonitorGraphDoc()
{
}

BOOL CSeismonitorGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSeismonitorGraphDoc::~CSeismonitorGraphDoc()
{
}


BEGIN_MESSAGE_MAP(CSeismonitorGraphDoc, CDocument)
END_MESSAGE_MAP()


// CSeismonitorGraphDoc 诊断

#ifdef _DEBUG
void CSeismonitorGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSeismonitorGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSeismonitorGraphDoc 序列化

void CSeismonitorGraphDoc::Serialize(CArchive& ar)
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


// CSeismonitorGraphDoc 命令
