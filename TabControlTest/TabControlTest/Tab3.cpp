// Tab3.cpp : 实现文件
//

#include "stdafx.h"
#include "TabControlTest.h"
#include "Tab3.h"


// CTab3 对话框

IMPLEMENT_DYNAMIC(CTab3, CDialog)

CTab3::CTab3(CWnd* pParent /*=NULL*/)
	: CDialog(CTab3::IDD, pParent)
{

}

CTab3::~CTab3()
{
}

void CTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab3, CDialog)
END_MESSAGE_MAP()


// CTab3 消息处理程序
