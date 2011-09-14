// SensorGraphDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "SensorGraphDoc.h"


// CSensorGraphDoc

IMPLEMENT_DYNCREATE(CSensorGraphDoc, CDocument)

CSensorGraphDoc::CSensorGraphDoc()
{
}

BOOL CSensorGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSensorGraphDoc::~CSensorGraphDoc()
{
}


BEGIN_MESSAGE_MAP(CSensorGraphDoc, CDocument)
END_MESSAGE_MAP()


// CSensorGraphDoc 诊断

#ifdef _DEBUG
void CSensorGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSensorGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSensorGraphDoc 序列化

void CSensorGraphDoc::Serialize(CArchive& ar)
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


// CSensorGraphDoc 命令
