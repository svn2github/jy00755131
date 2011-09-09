#pragma once
#include "..\common\LineStruct.h"
#include "..\common\OperaStruct.h"

class COperationServerParseXML
{
public:
	COperationServerParseXML(void);
	~COperationServerParseXML(void);

private:
	// 解析辅助道记录
	//int ParseAuxChannelRecord(CXMLDOMElement* pElement,CAuxChannel* pAuxChannel);	
public:
	// 获得系统配置信息
	bool ParseServerSetup(LPCTSTR pstrXMLFilePath);
	// 将字符串IP转化为DWORD型
	static DWORD StringIPToDWORDIP(CString& strIP);
	// 将服务器配置保存到XML文件中
	bool WriteServerSetup(LPCTSTR pstrXMLFilePath);

	// 解析辅助道
	//int ParseAuxChannel(LPCTSTR pstrXMLFilePath,CAuxChannels* pAllAuxes);
	// 得到程序上一次运行时最后记录的SEGD文件编号
	DWORD ParseLastUsedSEGDFileNumber(LPCTSTR pstrXMLFilePath);
	// 将当前的SEGD文件编号保存到XML文件中，一般在程序退出时调用
	bool WriteLastUsedSEGDFileNumber(LPCTSTR pstrXMLFilePath,DWORD dwSEGDFileNumber);
};
