#pragma once

#include "sitedata.h"
#include "ThreadProcDelete.h"

/**
*@brief 路由监视线程类
*
@note 功能：
@note 监视现场数据中的路由对象，把超时无刷新的路由加入删除线程的路由删除队列，唤醒删除线程执行删除操作。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcMonitorRout : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcMonitorRout)

protected:
	CThreadProcMonitorRout();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcMonitorRout();

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

	/** 现场数据对象指针*/
	CSiteData* m_pSiteData;

	/** 仪器删除对象指针*/
	CThreadProcDelete* m_pThreadProcDelete;
	/** 等待删除的仪器队列*/
	CList<CInstrument*, CInstrument*> m_olsInstrument;
	/** 等待删除的路由队列*/
	CList<CRout*, CRout*> m_olsRout;

public: //方法
	// 初始化
	bool OnInit();
	// 关闭
	bool OnClose();
	// 工作
	bool OnWork();
	// 停止
	bool OnStop();

	// 等待
	void Wait(int iWaitStopCount);
	// 判断是否可以处理的条件
	bool JudgeProcCondition();

	// 路由监视
	void ProcMonitorRout();
	// 删除路由IP地址被禁止的路由对象所链接的仪器
	bool DeleteLinkInstrumentRoutWhenRoutIPDisable(CRout* pRout );
	// 删除路由IP地址被禁止的路由对象
	bool DeleteRoutWhenRoutIPDisable(CRout* pRout);
	// 删除路由时间过期的路由对象所链接的仪器
	bool DeleteRoutLinkInstrumentWhenRoutTimeExpired(CRout* pRout);
	// 删除路由链接的仪器
	bool DeleteRoutLinkInstrument(CRout* pRout);
	// 删除的仪器和路由加入删除线程队列
	void AddDeleteInstrumentAndRoutToProcDelete();
};


