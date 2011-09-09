#include "StdAfx.h"
#include "InstallParseXML.h"

CInstallParseXML::CInstallParseXML(void)
{
}

CInstallParseXML::~CInstallParseXML(void)
{
}
/**
 * @brief 读安装程序的配置
 * @note  从配置文件中读取安装程序的配置参数
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CInstallParam* pInstallParam, 安装参数类
 * @return 解析是否成功
 */
bool  CInstallParseXML::ParseInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam)
{
	if(!pInstallParam)
		return false;
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

		oVariant = pstrFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到配置
		strKey =_T("Peripherals");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		
		// 得到可控震源类型
		strKey =_T("VibratorType");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pInstallParam->SetVibratorType(strTemp);		
		// 得到爆炸机类型
		strKey =_T("BlasterType");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pInstallParam->SetBlasterType(strTemp);
		// 得到波特率
		strKey =_T("BlasterBaudRate");
		pInstallParam->m_nBlasterBaudRate = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到绘图仪个数
		strKey =_T("NbOfPlotters");
		pInstallParam->m_nNbOfPlotters= CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 绘图仪1类型
		strKey =_T("PlotterType1");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pInstallParam->m_nPlotterType1 = pInstallParam->GetBlasterNbByType(strTemp);
		// 绘图仪1 Name
		strKey =_T("PlotterName1");
		pInstallParam->m_strPlotterName1 = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		// 绘图仪1 Mac地址
		strKey =_T("PlotterMacAddress1");
		pInstallParam->m_strPlotterMacAddress1 = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		// 绘图仪2类型
		strKey =_T("PlotterType2");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pInstallParam->m_nPlotterType2 = pInstallParam->GetBlasterNbByType(strTemp);
		// 绘图仪2 Name
		strKey =_T("PlotterName2");
		pInstallParam->m_strPlotterName2 = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		// 绘图仪2 Mac地址
		strKey =_T("PlotterMacAddress2");
		pInstallParam->m_strPlotterMacAddress2 = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);


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
 */
bool  CInstallParseXML::WriteInstallParam(LPCTSTR pstrFilePath,CInstallParam* pInstallParam)
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
}