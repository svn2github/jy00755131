#pragma once

#include "..\common\CLogBase.h"
/**
 * @class CLogFile
 * @brief 写日志文件类
 *
 * @note 封装日志记录存储类，包括创建文件、关闭文件、写文件等操作。
 * 
 */
class CLogFile
{
private:
	FILE* 		m_pLogFile;
	CString		m_strLogFile;
	DWORD		m_dwLogCount;
public:
	CLogFile(void);
	~CLogFile(void);
	bool	CreateLogFile(void);
	bool	CloseLogFile(void);
	bool	WriteLog(CLogBase* pLog);
};
