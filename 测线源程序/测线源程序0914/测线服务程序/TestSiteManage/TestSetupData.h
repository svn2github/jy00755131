#pragma once

#include "TestBaseList.h"
#include "TestLimitList.h"
#include "TestProjectList.h"
#include "TestElementList.h"
#include "TestDataBuffer.h"

/**
*@brief 测试设置类
*/
class CTestSetupData
{
public:
	CTestSetupData();
	~CTestSetupData();

public: // 属性
	/** XMLDOM文件对象指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 测试单元数组总数*/
	unsigned int m_uiTestElementCountAll;

	/** 仪器测试基本参数队列*/
	CTestBaseList m_oTestBaseListInstrument;
	/** 检波器测试基本参数队列*/
	CTestBaseList m_oTestBaseListSensor;

	/** 仪器测试判据参数队列*/
	CTestLimitList m_oTestLimitListInstrument;
	/** 检波器测试判据参数队列*/
	CTestLimitList m_oTestLimitListSensor;

	/** 测试项目队列对象*/
	CTestProjectList m_oTestProjectList;
	/** 测试单元队列，基本测试*/
	CTestElementList m_oTestElementListBase;
	/** 测试单元队列，噪声监测*/
	CTestElementList m_oTestElementListNoise;
	/** 测试数据缓冲区，基本测试*/
	CTestDataBuffer m_oTestDataBufferBase;
	/** 测试数据缓冲区，噪声监测*/
	CTestDataBuffer m_oTestDataBufferNoise;

	/** 测试项目队列对象，基本测试*/
	CTestProject m_oTestProjectBase;
	/** 测试项目队列对象，噪声监测*/
	CTestProject m_oTestProjectNoise;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 设置基本测试项目，来自测试设置的测试任务
	bool OnSetupTestProjectForTestBaseFromTestSetup(unsigned int uiTestNb);
	// 设置基本测试项目，来自测试图形的测试任务
	bool OnSetupTestProjectForTestBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread);
	// 设置基本测试项目，处理新加检波器的初始测试
	bool OnSetupTestProjectForSensorTestOnField(UINT uiTestResistance, UINT uiTestTilt, UINT uiTestLeakage);
	// 设置噪声监测项目
	bool OnSetupTestProjectForTestNoise();
	// 设置测试单元，基本测试
	bool OnSetupTestElementBase();
	// 设置测试单元，噪声监测
	bool OnSetupTestElementNoise();
	// 设置测试数据缓冲区，基本测试
	bool OnSetupTestDataBufferBase();
	// 设置测试数据缓冲区，噪声监测
	bool OnSetupTestDataBufferNoise();
};
