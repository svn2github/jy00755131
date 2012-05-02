#pragma once
#include "ClientRecFrame.h"
#include "ClientSndFrame.h"
#include "ClientCommThread.h"
#include "MatrixDllCall.h"
// 接收来自客户端的帧线程
class CClientRecThread : public CClientCommThread
{
public:
	CClientRecThread(void);
	~CClientRecThread(void);
public:
	// 接收客户端类指针
	CClientRecFrame* m_pClientRecFrame;
	// 发送客户端类指针
	CClientSndFrame* m_pClientSndFrame;
	// DLL函数调用类成员
	CMatrixDllCall* m_pMatrixDllCall;
	// 连接验证
	bool m_bCheckConnected;
public:
 	// 处理函数
 	void OnProc(void);
	// 将帧内容加入待处理任务中
	void SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame);
	// 处理接收命令函数
	void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 处理上电
	void OnProcSetFieldOn(void);
	// 处理断电
	void OnProcSetFieldOff(void);
	// 查询 SurveyXML 文件信息
	void OnProcQuerySurveyXMLInfo(unsigned short usCmd);
	// 查询 PointCode XML文件信息
	void OnProcQueryPointCodeXMLInfo(unsigned short usCmd);
	// 查询 Sensor XML文件信息
	void OnProcQuerySensorXMLInfo(unsigned short usCmd);
	// 查询 Marker XML文件信息
	void OnProcQueryMarkerXMLInfo(unsigned short usCmd);
	// 查询 Aux XML文件信息
	void OnProcQueryAuxXMLInfo(unsigned short usCmd);
	// 查询 Detour XML文件信息
	void OnProcQueryDetourXMLInfo(unsigned short usCmd);
	// 查询 Mute XML文件信息
	void OnProcQueryMuteXMLInfo(unsigned short usCmd);
	// 查询 BlastMachine XML文件信息
	void OnProcQueryBlastMachineXMLInfo(unsigned short usCmd);
	// 查询 Absolute XML文件信息
	void OnProcQueryAbsoluteXMLInfo(unsigned short usCmd);
	// 查询 Generic XML文件信息
	void OnProcQueryGenericXMLInfo(unsigned short usCmd);
	// 查询 Look XML文件信息
	void OnProcQueryLookXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTestBase XML文件信息
	void OnProcQueryInstrumentTestBaseXMLInfo(unsigned short usCmd);
	// 查询 SensorTestBase XML文件信息
	void OnProcQuerySensorTestBaseXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTestLimit XML文件信息
	void OnProcQueryInstrumentTestLimitXMLInfo(unsigned short usCmd);
	// 查询 SensorTestLimit XML文件信息
	void OnProcQuerySensorTestLimitXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTest XML文件信息
	void OnProcQueryInstrumentTestXMLInfo(unsigned short usCmd);
	// 查询 SensorTest XML文件信息
	void OnProcQuerySensorTestXMLInfo(unsigned short usCmd);
	// 查询 MultipleTest XML文件信息
	void OnProcQueryMultipleTestXMLInfo(unsigned short usCmd);
	// 查询 SeisMonitorTest XML文件信息
	void OnProcQuerySeisMonitorTestXMLInfo(unsigned short usCmd);
	// 查询 LAULeakage XML文件信息
	void OnProcQueryLAULeakageXMLInfo(unsigned short usCmd);
	// 查询 FormLine XML文件信息
	void OnProcQueryFormLineXMLInfo(unsigned short usCmd);
};

