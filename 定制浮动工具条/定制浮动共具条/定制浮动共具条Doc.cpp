// 定制浮动共具条Doc.cpp : C定制浮动共具条Doc 类的实现
//

#include "stdafx.h"
#include "定制浮动共具条.h"

#include "定制浮动共具条Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C定制浮动共具条Doc

IMPLEMENT_DYNCREATE(C定制浮动共具条Doc, CDocument)

BEGIN_MESSAGE_MAP(C定制浮动共具条Doc, CDocument)
END_MESSAGE_MAP()


// C定制浮动共具条Doc 构造/析构

C定制浮动共具条Doc::C定制浮动共具条Doc()
{
	// TODO: 在此添加一次性构造代码

}

C定制浮动共具条Doc::~C定制浮动共具条Doc()
{
}

BOOL C定制浮动共具条Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// C定制浮动共具条Doc 序列化

void C定制浮动共具条Doc::Serialize(CArchive& ar)
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


// C定制浮动共具条Doc 诊断

#ifdef _DEBUG
void C定制浮动共具条Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void C定制浮动共具条Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// C定制浮动共具条Doc 命令
