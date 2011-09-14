#include "StdAfx.h"
#include "SpreadAbsoluteData.h"

CSpreadAbsoluteData::CSpreadAbsoluteData(void)
{
}

CSpreadAbsoluteData::~CSpreadAbsoluteData(void)
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CSpreadAbsoluteData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Label";
	m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = "Spread";
	m_strSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CSpreadAbsoluteData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "Nb";
	oVariant = (long)m_uiNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Label";
	oVariant = m_strLabel;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Spread";
	oVariant = m_strSpread;
	pElement->setAttribute(strKey, oVariant);
}