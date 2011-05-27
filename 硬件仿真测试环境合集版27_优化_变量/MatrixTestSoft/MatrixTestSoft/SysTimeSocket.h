
#include "ADCSet.h"
#include "Parameter.h"
#include "InstrumentList.h"
#pragma once
// CSysTimeSocket 命令目标

class CSysTimeSocket : public CSocket
{
public:
	CSysTimeSocket();
	virtual ~CSysTimeSocket();
public:
	virtual void OnReceive(int nErrorCode);
protected:
	unsigned char udp_buf[RcvFrameSize];
	unsigned char ADCSampleCmd[SndFrameSize];
	// 采集站本地时间
	unsigned int m_uiSysTime;
	// 目标IP地址
	unsigned int m_uiIPAim;
	// 零漂偏移量
	unsigned char m_ucZeroDrift[InstrumentNum][ADCZeroDriftWritebytesNum];
public:
	// 源IP地址
	CString m_csIPSource;
	// 目标端口
	unsigned int m_uiSendPort;
	// ADC设置类指针
	CADCSet* m_pADCSet;
	// 设备类指针
	CInstrumentList* m_pInstrumentList;
	// 设备选择指针
	int* m_pSelectObject;
	// 界面指针
	CWnd* m_pwnd;
	// 本地时间查询应答计数
	unsigned int m_uiSysTimeCount;
protected:
	// ADC设置TB时刻开始采集
	void OnADCStartSample(unsigned int uiIPAim, unsigned int tnow);
	// 	// 生成数据采样设置帧
	// 	void MakeFrameData(unsigned int uiIPAim, unsigned int tnow);
	// 	// 发送数据采样设置帧
	// 	void SendFrameData(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};


