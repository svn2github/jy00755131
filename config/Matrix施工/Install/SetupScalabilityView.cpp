// SetupScalabilityView.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "SetupScalabilityView.h"


// CSetupScalabilityView

IMPLEMENT_DYNCREATE(CSetupScalabilityView, CBCGPDialog)

CSetupScalabilityView::CSetupScalabilityView()
	: CBCGPDialog(CSetupScalabilityView::IDD)
{
	EnableVisualManagerStyle();
}

CSetupScalabilityView::~CSetupScalabilityView()
{
}

void CSetupScalabilityView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetupScalabilityView, CBCGPDialog)
END_MESSAGE_MAP()


// CSetupScalabilityView 诊断
/*
#ifdef _DEBUG
void CSetupScalabilityView::AssertValid() const
{
	CBCGPDialog::AssertValid();
}

#ifndef _WIN32_WCE
void CSetupScalabilityView::Dump(CDumpContext& dc) const
{
	CBCGPDialog::Dump(dc);
}
#endif
#endif //_DEBUG
*/

// CSetupScalabilityView 消息处理程序
