#pragma once

#include "TestElementData.h"

/**
*@brief 测试单元数据队列类
*/
class CTestElementList
{
public:
	CTestElementList();
	~CTestElementList();

public: //属性
	/** 测试单元指针*/
	CTestElementData* m_pArrayTestElementData;
	/** 单元总数*/
	unsigned int m_uiCountAll;
	/** 使用中数量*/
	unsigned int m_uiCountUsed;
	/** 空闲数量*/
	unsigned int m_uiCountFree;
	/** 单元索引表*/
	CMap<unsigned int, unsigned int, CTestElementData*, CTestElementData*> m_oTestElementDataMap;

	/** 测试数据*/
	byte m_pTestData[65536];
	/** 每批次测试数最大数据个数*/
	unsigned int m_uiBatchDataCountMax;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();

	// 重置，为一次新的测试请求
	void ResetForNewTestRequest();
	// 增加一个单元到索引表
	void AddTestElementDataToMap(unsigned int uiIndex, CTestElementData* pTestElementData);
	// 由索引表得到一个单元
	BOOL GetTestElementDataFromMap(unsigned int uiIndex, CTestElementData* &pTestElementData);
	// 单元索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 得到一个单元
	CTestElementData* GetTestElementDataByIndex(unsigned int uiIndex);

	// 按批次计算，测试数据批次数
	unsigned int GetTestDataBatchCount();
	// 按批次计算，测试数据批次数
	bool GetTestDataByBatchIndex(unsigned int uiTestType, unsigned int uiBatchIndex, unsigned int& uiDataSize, unsigned short& usDataCount);
};

