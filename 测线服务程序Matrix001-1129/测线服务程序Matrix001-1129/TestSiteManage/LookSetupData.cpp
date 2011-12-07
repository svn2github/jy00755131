#include "StdAfx.h"
#include "LookSetupData.h"

CLookSetupData::CLookSetupData()
{
}

CLookSetupData::~CLookSetupData()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CLookSetupData::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CLookSetupData::OnReload()
{
	// 解析配置文件
	ParseXML();
}

/**
* 关闭
* @param void
* @return void
*/
void CLookSetupData::OnClose()
{

}

/**
* 解析配置文件
* @param void
* @return void
*/
void CLookSetupData::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到Look模式设置区
	strKey = "LookSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = "AutoLook";
	m_uiLookAuto = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "Resistance";
	m_uiTestResistance = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "Tilt";
	m_uiTestTilt = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "Leakage";
	m_uiTestLeakage = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}