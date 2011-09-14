#include "StdAfx.h"
#include "TestDataBuffer.h"

CTestDataBuffer::CTestDataBuffer()
{
	m_uiTestElementCountAll = 0;	// 测试单元总数

	m_pArrayTestData = NULL;	// 测试数据数组指针
	m_uiByteCountAll = 0;	// 总字节数

	m_uiDataByteSize = 3;	// 每个数据占用字节
	m_uiGroupDataCount = FrameADCount;	// 每组数据包含数据个数
	m_uiGroupDataByteSize = m_uiGroupDataCount * m_uiDataByteSize;	// 每组数据占用字节
	m_uiBlockGroupCount = 0;	// 每块数据包含数据组个数
	m_uiBlockCount = 32;	// 块数据个数	
	m_uiBlockByteSize = 0;	// 每块数据占用字节
	/** 数据区覆盖写入块起始位置*/
	m_uiBlockIndexForReWrite = 28;

	m_uiTestElementCountAllUsed = 0;	// 测试单元数组总数，使用的
	m_uiBlockGroupCountUsed = 0;	// 每块数据包含数据组个数，使用的
	m_uiBlockByteSizeUsed = 0;	// 每块数据占用字节，使用的
	m_uiByteCountAllUsed = 0;	// 总字节数，使用的

	m_uiFrameIndex = 0;	// 数据帧索引
	m_uiFrameIndexWrite = 0;	// 数据帧索引，写数据文件用
}

CTestDataBuffer::~CTestDataBuffer()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestDataBuffer::OnInit()
{
	m_uiGroupDataByteSize = m_uiGroupDataCount * m_uiDataByteSize;	// 每组数据占用字节
	m_uiBlockGroupCount = m_uiTestElementCountAll;	// 每块数据包含数据组个数
	m_uiBlockByteSize = m_uiBlockGroupCount * m_uiGroupDataByteSize;	// 每块数据占用字节
	m_uiByteCountAll = m_uiBlockCount * m_uiBlockByteSize;	// 总字节数

	// 生成测试数据缓冲区数组
	m_pArrayTestData = new byte[m_uiByteCountAll];
}

/**
* 关闭
* @param void
* @return void
*/
void CTestDataBuffer::OnClose()
{
	// 删除测试数据缓冲区数组
	delete[] m_pArrayTestData;
}

/**
* 重置
* @param void
* @return void
*/
void CTestDataBuffer::OnReset()
{
	memset(m_pArrayTestData, 0xFF, m_uiByteCountAll);
}

/**
* 重置，为一次新的测试请求
* @param void
* @return void
*/
void CTestDataBuffer::ResetForNewTestRequest()
{
	m_uiFrameIndex = 0;	// 数据帧索引
	m_uiFrameIndexWrite = 0;	// 数据帧索引，写数据文件用
	memset(m_pArrayTestData, 0xFF, m_uiByteCountAllUsed);
}

/**
* 测试设置
* @param unsigned int uiTestElementCountAllUsed 测试单元数组总数，使用的
* @return void
*/
void CTestDataBuffer::OnTestSetup(unsigned int uiTestElementCountAllUsed)
{
	m_uiTestElementCountAllUsed = uiTestElementCountAllUsed;	// 测试单元数组总数，使用的
	m_uiBlockGroupCountUsed = m_uiTestElementCountAllUsed;	// 每块数据包含数据组个数，使用的
	m_uiBlockByteSizeUsed = m_uiBlockGroupCountUsed * m_uiGroupDataByteSize;	// 每块数据占用字节，使用的
	m_uiByteCountAllUsed = m_uiBlockCount * m_uiBlockByteSizeUsed;	// 总字节数，使用的
}

/**
* 得到块索引
* @param void
* @return void
*/
int CTestDataBuffer::GetBlockIndexByFrameIndex(unsigned int uiFrameIndex)
{
	int iBlockIndex = 0;
	// 得到所在数据块索引
	// 判断：数据写入
	if(uiFrameIndex < m_uiBlockCount)
	{
		iBlockIndex = uiFrameIndex;
	}
	// 判断：数据要覆盖原来数据写入
	else
	{
		iBlockIndex = (uiFrameIndex - m_uiBlockIndexForReWrite) % (m_uiBlockCount - m_uiBlockIndexForReWrite);
		iBlockIndex = iBlockIndex + m_uiBlockIndexForReWrite;
	}
	return iBlockIndex;
}

/**
* 设置测试单元一帧测试数据
* @param byte* lpData 数据指针
* @param unsigned int uiElementIndex 单元索引
* @param unsigned int uiFrameIndex 帧索引
* @return void
*/
void CTestDataBuffer::SetElementTestDataOfOneFrame(byte* lpData, unsigned int uiElementIndex, unsigned int uiFrameIndex)
{
	// 得到所在数据块索引
	int iBlockIndex = GetBlockIndexByFrameIndex(uiFrameIndex);
	// 得到数据保存位置
	int iSaveIndex = iBlockIndex * m_uiBlockByteSizeUsed + uiElementIndex * m_uiGroupDataByteSize;
	// 保存数据
	memcpy(&m_pArrayTestData[iSaveIndex], lpData, m_uiGroupDataByteSize);
}

