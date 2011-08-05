#pragma once
#include "Parameter.h"
class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);
	// 日志文件的存储路径
	CString m_csSaveLogFilePath;
	// 保存成CFile文件
	CFile m_SaveLogFile;
	// 保存文件
	CArchive* m_arFileSave;
public:
	// 打开程序运行日志文件
	void OnOpenLogFile(void);
	// 关闭程序运行日志文件
	void OnCloseLogFile(void);
	// 写程序运行的日志文件
	void OnWriteLogFile(CString csFuncName, CString csLogNews, unsigned int uiStatus);
};
