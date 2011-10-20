// TabEepromUpdata.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TabEepromUpdata.h"

using std::vector;
// CTabEepromUpdata 对话框

IMPLEMENT_DYNAMIC(CTabEepromUpdata, CDialog)

CTabEepromUpdata::CTabEepromUpdata(CWnd* pParent /*=NULL*/)
	: CDialog(CTabEepromUpdata::IDD, pParent)
	, m_uiEditReadMax(0)
	, m_uiEditReadTotal(0)
	, m_uiEditReadAddr(0)
	, m_uiEditWriteMax(0)
	, m_uiEditWriteTotal(0)
	, m_uiEditWriteAddr(0)
	, m_pInstrumentList(NULL)
{

}

CTabEepromUpdata::~CTabEepromUpdata()
{
}

void CTabEepromUpdata::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READ_MAX, m_uiEditReadMax);
	DDX_Text(pDX, IDC_EDIT_READ_TOTAL, m_uiEditReadTotal);
	DDX_Text(pDX, IDC_EDIT_READ_ADDR, m_uiEditReadAddr);
	DDX_Text(pDX, IDC_EDIT_WRITE_MAX, m_uiEditWriteMax);
	DDX_Text(pDX, IDC_EDIT_WRITE_TOTAL, m_uiEditWriteTotal);
	DDX_Text(pDX, IDC_EDIT_WRITE_ADDR, m_uiEditWriteAddr);
	DDX_Control(pDX, IDC_LIST_INSTRUMENT, m_ctrlListInstrument);
	DDX_Control(pDX, IDC_LIST_UPDATA, m_ctrlListUpdata);
	DDX_Control(pDX, IDC_LIST_FAIL, m_ctrlListFail);
	DDX_Control(pDX, IDC_LIST_SUCCESS, m_ctrlListSuccess);
	DDX_Control(pDX, IDC_STATIC_INSTRUMENT_NUM, m_ctrlStaticInstrumentNum);
	DDX_Control(pDX, IDC_STATIC_UPDATA_NUM, m_ctrlStaticUpdataNum);
	DDX_Control(pDX, IDC_STATIC_FAIL_NUM, m_ctrlStaticFailNum);
	DDX_Control(pDX, IDC_STATIC_SUCCESS_NUM, m_ctrlStaticSuccessNum);
	DDX_Control(pDX, IDC_BUTTON_MOVERIGHT_ONE, m_ctrlBtnMoveRightOne);
	DDX_Control(pDX, IDC_BUTTON_MOVERIGHT_ALL, m_ctrlBtnMoveRightAll);
	DDX_Control(pDX, IDC_BUTTON_MOVELEFT_ONE, m_ctrlBtnMoveLeftOne);
	DDX_Control(pDX, IDC_BUTTON_MOVELEFT_ALL, m_ctrlBtnMoveLeftAll);
	DDX_Control(pDX, IDC_CHECK_BROADCAST_WRITE, m_ctrlCheckBroadCastWrite);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_ctrlListMessage);
	DDX_Control(pDX, IDC_BUTTON_UPDATA_INSTRUMENTLIST, m_ctrlBtnUpdataInstrumentList);
	DDX_Control(pDX, IDC_BUTTON_LOADFILE, m_ctrlBtnLoadFile);
	DDX_Control(pDX, IDC_BUTTON_WRITE_EEPROM, m_ctrlBtnWriteEeprom);
	DDX_Control(pDX, IDC_BUTTON_READ_EEPROM, m_ctrlBtnReadEeprom);
	DDX_Control(pDX, IDC_BUTTON_START_CHECK, m_ctrlBtnStartCheck);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_ctrlBtnReset);
	DDX_Control(pDX, IDC_CHECK_CONTINUEWORK, m_ctrlCheckContinueWork);
}


