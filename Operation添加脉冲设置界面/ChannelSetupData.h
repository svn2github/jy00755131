#pragma once

class CChannelSetupData
{
public:
	CChannelSetupData(void);
	~CChannelSetupData(void);

public: // 属性
	/** 在测道数组中的位置*/
	unsigned int m_uiIndex;

	/** 测道是否使用中*/
	bool m_bInUsed;

	/** 测线号*/
	unsigned int m_uiNbLine;
	/** 测点号*/
	unsigned int m_uiNbPoint;
	/** 测点索引号*/
	unsigned int m_uiIndexPoint;
	/** 测道号*/
	unsigned int m_uiNbChannel;
	/** IP地址*/
	unsigned int m_uiIP;

	/** 检波器号*/
	unsigned int m_uiSensorNb;
	/** 是否跳过道*/
	bool m_bJumped;
	/** 是否哑点*/
	bool m_bMute;

	/** 仪器设备号*/
	unsigned int m_uiSN;

	/** 左侧链接的测道*/
	CChannelSetupData* m_pChannelLeft;
	/** 左侧链接的测道*/
	CChannelSetupData* m_pChannelRight;
	/** 上面链接的测道*/
	CChannelSetupData* m_pChannelTop;
	/** 下面链接的测道*/
	CChannelSetupData* m_pChannelDown;

public: // 方法
	// 根据测线号、测点号、测道号，生成IP地址
	static unsigned int CreateIP(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel);
};
