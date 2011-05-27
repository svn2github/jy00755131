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
	void ADCSetFrameHead(unsigned int uiIPAim, bool bBroadCast, unsigned short	usCommand, unsigned short usPortAim);
	// 发送ADC设置的18指令
	int ADCCommand_18(int iPos, byte * cADCSet, unsigned int uiLength);
	// 设置正弦输出
	void OnSetSine(void);
	// 设置停止采集
	void OnStopSample(void);
	// 打开电源
	void OnOpenPowerTBLow(void);
	// 打开TB电源
	void OnOpenPowerTBHigh(void);
	// 打开开关
	void OnOpenSwitchTBLow(void);
	// 打开TB开关
	void OnOpenSwitchTBHigh(void);
	// 读ADC寄存器
	void OnADCRegisterRead(void);
	// 写ADC寄存器
	void OnADCRegisterWrite(void);
	// ADC设置参数返回
	void OnADCSetReturn(void);
	// ADC连续读取数据
	void OnADCReadContinuous(bool bBroadCast);
	// 设置TB时间
	void OnSetTB(unsigned int tnow);
	// 打开零漂矫正电源及开关
	void OnOpenPowerZeroDrift(void);
	// 查询ADC寄存器
	void OnADCRegisterQuery(void);
	// ADC数据采集同步
	void OnADCSampleSynchronization(void);
	// ADC零漂偏移矫正
	void OnADCZeroDriftCorrection(void);
	// ADC零漂设置（按IP地址）
	void OnADCZeroDriftSetFromIP(unsigned char* ucZeroDrift);
	// ADC参数设置选择界面指针
	CWnd* m_pWnd;
	// 记录TB开始时间
	unsigned int m_uiTBTime;
};
