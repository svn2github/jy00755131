#pragma once

/**
*@brief 测试请求类
*/
class CTestRequest
{
public:
	CTestRequest();
	~CTestRequest();

public: //属性
	/** 索引号*/
	unsigned int m_uiIndex;
	/** 测试类型*/
	unsigned int m_uiType;
	/** 增益*/
	unsigned int m_uiGain;
	/** 跳转到的索引号*/
	unsigned int m_uiLoopNb;
	/** 循环次数*/
	unsigned int m_uiLoopCount;

public: //方法
	// 解析XML，仪器测试
	void ParseXMLForInstrument(CXMLDOMElement* pElement);
	// 解析XML，检波器测试
	void ParseXMLForSensor(CXMLDOMElement* pElement);
	// 解析XML，复合测试
	void ParseXMLForMultiple(CXMLDOMElement* pElement);

	// 加入XML，仪器测试
	void AddToXMLForInstrument(CXMLDOMElement* pElement);
	// 加入XML，检波器测试
	void AddToXMLForSensor(CXMLDOMElement* pElement);
	// 加入XML，复合测试
	void AddToXMLForMultiple(CXMLDOMElement* pElement);
};
