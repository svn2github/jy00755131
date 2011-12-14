#pragma once

#include "SiteData.h"
#include "socketipsetframe.h"

/**
*@brief 仪器IP地址设置线程类
*
@note 功能：
@note 和野外仪器通信，为等待设置IP地址的仪器队列的仪器设置IP地址。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcIPSet : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcIPSet)

protected:
	CThreadProcIPSet();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcIPSet();

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

	/** 仪器IP地址设置网络对象指针*/
	CSocketIPSetFrame* m_pSocketIPSetFrame;

	/** 等待设置IP地址的仪器队列*/
	CList<CInstrument*, CInstrument*> m_olsInstrument;
	/** 等待设置IP地址的仪器索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oSNInstrumentMap;

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

	// 增加一个需要设置IP地址的仪器
	void AddInstrumentForIPSet(CInstrument* pInstrument);
	//处理仪器IP地址设置
	void ProcIPSet();
	// 发送仪器IP设置帧
	void SendIPSetFrame();
	// 得到仪器IP设置应答帧
	void GetIPSetReturnFrame();
};


