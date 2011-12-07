#pragma once

#include "logicdata.h"
#include "sitedata.h"
#include "SocketInterfaceCmdRev.h"
#include "SocketInterfaceCmdSend.h"
#include "FrameInterface.h"

#include "ThreadProcHeartBeat.h"
#include "threadprocheadframe.h"
#include "threadproctailframe.h"
#include "threadprocipset.h"
#include "ThreadProcTimeDelay.h"
#include "ThreadProcIPDistribute.h"
#include "ThreadProcMonitorRout.h"
#include "ThreadProcDelete.h"
#include "ThreadProcSiteDataOutput.h"
#include "ThreadProcTest.h"

class CSocketInterfaceCmdRev;

/**
*@brief 服务器和客户端之间网络命令和数据交互处理类
*/
class CNetProcInterface
{
public:
	CNetProcInterface();
	~CNetProcInterface();

public: //属性
	/** 是否工作状态*/
	bool m_bWork;
	/** 是否关闭线程*/
	bool m_bClose;

	/** 测线设置数据对象*/
	CLogicData* m_pLogicData;
	/** 现场数据对象*/
	CSiteData* m_pSiteData;

	/** 内部网络命令接收处理网络对象指针*/
	CSocketInterfaceCmdRev* m_pSocketInterfaceCmdRev;
	/** 服务器内部网络命令发送处理网络对象指针*/
	CSocketInterfaceCmdSend* m_pSocketInterfaceCmdSend;

	/** 心跳处理对象指针*/
	CThreadProcHeartBeat* m_pThreadProcHeartBeat;
	/** 仪器IP地址设置对象指针*/
	CThreadProcIPSet* m_pThreadProcIPSet;
	/** 首包处理对象指针*/
	CThreadProcHeadFrame* m_pThreadProcHeadFrame;
	/** 时延处理线程对象指针指针*/
	CThreadProcTimeDelay* m_pThreadProcTimeDelay;
	/** 尾包处理对象指针*/
	CThreadProcTailFrame* m_pThreadProcTailFrame;
	/** 仪器IP地址分配对象指针*/
	CThreadProcIPDistribute* m_pThreadProcIPDistribute;
	/** 路由监视对象指针*/
	CThreadProcMonitorRout* m_pThreadProcMonitorRout;
	/** 仪器删除对象指针*/
	CThreadProcDelete* m_pThreadProcDelete;
	/** 现场数据输出对象指针*/
	CThreadProcSiteDataOutput* m_pThreadProcSiteDataOutput;
	/** 测试对象，基本测试指针*/
	CThreadProcTest* m_pThreadProcTestBase;
	/** 测试对象，噪声监测指针*/
	CThreadProcTest* m_pThreadProcTestNoise;

public: //方法
	// 初始化
	bool OnInit();
	// 关闭
	bool OnClose();
	// 工作
	bool OnWork();
	// 停止
	bool OnStop();

	// 处理内部网络命令帧
	void OnProcInterFaceCommand(CFrameInterface* pFrameInterface);

	// 处理内部网络命令帧；命令	1-Field ON/OFF
	void ProcInterFaceCommandForField(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	1-Field ON/OFF
	void ProcInterFaceCommandForField(byte byFieldOperation);
	// 处理内部网络命令帧；命令	2-FormLine
	void ProcInterFaceCommandForFormLine(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	3-Look
	void ProcInterFaceCommandForLook(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	4、5、6-基本测试命令；来自测试设置的“GO”按钮
	void ProcInterFaceCommandForTestBaseFromTestSetup(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	7-噪声监测命令；来自测试设置的“GO”按钮
	void ProcInterFaceCommandForTestNoiseFromTestSetup(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	8、9-基本测试命令；来自仪器视图或检波器视图的“GO”按钮
	void ProcInterFaceCommandForTestBaseFromTestView(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	10-噪声监测命令；来自噪声监测视图的“Seismonitor”按钮
	void ProcInterFaceCommandForTestNoiseFromTestView(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令
	void ProcInterFaceCommandForSetupDataChange(CFrameInterface* pFrameInterface);
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	1-SurveySetup的ApplyAll
	void ProcInterFaceCommandForSetupDataChangeForSurveySetupApplyAll();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	2-SurveySetup的ApplySensor
	void ProcInterFaceCommandForSetupDataChangeForSurveySetupApplySensor();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	11-LayoutSetup的ApplyBlast
	void ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyBlast();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	12-LayoutSetup的ApplyMarkers
	void ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMarkers();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	13-LayoutSetup的ApplyAux
	void ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyAux();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	14-LayoutSetup的ApplyDetour
	void ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyDetour();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	15-LayoutSetup的ApplyMute
	void ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMute();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	21-LookSetup的Apply
	void ProcInterFaceCommandForSetupDataChangeForLookSetupApply();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	31-TestSetup的ApplyInstrument
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrument();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	32-TestSetup的ApplySensor
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensor();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	33-TestSetup的ApplyMultiple
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplyMultiple();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	34-TestSetup的ApplySeisMonitor
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplySeisMonitor();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	41-TestSetup的ApplyInstrumentTestBase
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestBase();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	42-TestSetup的ApplySensorTestBase
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestBase();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	51-TestSetup的ApplyInstrumentTestLimit
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestLimit();
	// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	52-TestSetup的ApplySensorTestLimit
	void ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestLimit();

	// 等待仪器IP地址分配线程暂停
	bool WaitForProcIPDistributePause();

	// 发送服务器Field命令
	void SendServerFieldCmd(byte byFieldOperation);
};