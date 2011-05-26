#include "StdAfx.h"
#include "PointCodeData.h"

CPointCodeData::CPointCodeData(void)
{
}

CPointCodeData::~CPointCodeData(void)
{
}

/**
* 解析XML
* @param void
* @return void
*/
void CPointCodeData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Label";
	m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = "SensorType";
	m_strSensorType = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CPointCodeData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;
	strKey = "Nb";
	oVariant = (long)m_uiNb;
	pElement->setAttribute(strKey, oVariant);
	strKey = "Label";
	oVariant = m_strLabel;
	pElement->setAttribute(strKey, oVariant);
	strKey = "SensorType";
	oVariant = m_strSensorType;
	pElement->setAttribute(strKey, oVariant);
}