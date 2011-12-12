#pragma once
 #include "InstrumentList.h"
#include <hash_map>
using stdext::hash_map;

// #include "indexinstrumentmap.h"
// #include "routlist.h"
// #include "indexroutmap.h"
// #include "iplist.h"
// #include "LogicData.h"
// 
// #include "RunTimeDataList.h"

/**
*@brief 现场数据类
*/
class CSiteData
{
public:
	CSiteData();
	~CSiteData();

public: //属性
// 	/** 首个主交叉站指针*/
// 	CInstrument* m_pFirstMainCross;
	/** 仪器队列仪器总数*/
	unsigned int m_uiInstrumentCountAll;
	/** 仪器队列*/
	CInstrumentList m_oInstrumentList;
// 	/** SN仪器索引表*/
// 	CIndexInstrumentMap m_oSNInstrumentMap;
// 	/** IP地址仪器索引表*/
// 	CIndexInstrumentMap m_oIPInstrumentMap;
// 	/** 路由队列*/
// 	CRoutList m_oRoutList;
// 	/** 路由地址路由索引表*/
// 	CIndexRoutMap m_oRoutIPRoutMap;
// 	/** IP地址队列临时IP地址总数*/
// 	unsigned int m_uiIPTempCountAll;
// 	/** IP地址队列*/
// 	CIPList m_oIPList;
// 	/** 运行日志*/
// 	CRunTimeDataList m_oRunTimeDataList;
	/** 野外设备系统时间*/
	unsigned int m_uiSystemTimeSite;

	/** 为服务器端命令帧发送网络接口准备的线程同步对象*/
	CCriticalSection m_oCriticalSectionInterfaceCmdSend;
	/** 为测试线程准备的线程同步对象*/
	CCriticalSection m_oCriticalSectionProcTest;
	/** 为删除仪器或路由处理线程准备的线程同步对象*/
	CCriticalSection m_oCriticalSectionProcDelete;
	/** 为路由监视线程准备的线程同步对象*/
	CCriticalSection m_oCriticalSectionProcMonitorRout;
	/** 为现场数据变化的线程同步对象*/
	CCriticalSection m_oCriticalSectionSiteDataChanged;
	/** 为现场系统时间变化的线程同步对象*/
	CCriticalSection m_oCriticalSectionSystemTimeSite;
	/** 为采集接收线程同步对象*/
	CCriticalSection m_oSocketTestDataFrame;
	/** 为采集接收线程ID对象*/
	DWORD m_oProcTestThreadID;

	/** 删除仪器或路由处理线程正在运行*/
	bool m_bProcDeleteRunning;
	/** 现场数据输出线程正在运行*/
	bool m_bProcSiteDataOutputRunning;

	/** 暂停要求，要求仪器IP地址分配线程暂停*/
	bool m_bPauseRequestForProcIPDistribute;

	/** 首包处理线程暂停*/
	bool m_bProcHeadFramePause;
	/** 尾包处理线程暂停*/
	bool m_bProcTailFramePause;
	/** 路由监视线程暂停*/
	bool m_bProcMonitorRoutPause;
	/** 仪器IP地址分配线程暂停*/
	bool m_bProcIPDistributePause;
	/** 仪器时延处理线程暂停*/
	bool m_bProcTimeDelayPause;
	/** 仪器IP地址设置线程暂停*/
	bool m_bProcIPSetPause;
	/** 删除仪器或路由处理线程暂停*/
	bool m_bProcDeletePause;
	/** 现场数据输出线程线程暂停*/
	bool m_bProcSiteDataOutputPause;

	/** 心跳处理线程停止*/
	bool m_bProcHeartBeatStop;
	/** 首包处理线程停止*/
	bool m_bProcHeadFrameStop;
	/** 尾包处理线程停止*/
	bool m_bProcTailFrameStop;
	/** 路由监视线程停止*/
	bool m_bProcMonitorRoutStop;
	/** 仪器IP地址分配线程停止*/
	bool m_bProcIPDistributeStop;
	/** 仪器时延处理线程停止*/
	bool m_bProcTimeDelayStop;
	/** 仪器时延处理线程停止*/
	bool m_bProcTimeDelayON_stop;
	/** 仪器时延计数*/
	int iDelayfrist;
	/** 仪器IP地址设置线程停止*/
	bool m_bProcIPSetStop;
	/** 删除仪器或路由处理线程停止*/
	bool m_bProcDeleteStop;
	/** 现场数据输出线程线程停止*/
	bool m_bProcSiteDataOutputStop;
	/** 测试线程线程停止，基本测试*/
	bool m_bProcTestBaseStop;
	/** 测试线程线程停止，噪声监测*/
	bool m_bProcTestNoiseStop;