/**
* 得到测试单元一帧测试数据
* @param unsigned int uiElementIndex 单元索引
* @param unsigned int uiFrameIndex 帧索引
* @return byte* 数据指针
*/
byte* CTestDataBuffer::GetElementTestDataOfOneFrame(unsigned int uiElementIndex, unsigned int uiFrameIndex)
{
	// 得到所在数据块索引
	int iBlockIndex = GetBlockIndexByFrameIndex(uiFrameIndex);
	// 得到数据保存位置
	int iSaveIndex = iBlockIndex * m_uiBlockByteSizeUsed + uiElementIndex * m_uiGroupDataByteSize;
	// 得到数据
	return &m_pArrayTestData[iSaveIndex];
}

/**
* 得到一帧测试数据，单位块
* @param unsigned int uiFrameIndex 帧索引
* @return byte* 数据指针
*/
byte* CTestDataBuffer::GetBlockTestDataOneFrame(unsigned int uiFrameIndex)
{
	// 得到所在数据块索引
	int iBlockIndex = GetBlockIndexByFrameIndex(uiFrameIndex);
	// 得到数据保存位置
	int iSaveIndex = iBlockIndex * m_uiBlockByteSizeUsed;
	// 得到数据
	return &m_pArrayTestData[iSaveIndex];
}

/**
* 得到测试单元一次测试数据
* @param unsigned int uiElementIndex 单元索引
* @param unsigned int uiSamplingPointCount 采样点数量
* @return byte* 数据指针
*/
byte* CTestDataBuffer::GetElementTestDataOfOneTest(unsigned int uiElementIndex, unsigned int uiSamplingPointCount)
{
	// 判断：采样点数大于2048
	//if(uiSamplingPointCount > ADoffset)
	if(uiSamplingPointCount > m_uiTestElementCountAll)
	{
		TRACE1("m_uiTestElementCountAll ERROR! %d TBH=%d\r\n",m_uiTestElementCountAll);
		return NULL;
	}
	// 数据置0
	//memset(m_pArrayTestDataOfOneElement, 0x00, 8288);
	memset(m_pArrayTestDataOfOneElement, 0x00, ArrayTestDataOfOne);
	// 得到数据帧数
	int iFrameCount = uiSamplingPointCount / m_uiGroupDataCount;
	if((uiSamplingPointCount % m_uiGroupDataCount) > 0)
	{
		iFrameCount++;
	}
	m_AVGNoiseValue=0;
	int iAVG=0;
	int iAVGCount=0;
	for(int i = 0; i < iFrameCount; i++)
	{
		// 得到测试单元一帧测试数据
		byte* byDataFrame = GetElementTestDataOfOneFrame(uiElementIndex, i);
		for(int j = 0; j < m_uiGroupDataCount; j++)
		{
			memcpy(&m_pArrayTestDataOfOneElement[(i * m_uiGroupDataCount * 4) + (j * 4)], &byDataFrame[j * 3], 3);
			int idata=0;
			if(j>1 && FrameADCount72==72){
				memcpy(&idata, &byDataFrame[j * 3], 3);
/*				if(idata>0x800000){
					idata=-(0xffffff-idata);
				}
*/				iAVG=iAVG+idata;
				iAVG=iAVG&0xffffff;
				iAVGCount++;
			}
			if(FrameADCount72==74){
				memcpy(&idata, &byDataFrame[j * 3], 3);
				if(idata>0x800000){
					idata=-(0xffffff-idata);
				}
				iAVG=iAVG+idata;
				iAVGCount++;
			}
		}
	}
	// 得到数据
	if(iAVG>0x400000)
		m_AVGNoiseValue=0xffffff-(0xffffff-iAVG)/iAVGCount;
	else
		m_AVGNoiseValue=(int)iAVG/iAVGCount;
	return m_pArrayTestDataOfOneElement;
}

/**
* 得到测试单元一次测试数据，噪声监测
* @param unsigned int uiFrameIndex 帧索引
* @param unsigned int uiElementIndex 单元索引
* @return byte* 数据指针
*/
byte* CTestDataBuffer::GetElementTestDataOfOneTestForNoiseMonitor(unsigned int uiFrameIndex, unsigned int uiElementIndex)
{
	// 数据置0
	memset(m_pArrayTestDataOfOneElement, 0x00, 296);
	// 得到测试单元一帧测试数据
	byte* byDataFrame = GetElementTestDataOfOneFrame(uiElementIndex, uiFrameIndex);
	for(int i = 0; i < m_uiGroupDataCount; i++)
	{
		memcpy(&m_pArrayTestDataOfOneElement[i * 4], &byDataFrame[i * 3], 3);
	}
	// 得到数据
	return m_pArrayTestDataOfOneElement;
}