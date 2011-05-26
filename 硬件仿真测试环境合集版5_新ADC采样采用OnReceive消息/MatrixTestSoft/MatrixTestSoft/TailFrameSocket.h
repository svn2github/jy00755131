#pragma once
#include "InstrumentList.h"
// CTailFrameSocket 命令目标

class CTailFrameSocket : public CSocket
{
public:
	CTailFrameSocket();
	virtual ~CTailFrameSocket();
	virtual void OnReceive(int nErrorCode);
public:
	// 尾包接收数据缓冲
	byte m_pTailFrameData[256];
	// 尾包时刻查询帧
	byte m_pTailTimeSendData[128];
	// 处理单个尾包
	void ProcTailFrameOne(void);
	// 解析尾包
	BOOL ParseTailFrame(void);
	// 仪器SN号
	DWORD m_uiSN;
	// 仪器列表指针
	CInstrumentList* m_oInstrumentList;
	// 向仪器发送尾包时刻查询帧
	void SendTailTimeFrame(void);
	// 生成尾包时刻查询帧
	void MakeTailTimeFrameData(CInstrument* pInstrument);
	// 发送尾包时刻查询帧
	void SendTailTimeFrameToSocket(void);
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
};


