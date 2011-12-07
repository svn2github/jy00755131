#pragma once

#include "Instrument.h"

/**
*@brief 仪器队列类
*/
class CInstrumentList
{
public:
	CInstrumentList();
	~CInstrumentList();

public: //属性
	/** 仪器数组指针*/
	CInstrument* m_pArrayInstrument;
	/** 仪器数组指针，为现场数据输出准备*/
	CInstrument* m_pArrayInstrumentOutput;
	/** 空闲仪器队列*/
	CList<CInstrument*, CInstrument*> m_olsInstrumentFree;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 得到一个空闲仪器
	CInstrument* GetFreeInstrument();
	// 增加一个空闲仪器
	void AddFreeInstrument(CInstrument* pInstrument);
	// 复制仪器数组到输出数组
	void CopyInstrumentArrayForOutput();
};
