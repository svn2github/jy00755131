#pragma once

/**
*@brief 仪器时统设置帧类
*/
class CFrameTimeSet
{
public:
	CFrameTimeSet();
	~CFrameTimeSet();

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

	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;

public: // 方法
	// 重置对象
	void Reset();
	// 解析时间设置应答帧
	bool ParseFrame();
	// 解析命令字
	bool ParseCommandWord();
	// 生成仪器时间地址设置帧
	void MakeFrameData();
};
