#pragma once
/**
@brief 该文件为服务端与客户端通讯线程的声明文件
@details
@file ClientCommThread.h
@note 
@see	
@version	
@author cxm
@bug
*/
#include "CommProtocol.h"
/**
* @class CClientCommThread
* @brief 与客户端通讯线程基类
*/
class CClientCommThread
{
public:
	/**
	* @fn CClientCommThread(void)
	* @detail CClientCommThread构造函数
	*/
	CClientCommThread(void);
	/**
	* @fn CClientCommThread(void)
	* @detail CClientCommThread析构函数
	* @note 虚函数
	*/
	virtual ~CClientCommThread(void);
public:
	/** 线程句柄*/
	HANDLE m_hThread;
	/** 线程ID*/
	DWORD m_dwThreadID;
	/** 线程关闭事件*/
	HANDLE m_hThreadClose;
	/** 线程关闭标志位*/
	bool m_bClose;
public:
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
	* @fn DWORD ThreadRunFunc(void)
	* @detail 线程函数
	* @param[in] void
	* @return DWORD
	*/
	DWORD ThreadRunFunc(void);
	/**
	* @fn void WaitForThread(void)
	* @detail 线程等待函数
	* @param[in] void
	* @return void
	*/
	void WaitForThread(void);
	/**
	* @fn void OnProc(void)
	* @detail 处理函数
	* @note 虚函数
	* @param[in] void
	* @return void
	*/
	virtual void OnProc(void);
};

