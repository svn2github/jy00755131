// AdminDoc.cpp : CAdminDoc 类的实现
//

#include "stdafx.h"
#include "Admin.h"

#include "AdminDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAdminDoc

IMPLEMENT_DYNCREATE(CAdminDoc, CDocument)

BEGIN_MESSAGE_MAP(CAdminDoc, CDocument)
END_MESSAGE_MAP()


// CAdminDoc 构造/析构

CAdminDoc::CAdminDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAdminDoc::~CAdminDoc()
{
}

BOOL CAdminDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	SetTitle(_T(""));
	return TRUE;
}




// CAdminDoc 序列化

void CAdminDoc::Serialize(CArchive& ar)
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


// CAdminDoc 诊断

#ifdef _DEBUG
void CAdminDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdminDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAdminDoc 命令
