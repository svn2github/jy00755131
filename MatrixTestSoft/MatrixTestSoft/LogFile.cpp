#include "StdAfx.h"
#include "LogFile.h"
#include "Parameter.h"

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
	m_SaveLogFile.Close();
}

// 写程序运行的日志文件
void CLogFile::OnWriteLogFile(CString csFuncName, CString csLogNews)
{
	CString str = _T("");

}
