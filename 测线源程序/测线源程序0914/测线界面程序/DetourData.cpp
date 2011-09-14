#include "StdAfx.h"
#include "DetourData.h"

CDetourData::CDetourData()
{
}

CDetourData::~CDetourData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CDetourData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "BoxTypeLow";
	m_uiBoxTypeLow = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "SNLow";
	m_uiSNLow = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "ChannelNbLow";
	m_uiChannelNbLow = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "BoxTypeHigh";
	m_uiBoxTypeHigh = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "SNHigh";
	m_uiSNHigh = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "ChannelNbHigh";
	m_uiChannelNbHigh = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "StopMarking";
	m_uiStopMarking = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CDetourData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "BoxTypeLow";
	oVariant = (long)m_uiBoxTypeLow;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SNLow";
	oVariant = (long)m_uiSNLow;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ChannelNbLow";
	oVariant = (long)m_uiChannelNbLow;
	pElement->setAttribute(strKey, oVariant);

	strKey = "BoxTypeHigh";
	oVariant = (long)m_uiBoxTypeHigh;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SNHigh";
	oVariant = (long)m_uiSNHigh;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ChannelNbHigh";
	oVariant = (long)m_uiChannelNbHigh;
	pElement->setAttribute(strKey, oVariant);

	strKey = "StopMarking";
	oVariant = (long)m_uiStopMarking;
	pElement->setAttribute(strKey, oVariant);
}