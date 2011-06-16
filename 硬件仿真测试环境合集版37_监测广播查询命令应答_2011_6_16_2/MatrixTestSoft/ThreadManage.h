#pragma once

#include "InstrumentList.h"
#include "SysTimeSocket.h"
#include "HeadFrameSocket.h"
#include "IPSetSocket.h"
#include "TailFrameSocket.h"
#include "TailTimeFrameSocket.h"
#include "HeartBeatThread.h"
#include "ADCDataRecThread.h"
#include "ADCSet.h"
#include "ADCDataSaveToFile.h"
#include "ADCFrameInfo.h"
class CThreadManage
{
public:
	CThreadManage(void);
	~CThreadManage(void);
public:
	CInstrumentList		m_oInstrumentList;
	// 首包接收Socket
	CHeadFrameSocket m_oHeadFrameSocket;
	// IP地址设置Socket
	CIPSetSocket m_oIPSetSocket;
	// 尾包接收Socket
	CTailFrameSocket m_oTailFrameSocket;
	// 尾包时刻查询应答Socket
	CTailTimeFrameSocket m_oTailTimeFrameSocket;
	// 查询采集站本地时间Socket
	CSysTimeSocket m_oSysTimeSocket;
	// ADC设置类对象
	CADCSet m_oADCSetSocket;
	// 心跳Socket对象
	CSocket m_oHeartBeatSocket;
	// ADC数据接收Socket
	CSocket m_oADCDataSocket;
	// 类CADCDataSaveToFile成员变量
	CADCDataSaveToFile m_oADCDataSaveToFile;
	// 类CADCFrameInfo成员变量
	CADCFrameInfo m_oADCFrameInfo;
	// 心跳线程
	CHeartBeatThread	m_oHeartBeatThread;
	// ADC数据接收线程
	CADCDataRecThread	m_oADCDataRecThread;

private:
	// 创建查询采集站本地时间的广播端口
	void OnCreateGetSysTimeSocket(void);
	// 创建首包Socket
	void OnCreateHeadSocket(void);
	// 创建IP地址设置Socket
	void OnCreateIPSetSocket(void);
	// 创建尾包Socket
	void OnCreateTailSocket(void);
	// 创建尾包时刻查询和时统Socket
	void OnCreateTailTimeSocket(void);
	// 创建ADC设置Socket
	void OnCreateADCSetSocket(void);
	// 创建心跳Socket
	void OnCreateHeartBeatSocket(void);
	// 创建ADC数据接收Socket
	void OnCreateADCDataSocket(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 避免端口阻塞
	void OnAvoidIOBlock(SOCKET socket);
	// 创建和设置Socket
	void OnCreateAndSetSocket(CSocket* socket, bool bBroadCast, int iSocketPort, CString str, int iRecBuf, int iSendBuf);
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
};
