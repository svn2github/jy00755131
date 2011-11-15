#pragma once
#include "InstrumentList.h"
#include "ADCSet.h"
#include "LogFile.h"
// CIPSetSocket 命令目标

class CIPSet
{
public:
	CIPSet();
	virtual ~CIPSet();
protected:
	// 仪器串号
	DWORD m_uiSN;
	// 仪器的IP地址
	DWORD m_uiIPAddress;
	// IP地址设置应答缓冲
	byte m_pIPSetReturnFrameData[RcvFrameSize];
public:
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// 界面指针
	CWnd* m_pWnd;
	// 日志类指针
	CLogFile* m_pLogFile;
	// Socket套接字
	sockaddr_in m_RecvAddr;
	SOCKET m_IPSetSocket;
protected:
	// 单个IP地址设置应答帧处理
	void ProcIPSetReturnFrameOne(void);
	// 解析IP地址设置应答帧
	BOOL ParseIPSetReturnFrame(void);
	// 防止程序在循环过程中无法响应消息
	void PorcessMessages(void);
public:
	// 消息处理函数
	void OnReceive(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
};


