#pragma once
#include "MatrixServerDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommServerDll
{
public:
	CCommServerDll(void);
	~CCommServerDll(void);
public:
	CMatrixServerDllCall* m_pMatrixServer;
	/** 测线客户端信息*/
	m_oLineSetupDataStruct* m_pLineSetupData;
	/** 施工客户端信息*/
	m_oOptSetupDataStruct* m_pOptSetupData;
public:
	/** 接收区域——行数*/
	unsigned int m_uiLineNum;
	/** 接收区域——列数*/
	unsigned int m_uiColumnNum;
	/** 仪器噪声测试判据*/
	float m_fInstrumentNoiseLimit;
	/** 仪器畸变测试判据*/
	float m_fInstrumentDistortionLimit;
	/** 仪器串音测试判据*/
	float m_fInstrumentCrosstalkLimit;
	/** 仪器增益相位测试判据*/
	float m_fInstrumentGainPhaseLimit;
	/** 仪器共模测试判据*/
	float m_fInstrumentCMRRLimit;
	/** 野外电阻测试判据，下限*/
	float m_fSensorResistanceLimitMin;
	/** 野外电阻测试判据，上限*/
	float m_fSensorResistanceLimitMax;
	/** 野外漏电测试判据*/
	float m_fSensorLeakageLimit;
	/** 野外噪声测试判据*/
	float m_fSensorNoiseLimit;
	/** 野外倾斜度测试判据*/
	float m_fSensorTiltLimit;
	/** 野外倾斜度模型测试判据*/
	float m_fSensorTiltModelLimit;
	/** 地震监测测试数据*/
	float m_fSeisMonitorLimit;
public:
	/**
	* @fn void OnProcSetFieldOn(CCommRecThread* pRecThread)
	* @detail 处理上电
	* @param[in] CCommRecThread*
	* @return void
	*/
	void OnProcSetFieldOn(CCommRecThread* pRecThread);
	/**
	* @fn void OnProcSetFieldOff(CCommRecThread* pRecThread)
	* @detail 处理断电
	* @param[in] CCommRecThread*
	* @return void
	*/
	void OnProcSetFieldOff(CCommRecThread* pRecThread);
		// 处理查询接收区域命令
	void OnProcQueryRevSection(CCommRecThread* pRecThread);
	// 从XML配置文件得到测试数据限制值
	float QueryTestDataLimitFromXML(bool bInstrument, string str);
	/**
	* @fn void OnProcQuerySurveyXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 SurveyXML 文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySurveyXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryPointCodeXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 PointCode XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryPointCodeXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySensorXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Sensor XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySensorXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryMarkerXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Marker XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryMarkerXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryAuxXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Aux XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryAuxXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryDetourXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Detour XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryDetourXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryMuteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Mute XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryMuteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryBlastMachineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 BlastMachine XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryBlastMachineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryAbsoluteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Absolute XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryAbsoluteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryGenericXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Generic XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryGenericXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryLookXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Look XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryLookXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryInstrumentTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 InstrumentTestBase XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryInstrumentTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySensorTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 SensorTestBase XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySensorTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryInstrumentTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 InstrumentTestLimit XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryInstrumentTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySensorTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 SensorTestLimit XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySensorTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryInstrumentTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 InstrumentTest XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryInstrumentTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySensorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 SensorTest XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySensorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryMultipleTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 MultipleTest XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryMultipleTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySeisMonitorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 SeisMonitorTest XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySeisMonitorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryLAULeakageXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 LAULeakage XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryLAULeakageXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryFormLineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 FormLine XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryFormLineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);

	// 按区域查询仪器信息
	unsigned int QueryInstrumentInfoBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器噪声测试数据和测试结果
	unsigned int QueryInstrNoiseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器失真测试数据和测试结果
	unsigned int QueryInstrDistortionTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器串扰测试数据和测试结果
	unsigned int QueryInstrCrosstalkTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器共模抑制比测试数据和测试结果
	unsigned int QueryInstrCMRRTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器增益相位测试数据和测试结果
	unsigned int QueryInstrGainPhaseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器阻抗测试数据和测试结果
	unsigned int QuerySensorResistanceTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器漏电测试数据和测试结果
	unsigned int QuerySensorLeakageTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器噪声测试数据和测试结果
	unsigned int QuerySensorNoiseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度测试数据和测试结果
	unsigned int QuerySensorTiltTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度模式测试数据和测试结果
	unsigned int QuerySensorTiltModelTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域地震监测测试数据和测试结果
	unsigned int QuerySeisMonitorTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);

	// 计算测试数据的算术均方根
	float CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument);

	// 处理仪器设备表更新
	void OnProcInstrumentTableUpdate(CCommRecThread* pRecThread);

	// 处理仪器信息查询
	void OnProcQueryBySN(CCommRecThread* pRecThread, char* pChar, unsigned int uiSize, 
		unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 处理全部信息查询
	void OnProcQueryInfoAll(CCommRecThread* pRecThread, 
		unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 按区域查询仪器信息
	unsigned int QueryByArea(CCommRecThread* pRecThread, unsigned int uiSN, 
		unsigned int uiSendPos, unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));

	/**
	* @fn void OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 OperationDelay XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 炮表 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Explo震源类型 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Vibro震源类型 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessRecord XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessAux XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessAcq XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessType XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 注释 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
};

