#pragma once
#include "CommProtocol.h"
#include <list>
using std::list;
class CClientFrame
{
public:
	CClientFrame(void);
	~CClientFrame(void);
public:
	// 空闲的接收帧结构体数
	unsigned int m_uiCountFree;
	// 接收帧结构体数组
	m_oCommFrameStruct m_oCommFrameArray[FrameStructNumMax];
	// 队列资源同步对象
	CRITICAL_SECTION m_oSecClientFrame;
	// 空闲的接收帧结构体队列
	list<m_oCommFrameStructPtr> m_olsCommFrameFree;
	// 等待处理帧结构体
	list<m_oCommFrameStructPtr> m_olsCommWorkFrame;
public:
	// 重置接收帧结构体
	void OnResetRecFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 得到一个空闲接收帧结构体
	m_oCommFrameStructPtr GetFreeRecFrameStruct(void);
	// 增加一个空闲接收帧结构体
	void AddFreeRecFrameStruct(m_oCommFrameStructPtr pFrameStruct);
};

