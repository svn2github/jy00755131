#pragma once
#include "Parameter.h"
#include "SaveFile.h"
// CSocketSend 命令目标

class CSocketSend : public CSocket
{
public:
	CSocketSend();
	virtual ~CSocketSend();
	virtual void OnReceive(int nErrorCode);
protected:
	// 数据接收缓冲
	unsigned char m_ucudp_buf[8][RcvFrameSize];
	// 数据接收缓冲区计数
	unsigned short m_usudp_count;
public:
	// 接收端口
	int m_iRecPort;
	// 发送端口
	int m_iSendPort;
	// 目标IP地址
	CString m_csIP;
protected:
	// 首包计数
	unsigned int m_uiHeadFrameNum;
	// IP地址设置应答计数
	unsigned int m_uiIPSetReturnNum;
	// 尾包计数
	unsigned int m_uiTailFrameNum;
	// 尾包时刻查询应答计数
	unsigned int m_uiTailTimeReturnNum;
	// 时延设置应答计数
	unsigned int m_uiDelayTimeReturnNum;
	// ADC设置应答计数
	unsigned int m_uiADCSetReturnNum;
	// ADC数据采集帧个数
	unsigned int m_uiADCDataRecNum;
	// 显示首包帧，包含SN和首包时刻
	CString m_csHeadFrameShow;
	// 显示IP地址设置应答帧，包含SN和设置的IP地址
	CString m_csIPSetReturnShow;
	// 显示尾包帧，包含SN
	CString m_csTailFrameShow;
	// 显示尾包时刻查询应答帧，包含IP地址和查询结果
	CString m_csTailTimeReturnShow;
	// 显示时延设置应答帧，包含IP地址和设置内容
	CString m_csDelayTimeReturnShow;
	// 显示ADC采样数据帧，包含IP地址、帧计数和采样数据帧个数
	CString m_csADCDataRecShow;
	// 接收帧数
	unsigned int m_uiRecFrameNum;
public:
	// 重置界面
	void OnReset(void);
	// 界面PortMonitoringRec指针
	CWnd* m_pwnd;
	// 界面TabPortMonitoring指针
	CWnd* m_pWndTab;
	// 保存文件类的指针
	CSaveFile* m_pSaveFile;
	// 端口分发功能
	BOOL m_bPortDistribution;
	// 防止程序在循环中运行无法响应消息
	unsigned short m_usADCLastDataPoint[GraphViewNum];
	void ProcessMessages(void);
};


