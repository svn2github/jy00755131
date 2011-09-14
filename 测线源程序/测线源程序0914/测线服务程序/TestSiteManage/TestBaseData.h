#pragma once

/**
*@brief 测试基本设置类
*/
class CTestBaseData
{
public:
	CTestBaseData();
	~CTestBaseData();

public: //属性
	/** 测试设置目标 1：仪器；2：检波器*/
	unsigned int m_uiTestAim;
	/** 索引号*/
	unsigned int m_uiNb;
	/** 描述*/
	CString m_strDesc;
	/** 测试类型*/
	unsigned int m_uiType;
	/** ADC输入 0-关闭；1-连接到内部测试网络；2-连接到检波器的电路输入端*/
	unsigned int m_uiADC;
	/** 增益*/
	unsigned int m_uiGain;
	/** DAC输入 0-关闭；1-连接到内部测试网络；2-连接到检波器*/
	unsigned int m_uiDAC;
	/** 滤波器类型 1-0.8 LIN （线性）；2-0.8 MIN （最小）*/
	unsigned int m_uiFilter;
	/** 采样率 单位微秒*/
	unsigned int m_uiSamplingRate;
	/** 采样时长 单位毫秒*/
	unsigned int m_uiSamplingLength;
	/** 采样点数量 单位个*/
	unsigned int m_uiSamplingPointCount;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
};