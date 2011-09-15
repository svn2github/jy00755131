#pragma once

#include "XMLFileNameParam.h"
/**
 * @class CStartParseXML
 * @brief 解析Start程序的XML配置文件
 *
 * 
 */
class CStartParseXML
{
public:
	CStartParseXML(void);
	~CStartParseXML(void);
 
public:
	bool  ParseClientComputer(LPCTSTR pstrFilePath,CString& strIP,CString& strFTPIP);
	// bool  WriteInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam);
};
