#pragma once

#include "logicdata.h"
#include "sitedata.h"
#include "SocketTestSetFrame.h"
#include "SocketTestDataFrame.h"
#include "SocketInterfaceDataSend.h"
#include "Matrixline.h"

/**
*@brief 仪器测试处理线程类
*/
class CThreadProcTest : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcTest)

protected:
	CThreadProcTest();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcTest();

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
	/** 启动状态*/
	bool m_bSwitchON;

	/** 处理方式 1-基本测试；2-噪声监测*/
	int m_iProcTestMethod;
	/** 处理新加检波器的初始测试*/
	bool m_bProcSensorTestOnField;
	/** 噪声监测数据网络发送计数*/
	unsigned int m_uiTestNoiseNetSendCount;
	/** 噪声监测数据网络发送步长*/
	unsigned int m_uiTestNoiseNetSendStep;

	/** 测线设置数据对象指针*/
	CLogicData* m_pLogicData;
	/** 现场数据对象指针*/
	CSiteData* m_pSiteData;	

	/** 仪器测试设置网络对象指针*/	
	CSocketTestSetFrame* m_pSocketTestSetFrame;
	/** 仪器测试数据接收网络对象指针*/
	CSocketTestDataFrame* m_pSocketTestDataFrame;
	/** 服务器内部网络数据发送处理网络对象*/
	CSocketInterfaceDataSend* m_pSocketInterfaceDataSend;

	/** 参加测试IP地址数量*/
	unsigned int m_uiIPCount;
	/** 测试项目队列对象*/
	CTestProject* m_pTestProject;
	/** 测试数据缓冲区*/
	CTestDataBuffer* m_pTestDataBuffer;
	/** 测试请求*/
	CTestRequest* m_pTestRequest;
	/** 测试基本设置*/
	CTestBaseData* m_pTestBaseData;

	/** 测试数据保存文件对象*/
	CStdioFile m_oFileTestData;
	byte * ViewBuf;
	
	/** 设备编号*/
	int FDUNum;
	/** 帧计数*/
	int FrameNum[MaxFDUCount];
	
	int irecevice;
	int lossframecount;
	int saveecount;
	int recevicecount;
	int totalcount;
	/** 抽点指针,拼帧的点位置*/
	int ViewOffset[MaxFDUCount];
	int ViewOffsetFrame[MaxFDUCount];
	
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
	// 等待
	void Wait(int iWaitStopCount);
	// 线程是否空闲
	bool IsThreadFree();
	// 锁定线程
	bool Lock();
	// 解锁线程
	void Unlock();
	// 启动处理
	void ResumeProcTest();

	// 测试处理
	void ProcTest();
	// 停止正在进行数据采集的仪器
	void StopInstrumentWhichIsCollecting();
	// 设置噪声监测数据网络发送步长
	void SetTestNoiseNetSendStep();
	// 测试处理，一次测试请求，开始
	void ProcTestRequestStart();
	// 测试处理，一次测试请求，处理
	void ProcTestRequest();
	// 测试处理，一次测试请求，结束
	void ProcTestRequestEnd();
	// 发送测试结果，基本测试-新加检波器的初始测试
	void SendTestResultBaseForOnFieldTest();
	// 更新上次现场数据变化时刻，基本测试
	void UpdateSiteDataChangedTimeForTestData();

	// 打开测试数据文件
	void FileTestDataOpen();
	// 写测试数据文件
	void FileTestDataWrite(byte* pBlockData, unsigned int uiSize);
	// 关闭测试数据文件
	void FileTestDataClose();

	// 处理发送测试设置帧
	void ProcSendTestSetFrame();
	// 处理发送测试开始帧
	void ProcSendTestStartFrame();
	// 处理发送测试停止帧
	void ProcSendTestEndFrame(unsigned int uiInstrumentIP);

	// 得到并处理测试数据
	void GetAndProcTestData();
	void GetAndProcTestData1();
	// 设置测试数据
	void SetTestData(byte* lpData, unsigned int uiIP, unsigned short usDataIndex);
	// 转发数据
	void TranferView(byte* lpData);
	// 处理测试数据
	void SetTestDataToProc();
	// 处理测试数据，噪声监测
	void SetTestDataToProcNoise();
	// 处理测试数据，保存测试数据
	void SetTestDataToProcFileTestDataWrite();
	// 处理测试数据，结束前
	void SetTestDataToProcAll();
	//处理测试数据，结束测试请求前，保存测试数据
	void SetTestDataToProcAllFileTestDataWrite();
	// 得到测试结果，基本测试
	void GetTestResultBase();
	// 设置测试结果，基本测试
	void SetTestResultBase();
	// 发送测试结果
	void SendTestResult();
	// 发送测试结果，基本测试
	void SendTestResultBase();
	// 发送测试结果，噪声监测
	void SendTestResultNoise();

	// 处理来自测试设置的基本测试任务
	bool OnTestTaskBaseFromTestSetup(unsigned int uiTestNb);
	// 处理来自测试视图的基本测试任务
	bool OnTestTaskBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread);
	// 处理来自测试设置的噪声监测任务
	bool OnTestTaskNoise(byte byTestOperation);
	// 处理来自测试设置的噪声监测任务
	bool OnTestTaskNoise(CString strTestAbsoluteSpread);
	// 处理新加检波器的初始测试
	void ProcSensorTestOnField();
	// AD参数设置
	void ADSet();
	// 处理发送AD测试开始帧
	void ProcSendTestADStart();
};


