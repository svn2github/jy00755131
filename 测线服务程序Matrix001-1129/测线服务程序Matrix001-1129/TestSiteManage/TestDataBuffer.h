#pragma once

/**
*@brief 测试数据缓冲区类
*/
#include "Matrixline.h"

class CTestDataBuffer
{
public:
	CTestDataBuffer();
	~CTestDataBuffer();

public: //属性	
	/** 测试单元总数*/
	unsigned int m_uiTestElementCountAll;

	/** 测试数据数组指针，一个测试单元的；74 * 4 * 28 = 8288；2048 * 4 = 8192*/
	//4个站采集30秒 74*4*4*30=35520,48个站=426240
	//byte m_pArrayTestDataOfOneElement[35520];
	byte m_pArrayTestDataOfOneElement[ArrayTestDataOfOne];
		
	/** 测试数据数组指针*/
	byte* m_pArrayTestData;
	/** 总字节数*/
	unsigned int m_uiByteCountAll;

	/** 每个数据占用字节*/
	unsigned int m_uiDataByteSize;
	/** 每组数据包含数据个数*/
	unsigned int m_uiGroupDataCount;
	/** 每组数据占用字节*/
	unsigned int m_uiGroupDataByteSize;
	/** 块数据个数*/
	unsigned int m_uiBlockCount;
	/** 每块数据包含数据组个数*/
	unsigned int m_uiBlockGroupCount;
	/** 每块数据占用字节*/
	unsigned int m_uiBlockByteSize;
	/** 数据区覆盖写入块起始位置*/
	unsigned int m_uiBlockIndexForReWrite;

	/** 测试单元数组总数，使用的*/
	unsigned int m_uiTestElementCountAllUsed;
	/** 每块数据包含数据组个数，使用的*/
	unsigned int m_uiBlockGroupCountUsed;
	/** 每块数据占用字节，使用的*/
	unsigned int m_uiBlockByteSizeUsed;
	/** 总字节数，使用的*/
	unsigned int m_uiByteCountAllUsed;
	/** 仪器噪声测试值*/
	int m_AVGNoiseValue;
	/** 数据帧索引*/
	unsigned int m_uiFrameIndex;
	/** 数据帧索引，写数据文件用*/
	unsigned int m_uiFrameIndexWrite;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 重置，为一次新的测试请求
	void ResetForNewTestRequest();
	// 测试设置
	void OnTestSetup(unsigned int uiTestElementCountAllUsed);
	// 得到块索引
	int GetBlockIndexByFrameIndex(unsigned int uiFrameIndex);
	// 设置测试单元一帧测试数据
	void SetElementTestDataOfOneFrame(byte* lpData, unsigned int uiElementIndex, unsigned int uiFrameIndex);
	// 得到测试单元一帧测试数据
	byte* GetElementTestDataOfOneFrame(unsigned int uiElementIndex, unsigned int uiFrameIndex);
	// 得到一帧测试数据，单位块
	byte* GetBlockTestDataOneFrame(unsigned int uiFrameIndex);
	// 得到测试单元一次测试数据
	byte* GetElementTestDataOfOneTest(unsigned int uiElementIndex, unsigned int uiSamplingPointCount);
	// 得到测试单元一次测试数据，噪声监测
	byte* GetElementTestDataOfOneTestForNoiseMonitor(unsigned int uiFrameIndex, unsigned int uiElementIndex);
};