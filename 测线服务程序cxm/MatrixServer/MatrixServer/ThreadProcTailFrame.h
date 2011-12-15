#pragma once

#include "SiteData.h"
#include "SocketTailFrame.h"

/**
*@brief 尾包处理线程类
*
@note 功能：
@note 得到野外仪器发来的尾包帧，处理尾包帧，维护现场数据。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcTailFrame : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcTailFrame)

protected:
	CThreadProcTailFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcTailFrame();

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

	/** 尾包接收网络对象指针*/
	CSocketTailFrame* m_pSocketTailFrame;

// 	/** 时延处理线程对象指针*/
// 	CThreadProcTimeDelay* m_pThreadProcTimeDelay;
// 	/** 仪器删除对象指针*/
// 	CThreadProcDelete* m_pThreadProcDelete;

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

	// 处理尾包帧
	void ProcTailFrame();
	// 处理单个尾包帧
	void ProcTailFrameOne(CFrameTail* pFrameTail);
// 	// 更新时统
// 	void ProcTimeDelay(CRout* pRout);
};


