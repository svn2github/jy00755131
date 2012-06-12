// SensorListDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "SensorListDoc.h"


// CSensorListDoc

IMPLEMENT_DYNCREATE(CSensorListDoc, CDocument)

CSensorListDoc::CSensorListDoc()
{
}

BOOL CSensorListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSensorListDoc::~CSensorListDoc()
{
}


BEGIN_MESSAGE_MAP(CSensorListDoc, CDocument)
END_MESSAGE_MAP()


// CSensorListDoc 诊断

#ifdef _DEBUG
void CSensorListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSensorListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSensorListDoc 序列化

void CSensorListDoc::Serialize(CArchive& ar)
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


// CSensorListDoc 命令
