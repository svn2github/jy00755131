#pragma once

class CADCSet
{
public:
	CADCSet(void);
	~CADCSet(void);
public:
	// ADC设置帧
	byte m_cFrameData[128];
	// ADC设置帧头
	void ADCSetFrameHead(unsigned int uiIPAim, bool bBroadCast);
	// 发送ADC设置的18指令
	int ADCCommand_18(int iPos, byte * cADCSet, unsigned int uiLength);
	// 设置正弦输出
	void OnSetSine(void);
	// 设置停止采集
	void OnStopSample(void);
	// 打开电源
	void OnOpenPower(void);
	// 打开TB电源
	void OnOpenPowerTB(void);
	// 打开开关
	void OnOpenSwitch(void);
	// 打开TB开关
	void OnOpenSwitchTB(void);
	// 读ADC寄存器
	void OnADCRegisterRead(void);
	// 写ADC寄存器
	void OnADCRegisterWrite(void);
	// ADC设置参数返回
	void OnADCSetReturn(void);
	// ADC连续读取数据
	void OnADCReadContinuous(void);
	// 设置TB时间
	void OnSetTB(unsigned int tnow);
};
