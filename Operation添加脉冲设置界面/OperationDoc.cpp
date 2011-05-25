// OperationDoc.cpp : COperationDoc 类的实现
//

#include "stdafx.h"
#include "Operation.h"

#include "OperationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationDoc

IMPLEMENT_DYNCREATE(COperationDoc, CDocument)

BEGIN_MESSAGE_MAP(COperationDoc, CDocument)
END_MESSAGE_MAP()


// COperationDoc 构造/析构

COperationDoc::COperationDoc()
{
	// TODO: 在此添加一次性构造代码

}

COperationDoc::~COperationDoc()
{
}

BOOL COperationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// COperationDoc 序列化

void COperationDoc::Serialize(CArchive& ar)
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


// COperationDoc 诊断

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


// COperationDoc 命令
