#pragma once

#include "TestRequest.h"

/**
*@brief 测试项目类
*/
class CTestProject
{
public:
	CTestProject();
	~CTestProject();

public: //属性
	/** 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测*/
	unsigned int m_uiProjectType;

	/** 测试项目索引号*/
	unsigned int m_uiTestNb;
	/** 测试项目名称*/
	CString m_strTestName;
	/** 辅助道定义*/
	CString m_strAuxiliaryDescr;
	/** 绝对排列定义*/
	CString m_strAbsoluteSpread;
	/** 每次测试之间的间隔（毫秒）*/
	unsigned int m_uiDelayBetweenTest;
	/** 是否记录测试数据  0-不记录；1-记录*/
	unsigned int m_uiRecordResult;
	/** 测试记录时间（毫秒）*/
	unsigned int m_uiRecordLength;
	/** 测试文件号*/
	unsigned int m_uiTestFileNb;

	/** 测试请求数量*/
	unsigned int m_uiCountAll;
	/** 测试请求队列*/
	CList<CTestRequest, CTestRequest> m_olsTestRequest;

	/**  Tab键数量*/
	unsigned int m_uiTabCount;
	/** 写格式化XML文件用*/
	CString m_strTabChild;
	/** 写格式化XML文件用*/
	CString m_strTabParent;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 解析XML，仪器测试
	void ParseXMLForInstrument(CXMLDOMElement* pElement);
	// 解析XML，检波器测试
	void ParseXMLForSensor(CXMLDOMElement* pElement);
	// 解析XML，复合测试
	void ParseXMLForMultiple(CXMLDOMElement* pElement);
	// 解析XML，噪声监测
	void ParseXMLForSeisMonitor(CXMLDOMElement* pElement);

	// 加入XML
	void AddToXML(CXMLDOMElement* pElement, CXMLDOMDocument* pXMLDOMDocument);
	// 加入XML，仪器测试
	void AddToXMLForInstrument(CXMLDOMElement* pElement);
	// 加入XML，检波器测试
	void AddToXMLForSensor(CXMLDOMElement* pElement);
	// 加入XML，复合测试
	void AddToXMLForMultiple(CXMLDOMElement* pElement, CXMLDOMDocument* pXMLDOMDocument);
	// 加入XML，噪声监测
	void AddToXMLForSeisMonitor(CXMLDOMElement* pElement);
};
