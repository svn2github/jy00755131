#pragma once


#include "InstallParam.h"
class CInstallParseXML
{
public:
	CInstallParseXML(void);
	~CInstallParseXML(void);

public:
	bool  ParseInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam);
	bool  WriteInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam);
};
