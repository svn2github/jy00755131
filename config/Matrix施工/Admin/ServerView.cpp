// ServerView.cpp : 实现文件
//

#include "stdafx.h"
#include "Admin.h"
#include "ServerView.h"


// CServerView

IMPLEMENT_DYNCREATE(CServerView, CBCGPFormView)

CServerView::CServerView()
	: CBCGPFormView(CServerView::IDD)
{
	EnableVisualManagerStyle();
}

CServerView::~CServerView()
{
}

void CServerView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerView, CBCGPFormView)
END_MESSAGE_MAP()


// CServerView 诊断

#ifdef _DEBUG
void CServerView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CServerView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CServerView 消息处理程序
