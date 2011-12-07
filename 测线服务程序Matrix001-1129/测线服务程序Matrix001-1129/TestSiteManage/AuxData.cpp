#include "StdAfx.h"
#include "AuxData.h"
#include "IPList.h"

CAuxData::CAuxData()
{
}

CAuxData::~CAuxData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CAuxData::ParseXML(CXMLDOMElement* pElement)
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

	// IP地址
	m_uiIP = CIPList::GetAuxIP(m_uiNb);	
}