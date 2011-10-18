// TabEepromUpdata.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TabEepromUpdata.h"


// CTabEepromUpdata 对话框

IMPLEMENT_DYNAMIC(CTabEepromUpdata, CDialog)

CTabEepromUpdata::CTabEepromUpdata(CWnd* pParent /*=NULL*/)
	: CDialog(CTabEepromUpdata::IDD, pParent)
{

}

CTabEepromUpdata::~CTabEepromUpdata()
{
}

void CTabEepromUpdata::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabEepromUpdata, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA_INSTRUMENTLIST, &CTabEepromUpdata::OnBnClickedButtonUpdataInstrumentlist)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT_ONE, &CTabEepromUpdata::OnBnClickedButtonMoverightOne)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT_ALL, &CTabEepromUpdata::OnBnClickedButtonMoverightAll)
	ON_BN_CLICKED(IDC_BUTTON_MOVELEFT_ONE, &CTabEepromUpdata::OnBnClickedButtonMoveleftOne)
	ON_BN_CLICKED(IDC_BUTTON_MOVELEFT_ALL, &CTabEepromUpdata::OnBnClickedButtonMoveleftAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CTabEepromUpdata::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CTabEepromUpdata 消息处理程序
BOOL CTabEepromUpdata::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i = 0;
	return TRUE;
}
void CTabEepromUpdata::OnBnClickedButtonUpdataInstrumentlist()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonMoverightOne()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonMoverightAll()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonMoveleftOne()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonMoveleftAll()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}
