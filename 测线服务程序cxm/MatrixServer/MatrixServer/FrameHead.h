#pragma once

/**
*@brief 仪器首包帧类
*/
class CFrameHead
{
public:
	CFrameHead();
	~CFrameHead();

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
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 仪器首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 路由IP地址，路由方向 1-上；2-下；3-左；4-右*/
	unsigned int m_uiRoutIP;

public: // 方法
	// 重置对象
	void Reset();
	// 解析首包帧
	bool ParseFrame();
	// 解析命令字
	bool ParseCommandWord();
};
