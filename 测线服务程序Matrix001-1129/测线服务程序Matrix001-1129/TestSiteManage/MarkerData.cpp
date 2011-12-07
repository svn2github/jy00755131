#include "StdAfx.h"
#include "MarkerData.h"

CMarkerData::CMarkerData()
{
}

CMarkerData::~CMarkerData()
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
	m_uiBoxType = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "SN";
	m_uiSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "LineName";
	m_uiLineName = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "PointNb";
	m_uiPointNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "ChannelNb";
	m_uiChannelNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "MarkerIncr";
	m_uiMarkerIncr = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Reversed";
	m_uiReversed = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
}