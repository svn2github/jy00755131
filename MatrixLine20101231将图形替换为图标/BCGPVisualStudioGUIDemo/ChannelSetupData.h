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

	/** 野外电阻测试值*/
	float m_fResistanceValue;
	/** 野外电阻测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byResistanceCheck;

	/** 野外漏电测试值*/
	float m_fLeakageValue;
	/** 野外漏电测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byLeakageCheck;

	/** 野外噪声测试值*/
	float m_fNoiseValue;
	/** 野外噪声测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byNoiseCheck;

	/** 野外倾斜度测试值*/
	float m_fTiltValue;
	/** 野外倾斜度测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byTiltCheck;

	/** 野外倾斜度模型测试值*/
	float m_fTiltModelValue;
	/** 野外倾斜度模型测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byTiltModelCheck;

	/** 野外测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号*/
	byte m_byCheck;

	/** 噪声监测测试值*/
	float m_fNoiseMonitorValue;
	/** 噪声监测测试结果 0：未测试；1：测试合格；2：测试部合格；3：无信号*/
	byte m_byNoiseMonitorCheck;

	/** 检波器是否进行了初始测试*/
	bool m_bTestOnField;

public: // 方法
	// 根据测线号、测点号、测道号，生成IP地址
	static unsigned int CreateIP(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel);

	// 得到测试结果字符串
	CString GetTestResultString(unsigned int uiType);
	// 得到测试结果字符串
	CString GetTestResultString(byte byCheck, float m_fValue);
	// 得到测试结果
	byte GetTestResultCheck(unsigned int uiType, float fSeismonitorLimit);
};
