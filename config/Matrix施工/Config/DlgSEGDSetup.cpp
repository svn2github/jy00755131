// DlgSEGDSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgSEGDSetup.h"


// CDlgSEGDSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSEGDSetup, CBCGPDialog)

CDlgSEGDSetup::CDlgSEGDSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSEGDSetup::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgSEGDSetup::~CDlgSEGDSetup()
{
}

void CDlgSEGDSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSEGDSetup, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgSEGDSetup 消息处理程序
