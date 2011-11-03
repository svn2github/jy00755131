
#include "ADCSet.h"
#include "Parameter.h"
#include "ADCFrameInfo.h"
#include "LogFile.h"
#pragma once
// CSysTimeSocket 命令目标

class CSysTime
{
public:
	CSysTime();
	virtual ~CSysTime();
protected:
	unsigned char udp_buf[RcvFrameSize];
	unsigned char ADCSampleCmd[SndFrameSize];
	// 采集站本地时间
	unsigned int m_uiSysTime;
	// 采集站本地时刻查询帧
	byte m_cCollectSysTimeSendData[SndFrameSize];
public:
	// 源IP地址
	unsigned int m_uiIPSource;
	// ADC设置类指针
	CADCSet* m_pADCSet;
	// 界面指针
	CWnd* m_pWnd;
	// 本地时间查询应答计数
	unsigned int m_uiSysTimeCount;
	// 类CADCFrameInfo成员变量指针
	CADCFrameInfo* m_pADCFrameInfo;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in m_RecvAddr, m_SendToAddr;
	SOCKET m_SysTimeSocket;
	// 仪器设备列表类CInstrumentList的指针
	CInstrumentList* m_pInstrumentList;
protected:
	// ADC设置TB时刻开始采集
	void OnADCStartSample(unsigned int tnow);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 生成采集站本地时间查询帧
	void MakeCollectSysTimeFrameData(void);
	// 发送采集站本地时间查询帧
	void SendCollectSysTimeFrameToSocket(void);
	// 处理本地时间查询应答
	void OnProcSysTimeReturn(int iPos);
	// 消息处理函数
	void OnReceive(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
};


