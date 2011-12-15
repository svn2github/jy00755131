#pragma once

#include "FrameHead.h"
#include "..\\parameter\\Parameter.h"
/**
*@brief 仪器首包帧接收网络接口类
*/
class CSocketHeadFrame : public CSocket
{
public:
	CSocketHeadFrame();
	virtual ~CSocketHeadFrame();

public: // 属性
	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 接收端口号*/
	UINT m_uiPortForHeadFrame;
	/** 首包数据对象*/
	CFrameHead m_oFrameHead;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiPortForHeadFrame);
	// 得到网络接收缓冲区收到的帧数量
	DWORD GetFrameCount();
	// 设置接收缓冲区可以接收的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 得到帧数据
	BOOL GetFrameData();
};


