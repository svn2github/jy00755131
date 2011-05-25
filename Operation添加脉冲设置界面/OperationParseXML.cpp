#include "StdAfx.h"
#include "Operation.h"
#include "OperationParseXML.h"
#include "../common/LineStruct.h"

/**
 * @brief COperationParseXML构造函数
 */
COperationParseXML::COperationParseXML(void)
{
}
/**
 * @brief COperationParseXML析构函数
 */
COperationParseXML::~COperationParseXML(void)
{

}
DWORD COperationParseXML::StringIPToDWORDIP(CString& strIP)
{
	DWORD  by1,by2,by3,by4;
	DWORD dwIP;
	strIP.Replace(_T("."),_T(" "));
	_stscanf_s(LPCTSTR(strIP),_T("%d%d%d%d"),&by1,&by2,&by3,&by4);
	dwIP= by4;
	dwIP= dwIP<<8 | by3;
	dwIP= dwIP<<8 | by2;
	dwIP= dwIP<<8 | by1;
	return dwIP;
}
/**
 * @brief 读客户端的系统配置
 * @note  从配置文件中读取客户端的主机IP等信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 解析是否成功
 */
bool COperationParseXML::ParseClientSetup(LPCTSTR pstrXMLFilePath)
{
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	try
	{	// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到放炮表区域
		strKey =_T("OperationClient");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到客户端IP地址
		strKey =_T("HostIP");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);		
		// theApp.m_dwHostIP = StringIPToDWORDIP(strTemp);
		theApp.m_strHostIP = strTemp;
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 解析采集站配置信息
 * @note  从XML配置文件中解析出采集站的基本配置信息，主要
 包括：前置放大、模拟门选择、5373模式、DA衰减、采样率、IIR选择、FIR选择
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CFDUConfig* pFDUConfig，配置对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseFDUConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig)
{
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	if(!pFDUConfig)
		return -1;
	try
	{	// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到入口
		strKey =_T("samplerate");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwSampleRateIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterFIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterFIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterIIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterIIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Gain3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwGain3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mux3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMux3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mode5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMode5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Attr5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwAttr5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}

}
/**
 * @brief 解析放炮表
 * @note  从XML的配置文件中读取出整个放炮表，放入放炮表对象pShotPoints中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotPoints* pShotPoints，放炮表对象，也是存储炮号的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到放炮表区域
	strKey =_T("OperationTable");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CShotPoint* pVP = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{
		pVP = new CShotPoint;
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 得到炮号
		ParseOperationTableRecord(&oElement, pVP);		
		// 加入放炮表
		pShotPoints->Add(pVP);
		
	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	//CoUninitialize();
	return dwCount;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：

			每条记录的XML格式如下：
			<Record VPStatus="0" ShotID="1" BreakPoint="N" SwathNb ="1" SourcePointIndex ="1"
			SourceLine = "1" SourceReceiver = "1" SpreadType ="Absolute" SpreadSFL ="" SpreadSFN ="" 
			SuperSpread="" ProcessTypeID ="1" Comments ="注释"/>

    修改历史：



*******************************************************************************/
int COperationParseXML::ParseOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pVP)
{
	CString strKey;	// 键名	
	if(NULL==pVP)
		return 0;
	// 放炮状态
	strKey = _T("VPStatus");
	pVP->m_VPState =(unsigned char)CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 炮号
	strKey =_T("ShotID");
	pVP->m_dwShotNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	//  
	strKey =_T("BreakPoint");
	strKey= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	if(strKey.CompareNoCase(_T("Y")))
		pVP->m_byBreakPoint = 0;
	else
		pVP->m_byBreakPoint = 1;
	// 
	strKey =_T("SwathNb");
	pVP->m_dwSwathNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SourcePointIndex");
	pVP->m_dwSourcePointIndex= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SourceLine");
	pVP->m_fSourceLine = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	// 
	strKey =_T("SourceReceiver");
	pVP->m_fSourceNb= CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	// 
	strKey =_T("SpreadSFL");
	pVP->m_dwSpreadSFL = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SpreadSFN");
	pVP->m_dwSpreadSFN= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	// 
	strKey =_T("SpreadType");
	pVP->m_dwSpreadNb= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	/*
	strKey =_T("SpreadType");
	strKey= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	if(strKey.CompareNoCase(_T("Absolute")))
		pVP->m_bySpreadType= SPREADTYPE_GENERIC;
	else
		pVP->m_byBreakPoint = SPREADTYPE_ABSOLUTE;
	*/
	// 
	strKey =_T("ProcessTypeID");
	pVP->m_dwProcessNb= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("Comments");
	pVP->m_szComments= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	return 1;
}
/**
 * @brief 解析震源表
 * @note  从XML的配置文件中读取出震源表，放入对象pShotSrcs中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSources* pShotSrcs，震源表对象，也是存储震源的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{

	return ParseSourceExplo(pstrXMLFilePath,pShotSrcs)+ParseSourceVibro(pstrXMLFilePath,pShotSrcs);
}
/******************************************************************************
    函    数：ParseSourceExplo
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析脉冲爆炸震源

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到脉冲震源
	strKey =_T("SourceExplo");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CShotSourceExplo* pSrc = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{
		pSrc = new CShotSourceExplo;
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 解析震源
		ParseSourceExploRecord(&oElement, pSrc);		
		// 加入震源表
		pShotSrcs->Add(pSrc);

	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	//CoUninitialize();
	return dwCount;

}
/******************************************************************************
    函    数：ParseSourceExploRecord
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析单条震源记录

				<Record SourceStatus="0" SourceNb ="1" Label="label1" 
				ShooterNb="123456" IncrNb="1" Comment="SourceExplo震源描述" />
    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc)
{
	CString strKey;	// 键名	
	if(NULL==pExploSrc)
		return 0;
	// 状态
	strKey = _T("SourceStatus");
	pExploSrc->m_bySourceState =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 编号
	strKey = _T("SourceNb");
	pExploSrc->m_dwSourceNb =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pExploSrc->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 所连采集站SN
	strKey = _T("ShooterNb");
	pExploSrc->m_dwShooterNb =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("IncrNb");
	pExploSrc->m_lStep =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// Comment
	strKey = _T("Comment");
	pExploSrc->m_strComment =CXMLDOMTool::GetElementAttributeString(pElement, strKey);	
	return 1;
}
/******************************************************************************
    函    数：ParseSourceVibro
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析可控震源

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到脉冲震源
	strKey =_T("SourceVibro");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CShotSourceVibro* pSrc = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{
		pSrc = new CShotSourceVibro;
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 解析震源
		ParseSourceVibroRecord(&oElement, pSrc);		
		// 加入震源表
		pShotSrcs->Add(pSrc);

	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	//CoUninitialize();
	return dwCount;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析单条可控震源的记录

	<Record SourceStatus="0" SourceNb ="1" Label="label1" ShooterNb="12345" 
	Moving="1" Step="1" WorkByAcq="0" ClusterNb="1" Comment="SourceVibro震源描述" />

	修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc)
{
	CString strKey;	// 键名	
	if(NULL==pVibroSrc)
		return 0;
	// 状态
	strKey = _T("SourceStatus");
	pVibroSrc->m_bySourceState =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 编号
	strKey = _T("SourceNb");
	pVibroSrc->m_dwSourceNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pVibroSrc->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 所连采集站SN
	strKey = _T("ShooterNb");
	pVibroSrc->m_dwShooterNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("Step");
	pVibroSrc->m_lStep =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// Comment
	strKey = _T("Comment");
	pVibroSrc->m_strComment =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("Moving");
	pVibroSrc->m_byMoving =(unsigned char)CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 叠加的多次采集
	strKey = _T("WorkByAcq");
	if(CXMLDOMTool::GetElementAttributeInt(pElement, strKey))
	    pVibroSrc->m_bWorkByAcq = true;
	else
		pVibroSrc->m_bWorkByAcq = false;
	// 同时双震源作业
	strKey = _T("ClusterNb");
	pVibroSrc->m_dwClusterNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	
	return 1;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 读取采集排列 ????????????????????

    修改历史： 20100729: 当前是测试代码，读取10*150的采集排列，后续要修改

*******************************************************************************/
/**
 * @brief 解析采集排列表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @return 读取的对象个数

int COperationParseXML::ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pSpreads)
{
	CShotSpread *pSpread = new CShotSpread;

	pSpread->m_dwNb = 1;
	int i,j;
	SHOTSPREAD    ShotSpread;
	for (i=1;i<=10;i++) // 大线数量
	{
		for (j=11;j<=20;j++) // 点号
		{
			ShotSpread.m_dwIP = CIPList::GetCollectIP(i,j,1);
			ShotSpread.m_dwGain = 1;
			pSpread->m_arrSpread.Add(ShotSpread);
		}
	}
	pSpreads->m_AllSpread.Add(pSpread);
	return 1;
} */
/**
 * @brief 解析采集排列表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。由于采集排列的定义中
 仅仅说明了线号、点号，没有定义道号；因此需要从2万道的记录中依据线号、点号来找出可能的所有采集道，
 将采集道添加到采集排列中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @param CChannelList* pChannelList，所有的采集道列表
 * @return 读取的对象个数
 */
