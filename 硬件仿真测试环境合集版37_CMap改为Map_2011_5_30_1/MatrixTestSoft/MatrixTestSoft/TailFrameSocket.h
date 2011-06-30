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
	// 仪器SN号
	DWORD m_uiSN;
	// 未收到尾包计数
	unsigned int m_uiTailRecCount;
	// 尾包开始计数标志位
	bool m_bTailCountStart;
public:
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
	// 尾包接收标志位
	BOOL m_bTailRec;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in addr, addr2;
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
	// 监测尾包
	bool OnTailMonitor(void);
	// 消息处理函数
	void OnReceive(void);
};


