#include "StdAfx.h"
#include "FormLineData.h"

CFormLineData::CFormLineData(void)
{
}

CFormLineData::~CFormLineData(void)
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CFormLineData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "BoxType";
	m_uiBoxType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "SN";
	m_uiSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CFormLineData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "Nb";
	oVariant = (long)m_uiNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "BoxType";
	oVariant = (long)m_uiBoxType;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SN";
	oVariant = (long)m_uiSN;
	pElement->setAttribute(strKey, oVariant);
}