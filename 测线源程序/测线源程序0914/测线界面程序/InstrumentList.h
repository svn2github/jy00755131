#pragma once

#include "Instrument.h"
#include "indexinstrumentmap.h"

class CInstrumentList
{
public:
	CInstrumentList(void);
	~CInstrumentList(void);

public: //属性
	// 仪器数组指针
	CInstrument* m_pArrayInstrument;
	// 仪器总数
	unsigned int m_uiCountAll;
	// 空闲仪器数量
	unsigned int m_uiCountFree;
	// 使用中仪器数量
	unsigned int m_uiCountUsed;

	CInstrument* m_pFirstMainCross;	// 首个主交叉站指针	
	CIndexInstrumentMap m_oSNInstrumentMap;	// SN仪器索引表	
	CIndexInstrumentMap m_oIPInstrumentMap;	// IP地址仪器索引表

	CIndexInstrumentMap m_oSNInstrumentCrossMap;	// SN仪器索引表，交叉站
	CIndexInstrumentMap m_oSNInstrumentPowerMap;	// SN仪器索引表，电源站

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 加载现场仪器数据队列
	void LoadSiteData();

	// 设置交叉站在测区中的位置
	void SetInstrumentCrossSurveryPosition();
	// 设置电源站在测区中的位置
	void SetInstrumentPowerSurveryPosition();
	// 设置仪器在测区中的位置
	void SetInstrumentSurveryPosition(CInstrument* &pInstrument);

	// 得到首个仪器位置
	POSITION GetFirstInstrumentPosition();
	// 得到下一个仪器
	void GetNextInstrumentByPosition(POSITION &pos, CInstrument* &pInstrument);

	// 得到迂回道中仪器数量
	int GetDetourInstrumentCount(unsigned int uiIndex);

	// 判断仪器A是否在仪器B左边
	bool JudgeInstrumentAAtInstrumentBLeft(CInstrument* pInstrumentA, CInstrument* pInstrumentB);
	// 判断仪器A是否在仪器B右边
	bool JudgeInstrumentAAtInstrumentBRight(CInstrument* pInstrumentA, CInstrument* pInstrumentB);
};
