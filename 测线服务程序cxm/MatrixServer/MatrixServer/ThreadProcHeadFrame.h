#pragma once

//#include "logicdata.h"
#include "SiteData.h"
#include "socketheadframe.h"
//#include "ThreadProcIPSet.h"

/**
*@brief 首包处理线程类
*
@note 功能：
@note 得到野外仪器发来的首包帧，处理首包帧，在现场数据中加入新仪器。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcHeadFrame : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcHeadFrame)

protected:
	CThreadProcHeadFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcHeadFrame();

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
	/** 是否发生了增加仪器操作*/
	bool m_bInstrumentAdd;

// 	/** 测线设置数据对象*/
// 	CLogicData* m_pLogicData;
	/** 现场数据对象指针*/
	CSiteData* m_pSiteData;

	/** 首包接收网络对象指针*/
	CSocketHeadFrame* m_pSocketHeadFrame;

// 	/** 仪器IP地址分配线程对象指针*/
// 	CThreadProcIPSet* m_pThreadProcIPSet;

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

	// 处理首包帧
	void ProcHeadFrame();
	// 处理单个首包帧
	void ProcHeadFrameOne(CFrameHead* pFrameHead);
	// 得到新仪器
	void GetFreeInstrument(CInstrument* &pInstrument, CFrameHead* pFrameHead);
// 	// 增加主交叉站
// 	void AddMainCross(CInstrument* pInstrument);
// 	// 增加交叉站交叉线方向，上边
// 	void AddCrossTop(CInstrument* pInstrument);
// 	// 增加交叉站交叉线方向，下边
// 	void AddCrossDown(CInstrument* pInstrument);
// 	// 增加交叉站测线方向，左边
// 	void AddCrossLeft(CInstrument* pInstrument);
// 	// 增加交叉站测线方向，右边
// 	void AddCrossRight(CInstrument* pInstrument);
// 	// 增加电源站测线方向，左边
// 	void AddPowerLeft(CInstrument* pInstrument);
// 	// 增加电源站测线方向，右边
// 	void AddPowerRight(CInstrument* pInstrument);
// 	// 增加采集站测线方向，左边
// 	void AddCollectLeft(CInstrument* pInstrument);
// 	// 增加采集站测线方向，右边
// 	void AddCollectRight(CInstrument* pInstrument);
// 	// 设置交叉站某个方向的路由
// 	void SetCrossRout(CInstrument* pInstrument, unsigned int uiRoutDirection);
// 	// 增加一个需要设置IP地址的仪器
// 	void AddInstrumentForIPSet(CInstrument* pInstrument);
// 	// 增加一个需要设置IP地址的仪器
// 	void AddInstrumentForIPSet(CFrameHead* pFrameHead);
// 	//重发时间靠后的首包
// 	void SendHeadFrame();
};