	/** 心跳处理线程关闭*/
	bool m_bProcHeartBeatClose;
	/** 首包处理线程关闭*/
	bool m_bProcHeadFrameClose;
	/** 尾包处理线程关闭*/
	bool m_bProcTailFrameClose;
	/** 路由监视线程关闭*/
	bool m_bProcMonitorRoutClose;
	/** 仪器IP地址分配线程关闭*/
	bool m_bProcIPDistributeClose;
	/** 仪器时延处理线程关闭*/
	bool m_bProcTimeDelayClose;
	/** 仪器IP地址设置线程关闭*/
	bool m_bProcIPSetClose;
	/** 删除仪器或路由处理线程关闭*/
	bool m_bProcDeleteClose;
	/** 现场数据输出线程线程关闭*/
	bool m_bProcSiteDataOutputClose;
	/** 测试线程线程关闭，基本测试*/
	bool m_bProcTestBaseClose;
	/** 测试线程线程关闭，噪声监测*/
	bool m_bProcTestNoiseClose;

	/** 现场数据是否变化*/
	bool m_bSiteDataChanged;
	/** 上次现场数据变化时刻*/
	unsigned int m_uiSiteDataChangedTime;
	/** 测试数据是否变化*/
	bool m_bTestDataChanged;

// 	/** 测线设置数据对象指针*/
// 	CLogicData* m_pLogicData;

	/** FTP服务器IP地址*/
	CString m_strIPFTPServer;
	/** 首包接收起始时间-延迟*/
	unsigned int HeadminTime;
	/** 首包延迟计数*/
	int OnMarkerCount;
	/** 首包延迟计数*/
	int HeadCount;
	/** 最大首包时间重复*/
	int HeadEtime;
// 	/** 网络数据错误计数*/
// 	int NetDataCount;
// 	/** 网络错误计数*/
// 	int NetOrderCount;
// 	/** OnReceive数据*/
// 	byte OnReceive_buf[256*UDPbufCount];

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();

	// 仪器SN号是否已加入SN索引表
	BOOL IfSNExistInSNMap(unsigned int uiSN);
	// 由仪器SN索引表得到一个仪器
	BOOL GetInstrumentFromSNMapBySN(unsigned int uiIndex, CInstrument* &pInstrument);
	// 仪器IP地址是否已加入IP地址索引表
	BOOL IfIPExistInIPMap(unsigned int uiIP);
	// 由仪器IP地址索引表得到一个仪器
	BOOL GetInstrumentFromIPMapByIP(unsigned int uiIndex, CInstrument* &pInstrument);
// 	// 更新路由对象的路由时刻
// 	void UpdateRoutTime(unsigned int uiRoutIP);
	// 得到一个空闲仪器
	CInstrument* GetFreeInstrument();
	// 得到连接的下一个仪器
	CInstrument* GetNextInstrument(CInstrument* pInstrument, unsigned int uiRoutDirection);
// 	// 删除仪器串
// 	void DeleteInstrumentLink(CInstrument* pInstrument, unsigned int uiDirection);
// 	// 删除仪器，递归
// 	void DeleteInstrument(CInstrument* pInstrument, unsigned int uiDirection);
// 	// 删除路由链接的仪器
// 	bool DeleteRoutLinkInstrument(CRout* pRout);
// 	// 删除路由IP地址被禁止的路由链接的仪器
// 	bool DeleteRoutLinkInstrumentWhenRoutIPDisable(CRout* pRout);
// 	// 删除路由时间过期的路由对象所链接的仪器
// 	bool DeleteRoutLinkInstrumentWhenRoutTimeExpired(CRout* pRout);
// 
// 	// 删除路由
// 	void DeleteRout(CRout* pRout);
// 	// 删除路由
// 	void DeleteRout(CInstrument* pInstrument);
// 	// 删除路由IP地址被禁止的路由
// 	bool DeleteRoutWhenRoutIPDisable(CRout* pRout);
// 	// 释放仪器IP地址
// 	void ReleaseInstrument(unsigned int uiIP, unsigned int uiIPDistributeTime);
// 
// 	// 得到采集站
// 	CInstrument* GetInstrumentCollect(CInstrument* pInstrument, unsigned int uiRoutDirection);

	// 更新上次现场数据变化时刻
	void UpdateSiteDataChangedTime();
	// 判断现场数据是否在规定时间内无变化
	bool JudgeSiteDataChangedTime();
	// 重置上次现场数据变化时刻
	void ResetSiteDataChangedTime();
// 
// 	// 得到路由对象
// 	CRout* GetRout(CInstrument* pInstrument, unsigned int uiRoutDirection);
// 	
// 	// 设置测试结果，基本测试
// 	void SetTestResultBase(int iTestType, CTestElementList* pTestElementList);

};
