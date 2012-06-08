#pragma once
/**
@brief 该文件为服务端与客户端通讯接收帧的声明文件
@details
@file ClientRecFrame.h
@note 
@see	
@version	
@author cxm
@bug
*/
#include "ClientCommFrame.h"
/**
* @class CClientRecFrame
* @brief 与客户端通讯接收帧类
* @note 继承自CClientCommFrame
*/
class CClientRecFrame : public CClientCommFrame
{
public:
	/**
	* @fn CClientRecFrame(void)
	* @detail CClientRecFrame构造函数
	*/
	CClientRecFrame(void);
	/**
	* @fn ~CClientRecFrame(void)
	* @detail CClientRecFrame析构函数
	*/
	~CClientRecFrame(void);
public:
	/**
	* @fn void PhraseFrame(char* cpFrame, unsigned short usSize)
	* @detail 解析帧
	* @param[in] cpFrame 缓冲区指针
	* @param[in] usSize 需要处理的字节数
	* @return void
	*/
	void PhraseFrame(char* cpFrame, unsigned short usSize);
};

