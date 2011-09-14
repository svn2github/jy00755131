#pragma once

#include "SiteData.h"
#include "ThreadProcTimeDelay.h"

/**
*@brief 仪器和路由删除线程类
*
@note 功能：
@note 得到待删除仪器和路由的队列，从现场数据中删除。
@note 执行流程：
@note 启动后处于休眠状态；被尾包处理线程或路由监视监视线程唤醒，执行仪器和路由删除操作；操作完成后处于休眠状态。
@note 关联线程：
@note 尾包处理线程或路由监视线程把要删除的仪器和路由加入仪器和路由删除线程的队列中，然后唤醒仪器和路由删除线程。
@note 删除线程工作时，需要暂停的线程：首包处理线程、尾包处理线程、路由监视线程、仪器IP地址分配线程、仪器IP地址设置线程、现场数据输出线程、时延处理线程
*/
class CThreadProcDelete : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcDelete)

protected:
	CThreadProcDelete();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcDelete();

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

	/** 时延处理线程对象指针*/
	CThreadProcTimeDelay* m_pThreadProcTimeDelay;

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

	// 判断是否可以处理的条件
	bool JudgeProcCondition();

	// 线程是否空闲
	bool IsThreadFree();
	// 启动时延处理
	void ResumeProcDelete();
	// 锁定线程
	bool Lock();
	// 解锁线程
	void Unlock();
	// 增加一个需要设置删除的仪器
	void AddInstrumentForDelete(CInstrument* pInstrument);
	// 增加一个需要设置删除的路由
	void AddRoutForDelete(CRout* pRout);
	// 处理删除操作
	void ProcDelete();
	// 等待其他线程暂停
	bool WaitOtherThreadProcPause();
	// 处理删除仪器操作
	void ProcDeleteInstrument();
	// 处理删除路由操作
	void ProcDeleteRout();
};


