// Tab4.cpp : 实现文件
//

#include "stdafx.h"
#include "TabControlTest.h"
#include "Tab4.h"


// CTab4 对话框

IMPLEMENT_DYNAMIC(CTab4, CDialog)

CTab4::CTab4(CWnd* pParent /*=NULL*/)
	: CDialog(CTab4::IDD, pParent)
{

}

CTab4::~CTab4()
{
}

void CTab4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab4, CDialog)
END_MESSAGE_MAP()


// CTab4 消息处理程序
