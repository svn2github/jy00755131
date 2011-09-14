#pragma once

/**
*@brief 仪器尾包时刻帧类
*/
class CFrameTailTime
{
public:
	CFrameTailTime();
	~CFrameTailTime();
public: // 属性
	/** 帧大小*/
	int m_iFrameSize;
	/** 帧数据*/
	byte m_pFrameData[256];
	/** 源IP地址*/
	unsigned int m_uiIPSource;
	/** 目标IP地址*/
	unsigned int m_uiIPAim;
	/** 目标端口号*/
	unsigned short m_usPortAim;
	/** 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发*/
	unsigned short m_usCommand;

	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiInstrumentType;

	/** 16bits时间，接收时刻低位*/
	unsigned short m_usReceiveTime;	
	/** 16bits时间，发送时刻低位*/
	unsigned short m_usSendTime;	

	/** 本地时间*/
	unsigned int m_uiTimeSystem;
	/** 网络时间*/
	unsigned int m_uiNetTime;
	/** 网络时间*/
	unsigned int m_uiNetState;
	/** 网络时间*/
	unsigned int m_uiNetOrder;

	/** 16bits 测线方向左面尾包接收时刻*/
	unsigned short m_usLineLeftReceiveTime;	
	/** 16bits 测线方向右面尾包接收时刻*/
	unsigned short m_usLineRightReceiveTime;	

	/** 16bits 交叉线方向上面尾包接收时刻*/
	unsigned short m_usCrossTopReceiveTime;	
	/** 16bits 交叉线方下面尾包接收时刻*/
	unsigned short m_usCrossDownReceiveTime;	

public: // 方法
	// 重置对象
	void Reset();
	// 解析尾包时刻查询应答帧
	bool ParseFrame();
	// 解析命令字
	bool ParseCommandWord();
	// 生成尾包时刻查询帧
	void MakeFrameData();
};
