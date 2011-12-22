#pragma once
#include "Parameter.h"
/**
*@brief 仪器首包帧类
*/
class CFrameHead
{
public:
	CFrameHead();
	~CFrameHead();

public: // 属性
	/** 帧数据*/
	byte m_pFrameData[RcvFrameSize];
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 仪器首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	// 仪器本地时间
	unsigned int m_uiSysTime;
	/** 路由IP地址，路由方向 1-上；2-下；3-左；4-右*/
	unsigned int m_uiRoutIP;

public: // 方法
	// 重置对象
	void Reset();
	// 解析首包帧
	bool ParseFrame();
};
