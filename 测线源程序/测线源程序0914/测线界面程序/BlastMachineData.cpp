#include "StdAfx.h"
#include "BlastMachineData.h"

CBlastMachineData::CBlastMachineData()
{
}

CBlastMachineData::~CBlastMachineData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CBlastMachineData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Label";
	m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = "BoxType";
	m_uiBoxType = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "SN";
	m_uiSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "ChannelNb";
	m_uiChannelNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Gain";
	m_uiGain = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "DPGNb";
	m_uiDPGNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Comments";
	m_strComments = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CBlastMachineData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "Nb";
	oVariant = (long)m_uiNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Label";
	oVariant = m_strLabel;
	pElement->setAttribute(strKey, oVariant);

	strKey = "BoxType";
	oVariant = (long)m_uiBoxType;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SN";
	oVariant = (long)m_uiSN;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ChannelNb";
	oVariant = (long)m_uiChannelNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Gain";
	oVariant = (long)m_uiGain;
	pElement->setAttribute(strKey, oVariant);

	strKey = "DPGNb";
	oVariant = (long)m_uiDPGNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Comments";
	oVariant = m_strComments;
	pElement->setAttribute(strKey, oVariant);
}