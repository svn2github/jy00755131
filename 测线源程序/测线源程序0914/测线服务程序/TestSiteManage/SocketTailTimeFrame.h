#pragma once

#include "FrameTailTime.h"
#include "Instrument.h"

/**
*@brief 仪器尾包时刻查询帧发送接收网络接口类
*/

class CSocketTailTimeFrame : public CSocket
{
public:
	CSocketTailTimeFrame();
	virtual ~CSocketTailTimeFrame();
public: // 属性
	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/**  LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 和现场仪器通讯的本机IP地址*/
	UINT m_uiIPForInstrument;
	/** 用于仪器尾包时刻查询的端口号*/
	UINT m_uiPortForTailTime;

	/** 帧字节数*/
	int m_iFrameSize;
	/** 仪器尾包时刻查询帧数据对象*/
	CFrameTailTime m_oFrameTailTime;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTailTime, CString strIPLCI, UINT uiPortLCI);
	// 得到网络接收缓冲区收到的帧数量
	DWORD GetFrameCount();
	// 设置接收缓冲区可以接收的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 得到帧数据
	BOOL GetFrameData();
	// 发送仪器尾包时刻查询帧
	void SendTailTimeFrame();
	// 生成仪器尾包时刻查询帧
	void MakeFrameData(CInstrument* pInstrument);
	// 读空网络接收缓冲区
	void RemoveFrameDataAll();
};


