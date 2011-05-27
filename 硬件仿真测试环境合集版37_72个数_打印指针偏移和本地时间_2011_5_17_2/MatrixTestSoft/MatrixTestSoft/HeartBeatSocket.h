#pragma once

// CHeartBeatSocket 命令目标

class CHeartBeatSocket : public CSocket
{
public:
	CHeartBeatSocket();
	virtual ~CHeartBeatSocket();
protected:
	// 帧数据
	byte m_pFrameData[128];
public:
	// 生成心跳帧
	void MakeHeartBeatFrame(void);
	// 发送心跳帧
	void SendHeartBeatFrame(void);
	// 发送端口
	unsigned int m_uiSendPort;
};