BEGIN_MESSAGE_MAP(CTabEepromUpdata, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA_INSTRUMENTLIST, &CTabEepromUpdata::OnBnClickedButtonUpdataInstrumentlist)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT_ONE, &CTabEepromUpdata::OnBnClickedButtonMoverightOne)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT_ALL, &CTabEepromUpdata::OnBnClickedButtonMoverightAll)
	ON_BN_CLICKED(IDC_BUTTON_MOVELEFT_ONE, &CTabEepromUpdata::OnBnClickedButtonMoveleftOne)
	ON_BN_CLICKED(IDC_BUTTON_MOVELEFT_ALL, &CTabEepromUpdata::OnBnClickedButtonMoveleftAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CTabEepromUpdata::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE, &CTabEepromUpdata::OnBnClickedButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_EEPROM, &CTabEepromUpdata::OnBnClickedButtonWriteEeprom)
	ON_BN_CLICKED(IDC_BUTTON_READ_EEPROM, &CTabEepromUpdata::OnBnClickedButtonReadEeprom)
	ON_BN_CLICKED(IDC_BUTTON_START_CHECK, &CTabEepromUpdata::OnBnClickedButtonStartCheck)
END_MESSAGE_MAP()


// CTabEepromUpdata 消息处理程序
BOOL CTabEepromUpdata::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnReset();
	return TRUE;
}
void CTabEepromUpdata::OnBnClickedButtonUpdataInstrumentlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	int icount = 0;
	if (m_pInstrumentList == NULL)
	{
		return;
	}
	if (m_pInstrumentList->m_oInstrumentSNMap.size() == 0)
	{
		str = _T("没有设备仪器在线！");
		m_ctrlListMessage.AddString(str);
		return;
	}
	icount = m_ctrlListInstrument.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListInstrument.DeleteString(i);
	}
	icount = m_ctrlListUpdata.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListUpdata.DeleteString(i);
	}
	icount = m_ctrlListFail.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListFail.DeleteString(i);
	}
	icount = m_ctrlListSuccess.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListSuccess.DeleteString(i);
	}
	hash_map<unsigned int, CInstrument*>::iterator  iter;

	for(iter = m_pInstrumentList->m_oInstrumentSNMap.begin(); iter!=m_pInstrumentList->m_oInstrumentSNMap.end(); iter++)
	{
		if (NULL != iter->second)
		{
			if (iter->second->m_bIPSetOK == true)
			{
				str.Format(_T("SN = %04x,	IP = %d"), iter->second->m_uiSN, iter->second->m_uiIPAddress);
				m_ctrlListInstrument.AddString(str);
			}
		}
	}
	m_ctrlBtnMoveRightOne.EnableWindow(TRUE);
	m_ctrlBtnMoveRightAll.EnableWindow(TRUE);
	m_ctrlBtnMoveLeftOne.EnableWindow(TRUE);
	m_ctrlBtnMoveLeftAll.EnableWindow(TRUE);
	str.Format(_T("Num = %ud"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
}

void CTabEepromUpdata::OnBnClickedButtonMoverightOne()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_ctrlListInstrument.GetCurSel();
	CString str = _T("");
	if (nIndex == CB_ERR)
	{
		return;
	}

	int n = m_ctrlListInstrument.GetTextLen(nIndex);
	m_ctrlListInstrument.GetText(nIndex, str.GetBuffer(n));
	str.ReleaseBuffer();
	m_ctrlListInstrument.DeleteString(nIndex);
	m_ctrlListUpdata.AddString(str);
	str.Format(_T("Num = %ud"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %ud"), m_ctrlListUpdata.GetCount());
	m_ctrlStaticUpdataNum.SetWindowText(str);
}

void CTabEepromUpdata::OnBnClickedButtonMoverightAll()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	for (int i = 0; i < m_ctrlListInstrument.GetCount(); i++)
	{
		int iSize = m_ctrlListInstrument.GetTextLen(i);
		m_ctrlListInstrument.GetText(i, str.GetBuffer(iSize));
		str.ReleaseBuffer();
		m_ctrlListUpdata.AddString(str);
	}
	int icount = m_ctrlListInstrument.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListInstrument.DeleteString(i);
	}
	str.Format(_T("Num = %ud"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %ud"), m_ctrlListUpdata.GetCount());
	m_ctrlStaticUpdataNum.SetWindowText(str);
}

void CTabEepromUpdata::OnBnClickedButtonMoveleftOne()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_ctrlListUpdata.GetCurSel();
	CString str = _T("");
	if (nIndex == CB_ERR)
	{
		return;
	}

	int n = m_ctrlListUpdata.GetTextLen(nIndex);
	m_ctrlListUpdata.GetText(nIndex, str.GetBuffer(n));
	str.ReleaseBuffer();
	m_ctrlListUpdata.DeleteString(nIndex);
	m_ctrlListInstrument.AddString(str);
	str.Format(_T("Num = %ud"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %ud"), m_ctrlListUpdata.GetCount());
	m_ctrlStaticUpdataNum.SetWindowText(str);
}

void CTabEepromUpdata::OnBnClickedButtonMoveleftAll()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	for (int i = 0; i < m_ctrlListUpdata.GetCount(); i++)
	{
		int iSize = m_ctrlListUpdata.GetTextLen(i);
		m_ctrlListUpdata.GetText(i, str.GetBuffer(iSize));
		str.ReleaseBuffer();
		m_ctrlListInstrument.AddString(str);
	}
	int icount = m_ctrlListUpdata.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListUpdata.DeleteString(i);
	}
	str.Format(_T("Num = %ud"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %ud"), m_ctrlListUpdata.GetCount());
	m_ctrlStaticUpdataNum.SetWindowText(str);
}

void CTabEepromUpdata::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	OnReset();
}

void CTabEepromUpdata::OnBnClickedButtonLoadfile()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonWriteEeprom()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonReadEeprom()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabEepromUpdata::OnBnClickedButtonStartCheck()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 重置
void CTabEepromUpdata::OnReset(void)
{
	CString str = _T("");
	int icount = 0;
	m_uiEditReadAddr = 0;
	m_uiEditReadMax = 0;
	m_uiEditReadTotal = 0;
	m_uiEditWriteAddr = 0;
	m_uiEditWriteMax = 0;
	m_uiEditWriteTotal = 0;

	icount = m_ctrlListInstrument.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListInstrument.DeleteString(i);
	}
	icount = m_ctrlListUpdata.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListUpdata.DeleteString(i);
	}
	icount = m_ctrlListFail.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListFail.DeleteString(i);
	}
	icount = m_ctrlListSuccess.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListSuccess.DeleteString(i);
	}
	icount = m_ctrlListMessage.GetCount();
	for (int i = icount - 1; i >= 0; i--)
	{
		m_ctrlListMessage.DeleteString(i);
	}

	str.Format(_T("Num = %d"), 0);
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	m_ctrlStaticUpdataNum.SetWindowText(str);
	m_ctrlStaticFailNum.SetWindowText(str);
	m_ctrlStaticSuccessNum.SetWindowText(str);

	m_ctrlCheckBroadCastWrite.SetCheck(0);
	m_ctrlCheckContinueWork.SetCheck(0);

	m_ctrlBtnMoveRightOne.EnableWindow(FALSE);
	m_ctrlBtnMoveRightAll.EnableWindow(FALSE);
	m_ctrlBtnMoveLeftOne.EnableWindow(FALSE);
	m_ctrlBtnMoveLeftAll.EnableWindow(FALSE);
	m_ctrlBtnUpdataInstrumentList.EnableWindow(TRUE);
	m_ctrlBtnLoadFile.EnableWindow(TRUE);
	m_ctrlBtnWriteEeprom.EnableWindow(FALSE);
	m_ctrlBtnReadEeprom.EnableWindow(FALSE);
	m_ctrlBtnStartCheck.EnableWindow(FALSE);
	m_ctrlBtnReset.EnableWindow(TRUE);
	UpdateData(FALSE);
}
