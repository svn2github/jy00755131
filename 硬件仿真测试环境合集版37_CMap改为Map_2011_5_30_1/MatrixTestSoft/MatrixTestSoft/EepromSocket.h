#pragma once
#include "Parameter.h"
// CEepromSocket 命令目标

class CEepromSocket : public CSocket
{
public:
	CEepromSocket();
	virtual ~CEepromSocket();
	virtual void OnReceive(int nErrorCode);
private:
	// 网络接收帧缓冲区
	unsigned char m_ucRecBuf[RcvFrameSize];
public:
	// 读取EEPROM的接收数据个数
	unsigned int m_uiReadEepromNum;
	// 读取EEPROM的接收缓冲区
	unsigned char m_ucReadEepromBuf[EEPROMCapacity];
private:
	// 单个帧处理
	void ProcFrameOne(void);
public:
	// 需要读取EEPROM的数据个数
	unsigned int m_uiNeedToReadNum;
	// 一帧数据可读取的最大个数
	unsigned int m_uiOneFrameReadMax;
	// 初始化
	void OnInit(void);
};


