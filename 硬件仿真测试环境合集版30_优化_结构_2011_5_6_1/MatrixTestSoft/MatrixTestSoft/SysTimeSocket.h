
#include "ADCSet.h"
#include "Parameter.h"
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
	// 采集站本地时刻查询帧
	byte m_cCollectSysTimeSendData[SndFrameSize];
public:
	// 源IP地址
	CString m_csIPSource;
	// 目标端口
	unsigned int m_uiSendPort;
	// ADC设置类指针
	CADCSet* m_pADCSet;
	// 界面指针
	CWnd* m_pwnd;
	// 本地时间查询应答计数
	unsigned int m_uiSysTimeCount;
protected:
	// ADC设置TB时刻开始采集
	void OnADCStartSample(unsigned int uiIPAim, unsigned int tnow);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 生成采集站本地时间查询帧
	void MakeCollectSysTimeFrameData(int* pSelectObject);
	// 发送采集站本地时间查询帧
	void SendCollectSysTimeFrameToSocket(void);
	// 处理本地时间查询应答
	void OnProcSysTimeReturn(int iPos);
};


