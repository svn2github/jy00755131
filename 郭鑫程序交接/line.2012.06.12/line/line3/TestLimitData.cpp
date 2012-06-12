#include "StdAfx.h"
#include "TestLimitData.h"

CTestLimitData::CTestLimitData()
{
}

CTestLimitData::~CTestLimitData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestLimitData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestAim";
	m_uiTestAim = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试设置目标 1：仪器；2：检波器
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 索引号
	strKey = "Descr";
	m_strDesc = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 描述
	strKey = "Unit";
	m_strUnit = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 单位
	strKey = "TestType";
	m_uiType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试类型
	strKey = "Limit";
	m_fLimit = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);	// 判据
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestLimitData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	CString strData;
	COleVariant oVariant;

	strKey = "Limit";
	oVariant = m_fLimit;
	pElement->setAttribute(strKey, oVariant);
}