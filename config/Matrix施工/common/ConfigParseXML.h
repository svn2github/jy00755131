#pragma once


#include "FDUConfig.h"
#include "DiskRecordConfig.h"
class CConfigParseXML
{
public:
	CConfigParseXML(void);
	~CConfigParseXML(void);

private:
 	
	bool  ParseFDUConfig(CXMLDOMDocument* pXMLDOMDocument,CFDUConfig* pFDUConfig);
public:


	bool  ParseCrewConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig);
	bool  WriteCrewConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig);


	bool  ParseUserInfo(LPCTSTR pstrXMLFilePath,CString&  strUserInfo);
	bool  WriteUserInfo(LPCTSTR pstrXMLFilePath,CString&  strUserInfo);

	bool  ParseDiskRecord(LPCTSTR pstrXMLFilePath,CDiskRecordConfig* pDiskRecord);
	bool  WriteDiskRecord(LPCTSTR pstrXMLFilePath,CDiskRecordConfig* pDiskRecord);
	// 解析配置程序中SPS文件设置参数
	bool ParseSPSFile(LPCTSTR pstrXMLFilePath, CString& strSPSRFilePath, CString& strSPSXFilePath);
	// 向配置文件中写入SPS文件设置参数
	bool WriteSPSFile(LPCTSTR pstrXMLFilePath, CString& strSPSRFilePath, CString& strSPSXFilePath);
};
