#pragma once
#include "Parameter.h"
/**
*@brief 仪器IP地址设置帧类
*/
class CFrameIPSet
{
public:
	CFrameIPSet();
	~CFrameIPSet();

public: // 属性
	/** 接收帧数据*/
	byte m_pRcvFrameData[RcvFrameSize];
	/** 发送帧数据*/
	byte m_pSndFrameData[SndFrameSize];
	/** 源IP地址*/
	unsigned int m_uiIPSource;
	/** 目标IP地址*/
	unsigned int m_uiIPAim;
	/** 目标端口号*/
	unsigned short m_usPortAim;
	/** 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发*/
	unsigned short m_usCommand;
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiInstrumentType;
	/** 仪器本地IP*/
	unsigned int m_uiIPInstrument;
	/** 路由IP地址，路由方向 1-上*/
	unsigned int m_uiRoutIPTop;
	/** 路由IP地址，路由方向 2-下*/
	unsigned int m_uiRoutIPDown;
	/** 路由IP地址，路由方向 3-左*/
	unsigned int m_uiRoutIPLeft;
	/** 路由IP地址，路由方向 4-右*/
	unsigned int m_uiRoutIPRight;
	/** 路由IP地址，路由方向 1-上；2-下；3-左；4右*/
	unsigned int m_uiRoutIP;
	// 广播端口
	unsigned int m_uiBroadCastPort;
	// 路由开关
	unsigned int m_uiRoutOpen;
public: // 方法
	// 重置对象
	void Reset();
	// 解析IP地址设置应答帧
	bool ParseFrame();
	// 生成仪器IP地址设置帧
	void MakeFrameData();
};
