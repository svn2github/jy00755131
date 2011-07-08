#include "StdAfx.h"
#include "SaveFile.h"
#include "Resource.h"
#include "Parameter.h"

CSaveFile::CSaveFile(void)
: m_csEditShow(_T(""))
, m_csSaveFile(_T(""))
, m_csSaveReceiveFile(_T(""))
, m_csSaveSendFile(_T(""))
, m_csSaveFilePath(_T(""))
, m_iSaveSize(0)
, m_bStartSave(false)
, m_pWndTab(NULL)
{
}

CSaveFile::~CSaveFile(void)
{
}

// 初始化
//************************************
// Method:    OnInit
// FullName:  CSaveFile::OnInit
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnInit(void)
{
	// 创建保存信息的文件夹
	m_csSaveFilePath += _T("\\数据备份");
	CreateDirectory(m_csSaveFilePath, NULL);
}

// 开始数据存储
//************************************
// Method:    OnSaveStart
// FullName:  CSaveFile::OnSaveStart
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnSaveStart(void)
{
	m_bStartSave = true;
}

// 停止数据存储
//************************************
// Method:    OnSaveStop
// FullName:  CSaveFile::OnSaveStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnSaveStop(void)
{
	m_bStartSave = false;
	CString strtemp = _T("");
	m_Sec_SavePortMonitorFrame.Lock();
	strtemp = m_csEditShow;
	m_Sec_SavePortMonitorFrame.Unlock();
	m_pWndTab->GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(strtemp);
	OnSaveToFile();
}

