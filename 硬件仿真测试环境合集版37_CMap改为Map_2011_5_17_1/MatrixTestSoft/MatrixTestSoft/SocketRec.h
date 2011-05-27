#pragma once
#include "Parameter.h"
#include "SaveFile.h"

// CSocketRec 命令目标

class CSocketRec : public CSocket
{
public:
	CSocketRec();
	virtual ~CSocketRec();
	virtual void OnReceive(int nErrorCode);
protected:
	// 数据接收缓冲
	unsigned char m_ucudp_buf[8][SndFrameSize];
	// 数据接收缓冲区计数
	unsigned short m_usudp_count;
public:
	// 接收端口
	int m_iRecPort;
	// 发送端口
	int m_iSendPort;
	// 目标IP地址
	CString m_csIP;
	// 重置界面
	void OnReset(void);
	// 界面PortMonitoringSend指针
	CWnd* m_pwnd;
	// 界面TabPortMonitoring指针
	CWnd* m_pWndTab;
	// 保存文件类的指针
	CSaveFile* m_pSaveFile;
protected:
	// 心跳计数
	unsigned int m_uiHeartBeatNum;
	// IP地址设置计数
	unsigned int m_uiIPSetNum;
	// 尾包时刻查询计数
	unsigned int m_uiTailTimeNum;
	// 时延设置计数
	unsigned int m_uiDelayTimeSetNum;
	// ADC设置计数
	unsigned int m_uiADCSetNum;
	// 发送帧数
	unsigned int m_uiSendFrameNum;
};


