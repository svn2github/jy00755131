// Dlgtemp.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "Dlgtemp.h"


// CDlgtemp 对话框

IMPLEMENT_DYNAMIC(CDlgtemp, CDialog)

CDlgtemp::CDlgtemp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgtemp::IDD, pParent)
{

}

CDlgtemp::~CDlgtemp()
{
}

void CDlgtemp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgtemp, CDialog)
END_MESSAGE_MAP()


// CDlgtemp 消息处理程序
