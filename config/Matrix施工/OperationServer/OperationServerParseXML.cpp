#include "StdAfx.h"
#include "OperationServer.h"
#include "OperationServerParseXML.h"

COperationServerParseXML::COperationServerParseXML(void)
{
}

COperationServerParseXML::~COperationServerParseXML(void)
{
}
/**
 * @brief 字符串类型IP地址类型转换
 * @note  将字符串类型的IP地址转化为整数型
 * @param CString& strIP，IP地址字符串 
 * @return 整数型IP地址
 */
DWORD COperationServerParseXML::StringIPToDWORDIP(CString& strIP)
{	
	DWORD dwIP;
	DWORD  by1,by2,by3,by4;
	strIP.Replace(_T("."),_T(" "));
	_stscanf_s(LPCTSTR(strIP),_T("%d %d %d %d"),&by1,&by2,&by3,&by4);
	dwIP= by4;
	dwIP= dwIP<<8 | by3;
	dwIP= dwIP<<8 | by2;
	dwIP= dwIP<<8 | by1;
	return dwIP;
}
/**
 * @brief 读客户端的系统配置
 * @note  从配置文件中读取主机IP、主交叉站等信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 解析是否成功
 */
bool COperationServerParseXML::ParseServerSetup(LPCTSTR pstrXMLFilePath)
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

		// 找到施工服务器
		strKey =_T("OperationServer");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到施工服务器IP地址
		strKey =_T("HostIP");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		theApp.m_strHostIP = strTemp;
		theApp.m_dwHostIP = StringIPToDWORDIP(strTemp);
		// 得到数据存储路径
		//strKey =_T("RevFilePath");
		//theApp.m_strFilePath = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		//theApp.m_strFilePath +=_T("\\");

		// 找到主交叉站
		strKey =_T("MainCross");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到主交叉站IP地址
		strKey =_T("MainCrossIP");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);		
		theApp.m_dwMainCrossIP = StringIPToDWORDIP(strTemp);
		// 得到主交叉站端口
		strKey =_T("MainCrossPort");
		theApp.m_wMainCrossPort = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
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
 * @brief 写服务器的系统配置
 * @note  向施工服务程序的配置文件中写入主机IP等信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 解析是否成功
 */
bool COperationServerParseXML::WriteServerSetup(LPCTSTR pstrXMLFilePath)
{
	CString strKey;
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;	
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	
	try
	{
		// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到施工服务器
		strKey =_T("OperationServer");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 保存本机IP地址
		oVariant = theApp.m_strHostIP;
		oElement.setAttribute(_T("HostIP"), oVariant);
		// 接收数据存储的路径
		// oVariant = theApp.m_strFilePath;
		// oElement.setAttribute(_T("RevFilePath"), oVariant);

		/*
		// 找到交叉站配置
		strKey =_T("MainCross");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 保存主交叉站地址
		oVariant = inet_ntoa(theApp.m_dwMainCrossIP);
		oElement.setAttribute(_T("MainCrossIP"), oVariant);
		// 接收数据存储的路径
		//oVariant = theApp.m_strFilePath;
		//oElement.setAttribute(_T("RevFilePath"), oVariant);
		*/
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
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
 * @brief 最后的解析SEGD文件编号
 * @note  从配置文件中上次程序运行所记录的SEGD文件编号
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 成功返回上次程序运行时SEGD文件编号，失败返回0
 */
DWORD COperationServerParseXML::ParseLastUsedSEGDFileNumber(LPCTSTR pstrXMLFilePath)
{
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	DWORD		dwSEGDFIleNumber=0;
	try
	{	// 创建XML对象
		strKey =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到磁盘记录
		strKey =_T("DiskRecord");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到LastusedSEGDFileNumber
		strKey =_T("LastusedSEGDFileNumber");
		dwSEGDFIleNumber = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return dwSEGDFIleNumber;
	}
	catch (CException* )
	{
		return 0;
	}
}
/**
 * @brief 最后的解析SEGD文件编号
 * @note  从配置文件中上次程序运行所记录的SEGD文件编号
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 成功返回上次程序运行时SEGD文件编号，失败返回0
 */
bool COperationServerParseXML::WriteLastUsedSEGDFileNumber(LPCTSTR pstrXMLFilePath,DWORD dwSEGDFileNumber)
{
	CString strKey;
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;	
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	
	try
	{
		// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到施工服务器
		strKey =_T("DiskRecord");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 保存文件编号
		oVariant = (long)dwSEGDFileNumber;
		oElement.setAttribute(_T("LastusedSEGDFileNumber"), oVariant);
	
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
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
 * @brief 解析辅助道记录
 * @note  从配置文件中读取解析辅助道记录信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 成功返回1，失败返回0
 
int COperationServerParseXML::ParseAuxChannelRecord(CXMLDOMElement* pElement,CAuxChannel* pAuxChannel)
{
	CString strKey;
	if(!pAuxChannel)
		return 0;
	strKey = _T("Nb");
	pAuxChannel->m_dwNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Label");
	pAuxChannel->m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = _T("BoxType");
	pAuxChannel->m_dwBoxType = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("SN");
	pAuxChannel->m_dwSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = _T("ChannelNb");
	pAuxChannel->m_dwChannelNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Gain");
	pAuxChannel->m_dwGain = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("DPGNb");
	pAuxChannel->m_dwDPGNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Comments");
	pAuxChannel->m_strComments = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// IP地址
	pAuxChannel->m_dwIP = CIPList::GetAuxIP(pAuxChannel->m_dwNb);		
	return 1;

}*/
/**
 * @brief 读客户端的系统配置
 * @note  从配置文件中读取解析辅助道信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
  * @param CAuxChannels* pAllAuxes，辅助道数组对象
 * @return 辅助道总数
 
int COperationServerParseXML::ParseAuxChannel(LPCTSTR pstrXMLFilePath,CAuxChannels* pAllAuxes)
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
	try
	{
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到辅助道区域
		strKey =_T("AuxSetup");
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

		CAuxChannel* pAuxChannel = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pAuxChannel = new CAuxChannel;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 得到炮号
			ParseAuxChannelRecord(&oElement, pAuxChannel);		
			// 加入放炮表
			pAllAuxes->Add(pAuxChannel);
		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return dwCount;
	}	
	catch (CException* )
	{
		return false;
	}
}*/