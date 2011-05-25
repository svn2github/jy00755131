// Tab5.cpp : 实现文件
//

#include "stdafx.h"
#include "TabControlTest.h"
#include "Tab5.h"


// CTab5 对话框

IMPLEMENT_DYNAMIC(CTab5, CDialog)

CTab5::CTab5(CWnd* pParent /*=NULL*/)
	: CDialog(CTab5::IDD, pParent)
{

}

CTab5::~CTab5()
{
}

void CTab5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab5, CDialog)
END_MESSAGE_MAP()


// CTab5 消息处理程序