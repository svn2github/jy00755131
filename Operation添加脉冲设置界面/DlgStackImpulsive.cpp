// DlgStackImpulsive.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgStackImpulsive.h"


// CDlgStackImpulsive 对话框

IMPLEMENT_DYNAMIC(CDlgStackImpulsive, CBCGPDialog)

CDlgStackImpulsive::CDlgStackImpulsive(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgStackImpulsive::IDD, pParent)
{

}

CDlgStackImpulsive::~CDlgStackImpulsive()
{
}

void CDlgStackImpulsive::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStackImpulsive, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgStackImpulsive 消息处理程序

BOOL CDlgStackImpulsive::OnInitDialog(void)
{
	CBCGPDialog::OnInitDialog();

	//设置对话框尺寸为IDC_STATICPICTURE的尺寸
	CWnd::SetWindowPos(NULL,0,0,dlgRect.Width(),dlgRect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	return 0;
}
