#pragma once

#include "Instrument.h"

/**
*@brief 仪器索引表类
*/
class CIndexInstrumentMap
{
public:
	CIndexInstrumentMap();
	~CIndexInstrumentMap();

public: //属性
	/** 仪器索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMap;

	CInstrument* m_pInstrument;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 增加一个仪器
	void AddInstrument(unsigned int uiIndex, CInstrument* pInstrument);
	// 得到一个仪器
	BOOL GetInstrument(unsigned int uiIndex, CInstrument* &pInstrument);
	// 删除一个仪器
	void DeleteInstrument(unsigned int uiIndex);
	// 删除所有仪器
	void DeleteInstrumentAll();
	// 仪器索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
};
