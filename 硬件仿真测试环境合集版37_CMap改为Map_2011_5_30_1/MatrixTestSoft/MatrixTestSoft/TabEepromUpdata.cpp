// TabEepromUpdata.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TabEepromUpdata.h"

//----------------------------------------------------------------------------------
//将 单字节char* 转换为 宽字节 wchar*
inline wchar_t* AnsiToUnicode( char* szStr )
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
	return pResult;
}

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
	, m_uiIPSource(0)
	, m_uiAimPort(0)
	, m_csAimIP(_T(""))
	, m_uiWriteEepromNum(0)
	, m_uiEditReadTimer(0)
	, m_uiEditWriteTimer(0)
	, m_uiWriteFrameNum(0)
	, m_uiWriteFrameCount(0)
	, m_uiInstrumentIP(0)
	, m_iCheckContinueWork(0)
	, m_iUpdataInstrumentCount(0)
	, m_uiReadFrameCount(0)
	, m_uiReadFrameNum(0)
	, m_uiSN(0)
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
	DDX_Text(pDX, IDC_EDIT_READ_TIMER, m_uiEditReadTimer);
	DDX_Text(pDX, IDC_EDIT_WRITE_TIMER, m_uiEditWriteTimer);
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
	DDX_Control(pDX, IDC_IPADDRESS_AIM, m_ctrlAimIP);
	DDX_Control(pDX, IDC_EDIT_AIMPORT, m_ctrlAimPort);
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
	ON_BN_CLICKED(IDC_CHECK_BROADCAST_WRITE, &CTabEepromUpdata::OnBnClickedCheckBroadcastWrite)
	ON_BN_CLICKED(IDC_CHECK_CONTINUEWORK, &CTabEepromUpdata::OnBnClickedCheckContinuework)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTabEepromUpdata 消息处理程序
