#pragma once

/**
*@brief 测试单元数据类
*/
class CTestElementData
{
public:
	CTestElementData();
	~CTestElementData();

public: //属性	
	/** 在数组中的位置*/
	unsigned int m_uiIndex;
	/** 是否使用中*/
	bool m_bInUsed;	
	/** IP地址*/
	unsigned int m_uiIP;
	/** 数据索引进位基数*/
	unsigned int m_uiDataStep;
	/** 帧计数*/
	unsigned int m_uiFrameIndex;
	/** 上一次的数据索引*/
	unsigned int m_uiDataIndexPreview;
	/** 上一次的数据索引*/
	unsigned int m_uiDataBegin;
	/** 补帧数据索引*/
	unsigned int IndexPreview;
	/** 采集寄存器*/
	unsigned char ofccheck[6];
	// ADC命令设置序号
	unsigned int m_uiADCSetOperationNb;

	/** 仪器噪声测试值*/
	float m_fInstrumentNoiseValue;
	/** 仪器噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentNoiseCheck;
	/** 仪器噪声测试判据*/
	float m_fInstrumentNoiseLimit;

	/** 仪器畸变测试值*/
	float m_fInstrumentDistortionValue;
	/** 仪器畸变测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentDistortionCheck;
	/** 仪器畸变测试判据*/
	float m_fInstrumentDistortionLimit;

	/** 仪器串音测试值*/
	float m_fInstrumentCrosstalkValue;
	/** 仪器串音测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentCrosstalkCheck;
	/** 仪器串音测试判据*/
	float m_fInstrumentCrosstalkLimit;

	/** 仪器增益测试值*/
	float m_fInstrumentGainValue;
	/** 仪器增益测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentGainCheck;
	/** 仪器增益测试判据*/
	float m_fInstrumentGainLimit;
	/** 仪器相位测试值*/
	float m_fInstrumentPhaseValue;
	/** 仪器相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentPhaseCheck;
	/** 仪器相位测试判据*/
	float m_fInstrumentPhaseLimit;
	/** 仪器增益和相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentGainAndPhaseCheck;

	/** 仪器共模测试值*/
	float m_fInstrumentCMRRValue;
	/** 仪器共模测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentCMRRCheck;
	/** 仪器共模测试判据*/
	float m_fInstrumentCMRRLimit;

	/** 仪器测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byInstrumentCheck;

	/** 野外电阻测试值*/
	float m_fSensorResistanceValue;
	/** 野外电阻测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorResistanceCheck;
	/** 野外电阻测试判据，下限*/
	float m_fSensorResistanceLimitMin;
	/** 野外电阻测试判据，上限*/
	float m_fSensorResistanceLimitMax;

	/** 野外漏电测试值*/
	float m_fSensorLeakageValue;
	/** 野外漏电测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorLeakageCheck;
	/** 野外漏电测试判据*/
	float m_fSensorLeakageLimit;

	/** 野外噪声测试值*/
	float m_fSensorNoiseValue;
	/** 野外噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorNoiseCheck;
	/** 野外噪声测试判据*/
	float m_fSensorNoiseLimit;

	/** 野外倾斜度测试值*/
	float m_fSensorTiltValue;
	/** 野外倾斜度测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorTiltCheck;	
	/** 野外倾斜度测试判据*/
	float m_fSensorTiltLimit;

	/** 野外倾斜度模型测试值*/
	float m_fSensorTiltModelValue;
	/** 野外倾斜度模型测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorTiltModelCheck;
	/** 野外倾斜度模型测试判据*/
	float m_fSensorTiltModelLimit;

	/** 野外测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorCheck;

	/** 噪声监测测试值*/
	float m_fSensorNoiseMonitorValue;
	/** 噪声监测测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_bySensorNoiseMonitorCheck;

	/** 检波器是否进行了初始测试*/
	byte m_byTestOnField;
	/** 初始测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byOnFieldCheck;
	/** 测试数据完成计数*/
	int m_uigroupendcount;

public: //方法
	// 重置
	void Reset();
	// 重置测试
	void ResetTest();
	// 重置，为一次新的测试请求
	void ResetForNewTestRequest();
	// 计算得到测试结果
	void CalculateTestResult(unsigned int uiTestType, byte* pData, unsigned int uiSamplingPointCount,float);
	//计算均方差
	float MeanSquare(byte* pData,unsigned int uiSamplingPointCount);
	// 判读测试结果
	void JudgeTestResult(unsigned int uiTestType);
	// 得到测试结果
	int GetTestResult(unsigned int uiTestType, byte* pData);
};
