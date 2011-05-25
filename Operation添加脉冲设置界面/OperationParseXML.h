#pragma once
#include "ShotPoints.h"
#include "shotsources.h"
#include "ShotSpreads.h"
#include "ProcessTypes.h"
#include "ChannelList.h"
#include "..\common\FDUConfig.h"
/******************************************************************************
    类    型：
    
    功    能： 读取配置文件

    修改历史：

*******************************************************************************/
/**
 * @class COperationParseXML
 * @brief 解析配置文件
 *
 * @note 从施工配置文件（XML格式）中读取配置信息。主要包括放炮表、震源、采集排列、处理类型等
 */
class COperationParseXML
{
private:
	int ParseOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pVP);	
	int ParseSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int ParseSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc);
	int ParseSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int ParseSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc);
	int ParseProcessImpulsive(CXMLDOMElement* pElement,CProcessImpulsive* pProcess);
	// 解析辅助道
	int ParseProcessAuxi(CXMLDOMElement* pElement,CProcessType* pProcess);
	// 解析采集排列的一条记录
	int ParseShotSpreadRecord(CXMLDOMElement* pElement, CShotSpread *pSpread,CChannelList* pChannelList);
	bool AppendChannelsToShotSpread(DWORD dwLine,DWORD dwPointMin,DWORD dwPointMax,DWORD dwGain,CShotSpread *pSpread,CChannelList* pChannelList);
public:
	COperationParseXML(void);
	~COperationParseXML(void);
	// 解析采集站配置信息
	int ParseFDUConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig);
	// 解析放炮表
	int ParseOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints);
	// 解析震源
	int ParseSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	// 解析采集排列
	// int ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pShotSrcs);
	int ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pShotSrcs,CChannelList* pChannelList);
	// 解析数据处理对象
	int ParseProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses);
	// 获得系统配置信息
	bool ParseClientSetup(LPCTSTR pstrXMLFilePath);
	// 将字符串IP转化为DWORD型
	DWORD StringIPToDWORDIP(CString& strIP);

};
