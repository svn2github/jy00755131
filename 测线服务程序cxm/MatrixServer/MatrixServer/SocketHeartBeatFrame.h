#pragma once

#include "FrameHeartBeat.h"

/**
*@brief 仪器心跳帧发送网络接口类
*/
class CSocketHeartBeatFrame : public CSocket
{
public:
	CSocketHeartBeatFrame();
	virtual ~CSocketHeartBeatFrame();

public: // 属性
	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/**  LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 用于心跳帧发送的端口号*/
	UINT m_uiPortForHeartBeat;

	/** 心跳帧数据对象*/
	CFrameHeartBeat m_oFrameHeartBeat;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiPortForHeartBeat, CString strIPLCI, UINT uiPortLCI);
	// 发送心跳帧
	void SendIHeartBeatFrame();

};


