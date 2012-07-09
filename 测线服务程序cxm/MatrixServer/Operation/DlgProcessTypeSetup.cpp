// DlgProcessTypeSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgProcessTypeSetup.h"
#include "afxdialogex.h"


// CDlgProcessTypeSetup 对话框

IMPLEMENT_DYNAMIC(CDlgProcessTypeSetup, CBCGPDialog)

CDlgProcessTypeSetup::CDlgProcessTypeSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgProcessTypeSetup::IDD, pParent)
	, m_radioProcessSetup(PROCESS_STANDARD)
	, m_cbindexProcessType(PROCESS_IMPULSIVE)
{
//	EnableVisualManagerStyle();
}

CDlgProcessTypeSetup::~CDlgProcessTypeSetup()
{
}

void CDlgProcessTypeSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_radioProcessSetup);
	DDX_CBIndex(pDX, IDC_COMBO_PROCESSTYPESELECT, m_cbindexProcessType);
}


BEGIN_MESSAGE_MAP(CDlgProcessTypeSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, &CDlgProcessTypeSetup::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_ADVANCED, &CDlgProcessTypeSetup::OnBnClickedRadioAdvanced)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESSTYPESELECT, &CDlgProcessTypeSetup::OnCbnSelchangeComboProcesstypeselect)
END_MESSAGE_MAP()


// CDlgProcessTypeSetup 消息处理程序


void CDlgProcessTypeSetup::OnBnClickedRadioStandard()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioProcessSetup = PROCESS_STANDARD;
}


void CDlgProcessTypeSetup::OnBnClickedRadioAdvanced()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioProcessSetup = PROCESS_ADVANCE;
}


void CDlgProcessTypeSetup::OnCbnSelchangeComboProcesstypeselect()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新控件变量的值
	UpdateData ();
	switch(m_cbindexProcessType)
	{
	case PROCESS_IMPULSIVE:
		break;
	case PROCESS_STACKIMPULSIVE:
		break;
	case PROCESS_CORRELAFTER:
		break;
	case PROCESS_CORRELBEFORE:
		break;
	case PROCESS_STACK:
		break;
	}
	// 刷新界面
	Invalidate(TRUE);
}
