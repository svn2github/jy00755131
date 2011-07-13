#pragma once
#include "InstrumentList.h"
#include "LogFile.h"
// CHeadFrameSocket 命令目标

class CHeadFrame
{
public:
	CHeadFrame();
	virtual ~CHeadFrame();
protected:
	// 仪器串号
	DWORD m_uiSN;
	// 首包时刻
	DWORD m_uiHeadFrameTime;
	// 路由地址
	DWORD m_uiRoutAddress;
	// 首包接收数据缓冲
	byte m_pHeadFrameData[RcvFrameSize];
	// 设置IP地址缓冲
	byte m_pIPSetFrameData[SndFrameSize];
public:
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// 源IP地址
	unsigned int m_uiIPSource;
	// 界面指针
	CWnd* m_pwnd;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in m_RecvAddr, m_SendToAddr;
	SOCKET m_HeadFrameSocket;
protected:
	// 处理单个首包
	void ProcHeadFrameOne();
	// 解析首包
	BOOL ParseHeadFrame(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 生成IP地址设置帧
	void MakeIPSetFrame(CInstrument* pInstrument, BOOL bSetIP);
	// 发送IP地址设置帧
	void SendIPSetFrame(void);
	// 监测IP地址设置应答帧
	bool OnMonitorIPSetReturn(void);
	// 消息处理函数
	void OnReceive(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
};