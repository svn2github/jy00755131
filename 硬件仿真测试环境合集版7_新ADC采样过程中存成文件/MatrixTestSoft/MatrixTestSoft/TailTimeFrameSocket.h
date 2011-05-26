#pragma once
#include "InstrumentList.h"
// CTailTimeFrameSocket 命令目标

class CTailTimeFrameSocket : public CSocket
{
public:
	CTailTimeFrameSocket();
	virtual ~CTailTimeFrameSocket();
	virtual void OnReceive(int nErrorCode);
public:
	// 尾包时刻查询结果数据接收缓冲
	byte m_pTailTimeRecData[256];
	// 时延设置帧
	byte m_pTimeDelayData[128];
	// 仪器位置索引表
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentLocationMap;
	// 仪器SN号
	DWORD m_uiSN;
	// 仪器列表指针
	CInstrumentList* m_oInstrumentList;
	// 单个尾包时刻查询处理
	void ProcTailTimeFrameOne(void);
	// 解析尾包时刻查询帧
	BOOL PraseTailTimeFrame(void);
	// 设备的网络时间
	DWORD m_uiNetTime;
	// 设备的本地时间
	DWORD m_uiSystemTime;
	// 设备尾包接收时刻
	unsigned short m_usTailRecTime;
	// 设备尾包发送时刻
	unsigned short m_usTailSendTime;
	// 时间同步计算
	void TimeDelayCalculation(void);
	// 时间修正高位
	int m_iTimeHigh;
	// 时间修正低位
	int m_iTimeLow;
	// 生成时统报文设置帧
	void MakeTimeDelayFrameData(CInstrument* pInstrument);
	// 发送时统报文设置帧
	void SendTimeDelayFrameData(void);
	// 时延设置修正次数
	unsigned int m_uiDelayTimeCount;
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
};


