#pragma once
#include "CommThread.h"
#include "NetFrameHeader.h"
#include "NetPcapComm.h"

class CPcapRcvThread : public CCommThread
{
public:
	CPcapRcvThread(void);
	~CPcapRcvThread(void);
public:
	/** Pcap通讯类指针*/
	CNetPcapComm* m_pNetPcapComm;
public:
	/**
	* @fn void OnProc(void)
	* @detail 处理函数
	* @param[in] void
	* @return void
	*/
 	void OnProc(void);
};

