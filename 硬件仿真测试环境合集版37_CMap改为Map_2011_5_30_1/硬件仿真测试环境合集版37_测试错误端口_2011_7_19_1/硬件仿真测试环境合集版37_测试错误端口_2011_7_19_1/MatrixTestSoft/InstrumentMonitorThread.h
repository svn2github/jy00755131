#pragma once
#include "LogFile.h"
#include "ADCSet.h"
#include "InstrumentList.h"
#include "TailFrameSocket.h"
#include <afxmt.h>
// CInstrumentMonitor

class CInstrumentMonitorThread : public CWinThread
{
	DECLARE_DYNCREATE(CInstrumentMonitorThread)

public:
	CInstrumentMonitorThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CInstrumentMonitorThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 日志类指针
	CLogFile* m_pLogFile;
	// 界面指针
	CWnd* m_pwnd;
	// ADC设置类指针
	CADCSet* m_pADCSet;
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// 尾包类指针
	CTailFrame* m_pTailFrame;
	// 交叉站尾包监测计数
	unsigned int m_uiTailFrameTimerLAUXCount;
	// 交叉站尾包个数稳定计数
	unsigned int m_uiTailFrameStableLAUXCount;
	// 采集站尾包监测计数
	unsigned int m_uiTailFrameTimerFDUCount;
	// 采集站尾包个数稳定计数
	unsigned int m_uiTailFrameStableFDUCount;
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hInstrumentMonitorThreadClose;
protected:
	// 定义临界区变量
	CCriticalSection m_Sec_InstrumentMonitor;
public:
	virtual int Run();
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 监视交叉站尾包
	void OnMonitorLAUX(void);
	// 监视采集站尾包
	void OnMonitorFDU(void);
	// 终止正在进行的操作
	void OnStopOperation(void);
	// 重置交叉站尾包计数器
	void OnResetLAUXCounter(void);
	// 重置采集站尾包计数器
	void OnResetFDUCounter(void);
};


