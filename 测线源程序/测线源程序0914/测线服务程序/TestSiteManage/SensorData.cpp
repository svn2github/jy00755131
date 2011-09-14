#include "StdAfx.h"
#include "SensorData.h"

CSensorData::CSensorData()
{
}

CSensorData::~CSensorData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CSensorData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "SEGDCode";
	m_uiSegdCode = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Label";
	m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = "ContinuityMin";
	m_fContinuityMin = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	strKey = "ContinuityMax";
	m_fContinuityMax = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	strKey = "Tilt";
	m_fTilt = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	strKey = "Noise";
	m_fNoise = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	strKey = "Leakage";
	m_fLeaKage = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
}