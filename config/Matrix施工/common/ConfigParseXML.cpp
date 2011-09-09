#include "StdAfx.h"
#include "ConfigParseXML.h"

CConfigParseXML::CConfigParseXML(void)
{
}

CConfigParseXML::~CConfigParseXML(void)
{

}
/**
 * @brief 读采集站配置参数
 * @note  从配置文件中读取采集站配置参数
 * @param CXMLDOMDocument* pXMLDOMDocument，XML文档指针 
 * @param CInstallParam* pFDUConfig, 安装参数类
 * @return 解析是否成功
 */
bool  CConfigParseXML::ParseFDUConfig(CXMLDOMDocument* pXMLDOMDocument,CFDUConfig* pFDUConfig)
{		
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	if(!pFDUConfig || !pXMLDOMDocument)
		return false;
	try
	{	
		// 找到入口
		strKey =_T("CrewSetup");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("CrewName");
		pFDUConfig->m_strCrewName= CXMLDOMTool::GetElementAttributeString(&oElement, strKey);

		// 找到入口
		strKey =_T("samplerate");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwSampleRateIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterFIR");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterFIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterIIR");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterIIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Gain3301");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwGain3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mux3301");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMux3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mode5373");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMode5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Attr5373");
		lpDispatch = pXMLDOMDocument->getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwAttr5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 读配置程序的设置参数
 * @note  从配置文件中读取配置程序的配置参数
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CInstallParam* pFDUConfig, 安装参数类
 * @return 解析是否成功
 */
bool  CConfigParseXML::ParseCrewConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig)
{
	if(!pFDUConfig)
		return false;
	try
	{	
		COleException oError;
		COleVariant oVariant;
		CString strTemp;
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.load(oVariant);

		ParseFDUConfig(&oXMLDOMDocument,pFDUConfig);
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 存储配置程序中采集站的参数设置信息
 * @note  向配置程序的配置文件中写入采集站参数信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CFDUConfig* pFDUConfig, 输出参数，采集站设置参数
 * @return 解析是否成功
 */
bool  CConfigParseXML::WriteCrewConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig)
{
	if(!pFDUConfig)
		return false;
	CString strKey;
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	VARIANT		vtVal;
	try
	{		
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strTemp, &oError);
		
		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.load(oVariant);
		
		// 找到Crew设置
		strKey =_T("CrewSetup");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);		
		oVariant = pFDUConfig->m_strCrewName;
		oElement.setAttribute(_T("CrewName"), oVariant);

		// 找到samplerate设置
		strKey =_T("samplerate");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwSampleRateIndex;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);
		
		// 找到Gain3301设置
		strKey =_T("Gain3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);	
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwGain3301Index;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);
		// 找到FilterFIR设置
		strKey =_T("FilterFIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwFilterFIRIndex;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);

		// 找到FilterIIR设置
		strKey =_T("FilterIIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);	
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwFilterIIRIndex;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);
		// 找到Mux3301设置
		strKey =_T("Mux3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);		
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwMux3301Index;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);
		// 找到Mode5373设置
		strKey =_T("Mode5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);		
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwMode5373Index;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);
		// 找到Attr5373设置
		strKey =_T("Attr5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);		
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		vtVal.vt = VT_UI4;
		vtVal.ulVal = pFDUConfig->m_dwAttr5373Index;
		oVariant = vtVal;
		oElement.setAttribute(_T("Index"), oVariant);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 解析配置程序中用户信息设置参数
 * @note  解析配置程序中用户信息设置参数
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CString&  strUserInfo, 输出参数，返回用户设置信息
 * @return 解析是否成功
 */
bool  CConfigParseXML::ParseUserInfo(LPCTSTR pstrXMLFilePath,CString&  strUserInfo)
{	
	try
	{	
		CXMLDOMNodeList oNodeList;
		CXMLDOMElement oElement;
		LPDISPATCH lpDispatch;	

		COleException oError;
		COleVariant oVariant;
		CString strKey;
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oXMLDOMDocument.load(oVariant);
		
		// 找到入口
		strKey =_T("UserInfo");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Text");
		strUserInfo= CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}

/**
 * @brief 写用户信息参数
 * @note  向XML配置文件中写入用户信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CString&  strUserInfo, 用户信息参数
 * @return 解析是否成功
 */
bool  CConfigParseXML::WriteUserInfo(LPCTSTR pstrXMLFilePath,CString&  strUserInfo)
{
	CString strKey;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	try
	{		
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.load(oVariant);

		// 找到Crew设置
		strKey =_T("UserInfo");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);		
		oVariant = strUserInfo;
		oElement.setAttribute(_T("Text"), oVariant);
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 解析配置程序中存盘设置参数
 * @note  解析配置程序中存盘设置参数
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CDiskRecordConfig* pDiskRecord, 输出参数，返回存盘的目录等
 * @return 解析是否成功
 */
bool  CConfigParseXML::ParseDiskRecord(LPCTSTR pstrXMLFilePath,CDiskRecordConfig* pDiskRecord)
{
	try
	{	
		CXMLDOMNodeList oNodeList;
		CXMLDOMElement oElement;
		LPDISPATCH lpDispatch;	

		COleException	oError;
		COleVariant		oVariant;
		CString			strKey;
		CString         strTemp;
		int             nTemp;
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oXMLDOMDocument.load(oVariant);

		// 找到入口
		strKey =_T("DiskRecord");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// Mode
		strKey =_T("Mode");
	    strTemp= CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		if(strTemp.CompareNoCase(_T("Advanced"))==0)
		{
			pDiskRecord->SetDiskRecordMode(1);
		}
		else
		{
			pDiskRecord->SetDiskRecordMode(0);
		}
		// BackupSettings
		strKey =_T("BackupSettings");
		nTemp= CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		pDiskRecord->SetBackupSetting(nTemp);
		// FileDirectory
		strKey =_T("FileDirectory");
		strTemp= CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pDiskRecord->SetFilePath(strTemp);

		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 写存盘配置信息
 * @note  向XML配置文件中写入存盘配置信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CDiskRecordConfig* pDiskRecord, 存盘配置信息参数
 * @return 解析是否成功
 */
bool  CConfigParseXML::WriteDiskRecord(LPCTSTR pstrXMLFilePath,CDiskRecordConfig* pDiskRecord)
{

	CString strKey;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	try
	{		
		CXMLDOMDocument oXMLDOMDocument;
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		oXMLDOMDocument.CreateDispatch(strKey, &oError);
		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.load(oVariant);

		// 找到DiskRecord设置
		strKey =_T("DiskRecord");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// Mode
		if(pDiskRecord->GetDiskRecordMode())
		{
			strKey = _T("Advanced");
		}
		else
		{
			strKey = _T("Standard");
		}
		oVariant = strKey;
		oElement.setAttribute(_T("Mode"), oVariant);
		// BackupSettings
		oVariant = (long)(pDiskRecord->GetBackupSetting());
		oElement.setAttribute(_T("BackupSettings"), oVariant);
		// FileDirectory
		oVariant = pDiskRecord->GetFilePath();
		oElement.setAttribute(_T("FileDirectory"), oVariant);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}