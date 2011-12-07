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