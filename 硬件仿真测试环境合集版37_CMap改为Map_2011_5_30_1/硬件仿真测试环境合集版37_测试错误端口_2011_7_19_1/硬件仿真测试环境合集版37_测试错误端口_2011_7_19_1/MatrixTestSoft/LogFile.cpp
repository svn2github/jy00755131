#include "StdAfx.h"
#include "LogFile.h"

CLogFile::CLogFile(void)
: m_csSaveLogFilePath(_T(""))
{
}

CLogFile::~CLogFile(void)
{
}

// 打开程序运行日志文件
void CLogFile::OnOpenLogFile(void)
{
	SYSTEMTIME  sysTime;
	CString str = _T("");
	str = _T("\\程序运行日志文件.log");
	m_csSaveLogFilePath += str;
	//保存成UNICODE格式的文件
	if(m_SaveLogFile.Open(m_csSaveLogFilePath, CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox(_T("程序运行日志文件创建失败！"));	
		return;
	}

	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日%02d:%02d:%02d:%03d开始记录程序运行日志：\r\n"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);

	//因为需要保存的内容包含中文，所以需要如下的转换过程
	WriteCHToCFile(&m_SaveLogFile, str);
}

// 关闭程序运行日志文件
void CLogFile::OnCloseLogFile(void)
{
	if (m_SaveLogFile.m_hFile != CFile::hFileNull)
	{
		m_SaveLogFile.Close();
	}
}

// 写程序运行的日志文件
void CLogFile::OnWriteLogFile(CString csFuncName, CString csLogNews, unsigned int uiStatus)
{
	SYSTEMTIME  sysTime;
	CString str = _T("");
	CString strOutput = _T("");
	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日 %02d:%02d:%02d:%03d\t"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	if (uiStatus == SuccessStatus)
	{
		strOutput += _T("Success\t");
	}
	else if (uiStatus == WarningStatus)
	{
		strOutput += _T("Warning\t");
	}
	else if (uiStatus == ErrorStatus)
	{
		strOutput += _T("Error\t");
	}
	strOutput += str;
	strOutput += _T("程序运行到函数：") + csFuncName + _T("\t");
	strOutput += _T("日志信息：") + csLogNews + _T("\r\n");
	//因为需要保存的内容包含中文，所以需要如下的转换过程
 	WriteCHToCFile(&m_SaveLogFile, strOutput);
}
