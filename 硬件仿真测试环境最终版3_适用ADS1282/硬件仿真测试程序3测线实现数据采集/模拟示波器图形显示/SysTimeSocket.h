#pragma once

// CSysTimeSocket 命令目标

class CSysTimeSocket : public CSocket
{
public:
	CSysTimeSocket();
	virtual ~CSysTimeSocket();
public:
	virtual void OnReceive(int nErrorCode);
protected:
	unsigned char udp_buf[1024];
	unsigned char ADCSampleCmd[128];
public:
	// 采集站本地时间
	unsigned int m_uiSysTime;
	// 目标IP地址
	unsigned int m_uiIPAim;
	// 生成数据采样设置帧
	void MakeFrameData(unsigned int uiIPAim);
	// 发送数据采样设置帧
	void SendFrameData(void);
	// ADC初始化
	void OnADCInit(unsigned int uiTestNb);
	// ADC零漂矫正
	void OnADCOffset(unsigned int uiTestNb);
	// ADC满量程矫正
	void OnADCFullScale(unsigned int uiTestNb);
	// 开始ADC数据采集
	void OnADCStart(unsigned int uiTestNb);
	// 停止ADC数据采集
	void OnADCStop(unsigned int uiTestNb);
	// 生成包头
	void MakeHeadFrame(unsigned int uiIPAim, unsigned short usCommand);
	// 生成ADC设置帧
	void MakeADCSetFrame(unsigned char* buf, int iBufLength);
};


