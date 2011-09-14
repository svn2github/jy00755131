#pragma once

#include "FrameTimeSet.h"
#include "Instrument.h"

/**
*@brief 仪器时延设置帧发送网络接口类
*/
class CSocketTimeSetFrame : public CSocket
{
public:
	CSocketTimeSetFrame();
	virtual ~CSocketTimeSetFrame();

public: // 属性
	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/**  LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 和现场仪器通讯的本机IP地址*/
	UINT m_uiIPForInstrument;
	/** 用于仪器时间设置的端口号*/
	UINT m_uiPortForTimeSet;

	// 帧字节数*/
	int m_iFrameSize;
	// 仪器时间设置帧数据对象*/
	CFrameTimeSet m_oFrameTimeSet;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTimeSet, CString strIPLCI, UINT uiPortLCI);
	// 设置发送缓冲区可以发送的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 发送仪器时间地址设置帧
	void SendTimeSetFrame();
	// 生成仪器时间地址设置帧
	void MakeFrameData(CInstrument* pInstrument);
};


