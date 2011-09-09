#include "StdAfx.h"
#include "LogFile.h"
#include "AdminServer.h"

CLogFile::CLogFile(void)
: m_dwLogCount(0)
{
	CreateLogFile();
}

CLogFile::~CLogFile(void)
{
	CloseLogFile();
}
/** 
 * @brief 创建日志文件
 * @note 创建日志文件
 * @param void
 * @return bool ,创建文件是否成功
*/
bool	CLogFile::CreateLogFile(void)
{
	SYSTEMTIME   sysTime;
	GetLocalTime(&sysTime);	
	// 生成日志文件名
	m_strLogFile.Format(_T("%s%04d%02d%02d%02d%02d%02d.Log"),SERVERDIR_LOGFILE,sysTime.wYear,sysTime.wMonth,sysTime.wDay,
															sysTime.wHour,sysTime.wMinute,sysTime.wSecond);	
	// 创建日志文件
	if(!_tfopen_s(&m_pLogFile,m_strLogFile,_T("a")))
		return true;
	else
		return false;
}
/** 
 * @brief 关闭日志文件
 * @note 关闭日志文件
 * @param void
 * @return bool ,关闭文件是否成功
*/
bool	CLogFile::CloseLogFile(void)
{
	if(m_pLogFile)
		fclose(m_pLogFile);
	m_pLogFile = NULL;
	return true;
}
/** 
 * @brief 写日志信息
 * @note 将一条日志写入文件中，先调用日志的GetLog函数生成一条文本记录，在写入文件。
 * @param CLogBase* pLog，需要保存到饿日志。
 * @return bool ,写入是否成功
*/
bool CLogFile::WriteLog(CLogBase* pLog)
{
	if(!m_pLogFile || !pLog)
	{		
		return false;
	}
	CString  strLog;
	// 生成日志文本记录
	pLog->GetAllAttributes(strLog);
	// 写日志
	_ftprintf( m_pLogFile, _T("%s\n"), strLog);
	// 如果已经保存的记录超过十万条，重新创建新文件
	if(++m_dwLogCount>=100000)
	{
		CloseLogFile();
		CreateLogFile();
	}
	return true;
}