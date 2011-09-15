#include "StdAfx.h"
#include "StartParseXML.h"

CStartParseXML::CStartParseXML(void)
{
}

CStartParseXML::~CStartParseXML(void)
{
}
/**
 * @brief 读取客户机的IP地址
 * @note  从配置文件中读取客户机的IP地址
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CString& strIP, 输入输出参数，客户机IP
 * @return 成功返回true，失败返回false
 */
bool  CStartParseXML::ParseClientComputer(LPCTSTR pstrFilePath,CString& strIP,CString& strFTPIP)
{
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	
	try
	{	// 创建XML对象
		strKey =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到配置ClientComputer
		strKey =_T("ClientComputer");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);		
		// 客户机的IP
		strKey =_T("HostIP");
		strIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);

		// 找到配置FTPServer
		strKey =_T("FTPServer");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// FTP的IP
		strKey =_T("HostIP");
		strFTPIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);	

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
 * @brief 存储安装程序的配置
 * @note  向配置文件中写入安装程序的配置参数
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CInstallParam* pInstallParam, 安装参数类
 * @return 解析是否成功

bool  CStartParseXML::WriteInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam)
{
	if(!pInstallParam)
		return false;
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

		oVariant = pstrFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到施工服务器
		strKey =_T("Peripherals");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 可控震源类型
		pInstallParam->GetVibratorType(strTemp);
		oVariant = strTemp;
		oElement.setAttribute(_T("VibratorType"), oVariant);
		// 爆炸震源类型
		pInstallParam->GetBlasterType(strTemp);
		oVariant = strTemp;
		oElement.setAttribute(_T("BlasterType"), oVariant);
		// 爆炸震源波特率		
		oVariant = (long)pInstallParam->m_nBlasterBaudRate;
		oElement.setAttribute(_T("BlasterBaudRate"), oVariant);
		// 绘图仪个数		
		oVariant = (long)pInstallParam->m_nNbOfPlotters;
		oElement.setAttribute(_T("NbOfPlotters"), oVariant);
		
		// 绘图仪1类型
		pInstallParam->GetBlasterTypeByNb(pInstallParam->m_nPlotterType1,strTemp);
		oVariant = strTemp;
		oElement.setAttribute(_T("PlotterType1"), oVariant);		
		// 绘图仪1 Name
		oVariant = pInstallParam->m_strPlotterName1;
		oElement.setAttribute(_T("PlotterName1"), oVariant);		
		// 绘图仪1 Mac地址
		oVariant = pInstallParam->m_strPlotterMacAddress1;
		oElement.setAttribute(_T("PlotterMacAddress1"), oVariant);
		// 绘图仪2类型
		pInstallParam->GetBlasterTypeByNb(pInstallParam->m_nPlotterType2,strTemp);
		oVariant = strTemp;
		oElement.setAttribute(_T("PlotterType2"), oVariant);		
		// 绘图仪2 Name
		oVariant = pInstallParam->m_strPlotterName2;
		oElement.setAttribute(_T("PlotterName2"), oVariant);		
		// 绘图仪2 Mac地址
		oVariant = pInstallParam->m_strPlotterMacAddress2;
		oElement.setAttribute(_T("PlotterMacAddress2"), oVariant);

		oVariant = pstrFilePath;
		oXMLDOMDocument.save(oVariant);
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
} */