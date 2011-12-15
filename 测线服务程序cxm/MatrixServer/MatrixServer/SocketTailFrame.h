#pragma once

#include "frametail.h"

/**
*@brief 仪器尾包帧接收网络接口类
*/
class CSocketTailFrame : public CSocket
{
public:
	CSocketTailFrame();
	virtual ~CSocketTailFrame();

public: // 属性
	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 用于尾包接收的端口号*/
	UINT m_uiPortForTailFrame;
	/** 用于尾包转发的端口号*/
	UINT m_uiPortForTailFrameSend;

	/** 尾包数据对象*/
	CFrameTail m_oFrameTail;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiPortForTailFrame, UINT uiPortForTailFrameSend);
	// 得到网络接收缓冲区收到的帧数量
	DWORD GetFrameCount();
	// 设置接收缓冲区可以接收的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 得到帧数据
	BOOL GetFrameData();
};