int COperationParseXML::ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pSpreads,CChannelList* pChannelList)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到脉冲震源
	strKey =_T("AbsoluteSetup");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CShotSpread *pSpread = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{
		pSpread= new CShotSpread;
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 解析采集排列
		ParseShotSpreadRecord(&oElement, pSpread,pChannelList);		
		// 加入震源表
		pSpreads->Add(pSpread);

	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	return dwCount;
}

int COperationParseXML::ParseShotSpreadRecord(CXMLDOMElement* pElement, CShotSpread *pSpread,CChannelList* pChannelList)
{
	CString			strKey;	// 键名	
	CString         strTemp;
	CString			strSpread;
	LPTSTR			lpStr;
	int				nPos;
	DWORD           dwLineNb,dwPointMin,dwPointMax;
	DWORD			dwGain;
	
	if(NULL==pSpread|| NULL==pChannelList)
		return 0;
	// 编号
	strKey = _T("Nb");
	pSpread->m_dwNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pSpread->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 采集排列
	strKey = _T("Spread");
	strSpread =CXMLDOMTool::GetElementAttributeString(pElement, strKey);

	// 解析采集排列，示例为： strSpread="10:101-103g1,106-115g2;20:101-103g1,106-115g2;30:101-103g1,106-115g2"
	while(strSpread!=_T(""))
	{
		nPos = strSpread.Find(_T(";"));
		if(nPos>0)
		{
			strKey = strSpread.Left(nPos);
			strSpread = strSpread.Right(strSpread.GetLength()-nPos-1);
		}
		else
		{
			strKey = strSpread;
			strSpread = _T("");
		}
		
		// 下面解析10:101-103g1,106-115g2
		// 解析出行号		
		dwLineNb = _tcstoul(LPCTSTR(strKey),&lpStr,10);
		// 此时lpStr = :101-103g1,106-115g2
		strKey = lpStr+1;	
		do // 解析完某一条测线的所有点号定义
		{		
			// 解析出点号的最小值
			dwPointMin= _tcstoul(LPCTSTR(strKey),&lpStr,10);
			// lpStr = -103g1,106-115g2
			strKey = lpStr+1;
			// 解析出点号的最大值
			dwPointMax= _tcstoul(LPCTSTR(strKey),&lpStr,10);
			// lpStr = g1,106-115g2
			strTemp = lpStr;	
			// 解析出增益
			strKey = strTemp.Left(2);
			if(!strKey.CompareNoCase(_T("g1")))
			{
				dwGain = 1600;
			}
			else
			{
				dwGain = 400;
			}
			// 对该条测线范围dwPointMin - dwPointMax之间测点读入内存
			AppendChannelsToShotSpread(dwLineNb, dwPointMin, dwPointMax,dwGain, pSpread, pChannelList);
			// 判断是否结束
			nPos= strTemp.Find(_T(","));
			// 找不到“，”，则表示该大线已经解析完成，退出解析下一条大线
			if (nPos<=0)
			{	
				break;
			}
			// 获得后面的点号定义，例如 106-115g2
			strKey = strTemp.Right(strTemp.GetLength()-nPos-1);

		} while (true);	
		
	}	
	return 1;	
}
bool COperationParseXML::AppendChannelsToShotSpread(DWORD dwLine,DWORD dwPointMin,DWORD dwPointMax,DWORD dwGain,
													CShotSpread *pSpread,CChannelList* pChannelList)
{
	DWORD			dwIndex;
	DWORD			dwIP;
	// DWORD           dwTemp;
	SHOTSPREAD		ShotSpread;
	CChannelSetupData* pChnData=NULL;
	if(!pSpread || !pChannelList)
		return false;
	// 防止定义的点号最大值小于最小值
	if(dwPointMin>dwPointMax)
	{	// 将最大值和最小值交换
		dwIndex=dwPointMin;
		dwPointMin = dwPointMax;
		dwPointMax = dwIndex;
	}
	for (dwIndex=dwPointMin;dwIndex<=dwPointMax;dwIndex++)
	{
		// 线号
		ShotSpread.m_dwLineNb = dwLine;
		// 点号
		ShotSpread.m_dwPointNb = dwIndex;
		// 增益
		ShotSpread.m_dwGain = dwGain;
		// 先找第一道的位置
		dwIP = CChannelSetupData::CreateIP(dwLine,dwIndex,1);
		if(!pChannelList->GetChannelFromMap(dwIP,pChnData))
		{
			continue;
		}
		// 找到第一道的对象，记录编号
		ShotSpread.m_dwChannelNb = 1;
		// IP地址
		ShotSpread.m_dwIP = dwIP;
		// 判断采集道类型
		if(pChnData->m_bMute)
			ShotSpread.m_byType = CHN_MUTE;
		else
			ShotSpread.m_byType = CHN_NORMAL;
		// 如果不是跳过道
		if(!pChnData->m_bJumped)
			pSpread->Append(ShotSpread);
		
		// 寻找是否存在
		while(pChnData!=NULL)
		{
			// 依次向采集道数组后面查找是否相同的测线、测点，而属于不同的道号对象
			pChnData = pChannelList->GetChannelByIndex(pChnData->m_uiIndex+1);
			if (pChnData==NULL)
			{
				break;
			}
			// 找到新的采集道对象，判断测线和点号是否一致
			if(pChnData->m_uiNbLine!=dwLine || pChnData->m_uiNbPoint != dwIndex)
				break;
			// 跳过道
			if(pChnData->m_bJumped)
				continue;
			// 采集道号
			ShotSpread.m_dwChannelNb =pChnData->m_uiNbChannel;
			// IP地址
			ShotSpread.m_dwIP = pChnData->m_uiIP;
			// 判断采集道类型
			if(pChnData->m_bMute)
				ShotSpread.m_byType = CHN_MUTE;
			else
				ShotSpread.m_byType = CHN_NORMAL;
			pSpread->Append(ShotSpread);
		}
	}
	return true;
}
/**
 * @brief 解析数据类型表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses)
{
	CString			strOLEObject;
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;	
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElement;
	LPDISPATCH		lpDispatch;
	ULONG			dwCount;
	CString			strKey;
	BYTE            byType;			
	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到脉冲震源
	strKey =_T("ProcessType");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CProcessType* pProcess = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{		
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		strKey =_T("Type");	// 得到处理对象的类型，根据类型创建不同对象
		byType = (BYTE)CXMLDOMTool::GetElementAttributeInt(&oElement,strKey);
		switch(byType )
		{
		case PROCESS_IMPULSIVE:
			pProcess = new CProcessImpulsive;
			ParseProcessImpulsive(&oElement,(CProcessImpulsive*)pProcess);
			pProcesses->Add(pProcess);// 加入震源表
			break;
		default:
			break;
		}		
	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	//CoUninitialize();
	return dwCount;

}
/****************************************************************************************
    函    数：ParseProcessAuxi
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：解析辅助道

			<Auxiliaries Count="1"  CorreWith = "Pilot" >	
				<Record AuxiNb ="1" AuxiProcessing="Aux1*Aux2"  />
			</Auxiliaries>    
    修改历史：	   

*****************************************************************************************/
int COperationParseXML::ParseProcessAuxi(CXMLDOMElement* pElement,CProcessType* pProcess)
{
	CXMLDOMNodeList oNodeList;	
	CXMLDOMElement	oElement;
	LPDISPATCH		lpDispatch;
	ULONG			dwCount;
	CString			strKey;
	if(NULL==pProcess || NULL==pElement)
		return 0;

	lpDispatch = pElement->get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	// CProcessType* pProcess = NULL;
	AUXICHANNEL  AuxiCHN;
	for(unsigned int i = 0; i < dwCount; i++)
	{		
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		AuxiCHN.m_dwNb = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, _T("AuxiNb"));
		strKey = CXMLDOMTool::GetElementAttributeString(&oElement, _T("AuxiProcessing"));
		_tcscpy_s(AuxiCHN.m_szProcessing,256,strKey);
		pProcess->m_arrAuxiChannel.Add(AuxiCHN);
	}
	return dwCount;
}
/******************************************************************************
    函    数：ParseProcessImpulsive
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析脉冲类型, 其中 类型 Type ="0" 

	<ProcessRecord Nb="1"  Label ="record" Type ="0" ProcessSetup = "0" SerialNb = "12345"  RecordLength="7" RefractingDelay="1000" TBWindow="200">      		
		<Auxiliaries Count="1"  CorreWith = "Pilot" >	
			<Record AuxiNb ="1" AuxiProcessing="Aux1*Aux2"  />
		</Auxiliaries>      	        	   	
	</ProcessRecord>

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseProcessImpulsive(CXMLDOMElement* pElement,CProcessImpulsive* pProcess)
{
	CString strKey;	// 键名	
	if(NULL==pProcess)
		return 0;
	// 编号
	strKey = _T("Nb");
	pProcess->m_dwProcessNb=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 类型
	strKey = _T("Type");
	pProcess->m_byProcessType =(BYTE)CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pProcess->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 设置：标准、高级
	strKey = _T("ProcessSetup");
	pProcess->m_ProcessSetup =(BYTE)CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 爆炸机控制器所连采集站SN
	strKey = _T("SerialNb");
	pProcess->m_dwFDUSN =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 记录长度
	strKey = _T("RecordLength");
	pProcess->m_dwRecordLen =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// Refraction Delay  折射延迟
	strKey = _T("RefractingDelay");
	pProcess->m_dwRefractionDelay=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
    // TB Window
	strKey = _T("TBWindow");
	pProcess->m_dwTBWindow=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 辅助道
	ParseProcessAuxi( pElement,pProcess);
	return 1;
}