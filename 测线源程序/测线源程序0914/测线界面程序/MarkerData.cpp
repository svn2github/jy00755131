#include "StdAfx.h"
#include "MarkerData.h"

CMarkerData::CMarkerData(void)
{
}

CMarkerData::~CMarkerData(void)
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CMarkerData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "BoxType";
	m_uiBoxType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "SN";
	m_uiSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "LineName";
	m_uiLineName = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "PointNb";
	m_uiPointNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "ChannelNb";
	m_uiChannelNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "MarkerIncr";
	m_uiMarkerIncr = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "Reversed";
	m_uiReversed = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CMarkerData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "BoxType";
	oVariant = (long)m_uiBoxType;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SN";
	oVariant = (long)m_uiSN;
	pElement->setAttribute(strKey, oVariant);

	strKey = "LineName";
	oVariant = (long)m_uiLineName;
	pElement->setAttribute(strKey, oVariant);

	strKey = "PointNb";
	oVariant = (long)m_uiPointNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ChannelNb";
	oVariant = (long)m_uiChannelNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "MarkerIncr";
	oVariant = (long)m_uiMarkerIncr;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Reversed";
	oVariant = (long)m_uiReversed;
	pElement->setAttribute(strKey, oVariant);
}