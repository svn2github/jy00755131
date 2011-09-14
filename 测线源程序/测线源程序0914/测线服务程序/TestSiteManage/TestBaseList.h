#pragma once

#include "TestBaseData.h"

/**
*@brief 测试基本设置队列类
*/
class CTestBaseList
{
public:
	CTestBaseList();
	~CTestBaseList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 测试对象 1-仪器；2-检波器*/
	unsigned int m_uiAim;

	/** 数量*/
	unsigned int m_uiCountAll;
	/** 队列*/
	CList<CTestBaseData*, CTestBaseData*> m_olsTestBase;
	/** 索引表*/
	CMap<unsigned int, unsigned int, CTestBaseData*, CTestBaseData*> m_oTestBaseMap;

	CTestBaseData* m_pTestBaseData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析XML
	void ParseXML();

	// 得到测试基本设置，根据索引号
	CTestBaseData* GetTestBaseDataByIndex(unsigned int uiIndex);
	// 得到测试基本设置，根据测试类型
	CTestBaseData* GetTestBaseDataByTestType(unsigned int uiTestType);

	// 重新加载测试配置文件
	void OnReloadTestSetupDataForApplyTestBase();
};
