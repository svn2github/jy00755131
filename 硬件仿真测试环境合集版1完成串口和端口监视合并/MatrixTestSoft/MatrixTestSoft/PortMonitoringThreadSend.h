#pragma once
#include "afxsock.h"
#include "Parameter.h"
#include "SaveFile.h"
// CPortMonitoringThreadSend

class CPortMonitoringThreadSend : public CWinThread
{
	DECLARE_DYNCREATE(CPortMonitoringThreadSend)

public:
	CPortMonitoringThreadSend();           // 动态创建所使用的受保护的构造函数
	virtual ~CPortMonitoringThreadSend();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 线程初始化
	void OnInit(void);
	// 线程关闭
	void OnClose(void);
	virtual int Run();
protected:
	// 端口监视发送端口Socket
	CSocket m_hSocketSend;
public:
	// 线程结束事件
	HANDLE m_hThreadSendClose;
protected:
	// 线程关闭标志位
	bool m_bClose;
	// 数据接收缓冲
	unsigned char m_ucudp_buf[8][256];
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
	// ADC数据采集上一帧的数据指针偏移量
	unsigned short m_usADCLastDataPoint;
public:
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 重置界面
	void OnReset(void);
	// 界面PortMonitoringRec指针
	CWnd* m_pwnd;
	// 界面TabPortMonitoring指针
	CWnd* m_pWndTab;
	// 保存文件类的指针
	CSaveFile* m_pSaveFile;
};


