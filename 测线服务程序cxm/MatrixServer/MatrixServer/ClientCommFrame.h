#pragma once
#include "CommProtocol.h"
#include <list>
using std::list;
/**
* @brief 与客户端通讯帧基类
*/
class CClientCommFrame
{
public:
	CClientCommFrame(void);
	virtual ~CClientCommFrame(void);
public:
	/** 空闲的帧结构体数*/
	unsigned int m_uiCountFree;
	/** 帧结构体数组*/
	m_oCommFrameStruct m_oCommFrameArray[FrameStructNumMax];
	/** 队列资源同步对象*/
	CRITICAL_SECTION m_oSecClientFrame;
	/** 空闲帧结构体队列*/
	list<m_oCommFrameStructPtr> m_olsCommFrameFree;
	/** 等待处理帧结构体*/
	list<m_oCommFrameStructPtr> m_olsCommWorkFrame;
	/** 包的流水号*/
	unsigned int m_uiPacketIndex;
	/** 命令流水号*/
	unsigned int m_uiCmdIndex;
	/** 数据处理缓冲区*/
	char m_cProcBuf[ProcBufferSize];
public:
	/** 重置接收帧结构体*/
	void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/** 初始化*/
	void OnInit(void);
	/** 关闭*/
	void OnClose(void);
	/** 得到一个空闲接收帧结构体*/
	m_oCommFrameStructPtr GetFreeFrameStruct(void);
	/** 增加一个空闲接收帧结构体*/
	void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/** 重置数据处理缓冲区*/
	void OnResetProcBuf(void);
};

