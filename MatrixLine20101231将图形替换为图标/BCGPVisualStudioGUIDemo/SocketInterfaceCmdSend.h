#pragma once

#include "FrameInterface.h"

// CSocketInterfaceCmdSend 命令目标

class CSocketInterfaceCmdSend : public CSocket
{
public:
	CSocketInterfaceCmdSend();
	virtual ~CSocketInterfaceCmdSend();
	virtual void OnReceive(int nErrorCode);

public: // 属性

	// 和内部通讯的本机IP地址
	CString m_strIPForInterface;
	// 用于命令帧的端口号
	UINT m_uiPortForCommandSend;
	// 客户端目标IP地址
	CString m_strIPAim;
	// 客户端目标IP端口号
	UINT m_uiPortAim;

	// 帧字节数
	int m_iFrameSize;
	// 发送帧数据对象
	CFrameInterface m_oFrameInterface;
	// 应答帧数据对象
	CFrameInterface m_oFrameReply;

	// 收到应答帧的帧计数队列最大长度
	int m_iFrameCountListLengthMaxReply;
	// 收到应答帧的帧计数队列
	CList<UINT, UINT> m_oFrameCountListReply;
	// 收到应答帧的帧计数索引表
	CMap<UINT, UINT, UINT, UINT> m_oFrameCountMapReply;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInterface, UINT uiPortForCommandSend, UINT uiPortAim);
	// 设置缓冲区大小
	BOOL SetBufferSize(int iBufferSize);
	// 发送命令帧
	void SendCommandFrame();
	// 保存收到的应答帧
	void SaveReplyFrame();
};


