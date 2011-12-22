#pragma once
#include "Parameter.h"
/**
*@brief 仪器尾包帧类
*/
class CFrameTail
{
public:
	CFrameTail();
	~CFrameTail();

public: // 属性
	/** 帧数据*/
	byte m_pFrameData[RcvFrameSize];
	/** 仪器本地IP地址*/
	unsigned int m_uiIPInstrument;
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiInstrumentType;
	/** 仪器系统时间*/
	unsigned int m_uiTimeSystem;
	/** 路由IP地址，路由方向 1-上；2-下；3-左；4-右*/
	unsigned int m_uiRoutIP;

public: // 方法
	// 重置对象
	void Reset();
	// 解析首包帧
	bool ParseFrame();
};
