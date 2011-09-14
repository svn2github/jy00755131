#pragma once

#include "NetProcInterface.h"
#include "FrameInterface.h"

class CNetProcInterface;

/**
*@brief 服务器端命令帧接收网络接口类
*/
class CSocketInterfaceCmdRev : public CSocket
{
public:
	CSocketInterfaceCmdRev();
	virtual ~CSocketInterfaceCmdRev();
	virtual void OnReceive(int nErrorCode);

public: // 属性

	/** 和内部通讯的本机IP地址*/
	CString m_strIPForInterface;
	/** 用于命令帧接收的端口号*/
	UINT m_uiPortForCommandReceive;
	/** 客户端来源IP地址*/
	CString m_strIPSource;
	/** 客户端来源IP端口号*/
	UINT m_uiPortSource;
	/** 客户端目标IP地址*/
	CString m_strIPAim;
	/** 客户端目标IP端口号*/
	UINT m_uiPortAim;

	/** 帧字节数*/
	int m_iFrameSize;
	/** 接收帧数据对象*/
	CFrameInterface m_oFrameInterface;
	/** 应答帧数据对象*/
	CFrameInterface m_oFrameReply;

	/** 收到命令帧的帧计数队列最大长度*/
	int m_iFrameCountListLengthMaxReceive;
	/** 收到命令帧的帧计数队列*/
	CList<UINT, UINT> m_oFrameCountListReceive;
	/** 收到命令帧的帧计数索引表*/
	CMap<UINT, UINT, UINT, UINT> m_oFrameCountMapReceive;

	/** 内部网络命令接收处理对象指针*/
	CNetProcInterface* m_pNetProcInterface;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInterface, UINT uiPortForCommandReceive);
	// 设置接收缓冲区大小
	BOOL SetBufferSize(int iBufferSize);
	// 发送回令帧
	void SendReplyFrame();
	// 检查是否是重复接收到的命令帧
	bool CheckIfRepeatFrameCommand();
};


