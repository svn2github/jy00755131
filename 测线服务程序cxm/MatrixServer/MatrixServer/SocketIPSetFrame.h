#pragma once

#include "FrameIPSet.h"
#include "Instrument.h"

/**
*@brief 仪器IP地址设置帧发送接收网络接口类
*/
class CSocketIPSetFrame : public CSocket
{
public:
	CSocketIPSetFrame();
	virtual ~CSocketIPSetFrame();

public: // 属性
	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/**  LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 和现场仪器通讯的本机IP地址*/
	UINT m_uiIPForInstrument;
	/** 用于仪器IP地址设置的端口号*/
	UINT m_uiPortForIPSet;

	/** 仪器IP设置帧数据对象*/
	CFrameIPSet m_oFrameIPSet;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForIPSet, CString strIPLCI, UINT uiPortLCI);
	// 得到网络接收缓冲区收到的帧数量
	DWORD GetFrameCount();
	// 设置缓冲区可以收发的包帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 得到帧数据
	BOOL GetFrameData();
	// 发送仪器IP地址设置帧
	void SendIPSetFrame();
	// 生成仪器IP地址设置帧
	void MakeFrameData(CInstrument* pInstrument, unsigned short usCommand);
};


