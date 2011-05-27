#pragma once

#include "InstrumentList.h"
#include "SysTimeSocket.h"
#include "Parameter.h"
#include "HeadFrameSocket.h"
#include "IPSetSocket.h"
#include "TailFrameSocket.h"
#include "TailTimeFrameSocket.h"
#include "HeartBeatThread.h"
#include "ADCDataRecThread.h"
class CThreadManage
{
public:
	CThreadManage(void);
	~CThreadManage(void);
public:
//	CHeartBeatThread	m_oHeartBeatThread;
	CInstrumentList		m_oInstrumentList;
	// 首包接收Socket
	CHeadFrameSocket m_HeadFrameSocket;
	// IP地址设置Socket
	CIPSetSocket m_IPSetSocket;
	// 尾包接收Socket
	CTailFrameSocket m_TailFrameSocket;
	// 尾包时刻查询应答Socket
	CTailTimeFrameSocket m_TailTimeFrameSocket;
	// 查询采集站本地时间Socket
	CSysTimeSocket m_oSysTimeSocket;
	
	// 心跳线程
	CHeartBeatThread	m_oHeartBeatThread;
	// ADC数据接收线程
	CADCDataRecThread	m_oADCDataRecThread;
	// 采集站本地时刻查询帧
	byte m_cCollectSysTimeSendData[SndFrameSize];
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 创建查询采集站本地时间的广播端口
	void OnCreateCollectSysTimeBroadcastSocket(void);
	// 生成采集站本地时间查询帧
	void MakeCollectSysTimeFrameData(int* pSelectObject);
	// 发送采集站本地时间查询帧
	void SendCollectSysTimeFrameToSocket(void);
	// 创建首包Socket
	void OnCreateHeadSocket(void);
	// 创建IP地址设置Socket
	void OnCreateIPSetSocket(void);
	// 创建尾包Socket
	void OnCreateTailSocket(void);
	// 创建尾包时刻查询和时统Socket
	void OnCreateTailTimeSocket(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};
