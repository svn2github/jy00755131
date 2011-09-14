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
}

/**
* 加入XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestBaseData::AddToXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	CString strData;
	COleVariant oVariant;

	strKey = "ADC";
	oVariant = (long)m_uiADC;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Gain";
	oVariant = (long)m_uiGain;
	pElement->setAttribute(strKey, oVariant);

	strKey = "DAC";
	oVariant = (long)m_uiDAC;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Filter";
	oVariant = (long)m_uiFilter;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SamplingRate";
	oVariant = (long)m_uiSamplingRate;
	pElement->setAttribute(strKey, oVariant);

	strKey = "SamplingLength";
	oVariant = (long)m_uiSamplingLength;
	pElement->setAttribute(strKey, oVariant);
}