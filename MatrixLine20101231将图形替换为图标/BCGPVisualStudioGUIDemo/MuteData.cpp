#include "StdAfx.h"
#include "MuteData.h"

CMuteData::CMuteData()
{
}

CMuteData::~CMuteData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CMuteData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "LineName";
	m_uiLineName = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "PointNb";
	m_uiPointNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CMuteData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "LineName";
	oVariant = (long)m_uiLineName;
	pElement->setAttribute(strKey, oVariant);

	strKey = "PointNb";
	oVariant = (long)m_uiPointNb;
	pElement->setAttribute(strKey, oVariant);
}