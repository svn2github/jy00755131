#pragma once

#include "TestLimitData.h"

/**
*@brief 测试判据设置队列类
*/
class CTestLimitList
{
public:
	CTestLimitList();
	~CTestLimitList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 测试对象 1-仪器；2-检波器*/
	unsigned int m_uiAim;

	/** 数量*/
	unsigned int m_uiCountAll;
	/** 队列*/
	CList<CTestLimitData*, CTestLimitData*> m_olsTestLimit;
	/** 索引表*/
	CMap<unsigned int, unsigned int, CTestLimitData*, CTestLimitData*> m_oTestLimitMap;

	CTestLimitData* m_pTestLimitData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析XML
	void ParseXML();

	// 得到测试判据设置，根据索引号
	CTestLimitData* GetTestLimitDataByIndex(unsigned int uiIndex);
	// 得到测试判据，根据索引号
	float GetTestLimitValueByIndex(unsigned int uiIndex);

	// 重新加载测试配置文件
	void OnReloadTestSetupDataForApplyTestLimit();
};
