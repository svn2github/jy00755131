#pragma once

#include "logicdata.h"
#include "sitedata.h"
#include "threadprocipset.h"

/**
*@brief 仪器IP地址分配线程
*
@note 功能：
@note 根据布局设置信息，为现场仪器分配IP地址。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcIPDistribute : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcIPDistribute)

protected:
	CThreadProcIPDistribute();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcIPDistribute();

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

	/** 仪器IP地址分配时间*/
	unsigned int m_iIPDistributeTime;

	/** 测线设置数据对象*/
	CLogicData* m_pLogicData;
	/** 现场数据对象*/
	CSiteData* m_pSiteData;

	/** 仪器IP地址设置线程对象指针*/
	CThreadProcIPSet* m_pThreadProcIPSet;

	/** 辅助道仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentAuxSNMap; 
	/** 爆炸机控制器仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentBlastMachineSNMap; 
	/** 迂回道标记仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentDetourLogoSNMap; 
	/** 迂回道仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentDetourSNMap; 
	/** 采集道标记仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMarkerLogoSNMap; 
	/** 采集道仪器SN索引表*/
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMarkerSNMap; 
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

	// 处理IP地址分配
	void ProcIPDistribute();
	// 处理辅助道IP地址分配
	bool ProcIPDistributeAux();
	// 处理一个辅助道IP地址分配
	void ProcIPDistributeAuxOne(CAuxData* pAuxData);
	// 清理过期辅助道标记
	bool ProcClearTimeOutAux();
	// 处理爆炸机控制器IP地址分配
	bool ProcIPDistributeBlastMachine();
	// 处理一个爆炸机控制器IP地址分配
	void ProcIPDistributeBlastMachineOne(CBlastMachineData* pBlastMachineData);
	// 清理过期爆炸机控制器标记
	bool ProcClearTimeOutBlastMachine();
	// 处理迂回道IP地址分配
	bool ProcIPDistributeDetour();
	// 处理一个迂回道IP地址分配
	void ProcIPDistributeDetourOne(CDetourData* pDetourData);
	// 处理一个仪器的迂回道IP地址分配
	void ProcIPDistributeDetourOneInstrument(CInstrument* pInstrument);
	// 清理过期迂回道标记
	bool ProcClearTimeOutDetour();
	// 处理标记点IP地址分配
	void ProcIPDistributeMarker();
	// 处理一个标记点IP地址分配
	void ProcIPDistributeMarkerOne(CMarkerData* pMarkerData);
	// 处理单个标记点IP地址分配，正常顺序
	void ProcIPDistributeMarkerOneNormal(CMarkerData* pMarkerData);
	// 处理单个标记点IP地址分配，翻转顺序
	void ProcIPDistributeMarkerOneReversed(CMarkerData* pMarkerData);
	// 处理单个采集站IP地址分配
	void ProcIPDistributeMarkerOneInstrument(CInstrument* pInstrumentCollect, CChannelSetupData* pChannel);
	// 处理单个测点IP地址分配，向左
	CInstrument* ProcIPDistributeMarkerOnePointLeft(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel);
	// 处理单个测点IP地址分配，向右
	CInstrument* ProcIPDistributeMarkerOnePointRight(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel);
	// 处理测点IP地址分配，向左
	void ProcIPDistributeMarkerPointLeft(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement);
	// 处理测点IP地址分配，向右
	void ProcIPDistributeMarkerPointRight(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement);
	// 处理单个测点IP地址分配，向左，翻转顺序
	CInstrument* ProcIPDistributeMarkerOnePointLeftReversed(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel);
	// 处理单个测点IP地址分配，向右，翻转顺序
	CInstrument* ProcIPDistributeMarkerOnePointRightReversed(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel);
	// 处理测点IP地址分配，向左，翻转顺序
	void ProcIPDistributeMarkerPointLeftReversed(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement);
	// 处理测点IP地址分配，向右，翻转顺序
	void ProcIPDistributeMarkerPointRightReversed(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement);
	// 清理过期采集道标记
	bool ProcClearTimeOutMarker();
};


