#pragma once
#include "CommThread.h"
#include "NetPcapComm.h"
class CPcapSndThread : public CCommThread
{
public:
	CPcapSndThread(void);
	~CPcapSndThread(void);
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

