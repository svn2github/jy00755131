// ActiveSourceCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "ActiveSourceCtrl.h"



// CActiveSourceCtrl 对话框

IMPLEMENT_DYNCREATE(CActiveSourceCtrl, CBCGPFormView)
CActiveSourceCtrl::CActiveSourceCtrl()
	: CBCGPFormView(CActiveSourceCtrl::IDD)
{

}

CActiveSourceCtrl::~CActiveSourceCtrl()
{
}

void CActiveSourceCtrl::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CActiveSourceCtrl, CBCGPFormView)
	ON_BN_CLICKED(IDC_BUTTONGO, &CActiveSourceCtrl::OnBnClickedButtongo)
END_MESSAGE_MAP()


// CActiveSourceCtrl 消息处理程序

BOOL CActiveSourceCtrl::CreateView(CWnd* pParent, CRect rect, CCreateContext* pContext)
{
	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CActiveSourceCtrl!\n");
		return FALSE;
	}
	return TRUE;
}

void CActiveSourceCtrl::OnBnClickedButtongo()
{
	// TODO: 在此添加控件通知处理程序代码
}
