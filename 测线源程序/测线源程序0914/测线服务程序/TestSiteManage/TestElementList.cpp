#include "StdAfx.h"
#include "TestElementList.h"

CTestElementList::CTestElementList()
{
	// 每批次测试数最大数据个数
	m_uiBatchDataCountMax = 50;
}

CTestElementList::~CTestElementList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestElementList::OnInit()
{
	// 生成单元数组
	m_pArrayTestElementData = new CTestElementData[m_uiCountAll];
	// 使用中数量
	m_uiCountUsed = 0;
	// 空闲数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 单元在单元数组中的位置
		m_pArrayTestElementData[i].m_uiIndex = i;
		// 重置单元
		m_pArrayTestElementData[i].Reset();
	}
}

/**
* 重置
* @param void
* @return void
*/
void CTestElementList::OnReset()
{
	// 使用中单元数量
	m_uiCountUsed = 0;
	// 空闲单元数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountUsed; i++)
	{
		// 重置单元
		m_pArrayTestElementData[i].Reset();
	}
	// 清空索引表
	m_oTestElementDataMap.RemoveAll();
}

/**
* 关闭
* @param void
* @return void
*/
void CTestElementList::OnClose()
{
	// 删除单元数组
	delete[] m_pArrayTestElementData;
	// 清空索引表
	m_oTestElementDataMap.RemoveAll();
}

/**
* 重置，为一次新的测试请求
* @param void
* @return void
*/
void CTestElementList::ResetForNewTestRequest()
{
	for(unsigned int i = 0; i < m_uiCountUsed; i++)
	{
		// 重置单元，为一次新的测试请求
		m_pArrayTestElementData[i].ResetForNewTestRequest();
	}
}

/**
* 增加一个单元到索引表
* @param unsigned int uiIndex 单元索引
* @param CTestElementData* pTestElementData 单元指针
* @return void
*/
void CTestElementList::AddTestElementDataToMap(unsigned int uiIndex, CTestElementData* pTestElementData)
{
	m_oTestElementDataMap.SetAt(uiIndex, pTestElementData);
}

/**
* 单元索引号是否已加入索引表
* @param unsigned int uiIndex 单元索引
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CTestElementList::IfIndexExistInMap(unsigned int uiIndex)
{
	CTestElementData* pTestElementData = NULL;
	return m_oTestElementDataMap.Lookup(uiIndex, pTestElementData);
}

/**
* 根据输入索引号，由索引表得到单元指针
* @param unsigned int uiIndex 索引号
* @param CTestElementData* &pTestElementData	单元指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CTestElementList::GetTestElementDataFromMap(unsigned int uiIndex, CTestElementData* &pTestElementData)
{
	return m_oTestElementDataMap.Lookup(uiIndex, pTestElementData);
}

/**
* 根据单元的数组索引号，得到单元指针
* @param unsigned int uiIndex 单元的数组索引号
* @return CTestElementData* 单元指针
*/
CTestElementData* CTestElementList::GetTestElementDataByIndex(unsigned int uiIndex)
{
	return &m_pArrayTestElementData[uiIndex];
}

/**
* 按批次计算，测试数据批次数
* @param void
* @return unsigned int 测试数据批次数
*/
unsigned int CTestElementList::GetTestDataBatchCount()
{
	unsigned int uiBatchCount = 0;
	uiBatchCount = m_uiCountUsed / m_uiBatchDataCountMax;
	if((m_uiCountUsed % m_uiBatchDataCountMax) > 0)
	{
		uiBatchCount++;
	}
	return uiBatchCount;
}

/**
* 按批次计算，测试数据批次数
* @param unsigned int uiTestType 测试类型
* @param unsigned int uiBatchIndex 测试数据批次索引
* @param unsigned unsigned int uiDataSize 每个测试数据占用字节数，引用传值
* @paramunsigned short usDataCount 测试数据个数，引用传值
* @return bool true：成功；false：失败
*/
bool CTestElementList::GetTestDataByBatchIndex(unsigned int uiTestType, unsigned int uiBatchIndex, unsigned int& uiDataSize, unsigned short& usDataCount)
{
	bool bReturn = true;
	unsigned int uiBatchCount = 0;
	usDataCount = m_uiBatchDataCountMax;
	uiBatchCount = m_uiCountUsed / m_uiBatchDataCountMax;
	if((m_uiCountUsed % m_uiBatchDataCountMax) > 0)
	{
		uiBatchCount++;
		if((uiBatchIndex + 1) == uiBatchCount)
		{
			usDataCount = m_uiCountUsed % m_uiBatchDataCountMax;
		}
	}
	int iPosition = 0;
	for(unsigned int i = uiBatchIndex * m_uiBatchDataCountMax; i < uiBatchIndex * m_uiBatchDataCountMax + ((unsigned int)usDataCount); i++)
	{
		uiDataSize = m_pArrayTestElementData[i].GetTestResult(uiTestType, &m_pTestData[iPosition]);
		iPosition = iPosition + uiDataSize;
	}
	return bReturn;
}