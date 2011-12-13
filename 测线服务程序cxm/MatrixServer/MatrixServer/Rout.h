#pragma once

#include "Instrument.h"

/**
*@brief 路由数据类
*/
class CRout
{
public:
	CRout();
	~CRout();

public: //属性
	/** 在路由数组中的位置*/
	unsigned int m_uiIndex;
// 
	/** 路由是否使用中*/
	bool m_bInUsed;

	/** 路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	unsigned int m_uiRoutDirection;
	/** 路由头仪器*/
	CInstrument* m_pHead;
	/** 路由尾仪器*/
	CInstrument* m_pTail;

// 	/** 路由头仪器*/
// 	unsigned int m_uiSNHead;
// 	/** 路由尾仪器*/
// 	unsigned int m_uiSNTail;
// 	/** 路由头仪器*/
// 	int m_iIndexHead;
// 	/** 路由尾仪器*/
// 	int m_iIndexTail;
	/** 路由时刻*/
	unsigned int m_uiRoutTime;
	/** 上次时统处理时刻*/
	unsigned int m_uiDelayProcTime;

public: //方法
	// 重置
	void OnReset();
	// 更新路由时间
	void UpdateRoutTime();
	// 更新上次时统处理时刻
	void UpdateDelayProcTime();
};
