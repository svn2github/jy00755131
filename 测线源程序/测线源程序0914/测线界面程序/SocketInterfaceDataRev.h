#pragma once

#include "FrameInterface.h"

// CSocketInterfaceDataRev 命令目标

class CSocketInterfaceDataRev : public CSocket
{
public:
	CSocketInterfaceDataRev();
	virtual ~CSocketInterfaceDataRev();
	virtual void OnReceive(int nErrorCode);

public: // 属性

	// 和内部通讯的本机IP地址
	CString m_strIPForInterface;
	// 用于数据帧接收的端口号
	UINT m_uiPortForDataReceive;
	// 客户端来源IP地址
	CString m_strIPSource;
	// 客户端来源IP端口号
	UINT m_uiPortSource;

	// 帧字节数
	int m_iFrameSize;
	// 接收帧数据对象
	CFrameInterface m_oFrameInterface;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInterface, UINT uiPortForDataReceive);
	// 设置接收缓冲区大小
	BOOL SetBufferSize(int iBufferSize);
};


