#pragma once
#include "ClientCommFrame.h"
/**
* @class CClientRecFrame
* @brief 与客户端通讯接收帧类
*/
class CClientRecFrame : public CClientCommFrame
{
public:
	CClientRecFrame(void);
	~CClientRecFrame(void);
public:
	/** 解析帧*/
	void PhraseFrame(char* cpFrame, unsigned short usSize);
};

