#pragma once

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
	/** 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测*/
	unsigned int m_uiProjectType;

	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 数量*/
	unsigned int m_uiCountAll;
	/** 队列*/
	CList<CTestProject*, CTestProject*> m_olsTestProject;

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
	// 保存配置文件
	void OnSave();
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

	// 加入XML，仪器测试
	void AddToXMLForInstrument();
	// 加入XML，检波器测试
	void AddToXMLForSensor();
	// 加入XML，复合测试
	void AddToXMLForMultiple();
	// 加入XML，噪声监测
	void AddToXMLForSeisMonitor();
};
