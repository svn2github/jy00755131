#pragma once
#include "Parameter.h"
#include <afxmt.h>
#include "ADCDataProcessThread.h"

// CADCRecSocket 命令目标

class CADCRecSocket : public CSocket
{
public:
	CADCRecSocket();
	virtual ~CADCRecSocket();
	virtual void OnReceive(int nErrorCode);
public:
	// 数据帧接收缓冲
	unsigned char udp_buf[ADCRecBufNum][RcvFrameSize];
	// 缓冲区序号
	unsigned int udp_count;
	// UDP接收帧指针偏移量
	unsigned int m_uiUdpCount;
	// UDP接收缓冲区
	unsigned char m_ucUdpBuf[ADCDataBufSize];
	// 判断ADC采样开始标志位，用于丢弃TB之前的无效数据
	BOOL m_bStartSample;
	// ADC数据界面显示缓冲区
	double m_dADCDataShow[GraphViewNum];
	// 选择仪器对象
	int* m_pSelectObject;
	// 定义临界区变量
	CCriticalSection m_Sec;
	// ADC数据处理线程指针
	CADCDataProcessThread* m_pADCDataProcessThread;

public:
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 对ADC数据进行数据处理
	void OnProcess(unsigned char* ucUdpBuf);
	// 将数据交给ADC数据处理线程
	void OnProcessADCData(unsigned int uiInstrumentNb, unsigned short usDataPoint, double* pReceiveData);
	// ADC数据处理次数计数
	unsigned int m_uiADCProcCountForTest;
};


