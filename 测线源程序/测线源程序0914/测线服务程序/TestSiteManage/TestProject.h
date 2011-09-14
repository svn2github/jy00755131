#pragma once

#include "TestBaseList.h"
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
	CList<CTestRequest*, CTestRequest*> m_olsTestRequest;
	/** 测试请求队列，临时*/
	CList<CTestRequest*, CTestRequest*> m_olsTestRequestTemp;
	/** 参加测试仪器的IP地址队列*/
	CList<unsigned int, unsigned int> m_olsIP;

	/** 仪器测试基本参数队列*/
	CTestBaseList* m_pTestBaseListInstrument;
	/** 检波器测试基本参数队列*/
	CTestBaseList* m_pTestBaseListSensor;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();

	// 解析XML，仪器测试
	void ParseXMLForInstrument(CXMLDOMElement* pElement);
	// 解析XML，检波器测试
	void ParseXMLForSensor(CXMLDOMElement* pElement);
	// 解析XML，复合测试
	void ParseXMLForMultiple(CXMLDOMElement* pElement);
	// 解析XML，噪声监测
	void ParseXMLForSeisMonitor(CXMLDOMElement* pElement);

	// 整理要求一次测试全部项目的测试请求
	void ReArrangeByTestType(CTestRequest* pTestRequest);
	// 整理复合测试的测试请求
	void ReArrangeForMultiple();

	// 设置测试请求的测试目标 1-仪器；2-检波器
	void SetTestRequestAim(CTestRequest* pTestRequest);
	// 根据测试类型，得到测试测试基本设置指针
	CTestBaseData* GetTestBaseDataByTestType(unsigned int uiType);

	// 复制测试项目
	void Clone(CTestProject* pTestProject);

	// 设置测试项目，来自测试视图
	void SetupTestProjectFromView(unsigned int uiTestAim, unsigned int uiTestType);
	// 设置测试项目，处理新加检波器的初始测试
	void SetupTestProjectForSensorTestOnField(UINT uiTestResistance, UINT uiTestTilt, UINT uiTestLeakage);
};
