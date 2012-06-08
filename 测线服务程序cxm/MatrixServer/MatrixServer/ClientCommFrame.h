#pragma once
/**
@brief 该文件为服务端与客户端通讯帧的声明文件
@details
@file ClientCommFrame.h
@note 
@see	
@version	
@author cxm
@bug
*/
#include "CommProtocol.h"
#include <list>
using std::list;
/**
* @class CClientCommFrame
* @brief 与客户端通讯帧基类
*/
class CClientCommFrame
{
public:
	/**
	* @fn CClientCommFrame(void)
	* @detail CClientCommFrame构造函数
	*/
	CClientCommFrame(void);
	/**
	* @fn ~CClientCommFrame(void)
	* @detail CClientCommFrame析构函数
	* @note 虚函数
	*/
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
	/**
	* @fn void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct)
	* @detail 重置接收帧结构体
	* @param[in] pFrameStruct 通讯帧指针	
	* @return void
	*/
	void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/**
	* @fn void OnInit(void)
	* @detail 初始化
	* @param[in] void	
	* @return void
	*/
	void OnInit(void);
	/**
	* @fn void OnClose(void)
	* @detail 关闭
	* @param[in] void
	* @return void
	*/
	void OnClose(void);
	/**
	* @fn m_oCommFrameStructPtr GetFreeFrameStruct(void)
	* @detail 得到一个空闲接收帧结构体
	* @param[in] void	
	* @return 返回一个通讯帧指针
	*/
	m_oCommFrameStructPtr GetFreeFrameStruct(void);
	/**
	* @fn void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct)
	* @detail 增加一个空闲接收帧结构体
	* @param[in] pFrameStruct 通讯帧指针
	* @return void
	*/
	void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/**
	* @fn void OnResetProcBuf(void)
	* @detail 重置数据处理缓冲区
	* @param[in] void
	* @return void
	*/
	void OnResetProcBuf(void);
};

