#pragma once

#include "SiteData.h"
#include "SocketHeartBeatFrame.h"

/**
*@brief 心跳帧发送线程
*
@note 功能：
@note 在工作状态下周期性向野外仪器发送心跳帧。
*/
class CThreadProcHeartBeat : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcHeartBeat)

protected:
	CThreadProcHeartBeat();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcHeartBeat();

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

	/** 心跳帧发送网络对象指针*/
	CSocketHeartBeatFrame* m_pSocketHeartBeatFrame;

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

	// 发送心跳帧
	void SendHeartBeatFrame();
};


