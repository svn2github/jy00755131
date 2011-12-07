#include "StdAfx.h"
#include "TestRequest.h"

CTestRequest::CTestRequest()
{
	OnReset();
}

CTestRequest::~CTestRequest()
{
}

/**
* 重置
* @param void
* @return void
*/
void CTestRequest::OnReset()
{
	m_uiIndex = 0;	// 索引号	
	m_uiAim = 0;	// 测试对象	1-仪器；2-检波器；3-全部
	m_uiType = 0;	// 测试类型
	m_uiGain = 0;	// 增益
	m_uiLoopNb = 0;	// 跳转到的索引号
	m_uiLoopCount = 0;	// 循环次数
}

/**
* 解析XML，仪器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestRequest::ParseXMLForInstrument(CXMLDOMElement* pElement)
{
	m_uiIndex = 1;	// 索引号	
	m_uiAim = 1;	// 测试对象	1-仪器；2-检波器；3-全部

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
	m_uiIndex = 1;	// 索引号	
	m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
	m_uiGain = 1600;	// 增益

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

	m_uiAim = 3;	// 测试对象	1-仪器；2-检波器；3-全部
}