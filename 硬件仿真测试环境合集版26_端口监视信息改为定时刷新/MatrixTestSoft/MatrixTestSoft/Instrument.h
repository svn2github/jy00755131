#pragma once

class CInstrument
{
public:
	CInstrument(void);
	~CInstrument(void);

	// 本地IP地址
	DWORD m_uiIPAddress;
	// 仪器串号
	DWORD m_uiSN;
	// 路由地址
	DWORD m_uiRoutAddress;
	// 首包时刻
	DWORD m_uiHeadFrameTime;
	// 本地系统时间
	DWORD m_uiSystemTime;
	// 首包计数
	int m_iHeadFrameCount;
	// 尾包计数
	int m_iTailFrameCount;
	// 重置成员变量
	void OnReset(void);
	// 在仪器数组中的位置
	unsigned int m_uiIndex;
	// 仪器使用标志位
	bool m_bInUsed;
	// 仪器的仪器类型：0-未设置，1-接口站，3-采集站
	unsigned int m_uiInstrumentType;
	// IP地址设置成功标志位
	bool m_bIPSetOK;
	// 设备位置序号
	unsigned int m_uiLocation;
	// 设备的网络时间
	DWORD m_uiNetTime;
	// 设备尾包接收时刻
	unsigned short m_usTailRecTime;
	// 设备尾包发送时刻
	unsigned short m_usTailSendTime;
	// 尾包时刻查询成功标志位
	bool m_bTailTimeReturnOK;
	// 尾包时刻查询过期标志位
	bool m_bTailTimeExpired;
	// 发送了尾包时刻查询帧
	bool m_bSendTailTimeFrame;
	// ADC参数设置标志位
	bool m_bADCSet;
	// IP地址重设次数
	unsigned int m_uiIPResetTimes;
};
