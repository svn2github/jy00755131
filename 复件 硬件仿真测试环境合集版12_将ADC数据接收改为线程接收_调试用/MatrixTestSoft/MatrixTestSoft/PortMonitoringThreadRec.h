#pragma once
#include "afxsock.h"
#include "Parameter.h"
#include "SaveFile.h"

// CPortMonitoringThreadRec

class CPortMonitoringThreadRec : public CWinThread
{
	DECLARE_DYNCREATE(CPortMonitoringThreadRec)

public:
	CPortMonitoringThreadRec();           // 动态创建所使用的受保护的构造函数
	virtual ~CPortMonitoringThreadRec();

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
	// 端口监视接收端口Socket
	CSocket m_hSocketRec;
public:
	// 线程结束事件
	HANDLE m_hThreadRecClose;
protected:
	// 线程关闭标志位
	bool m_bClose;
	// 数据接收缓冲
	unsigned char m_ucudp_buf[8][128];
	// 数据接收缓冲区计数
	unsigned short m_usudp_count;
public:
	// 接收端口
	int m_iRecPort;
	// 发送端口
	int m_iSendPort;
	// 目标IP地址
	CString m_csIP;
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
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


