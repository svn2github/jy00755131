#include "StdAfx.h"
#include "SensorData.h"

CSensorData::CSensorData(void)
{
}

CSensorData::~CSensorData(void)
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

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CSensorData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "Nb";
	oVariant = (long)m_uiNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SEGDCode";
	oVariant = (long)m_uiSegdCode;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Label";
	oVariant = m_strLabel;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ContinuityMin";
	oVariant = m_fContinuityMin;
	pElement->setAttribute(strKey, oVariant);

	strKey = "ContinuityMax";
	oVariant = m_fContinuityMax;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Tilt";
	oVariant = m_fTilt;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Noise";
	oVariant = m_fNoise;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Leakage";
	oVariant = m_fLeaKage;
	pElement->setAttribute(strKey, oVariant);
}