BOOL CTabEepromUpdata::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnReset();
	// 得到本机IP地址
	char		name[256]; 
	char* localIP = "127.0.0.1";
	PHOSTENT	hostinfo; 
	if(   gethostname(name, sizeof(name)) == 0) 
	{ 
		if((hostinfo = gethostbyname(name)) != NULL) 
		{ 
			localIP = inet_ntoa(*(struct in_addr*)*hostinfo-> h_addr_list); 
		} 
	} 
	// CString转换为const char*
	m_uiIPSource = inet_addr(localIP);
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
		str = _T("警告：没有设备仪器在线！");
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
	str.Format(_T("Num = %d"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %d"), 0);
	m_ctrlStaticUpdataNum.SetWindowText(str);
	m_ctrlStaticFailNum.SetWindowText(str);
	m_ctrlStaticSuccessNum.SetWindowText(str);
	str = _T("更新设备列表！");
	m_ctrlListMessage.AddString(str);
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
	str.Format(_T("Num = %d"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %d"), m_ctrlListUpdata.GetCount());
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
	str.Format(_T("Num = %d"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %d"), m_ctrlListUpdata.GetCount());
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
	str.Format(_T("Num = %d"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %d"), m_ctrlListUpdata.GetCount());
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
	str.Format(_T("Num = %d"), m_ctrlListInstrument.GetCount());
	m_ctrlStaticInstrumentNum.SetWindowText(str);
	str.Format(_T("Num = %d"), m_ctrlListUpdata.GetCount());
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
	// 打开文件
	CString str = _T("");
	const wchar_t pszFilter[] = _T("文本文件(*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".txt"), _T("aa.txt"), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);

	if ( dlg.DoModal()!=IDOK )
	{		
		str = _T("警告：载入文件对话框创建失败！");
		m_ctrlListMessage.AddString(str);
		return;
	}
	if (FALSE == LoadFile(dlg.GetPathName()))
	{
		str = _T("警告：载入文件 ") + dlg.GetFileName() + _T(" 失败！");
		m_ctrlListMessage.AddString(str);
		return;
	}
	else
	{
		str = _T("警告：载入文件 ") + dlg.GetFileName() + _T(" 成功！");
		m_ctrlListMessage.AddString(str);
		m_uiEditWriteTotal = m_uiWriteEepromNum;
		m_uiEditReadTotal = m_uiWriteEepromNum;
		UpdateData(FALSE);
	}
	m_ctrlBtnReadEeprom.EnableWindow(TRUE);
	m_ctrlBtnWriteEeprom.EnableWindow(TRUE);
	m_ctrlBtnStartCheck.EnableWindow(TRUE);
}

void CTabEepromUpdata::OnBnClickedButtonWriteEeprom()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	CString strOut = _T("");
	UpdateData(TRUE);
	if (0 == m_ctrlListUpdata.GetCount())
	{
		strOut = _T("警告：更新EEPROM设备列表为空！");
		m_ctrlListMessage.AddString(strOut);
		return;
	}
	m_iCheckContinueWork = m_ctrlCheckContinueWork.GetCheck();
	int iCheckBroadCast = m_ctrlCheckBroadCastWrite.GetCheck();
	m_ctrlAimPort.GetWindowText(str);
	_stscanf_s(str,_T("%x"), &m_uiAimPort);
	m_ctrlAimIP.GetWindowText(m_csAimIP);

	if (m_uiEditWriteTotal > m_uiWriteEepromNum)
	{
		str = _T("警告：写入EEPROM的数据总数不能超过文件内数据个数！");
		m_ctrlListMessage.AddString(str);
		m_uiEditWriteTotal = m_uiWriteEepromNum;
	}
	if ((m_uiEditWriteTotal + m_uiEditWriteAddr) > EEPROMCapacity)
	{
		str = _T("警告：写入EEPROM的数据总数不能超过EEPROM容量！");
		m_ctrlListMessage.AddString(str);
		m_uiEditWriteTotal = EEPROMCapacity - m_uiEditWriteAddr;
	}
	if (m_uiEditWriteMax > EEPROMPerFrameWriteMax)
	{
		m_uiEditWriteMax = EEPROMPerFrameWriteMax;
		str.Format(_T("警告：一帧所能写入EEPROM的数据个数不能超过%d个！"), EEPROMPerFrameWriteMax);
		m_ctrlListMessage.AddString(str);
	}

	if (m_uiEditWriteTotal % m_uiEditWriteMax == 0)
	{
		m_uiWriteFrameNum = m_uiEditWriteTotal / m_uiEditWriteMax;
	} 
	else
	{
		m_uiWriteFrameNum = m_uiEditWriteTotal / m_uiEditWriteMax + 1;
	}
	if (m_iCheckContinueWork == 0)
	{
		if (iCheckBroadCast == 1)
		{
			m_uiInstrumentIP = IPBroadcastAddr;
			OnWriteEeprom();
			strOut = _T("广播写入EEPROM！");
			m_ctrlListMessage.AddString(strOut);
		} 
		else
		{
			int nIndex = m_ctrlListUpdata.GetCurSel();
			if (nIndex == CB_ERR)
			{
				return;
			}

			int n = m_ctrlListUpdata.GetTextLen(nIndex);
			m_ctrlListUpdata.GetText(nIndex, str.GetBuffer(n));
			str.ReleaseBuffer();
			_stscanf_s(str, _T("SN = %04x,	IP = %d"), &m_uiSN, &m_uiInstrumentIP);
			strOut = _T("向 ") + str + _T(" 的仪器写入EEPROM！");
			m_ctrlListMessage.AddString(strOut);
			OnWriteEeprom();
		}
	} 
	else
	{
		strOut = _T("开始连续写入仪器的EEPROM！");
		m_ctrlListMessage.AddString(strOut);
		OnWriteEeprom();
	}
	m_ctrlBtnWriteEeprom.EnableWindow(FALSE);
}

void CTabEepromUpdata::OnBnClickedButtonReadEeprom()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	CString strOut = _T("");
	int nIndex = 0;

	UpdateData(TRUE);
	nIndex = m_ctrlListUpdata.GetCurSel();
	if (nIndex == CB_ERR)
	{
		return;
	}
	m_oEepromSocket.m_uiNeedToReadNum = m_uiEditReadTotal;
	m_oEepromSocket.m_uiOneFrameReadMax = m_uiEditReadMax;
	if (m_uiEditReadTotal == 0)
	{
		str = _T("警告：读取的EEPROM数据个数不能为0！");
		m_ctrlListMessage.AddString(str);
		return;
	}
	if ((m_uiEditReadTotal + m_uiEditReadAddr) > EEPROMCapacity)
	{
		str = _T("警告：读取EEPROM的数据总数不能超过EEPROM容量！");
		m_ctrlListMessage.AddString(str);
		m_uiEditReadTotal = EEPROMCapacity - m_uiEditReadAddr;
	}
	if (m_uiEditReadMax > EEPROMPerFrameReadMax)
	{
		m_uiEditReadMax = EEPROMPerFrameReadMax;
		str.Format(_T("警告：一帧所能读取的EEPROM数据个数不能超过%d个！"), EEPROMPerFrameReadMax);
		m_ctrlListMessage.AddString(str);
	}
	if (m_uiEditReadTotal % m_uiEditReadMax == 0)
	{
		m_uiReadFrameNum = m_uiEditReadTotal / m_uiEditReadMax;
	} 
	else
	{
		m_uiReadFrameNum = m_uiEditReadTotal / m_uiEditReadMax + 1;
	}
	int n = m_ctrlListUpdata.GetTextLen(nIndex);
	m_ctrlListUpdata.GetText(nIndex, str.GetBuffer(n));
	str.ReleaseBuffer();
	_stscanf_s(str, _T("SN = %04x,	IP = %d"), &m_uiSN, &m_uiInstrumentIP);
	OnReadEeprom();
	strOut = _T("读取 ") + str + _T(" 仪器的EEPROM！");
	m_ctrlListMessage.AddString(strOut);
	m_ctrlBtnReadEeprom.EnableWindow(FALSE);
}

void CTabEepromUpdata::OnBnClickedButtonStartCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	CString strOut = _T("");
	UpdateData(TRUE);
	if (0 == m_ctrlListUpdata.GetCount())
	{
		strOut = _T("警告：更新EEPROM设备列表为空！");
		m_ctrlListMessage.AddString(strOut);
		return;
	}
	int iCheckContinueWork = m_ctrlCheckContinueWork.GetCheck();
	m_ctrlAimPort.GetWindowText(str);
	_stscanf_s(str,_T("%x"), &m_uiAimPort);
	m_ctrlAimIP.GetWindowText(m_csAimIP);
	if (iCheckContinueWork == 0)
	{
		int nIndex = m_ctrlListUpdata.GetCurSel();
		if (nIndex == CB_ERR)
		{
			return;
		}
		m_ctrlBtnStartCheck.EnableWindow(FALSE);
		int n = m_ctrlListUpdata.GetTextLen(nIndex);
		m_ctrlListUpdata.GetText(nIndex, str.GetBuffer(n));
		str.ReleaseBuffer();
		_stscanf_s(str, _T("SN = %04x,	IP = %d"), &m_uiSN, &m_uiInstrumentIP);
		OnCheckEepromOne(m_uiSN, m_uiInstrumentIP);
		strOut = _T("校验 ") + str + _T(" 仪器的EEPROM！");
		m_ctrlListMessage.AddString(strOut);
		m_ctrlListUpdata.DeleteString(nIndex);
		m_ctrlBtnStartCheck.EnableWindow(TRUE);
	} 
	else
	{
		strOut = _T("开始连续校验仪器的EEPROM！");
		m_ctrlListMessage.AddString(strOut);
		m_oEepromSocket.m_uiNeedToReadNum = m_uiEditReadTotal;
		m_oEepromSocket.m_uiOneFrameReadMax = m_uiEditReadMax;
		if (m_uiEditReadTotal == 0)
		{
			str = _T("警告：读取的EEPROM数据个数不能为0！");
			m_ctrlListMessage.AddString(str);
		}
		if ((m_uiEditReadTotal + m_uiEditReadAddr) > EEPROMCapacity)
		{
			str = _T("警告：读取EEPROM的数据总数不能超过EEPROM容量！");
			m_ctrlListMessage.AddString(str);
			m_uiEditReadTotal = EEPROMCapacity - m_uiEditReadAddr;
		}
		if (m_uiEditReadMax > EEPROMPerFrameReadMax)
		{
			m_uiEditReadMax = EEPROMPerFrameReadMax;
			str.Format(_T("警告：一帧所能读取的EEPROM数据个数不能超过%d个！"), EEPROMPerFrameReadMax);
			m_ctrlListMessage.AddString(str);
		}
		if (m_uiEditReadTotal % m_uiEditReadMax == 0)
		{
			m_uiReadFrameNum = m_uiEditReadTotal / m_uiEditReadMax;
		} 
		else
		{
			m_uiReadFrameNum = m_uiEditReadTotal / m_uiEditReadMax + 1;
		}
		m_iUpdataInstrumentCount = m_ctrlListUpdata.GetCount();
		OnReadEeprom();
		m_ctrlBtnReadEeprom.EnableWindow(FALSE);
		m_ctrlBtnStartCheck.EnableWindow(FALSE);
	}
}

// 重置
void CTabEepromUpdata::OnReset(void)
{
	CString str = _T("");
	int icount = 0;
	
	m_uiWriteEepromNum = 0;
	memset(m_ucWriteEepromBuf, SndFrameBufInit, EEPROMCapacity);

	m_uiEditReadAddr = 0x100;
	m_uiEditReadMax = 4;
	m_uiEditReadTotal = 0;
	m_uiEditWriteAddr = 0x100;
	m_uiEditWriteMax = 4;
	m_uiEditWriteTotal = 0;
	m_uiWriteFrameNum = 0;
	m_uiInstrumentIP = 0;
	m_uiSN = 0;
	m_uiWriteFrameCount = 0;
	m_iCheckContinueWork = 0;
	m_iUpdataInstrumentCount = 0;
	m_uiReadFrameNum = 0;
	m_uiReadFrameCount = 0;

	m_uiAimPort = 0x8202;
	m_csAimIP = _T("127.0.0.1");
	str.Format(_T("0x%04x"), m_uiAimPort);
	m_ctrlAimPort.SetWindowText(str);
	m_ctrlAimIP.SetWindowText(m_csAimIP);

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

	m_oEepromSocket.OnInit();
	UpdateData(FALSE);
	
	OnCreateEepromSocket();
}

// 载入程序文件
// 在UNICODE环境下读取ANSI文件
BOOL CTabEepromUpdata::LoadFile(CString strPath)
{
	if ((_taccess(strPath, 0)) != -1)
	{
		CFile file;
		if(file.Open(strPath, CFile::modeRead) == FALSE)
		{
			return FALSE;
		}
		else
		{
			const int LeftGiveUpNum = 1;
			unsigned int uiTemp = 0;
			char ucTemp[2];
			wchar_t* wchTemp;
			ULONGLONG ullEnd = 0;
			ULONGLONG lActual = 0;

			m_uiWriteEepromNum = 0;
			memset(m_ucWriteEepromBuf, SndFrameBufInit, EEPROMCapacity);

			file.SeekToBegin();
			ullEnd = file.SeekToEnd();
			file.SeekToBegin();
			while (lActual < ullEnd)
			{
				file.Seek(LeftGiveUpNum, CFile::current);
				file.Read(ucTemp, 2);
				wchTemp = AnsiToUnicode(ucTemp);
				_stscanf_s(wchTemp, _T("%x"), &uiTemp);
				m_ucWriteEepromBuf[m_uiWriteEepromNum] = static_cast<unsigned char>(uiTemp);
				m_uiWriteEepromNum++;
				file.Read(ucTemp, 2);
				wchTemp = AnsiToUnicode(ucTemp);
				_stscanf_s(wchTemp, _T("%x"), &uiTemp);
				m_ucWriteEepromBuf[m_uiWriteEepromNum] = static_cast<unsigned char>(uiTemp);
				m_uiWriteEepromNum++;
				lActual = file.Seek(2, CFile::current);
			}
			file.Close();
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}

// 创建EEPROM的Socket
void CTabEepromUpdata::OnCreateEepromSocket(void)
{
	m_oEepromSocket.Close();
	unsigned int uiRcvPort = EepromRecPort;
	m_oEepromSocket.Create(uiRcvPort, SOCK_DGRAM, NULL);
}

// 关闭
void CTabEepromUpdata::OnClose(void)
{
	m_oEepromSocket.Close();
}

// 写EEPROM
void CTabEepromUpdata::OnWriteEeprom(void)
{
	if (m_iCheckContinueWork == 1)
	{
		CString str = _T("");
		CString strOut = _T("");
		int n = m_ctrlListUpdata.GetTextLen(m_iUpdataInstrumentCount);
		m_ctrlListUpdata.GetText(m_iUpdataInstrumentCount, str.GetBuffer(n));
		str.ReleaseBuffer();
		_stscanf_s(str, _T("SN = %04x,	IP = %d"), &m_uiSN, &m_uiInstrumentIP);
		strOut = _T("向 ") + str + _T(" 的仪器写入EEPROM！");
		m_ctrlListMessage.AddString(strOut);
	}
	SetTimer(TabEepromUpdataWriteTimerNb, m_uiEditWriteTimer, NULL);
}

void CTabEepromUpdata::OnBnClickedCheckBroadcastWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bStatus = (BOOL)::SendMessage(GetDlgItem(IDC_CHECK_BROADCAST_WRITE)->GetSafeHwnd(),   BM_GETCHECK,   0,   0);
	if (bStatus == TRUE)
	{
		m_ctrlCheckContinueWork.SetCheck(0);
	}
}

void CTabEepromUpdata::OnBnClickedCheckContinuework()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bStatus = (BOOL)::SendMessage(GetDlgItem(IDC_CHECK_CONTINUEWORK)->GetSafeHwnd(),   BM_GETCHECK,   0,   0);
	if (bStatus == TRUE)
	{
		m_ctrlCheckBroadCastWrite.SetCheck(0);
	}
}
// 自动生成0x18命令并自动补零
int CTabEepromUpdata::ADCCommand_18(int iPos, byte * cADCSet, unsigned int uiLength)
{
	for(unsigned int i=0; i<uiLength; i+=FramePacketSize4B)
	{
		m_ucSendBuf[iPos] = CmdADCSet;
		iPos += FrameCmdSize1B;
		if (uiLength < (i + FramePacketSize4B))
		{
			memcpy(&m_ucSendBuf[iPos], cADCSet+i, uiLength - i);
			iPos += uiLength - i;
			memset(&m_ucSendBuf[iPos], SndFrameBufInit, i + FramePacketSize4B - uiLength);
			iPos += i + FramePacketSize4B - uiLength;
			return iPos;
		}
		memcpy(&m_ucSendBuf[iPos], cADCSet+i, FramePacketSize4B);
		iPos += FramePacketSize4B;
	}
	return iPos;
}
// 读EEPROM
void CTabEepromUpdata::OnReadEeprom(void)
{
	if (m_iCheckContinueWork == 1)
	{
		CString str = _T("");
		CString strOut = _T("");
		int n = m_ctrlListUpdata.GetTextLen(m_iUpdataInstrumentCount);
		m_ctrlListUpdata.GetText(m_iUpdataInstrumentCount, str.GetBuffer(n));
		str.ReleaseBuffer();
		_stscanf_s(str, _T("SN = %04x,	IP = %d"), &m_uiSN, &m_uiInstrumentIP);
		strOut = _T("校验 ") + str + _T(" 仪器的EEPROM！");
		m_ctrlListMessage.AddString(strOut);
		m_ctrlListUpdata.DeleteString(m_iUpdataInstrumentCount);
	}
	m_oEepromSocket.m_uiReadEepromNum = 0;
	memset(m_oEepromSocket.m_ucReadEepromBuf, SndFrameBufInit, EEPROMCapacity);
	SetTimer(TabEepromUpdataReadTimerNb, m_uiEditReadTimer, NULL);
}

// 生成发送帧帧格式
void CTabEepromUpdata::OnMakeSendFrame(unsigned int uiInstrumentIP, unsigned int uiStartAddr, unsigned char * ucBuf, unsigned int uiLength, unsigned int uiOptCmd)
{
	unsigned short	usPortAim = 0;
	unsigned short	usCommand = 0;
	unsigned short usCRC16 = 0;
	unsigned int	itmp	=	0;
	unsigned char	ucCommand = 0;
	unsigned int	uiADCBroadcastPort = 0;
	unsigned char * pBuf = NULL;
	int iPos = 0;
	memset(m_ucSendBuf, SndFrameBufInit, SndFrameSize);
	m_ucSendBuf[0] = FrameHeadCheck0;
	m_ucSendBuf[1] = FrameHeadCheck1;
	m_ucSendBuf[2] = FrameHeadCheck2;
	m_ucSendBuf[3] = FrameHeadCheck3;
	usPortAim = EepromRecPort;
	usCommand = SendSetCmd;
	// 源IP地址
	iPos = FrameHeadSize;
	memcpy(&m_ucSendBuf[iPos], &m_uiIPSource, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&m_ucSendBuf[iPos], &uiInstrumentIP, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&m_ucSendBuf[iPos], &usPortAim, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_ucSendBuf[iPos], &usCommand, FramePacketSize2B);
	iPos += FramePacketSize2B;
	memcpy(&m_ucSendBuf[iPos], &itmp, FramePacketSize4B);
	iPos += FramePacketSize4B;
	if (uiInstrumentIP == IPBroadcastAddr)
	{
		//广播命令
		ucCommand = CmdBroadCastPortSet;
		memcpy(&m_ucSendBuf[iPos], &ucCommand, FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		//广播命令端口
		uiADCBroadcastPort = ADCSetBroadcastPort;
		memcpy(&m_ucSendBuf[iPos], &uiADCBroadcastPort, FramePacketSize4B);
		iPos += FramePacketSize4B;
	}
	if (uiOptCmd == WriteEEPROMCmd)
	{
		pBuf = new unsigned char[4 + uiLength];
		memset(pBuf, SndFrameBufInit, 4 + uiLength);
		pBuf[0] = (0xc0 + 2 + uiLength) & 0xff;
		pBuf[1] = 0xa0;
		pBuf[2] = static_cast<unsigned char>(uiStartAddr >> 8);
		pBuf[3] = uiStartAddr & 0xff;
		memcpy(&pBuf[4], &ucBuf[0], uiLength);
		iPos = ADCCommand_18(iPos, pBuf, 4 + uiLength);
		delete [] pBuf;
	} 
	else if (uiOptCmd == ReadEEPROMCmd)
	{
		pBuf = new unsigned char[5 + uiLength];
		memset(pBuf, SndFrameBufInit, 5 + uiLength);
		pBuf[0] = (0x40 + 3 + uiLength) & 0xff;
		pBuf[1] = 0xa0;
		pBuf[2] = static_cast<unsigned char>(uiStartAddr >> 8);
		pBuf[3] = uiStartAddr & 0xff;
		pBuf[4] = 0xa1;
		iPos = ADCCommand_18(iPos, pBuf, 5 + uiLength);
		delete [] pBuf;
	}
	// 设置命令字结束
	m_ucSendBuf[iPos] = SndFrameBufInit;
	usCRC16 = get_crc_16(&m_ucSendBuf[FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize);
	memcpy(&m_ucSendBuf[SndFrameSize - CRCSize], &usCRC16, CRCSize);

	m_oEepromSocket.SendTo(&m_ucSendBuf, SndFrameSize, m_uiAimPort, m_csAimIP);
}

// 生成0x18命令查询帧
void CTabEepromUpdata::OnMakeCmd_18(unsigned int uiInstrumentIP)
{
	unsigned short	usPortAim = 0;
	unsigned short	usCommand = 0;
	unsigned short usCRC16 = 0;
	unsigned int	itmp	=	0;
	int iPos = 0;
	memset(m_ucSendBuf, SndFrameBufInit, SndFrameSize);
	m_ucSendBuf[0] = FrameHeadCheck0;
	m_ucSendBuf[1] = FrameHeadCheck1;
	m_ucSendBuf[2] = FrameHeadCheck2;
	m_ucSendBuf[3] = FrameHeadCheck3;
	usPortAim = EepromRecPort;
	usCommand = SendQueryCmd;
	// 源IP地址
	iPos = FrameHeadSize;
	memcpy(&m_ucSendBuf[iPos], &m_uiIPSource, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&m_ucSendBuf[iPos], &uiInstrumentIP, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&m_ucSendBuf[iPos], &usPortAim, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_ucSendBuf[iPos], &usCommand, FramePacketSize2B);
	iPos += FramePacketSize2B;
	memcpy(&m_ucSendBuf[iPos], &itmp, FramePacketSize4B);
	iPos += FramePacketSize4B;
	m_ucSendBuf[iPos] = CmdADCSet;
	iPos += FrameCmdSize1B;
	memcpy(&m_ucSendBuf[iPos], &itmp, FramePacketSize4B);
	iPos += FramePacketSize4B;

	// 设置命令字结束
	m_ucSendBuf[iPos] = SndFrameBufInit;
	usCRC16 = get_crc_16(&m_ucSendBuf[FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize);
	memcpy(&m_ucSendBuf[SndFrameSize - CRCSize], &usCRC16, CRCSize);

	m_oEepromSocket.SendTo(&m_ucSendBuf, SndFrameSize, m_uiAimPort, m_csAimIP);
}

// 校验一个仪器的EEPROM
void CTabEepromUpdata::OnCheckEepromOne(unsigned int uiInstrumentSN, unsigned int uiInstrumentIP)
{
	CString str = _T("");
	unsigned int uiCheckErrorNum = 0;
	if (m_oEepromSocket.m_uiReadEepromNum == 0)
	{
		str = _T("警告：接收数据个数为0！");
		m_ctrlListMessage.AddString(str);
	}
	for (unsigned int i=0; i<m_oEepromSocket.m_uiReadEepromNum; i++)
	{
		if (m_oEepromSocket.m_ucReadEepromBuf[i] != m_ucWriteEepromBuf[i])
		{
			uiCheckErrorNum++;
		}
	}
	str.Format(_T("校验数据个数为%d，其中错误数据个数为%d！"), m_oEepromSocket.m_uiReadEepromNum, uiCheckErrorNum);
	m_ctrlListMessage.AddString(str);
	if (uiCheckErrorNum == 0)
	{
		// 更新成功列表中没有该仪器则加入
		OnFindInListBox(&m_ctrlListSuccess, uiInstrumentSN, uiInstrumentIP, EEPROMListAdd);
		// 更新失败列表中有该仪器则删除
		OnFindInListBox(&m_ctrlListFail, uiInstrumentSN, uiInstrumentIP, EEPROMListDelete);
	} 
	else
	{
		// 更新成功列表中没有该仪器则删除
		OnFindInListBox(&m_ctrlListSuccess, uiInstrumentSN, uiInstrumentIP, EEPROMListDelete);
		// 更新失败列表中有该仪器则加入
		OnFindInListBox(&m_ctrlListFail, uiInstrumentSN, uiInstrumentIP, EEPROMListAdd);
	}
}

// 在列表控件中寻找是否有该仪器
void CTabEepromUpdata::OnFindInListBox(CListBox * pListBox, unsigned int uiInstrumentSN, unsigned int uiInstrumentIP, unsigned int uiOpt)
{
	unsigned int uiSN = 0;
	unsigned int uiIP = 0;
	CString str = _T("");
	for (int i=0; i<pListBox->GetCount(); i++)
	{
		int n = pListBox->GetTextLen(i);
		pListBox->GetText(i, str.GetBuffer(n));
		str.ReleaseBuffer();
		_stscanf_s(str, _T("SN = %04x,	IP = %d"), &uiSN, &uiIP);
		if ((uiSN == uiInstrumentSN) && (uiIP == uiInstrumentIP))
		{
			if (uiOpt == EEPROMListDelete)
			{
				pListBox->DeleteString(i);
				return;
			}
			else if (uiOpt == EEPROMListAdd)
			{
				return;
			}
		}
	}
	if (uiOpt == EEPROMListDelete)
	{
		str = _T("警告：要删除仪器不在列表中！");
		m_ctrlListMessage.AddString(str);
	} 
	else if (uiOpt == EEPROMListAdd)
	{
		str.Format(_T("SN = %04x,	IP = %d"), uiSN, uiIP);
		pListBox->AddString(str);
	}
}

void CTabEepromUpdata::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TabEepromUpdataWriteTimerNb)
	{
		unsigned int uiLength = 0;
		KillTimer(TabEepromUpdataWriteTimerNb);
		if (m_uiEditWriteTotal > (m_uiEditWriteMax * (m_uiWriteFrameCount + 1)))
		{
			uiLength = m_uiEditWriteMax;
		}
		else
		{
			uiLength = m_uiEditWriteTotal - m_uiEditWriteMax * m_uiWriteFrameCount;
		}
		OnMakeSendFrame(m_uiInstrumentIP, m_uiEditWriteAddr + m_uiEditWriteMax * m_uiWriteFrameCount, 
			&m_ucWriteEepromBuf[m_uiEditWriteMax * m_uiWriteFrameCount], uiLength, WriteEEPROMCmd);
		m_uiWriteFrameCount++;
		if (m_uiWriteFrameCount == m_uiWriteFrameNum)
		{
			m_uiWriteFrameCount = 0;
			if (m_iCheckContinueWork == 1)
			{
				m_iUpdataInstrumentCount++;
				if (m_iUpdataInstrumentCount == m_ctrlListUpdata.GetCount())
				{
					CString strOut = _T("");
					m_iUpdataInstrumentCount = 0;
				 	strOut = _T("完成连续写入仪器的EEPROM！");
				 	m_ctrlListMessage.AddString(strOut);
					m_ctrlBtnWriteEeprom.EnableWindow(TRUE);
				} 
				else
				{
					OnWriteEeprom();
				}
			} 
			else
			{
				m_ctrlBtnWriteEeprom.EnableWindow(TRUE);
			}
		}
		else
		{
			SetTimer(TabEepromUpdataWriteTimerNb, m_uiEditWriteTimer, NULL);
		}
	}
	else if (nIDEvent == TabEepromUpdataReadTimerNb)
	{
		unsigned int uiLength = 0;
		KillTimer(TabEepromUpdataReadTimerNb);
		if (m_uiReadFrameCount % 2 == 0)
		{
			if (m_uiEditReadTotal > (m_uiEditReadMax * (m_uiReadFrameCount / 2 + 1)))
			{
				uiLength = m_uiEditReadMax;
			}
			else
			{
				uiLength = m_uiEditReadTotal - m_uiEditReadMax * m_uiReadFrameCount / 2;
			}
			// 发送读EEPROM命令
			OnMakeSendFrame(m_uiInstrumentIP, m_uiEditReadAddr + m_uiEditReadMax * m_uiReadFrameCount / 2,
				NULL, uiLength, ReadEEPROMCmd);
		} 
		else
		{
			// 发送0x18命令查询帧读取缓冲区中数据
			OnMakeCmd_18(m_uiInstrumentIP);
		}
		m_uiReadFrameCount ++;
		if (m_uiReadFrameCount / 2 == m_uiReadFrameNum)
		{
			m_uiReadFrameCount = 0;
			if (m_iCheckContinueWork == 1)
			{
				m_iUpdataInstrumentCount--;
				if (m_iUpdataInstrumentCount == 0)
				{
					CString strOut = _T("");
					strOut = _T("完成连续校验仪器的EEPROM！");
					m_ctrlListMessage.AddString(strOut);
					m_ctrlBtnReadEeprom.EnableWindow(TRUE);
					m_ctrlBtnStartCheck.EnableWindow(TRUE);
				} 
				else
				{
					OnReadEeprom();
					OnCheckEepromOne(m_uiSN, m_uiInstrumentIP);
				}
			} 
			else
			{
				m_ctrlBtnReadEeprom.EnableWindow(TRUE);
			}
		}
		else
		{
			SetTimer(TabEepromUpdataReadTimerNb, m_uiEditReadTimer, NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);
}
