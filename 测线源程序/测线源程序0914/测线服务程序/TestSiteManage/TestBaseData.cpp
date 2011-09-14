#include "StdAfx.h"
#include "TestBaseData.h"

CTestBaseData::CTestBaseData()
{
}

CTestBaseData::~CTestBaseData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestBaseData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestAim";
	m_uiTestAim = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试设置目标 1：仪器；2：检波器
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 索引号
	strKey = "Descr";
	m_strDesc = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 描述
	strKey = "TestType";
	m_uiType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试类型
	strKey = "ADC";
	m_uiADC = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// ADC输入 0-关闭；1-连接到内部测试网络；2-连接到检波器的电路输入端
	strKey = "Gain";
	m_uiGain = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 增益
	strKey = "DAC";
	m_uiDAC = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// DAC输入 0-关闭；1-连接到内部测试网络；2-连接到检波器
	strKey = "Filter";
	m_uiFilter = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 滤波器类型 1-0.8 LIN （线性）；2-0.8 MIN （最小）
	strKey = "SamplingRate";
	m_uiSamplingRate = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 采样率 单位微秒
	strKey = "SamplingLength";
	m_uiSamplingLength = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 采样时长 单位毫秒
	m_uiSamplingPointCount = m_uiSamplingLength * m_uiSamplingRate / 1000;	// 采样点数量 单位个
}	