// 选择文件存储路径
//************************************
// Method:    OnSelectSaveFilePath
// FullName:  CSaveFile::OnSelectSaveFilePath
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnSelectSaveFilePath(void)
{
	wchar_t szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;

	bi.hwndOwner = m_hWnd;	// 指定父窗口，在对话框显示期间，父窗口将被禁用 
	bi.pidlRoot = NULL;				// 如果指定NULL，就以“桌面”为根 
	bi.pszDisplayName = szDir; 
	bi.lpszTitle = _T("请选择数据存储目录");	// 这一行将显示在对话框的顶端 
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
//************************************
// Method:    OnReset
// FullName:  CSaveFile::OnReset
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnReset(void)
{
	m_csEditShow.Empty();
	// 保存通讯文件
	m_csSaveFile.Empty();
	// 保存接收的数据到文件
	m_csSaveReceiveFile.Empty();
	// 保存发送的数据到文件
	m_csSaveSendFile.Empty();
	m_pWndTab->GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(_T(""));
}

// 保存到文件中
//************************************
// Method:    OnSaveToFile
// FullName:  CSaveFile::OnSaveToFile
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::OnSaveToFile(void)
{
	CString csSaveFileTemp = _T("");
	CString csSaveReceiveFileTemp = _T("");
	CString csSaveSendFileTemp = _T("");
	CString strFileName = _T("");
	CString str = _T("");
	SYSTEMTIME  sysTime;
//	errno_t err;
	m_Sec_SavePortMonitorFrame.Lock();
	csSaveFileTemp = m_csSaveFile;
	csSaveReceiveFileTemp = m_csSaveReceiveFile;
	csSaveSendFileTemp = m_csSaveSendFile;
	m_csSaveFile.Empty();
	m_csSaveReceiveFile.Empty();
	m_csSaveSendFile.Empty();
	m_Sec_SavePortMonitorFrame.Unlock();

	if (csSaveFileTemp.GetLength() == 0)
	{	
		return;
	}
	// 得到当前系统时间
	GetLocalTime(&sysTime);
	strFileName.Empty();
	strFileName += m_csSaveFilePath;
	str.Format(_T("\\%04d%02d%02d%02d%02d%02d%03d.text"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	strFileName += str;
	//	m_file = NULL;
	//保存成ANSI格式的文件
	// 	if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
	// 	{
	// 		if (m_file != NULL)
	// 		{
	// 			fprintf(m_file,_T("%s"),csSaveFileTemp); 
	// 			fclose(m_file);
	// 		}
	// 	}
	//保存成UNICODE格式的文件
	if(m_file.Open(strFileName, CFile::modeCreate|CFile::modeWrite) == TRUE)
	{
// 		CArchive ar(&m_file, CArchive::store);
// 		ar.WriteString(csSaveFileTemp);
// 		ar.Close();
		//因为需要保存的内容包含中文，所以需要如下的转换过程
		WriteCHToCFile(&m_file, csSaveFileTemp);
		m_file.Close();
	}
	// 保存接收到的数据到文件
	if (csSaveReceiveFileTemp.GetLength() != 0)
	{
		strFileName.Empty();
		strFileName += m_csSaveFilePath;
		str.Format(_T("\\Rec%04d%02d%02d%02d%02d%02d%03d.text"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
		strFileName += str;
		// 		m_file = NULL;
		// 		//保存成ANSI格式的文件
		// 		if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
		// 		{
		// 			if (m_file != NULL)
		// 			{
		// 				fprintf(m_file,_T("%s"),csSaveReceiveFileTemp); 
		// 				fclose(m_file);
		// 			}
		// 		}
		//保存成UNICODE格式的文件
		if(m_file.Open(strFileName, CFile::modeCreate|CFile::modeWrite) == TRUE)
		{
// 			CArchive ar(&m_file, CArchive::store);
// 			ar.WriteString(csSaveReceiveFileTemp);
// 			ar.Close();
			//因为需要保存的内容包含中文，所以需要如下的转换过程
			WriteCHToCFile(&m_file, csSaveReceiveFileTemp);
			m_file.Close();
		}
	}
	// 保存发送的数据到文件
	if (csSaveSendFileTemp.GetLength() != 0)
	{
		strFileName.Empty();
		strFileName += m_csSaveFilePath;
		str.Format(_T("\\Send%04d%02d%02d%02d%02d%02d%03d.text"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
		strFileName += str;
		// 		m_file = NULL;
		// 		//保存成ANSI格式的文件
		// 		if((err = fopen_s(&m_file,strFileName,"w+"))==NULL)
		// 		{
		// 			if (m_file != NULL)
		// 			{
		// 					fprintf(m_file,_T("%s"),csSaveSendFileTemp); 
		// 					fclose(m_file);
		// 			}
		// 		}
		//保存成UNICODE格式的文件
		if(m_file.Open(strFileName, CFile::modeCreate|CFile::modeWrite) == TRUE)
		{
// 			CArchive ar(&m_file, CArchive::store);
// 			ar.WriteString(csSaveSendFileTemp);
// 			ar.Close();
			//因为需要保存的内容包含中文，所以需要如下的转换过程
			WriteCHToCFile(&m_file, csSaveSendFileTemp);
			m_file.Close();
		}
	}
}

// 保存接收数据
//************************************
// Method:    OnSaveReceiveData
// FullName:  CSaveFile::OnSaveReceiveData
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned char * buf
// Parameter: int iRecLength
//************************************
void CSaveFile::OnSaveReceiveData(unsigned char* buf, int iRecLength)
{
	if (m_bStartSave == FALSE)
	{
		return;
	}

	CString str = _T("");
	CString strtemp = _T("");
	int icsSaveFileLength = 0;
	wchar_t buffer[_CVTBUFSIZE];

	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);

	str.Format(_T("%04d.%02d.%02d %02d:%02d:%02d:%03d 接收数据 数据包大小为 %d 数据为：\r\n"),sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, iRecLength);
	strtemp += str;

	for (int i=0; i<iRecLength; i++)
	{
		// 方法2：采用_stprintf_s函数的方法，CPU占用率达到10%
		_stprintf_s(buffer, _CVTBUFSIZE, _T("%02x "), buf[i]);
		strtemp += buffer;
// 		str.Format(_T("%02x "),buf[i]);
// 		strtemp += str;
	}

	strtemp += _T("\r\n");
	m_Sec_SavePortMonitorFrame.Lock();
	m_csSaveFile += strtemp;
	m_csSaveReceiveFile += strtemp;
	m_csEditShow += strtemp;
	icsSaveFileLength = m_csSaveFile.GetLength();
	m_Sec_SavePortMonitorFrame.Unlock();
	if (icsSaveFileLength> (m_iSaveSize*1024))
	{	
		OnSaveToFile();
	}
}

// 保存发送数据
//************************************
// Method:    OnSaveSendData
// FullName:  CSaveFile::OnSaveSendData
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned char * buf
// Parameter: int iSendLength
//************************************
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
	wchar_t buffer[_CVTBUFSIZE];

	str.Format(_T("%04d.%02d.%02d %02d:%02d:%02d:%03d 发送数据 数据包大小为 %d 数据为：\r\n"),sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, iSendLength);
	strtemp += str;

	for (int i=0; i<iSendLength; i++)
	{
		// 方法2：采用_stprintf_s函数的方法，CPU占用率达到10%
		_stprintf_s(buffer, _CVTBUFSIZE, _T("%02x "), buf[i]);
		strtemp += buffer;
// 		str.Format(_T("%02x "),buf[i]);
// 		strtemp += str;
	}

	strtemp += _T("\r\n");
	m_Sec_SavePortMonitorFrame.Lock();
	m_csSaveFile += strtemp;
	m_csSaveSendFile += strtemp;
	m_csEditShow += strtemp;
	icsSaveFileLength = m_csSaveFile.GetLength();
	m_Sec_SavePortMonitorFrame.Unlock();
	//	GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(m_cseditshow);
	// 用UpdateData(FALSE)快速反复刷新界面会占用大量资源，拖慢速度
	//	UpdateData(FALSE);
	if (icsSaveFileLength > (m_iSaveSize*1024))
	{
		OnSaveToFile();
	}
}

// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CSaveFile::ProcessMessages
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSaveFile::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}
