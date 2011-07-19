#pragma once
#include "TabADCSettings.h"
#include "Parameter.h"
#include "InstrumentList.h"
#include "LogFile.h"
class CADCSet
{
public:
	CADCSet(void);
	~CADCSet(void);
public:
	// ADC设置帧
	unsigned char m_ucFrameData[SndFrameSize];
	// 源IP地址
	unsigned int m_uiIPSource;
	// 设备类指针
	CInstrumentList* m_pInstrumentList;
	// ADC参数设置选择界面指针
	CTabADCSettings* m_pTabADCSettings;
	// 设备选择指针
	int* m_pSelectObject;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in m_RecvAddr, m_SendToAddr;
	SOCKET m_ADCSetSocket;
protected:
	// 数据接收缓冲区
	unsigned char udp_buf[RcvFrameSize];
	// 零漂偏移量
	unsigned char m_ucZeroDrift[InstrumentNum][ADCZeroDriftWritebytesNum];
protected:
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 处理零漂校正查询应答
	void OnProcADCZeroDriftReturn(int iPos);
public:
	// ADC设置帧头
	int ADCSetFrameHead(unsigned int uiIPAim, unsigned short	usCommand, unsigned short usPortAim);
	// 发送ADC设置的18指令
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
	// 设置TB时间
	void OnSetTB(int iPos, unsigned int tbh, unsigned int tbl, bool bSwitch);
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
	// 计算CRC校验值
	void OnCRCCal(void);
	// 打开TB开关
	void OnSetTBSwitchOpen(int iPos);
	// 查询采集站错误代码
	void OnQueryErrorCodeFdu(int iPos);
	// 查询交叉站故障计数
	void OnQueryErrorCodeLAUX(int iPos);
	// 查询仪器误码
	void OnQueryErrorCode(void);
	// ADC参数设置
	void OnADCSet(void);
	// ADC数据采集停止
	void OnADCSampleStop(void);
	// ADC零漂校正
	void OnADCZeroDrift(void);
	// ADC设置TB时刻开始采集
	void OnADCStartSample(unsigned int tnow);
	// 消息处理函数
	void OnReceive(void);
	// 广播查询采集站TB时刻
	void OnQueryTBTime(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
	// 测试端口
	void OnPortTest(unsigned short usPortAim);
};
