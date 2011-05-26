#pragma once
#include "ADDataThread.h"
#include "HeartBeatThread.h"
#include "HeadFrameThread.h"
#include "InstrumentList.h"
#include "TailFrameThread.h"
#include "SysTimeSocket.h"
#include "Parameter.h"
class CThreadManage
{
public:
	CThreadManage(void);
	~CThreadManage(void);
public:
	CHeartBeatThread	m_oHeartBeatThread;
	CHeadFrameThread	m_oHeadFrameThread;
	CADDataThread		m_oADDataThread;
	CInstrumentList		m_oInstrumentList;
	CTailFrameThread	m_oTailFrameThread;
	// 查询采集站本地时间Socket
	CSysTimeSocket m_oSysTimeSocket;
	// 采集站本地时刻查询帧
	byte m_cCollectSysTimeSendData[128];
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
	void MakeCollectSysTimeFrameData(unsigned int uiTestNb);
	// 发送采集站本地时间查询帧
	void SendCollectSysTimeFrameToSocket(void);
};
