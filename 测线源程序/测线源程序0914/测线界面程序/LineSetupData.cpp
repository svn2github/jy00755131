#include "StdAfx.h"
#include "LineSetupData.h"

CLineSetupData::CLineSetupData(void)
{
}

CLineSetupData::~CLineSetupData(void)
{
}

/**
* 解析XML
* @param void
* @return void
*/
void CLineSetupData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名	
	// 得到测线号
	strKey = "LineName";
	m_uiNbLine = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 得到测线的测点定义
	strKey = "ReceiverSection";
	m_strReceiverSection = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CLineSetupData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;
	// 得到测线号
	strKey = "LineName";
	oVariant = (long)m_uiNbLine;
	pElement->setAttribute(strKey, oVariant);
	// 得到测线的测点定义
	strKey = "ReceiverSection";
	oVariant = m_strReceiverSection;
	pElement->setAttribute(strKey, oVariant);
}