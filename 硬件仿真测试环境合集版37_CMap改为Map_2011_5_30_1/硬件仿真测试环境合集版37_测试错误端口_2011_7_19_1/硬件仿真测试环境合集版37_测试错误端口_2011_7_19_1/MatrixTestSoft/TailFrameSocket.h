#pragma once
#include "InstrumentList.h"
#include "LogFile.h"
// CTailFrameSocket 命令目标

class CTailFrame
{
public:
	CTailFrame();
	virtual ~CTailFrame();
private:
	// 尾包接收数据缓冲
	byte m_pTailFrameData[RcvFrameSize];
	// 尾包时刻查询帧
	byte m_pTailTimeSendData[SndFrameSize];
public:
	// 仪器SN号
	DWORD m_uiSN;
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// 源IP地址
	unsigned int m_uiIPSource;
	// 交叉站尾包接收标志位
	BOOL m_bTailRecLAUX;
	// 交叉站收到尾包计数
	unsigned int m_uiTailRecLAUXCount;
	// 采集站尾包接收标志位
	BOOL m_bTailRecFDU;
	// 采集站收到尾包计数
	unsigned int m_uiTailRecFDUCount;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in m_RecvAddr, m_SendToAddr;
	SOCKET m_TailFrameSocket;
private:
	// 处理单个尾包
	void ProcTailFrameOne(void);
	// 解析尾包
	BOOL ParseTailFrame(void);
	// 生成尾包时刻查询帧
	void MakeTailTimeFrameData(CInstrument* pInstrument);
	// 发送尾包时刻查询帧
	void SendTailTimeFrameToSocket(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 向仪器发送尾包时刻查询帧
	void SendTailTimeFrame(void);
	// 消息处理函数
	void OnReceive(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
};


