#pragma once

#include "Instrument.h"

class CIndexInstrumentMap
{
public:
	CIndexInstrumentMap(void);
	~CIndexInstrumentMap(void);

public: //属性
	// 哈希表
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMap;

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
	void DeleteInstrumentAll(void);
	// 仪器索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);

	// 得到首个仪器位置
	POSITION GetFirstInstrumentPosition();
	// 得到下一个仪器
	void GetNextInstrumentByPosition(POSITION &pos, CInstrument* &pInstrument);
};
