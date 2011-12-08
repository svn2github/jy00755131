#pragma once

/**
*@brief 仪器测试设置帧类
*/
class CFrameTestSet
{
public:
	CFrameTestSet();
	~CFrameTestSet();

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

	/** AD采集开始时刻*/
	unsigned int m_uiADTimeBegin;
	/** AD采集开始时刻高位*/
	unsigned int m_uiADTimeBeginHight;
	/** AD采集开始时刻低位*/
	unsigned int m_uiADTimeBeginLow;
	/** 采集开始时刻*/
	unsigned int m_uiCollectTimeBegin;
	/** 采集开始时刻高位*/
	unsigned int m_uiCollectTimeBeginHight;
	/** 采集开始时刻低位*/
	unsigned int m_uiCollectTimeBeginLow;
	/** 采集结束时刻*/
	unsigned int m_uiCollectTimeEnd;
	/** 采集结束时刻高位*/
	unsigned int m_uiCollectTimeEndHight;
	/** 采集结束时刻低位*/
	unsigned int m_uiCollectTimeEndLow;
	/** 采集命令监听端口*/
	unsigned short m_usPortListen;
	/** 数据采集输出的目的IP地址*/
	unsigned int m_uiIPDataOut;
	/** 数据采集输出的目的端口*/
	unsigned short m_usPortDataOut;
	/** 数据采集输出的目的端口命令 ad_cmd(15)=1，端口递增；=0，端口不变*/
	unsigned short m_usPortDataOutCMD;
	/** 命令码： 采样参数配置*/
	DWORD m_dwConfig;
	/** 命令码： 滤波器参数设置0*/
	DWORD m_dwFilter17;
	/** 命令码： 滤波器参数设置1*/
	DWORD m_dwFilter18;

	/** 采样率 单位微秒*/
	unsigned int m_uiSamplingRate;
	/** 滤波器参数*/
	unsigned int filter_hex;
	/** 高通滤波器参*/
	float high_pass_freq;

public: // 方法
	// 重置对象
	void Reset();
	// 生成测试设置帧
	void MakeFrameDataForTestSet();
	// 生成测试启动帧
	void MakeFrameDataForTestStart();
	// 生成测试停止帧
	void MakeFrameDataForTestEnd();
	// ADC设置帧头
	int ADCSetFrameHead(unsigned int uiIPAim, unsigned short	usCommand, unsigned short usPortAim);
// 	//设置AD帧头
// 	int MakeFrameHead();
// 	//不广播
// 	int MakeFrameHead1();
	//设置18命令组
	int ADCCommand_18(int iPos, byte * cADCSet, unsigned int uiLength);
	// 设置正弦输出
	void OnSetSine(int iPos);
	// 设置停止采集
	void OnStopSample(int iPos);
	// 打开电源
	void OnOpenPowerTBLow(int iPos);
	// 打开TB电源
	void OnOpenPowerTBHigh(int iPos);
	// 打开开关
	void OnOpenSwitchTBLow(int iPos);
	// 打开TB开关
	void OnOpenSwitchTBHigh(int iPos);
	// 读ADC寄存器
	void OnADCRegisterRead(int iPos);
	// 写ADC寄存器
	void OnADCRegisterWrite(int iPos, bool bSwitch);
	// ADC设置参数返回
	void OnADCSetReturn(int iPos);
	// ADC连续读取数据
	void OnADCReadContinuous(int iPos);
	void OnNULL(int iPos);
	// 设置TB时间
	void OnSetTB(int iPos, unsigned int tbh, unsigned int tbl, bool bSwitch);
//	int OnSetTB(unsigned int tnow);
	//设置AD帧头
//	int MakeFrameHead();
	//不广播
//	int MakeFrameHead1();
	// 打开零漂矫正电源及开关
	void OnOpenPowerZeroDrift(int iPos);
	// 查询ADC寄存器
	void OnADCRegisterQuery(int iPos);
	// ADC数据采集同步
	void OnADCSampleSynchronization(int iPos);
	// ADC零漂偏移矫正
	void OnADCZeroDriftCorrection(int iPos);
	// ADC零漂设置（按IP地址）
	void OnADCZeroDriftSetFromIP(int iPos, unsigned char* ucZeroDrift);
	// 打开TB开关
	void OnSetTBSwitchOpen(int iPos);
	// 广播查询采集站TB时刻
	void OnQueryTBTime(int iPos);
	int GetTB();
	void MakeFrameDataForADTestStart();
	void MakeFrameDataForADTestEnd();
};
