#include "StdAfx.h"
#include "SaveFile.h"
#include "Resource.h"
CSaveFile::CSaveFile(void)
: m_csEditShow(_T(""))
, m_csSaveFile(_T(""))
, m_csSaveReceiveFile(_T(""))
, m_csSaveSendFile(_T(""))
, m_csSaveFilePath(_T(""))
, m_iSaveSize(1024)
, m_bStartSave(false)
, m_pWndTab(NULL)
, m_file(NULL)
{
}

CSaveFile::~CSaveFile(void)
{
}

// 初始化
void CSaveFile::OnInit(void)
{
	char cSaveToFilePath[MAX_PATH];
	// 得到当前路径
	GetCurrentDirectory(MAX_PATH, cSaveToFilePath);
	// 创建保存信息的文件夹
	CreateDirectory(_T("数据备份"),NULL);
	m_csSaveFilePath = cSaveToFilePath;
	m_csSaveFilePath += _T("\\数据备份");
}

// 开始数据存储
void CSaveFile::OnSaveStart(void)
{
	m_bStartSave = true;
}

// 停止数据存储
void CSaveFile::OnSaveStop(void)
{
	m_bStartSave = false;
	CString strtemp = _T("");
	m_Sec.Lock();
	strtemp = m_csEditShow;
	m_Sec.Unlock();
	m_pWndTab->GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(strtemp);
	OnSaveToFile();
}

// 选择文件存储路径
void CSaveFile::OnSelectSaveFilePath(void)
{
	char szDir[MAX_PATH];

	BROWSEINFO bi;

	ITEMIDLIST *pidl;

	bi.hwndOwner = m_hWnd; // 指定父窗口，在对话框显示期间，父窗口将被禁用 


	bi.pidlRoot = NULL; // 如果指定NULL，就以“桌面”为根 

	bi.pszDisplayName = szDir; 

	bi.lpszTitle = "请选择数据存储目录"; // 这一行将显示在对话框的顶端 

	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

	bi.lpfn = NULL;

	bi.lParam = 0;

	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
	{
		return;
	}
	if(!SHGetPathFromIDList(pidl, szDir))
	{	
		return;
	}
	else
	{
		m_csSaveFilePath = szDir;
	}
}

// 重置
void CSaveFile::OnReset(void)
{
	m_csEditShow = _T("");
	// 保存通讯文件
	m_csSaveFile = _T("");
	// 保存接收的数据到文件
	m_csSaveReceiveFile = _T("");
	// 保存发送的数据到文件
	m_csSaveSendFile = _T("");
	m_pWndTab->GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(_T(""));
}

// 保存到文件中
void CSaveFile::OnSaveToFile(void)
{
	CString csSaveFileTemp = "";
	CString csSaveReceiveFileTemp = "";
	CString csSaveSendFileTemp = "";
	CString strFileName = "";
	SYSTEMTIME  sysTime;
	m_Sec.Lock();
	csSaveFileTemp = m_csSaveFile;
	csSaveReceiveFileTemp = m_csSaveReceiveFile;
	csSaveSendFileTemp = m_csSaveSendFile;
	m_csSaveFile = "";
	m_csSaveReceiveFile = "";
	m_csSaveSendFile = "";
	m_Sec.Unlock();

	if (csSaveFileTemp.GetLength() == 0)
	{	
		return;
	}
	// 得到当前系统时间
	GetLocalTime(&sysTime);
	strFileName.Format("%s\\%04d%02d%02d%02d%02d%02d%04d.text", m_csSaveFilePath,sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);

	errno_t err;
	//保存成ANSI格式的文件
	if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
	{
		fprintf(m_file,_T("%s"),csSaveFileTemp); 
	}
	fclose(m_file);
	// 保存接收到的数据到文件
	if (csSaveReceiveFileTemp.GetLength() != 0)
	{
		strFileName.Format("%s\\Rec%04d%02d%02d%02d%02d%02d%04d.text", m_csSaveFilePath,sysTime.wYear,sysTime.wMonth,sysTime.wDay,
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);

		errno_t err;
		//保存成ANSI格式的文件
		if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
		{
			fprintf(m_file,_T("%s"),csSaveReceiveFileTemp); 
		}
		fclose(m_file);
	}
	// 保存发送的数据到文件
	if (csSaveSendFileTemp.GetLength() != 0)
	{
		strFileName.Format("%s\\Send%04d%02d%02d%02d%02d%02d%04d.text", m_csSaveFilePath,sysTime.wYear,sysTime.wMonth,sysTime.wDay,
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);

		errno_t err;
		//保存成ANSI格式的文件
		if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
		{
			fprintf(m_file,_T("%s"),csSaveSendFileTemp); 
		}
		fclose(m_file);
	}
}

