#pragma once
#include "Parameter.h"
#include "SaveFile.h"


// CPortMonitoringSendThread

class CPortMonitoringSendThread : public CWinThread
{
	DECLARE_DYNCREATE(CPortMonitoringSendThread)

public:
	CPortMonitoringSendThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CPortMonitoringSendThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 数据接收缓冲
	unsigned char m_ucudp_buf[8][RcvFrameSize];
	// 数据接收缓冲区计数
	unsigned short m_usudp_count;
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
public:
	// 发送方Socket
	CSocket m_SendSocket;
	// 接收端口
	int m_iRecPort;
	// 发送端口
	int m_iSendPort;
	// 目标IP地址
	CString m_csIP;
	// 界面PortMonitoringRec指针
	CWnd* m_pwnd;
	// 界面TabPortMonitoring指针
	CWnd* m_pWndTab;
	// 保存文件类的指针
	CSaveFile* m_pSaveFile;
	// 端口分发功能
	BOOL m_bPortDistribution;
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hPortMonitoringSendThreadClose;
	// 接收帧数
	unsigned int m_uiRecFrameNum;
public:
	virtual int Run();
	// 初始化
	void OnInit(void);
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 关闭
	void OnClose(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 重置界面
	void OnReset(void);
};


