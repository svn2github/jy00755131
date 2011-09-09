#pragma once
#include "ShotPoints.h"
#include "shotsources.h"
#include "ShotSpreads.h"
#include "ProcessTypes.h"
#include "ShotComment.h"

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
	int ParseCommentRecord(CXMLDOMElement* pElement,CShotComment* pComment);
	int SaveCommentRecord(CXMLDOMElement* pElement,CShotComment* pComment);

	int ParseOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pVP);
	int SaveOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pVP);

	int ParseSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int ParseSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc);
	int SaveSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int SaveSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc);

	int ParseSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int ParseSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc);
	int SaveSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	int SaveSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc);
	// 解析测线定义的辅助道记录
	int ParseAuxChannelRecord(CXMLDOMElement* pElement,CAuxChannel* pAuxChannel);	
	// 脉冲处理类型
	int ParseProcessImpulsive(CXMLDOMElement* pElement,CProcessImpulsive* pProcess);
	int SaveProcessImpulsive(CXMLDOMDocument* pXMLDOMDocument,CXMLDOMElement* pElement,CProcessImpulsive* pProcess);
	// 解析处理类型辅助道
	int ParseProcessAuxi(CXMLDOMElement* pElement,CProcessType* pProcess);
	int SaveProcessAuxi(CXMLDOMDocument* pXMLDOMDocument,CXMLDOMElement* pElement,CProcessType* pProcess);
	// 解析采集排列的一条记录
	int ParseShotSpreadRecord(CXMLDOMElement* pElement, CShotSpread *pSpread,CChannelList* pChannelList);
	bool AppendChannelsToShotSpread(DWORD dwLine,DWORD dwPointMin,DWORD dwPointMax,DWORD dwGain,CShotSpread *pSpread,CChannelList* pChannelList);
public:
	COperationParseXML(void);
	~COperationParseXML(void);
	// 解析采集站配置信息
	// bool ParseFDUConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig);
	// 
	int ParseComments(LPCTSTR pstrXMLFilePath,CShotComments* pComments);
	int SaveComments(LPCTSTR pstrXMLFilePath,CShotComments* pComments);
	// 解析放炮表
	int ParseOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints);
	// 保存放炮表
	int SaveOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints);
	// 解析震源
	int ParseSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	// 保存震源
	int SaveSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs);
	// 解析辅助道
	int ParseAuxChannel(LPCTSTR pstrXMLFilePath,CAuxChannels* pAllAuxes);

	// 解析采集排列
	// int ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pShotSrcs);
	int ParseAllAbsoluteSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pShotSrcs,CChannelList* pChannelList);
	// 解析数据处理对象
	int ParseProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses);
	int SaveProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses);
	// 获得系统配置信息
	//bool ParseClientSetup(LPCTSTR pstrXMLFilePath);
	
};
