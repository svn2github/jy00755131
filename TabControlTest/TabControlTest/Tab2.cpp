// Tab2.cpp : 实现文件
//

#include "stdafx.h"
#include "TabControlTest.h"
#include "Tab2.h"


// CTab2 对话框

IMPLEMENT_DYNAMIC(CTab2, CDialog)

CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CTab2::IDD, pParent)
{

}

CTab2::~CTab2()
{
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab2, CDialog)
END_MESSAGE_MAP()


// CTab2 消息处理程序
