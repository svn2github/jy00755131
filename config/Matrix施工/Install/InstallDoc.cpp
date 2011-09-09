// InstallDoc.cpp : CInstallDoc 类的实现
//

#include "stdafx.h"
#include "Install.h"

#include "InstallDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInstallDoc

IMPLEMENT_DYNCREATE(CInstallDoc, CDocument)

BEGIN_MESSAGE_MAP(CInstallDoc, CDocument)
END_MESSAGE_MAP()


// CInstallDoc 构造/析构

CInstallDoc::CInstallDoc()
{
	// TODO: 在此添加一次性构造代码

}

CInstallDoc::~CInstallDoc()
{
}

BOOL CInstallDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	SetTitle(_T(""));
	//SetTitle(NULL);
	return TRUE;
}




// CInstallDoc 序列化

void CInstallDoc::Serialize(CArchive& ar)
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


// CInstallDoc 诊断

#ifdef _DEBUG
void CInstallDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInstallDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInstallDoc 命令
