#include "StdAfx.h"
#include "LookSetupData.h"

CLookSetupData::CLookSetupData(void)
{
}

CLookSetupData::~CLookSetupData(void)
{
}

// 初始化
void CLookSetupData::OnInit()
{
	// 解析配置文件
	ParseXML();
}

// 重新加载
void CLookSetupData::OnReload(void)
{
	// 解析配置文件
	ParseXML();
}

// 关闭
void CLookSetupData::OnClose()
{

}

// 解析配置文件
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

// 保存配置文件
void CLookSetupData::OnSave()
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

	COleVariant oVariant;
	strKey = "AutoLook";
	oVariant = (long)m_uiLookAuto;
	oElement.setAttribute(strKey, oVariant);
	strKey = "Resistance";
	oVariant = (long)m_uiTestResistance;
	oElement.setAttribute(strKey, oVariant);
	strKey = "Tilt";
	oVariant = (long)m_uiTestTilt;
	oElement.setAttribute(strKey, oVariant);
	strKey = "Leakage";
	oVariant = (long)m_uiTestLeakage;
	oElement.setAttribute(strKey, oVariant);
}