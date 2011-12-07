#pragma once

#include "TestBaseList.h"
#include "TestProject.h"

/**
*@brief 测试项目队列类
*/
class CTestProjectList
{
public:
	CTestProjectList();
	~CTestProjectList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 数量*/
	unsigned int m_uiCountAll;
	/** 队列*/
	CList<CTestProject*, CTestProject*> m_olsTestProject;
	/** 索引表*/
	CMap<unsigned int, unsigned int, CTestProject*, CTestProject*> m_oTestProjectMap;
	/** 仪器测试基本参数队列*/
	CTestBaseList* m_pTestBaseListInstrument;
	/** 检波器测试基本参数队列*/
	CTestBaseList* m_pTestBaseListSensor;

	CTestProject* m_pTestProject;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 重新加载仪器测试配置文件
	void OnReloadTestSetupDataForApplyInstrument();
	// 重新加载检波器测试配置文件
	void OnReloadTestSetupDataForApplySensor();
	// 重新加载复合测试测试配置文件
	void OnReloadTestSetupDataForApplyMultiple();
	// 重新加载噪声监测配置文件
	void OnReloadTestSetupDataForApplySeisMonitor();
	// 删除测试项目，根据测试类型
	void DeleteTestProjectByProjectType(unsigned int uiProjectType);
	// 关闭
	void OnClose();

	// 解析XML
	void ParseXML();
	// 解析XML，仪器测试
	void ParseXMLForInstrument();
	// 解析XML，检波器测试
	void ParseXMLForSensor();
	// 解析XML，复合测试
	void ParseXMLForMultiple();
	// 解析XML，噪声监测
	void ParseXMLForSeisMonitor();

	// 测试索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 根据输入索引号，由索引表得到测试项目指针
	BOOL GetTestProject(unsigned int uiIndex, CTestProject* &pTestProject);
};
