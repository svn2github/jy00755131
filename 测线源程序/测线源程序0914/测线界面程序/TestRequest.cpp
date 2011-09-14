#include "StdAfx.h"
#include "TestRequest.h"

CTestRequest::CTestRequest()
{
}

CTestRequest::~CTestRequest()
{
}

/**
* 解析XML，仪器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::ParseXMLForInstrument(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestType";
	m_uiType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "Gain";
	m_uiGain = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
}

/**
* 解析XML，检波器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::ParseXMLForSensor(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestType";
	m_uiType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
}

/**
* 解析XML，复合测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::ParseXMLForMultiple(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "LineNb";
	m_uiIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "TestType";
	m_uiType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "Gain";
	m_uiGain = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "LoopLineNb";
	m_uiLoopNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = "NbLoops";
	m_uiLoopCount = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
}

/**
* 加入XML，仪器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::AddToXMLForInstrument(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "TestType";
	oVariant = (long)m_uiType;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Gain";
	oVariant = (long)m_uiGain;
	pElement->setAttribute(strKey, oVariant);
}

/**
* 加入XML，检波器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::AddToXMLForSensor(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "TestType";
	oVariant = (long)m_uiType;
	pElement->setAttribute(strKey, oVariant);
}

/**
* 加入XML，复合测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::AddToXMLForMultiple(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "LineNb";
	oVariant = (long)m_uiIndex;
	pElement->setAttribute(strKey, oVariant);

	strKey = "TestType";
	oVariant = (long)m_uiType;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Gain";
	oVariant = (long)m_uiGain;
	pElement->setAttribute(strKey, oVariant);

	strKey = "LoopLineNb";
	oVariant = (long)m_uiLoopNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "NbLoops";
	oVariant = (long)m_uiLoopCount;
	pElement->setAttribute(strKey, oVariant);
}