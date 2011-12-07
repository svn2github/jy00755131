#pragma once

#include "SiteData.h"
#include "SocketTailTimeFrame.h"
#include "SocketTimeSetFrame.h"
//#include "ThreadProcDelete.h"

/**
*@brief 仪器时统处理线程类
*
@note 功能：
@note 处理路由对应仪器串的时统。
@note 执行流程：
@note 启动后处于休眠状态；被尾包处理线程唤醒，执行仪器时统处理操作；操作完成后处于休眠状态。
*/
class CThreadProcTimeDelay : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcTimeDelay)

protected:
	CThreadProcTimeDelay();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcTimeDelay();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();

public: //属性
	/** 是否工作状态*/
	bool m_bWork;
	/** 是否关闭线程*/
	bool m_bClose;
	/** 是否空闲状态*/
	bool m_bFree;

	/** 现场数据对象指针*/
	CSiteData* m_pSiteData;

	/** 需要进行时间延迟设置的路由对象指针*/
	CRout* m_pRout;

	/** 仪器尾包时刻查询网络对象指针*/
	CSocketTailTimeFrame* m_pSocketTailTimeFrame;
	/** 仪器时间延迟设置网络对象指针*/
	CSocketTimeSetFrame* m_pSocketTimeSetFrame;
	/** 仪器删除对象指针*/
	//CThreadProcDelete* m_pThreadProcDelete;

	/** 仪器队列*/
	CList<CInstrument*, CInstrument*> m_olsInstrument;
	/** 仪器索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMap;
	int Calc_skip;
	int time_init0;
	int inDelay;

public: //方法
	// 初始化
	bool OnInit();
	// 关闭
	bool OnClose();
	// 工作
	bool OnWork();
	// 停止
	bool OnStop();

	// 判断是否可以处理的条件
	bool JudgeProcCondition();

	// 线程是否空闲
	bool IsThreadFree();
	// 启动时延处理
	void ResumeProcTimeDelay();
	// 设置需要进行时延修正的路由
	void SetRout(CRout* pRout);
	// 时延修正处理
	void ProcTimeDelay();
	// 得到需要时延修正的仪器，加入队列和索引表
	void GetInstrumentForTailTimeQuery();
	// 处理尾包时刻查询
	bool ProcTailTimeQuery();
	// 处理一次尾包时刻查询
	bool ProcTailTimeQueryOnce(int iTimeDelay);
	// 计算时延
	bool ProcTimeDelayCalculate();
	bool ProcTimeDelayCalculate_sys();
	// 设置时延
	bool ProcTimeDelaySet();
	// 得到连接的下一个仪器
	CInstrument* GetNextInstrument(CInstrument* pInstrument);
	// 向仪器发送尾包时刻查询帧
	void SendTailTimeFrame();
	// 得到仪器尾包时刻查询应答帧
	void GetTailTimeFrame();
	// 根据仪器IP地址，由索引表得到仪器
	BOOL GetInstrumentByIP(unsigned int uiIP, CInstrument* &pInstrument);
	// 设置仪器尾包时刻查询结果
	void SetInstrumentTimeDelayData(CInstrument* pInstrument, CFrameTailTime* pFrameTailTime);
	bool SetInstrumentTimeDelayData1(CInstrument* pInstrument, CFrameTailTime* pFrameTailTime);
};


