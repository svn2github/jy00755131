#pragma once
#include "InstrumentList.h"

// CTailFrameThread

class CTailFrameThread : public CWinThread
{
	DECLARE_DYNCREATE(CTailFrameThread)

public:
	CTailFrameThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTailFrameThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hTailFrameThreadClose;
	// 尾包接收Socket
	CSocket m_TailFrameSocket;
	// 尾包接收数据缓冲
	byte m_pTailFrameData[256];
	// 尾包时刻查询帧
	byte m_pTailTimeSendData[128];
	// 尾包时刻查询结果数据接收缓冲
	byte m_pTailTimeRecData[256];
	// 时延设置帧
	byte m_pTimeDelayData[128];
	// 尾包时刻查询Socket
	CSocket m_TailTimeFrameSocket;
	// 仪器位置索引表
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentLocationMap;
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
	// 发送尾包时刻查询帧计数
	unsigned int m_uiSendTailTimeFrameCount;
	// 接收到的尾包时刻查询帧计数
	unsigned int m_uiRecTailTimeFrameCount;
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
};