// 保存接收数据
void CSaveFile::OnSaveReceiveData(unsigned char* buf, int iRecLength)
{
	if (m_bStartSave == FALSE)
	{
		return;
	}

	CString str = _T("");
	CString strtemp = _T("");
	int icsSaveFileLength = 0;

	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);

	str.Format(_T("%04d.%02d.%02d %02d:%02d:%02d:%04d 接收数据 数据包大小为 %d 数据为：\r\n"),sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, iRecLength);
	strtemp += str;

	for (int i=0; i<iRecLength; i++)
	{
		str.Format(_T("%02x "),buf[i]);
		strtemp += str;
	}

	strtemp += _T("\r\n");
	m_Sec.Lock();
	m_csSaveFile += strtemp;
	m_csSaveReceiveFile += strtemp;
	m_csEditShow += strtemp;
	icsSaveFileLength = m_csSaveFile.GetLength();
	m_Sec.Unlock();
	//	GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(m_cseditshow);
	//	UpdateData(FALSE);
	if (icsSaveFileLength> (m_iSaveSize*1024))
	{	
		OnSaveToFile();
	}
}

// 保存发送数据
void CSaveFile::OnSaveSendData(unsigned char* buf, int iSendLength)
{
	if (m_bStartSave == FALSE)
	{
		return;
	}
	CString str = _T("");
	CString strtemp = _T("");
	int icsSaveFileLength = 0;
	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);

	str.Format(_T("%04d.%02d.%02d %02d:%02d:%02d:%04d 发送数据 数据包大小为 %d 数据为：\r\n"),sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, iSendLength);
	strtemp += str;

	for (int i=0; i<iSendLength; i++)
	{
		str.Format(_T("%02x "),buf[i]);
		strtemp += str;

	}

	strtemp += _T("\r\n");
	m_Sec.Lock();
	m_csSaveFile += strtemp;
	m_csSaveSendFile += strtemp;
	m_csEditShow += strtemp;
	icsSaveFileLength = m_csSaveFile.GetLength();
	m_Sec.Unlock();
	//	GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(m_cseditshow);
	// 用UpdateData(FALSE)快速反复刷新界面会占用大量资源，拖慢速度
	//	UpdateData(FALSE);
	if (icsSaveFileLength > (m_iSaveSize*1024))
	{
// 		CWnd* pwnd = AfxGetMainWnd();
// 		m_oThreadManage.m_SendThread.m_csTailFrameShow = _T("");
// 		pwnd->GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(_T(""));
// 		m_oThreadManage.m_SendThread.m_csTailTimeReturnShow = _T("");
// 		pwnd->GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(_T(""));
// 		m_oThreadManage.m_SendThread.m_csDelayTimeReturnShow = _T("");
// 		pwnd->GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(_T(""));
// 		m_oThreadManage.m_SendThread.m_csADCDataRecShow = _T("");
// 		pwnd->GetDlgItem(IDC_EDIT_ADCDATARECSHOW)->SetWindowText(_T(""));
		OnSaveToFile();
	}
}
