#pragma once
#include "MatrixLineDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommLineDll
{
public:
	CCommLineDll(void);
	~CCommLineDll(void);
public:
	CMatrixLineDllCall* m_pMatrixLine;
	/** 测线客户端信息*/
	m_oLineSetupDataStruct* m_pLineSetupData;
public:
public:
	/** 初始化*/
	void OnInit(CString strPath);
	/** 关闭*/
	void OnClose();
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
	// 处理查询接收区域命令
	void OnProcQueryRevSection(CCommRecThread* pRecThread);
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
	/**
	* @fn void OnProcSetSurveyXMLInfo(char* pChar, unsigned int uiSize)
	* @detail 设置 SurveyXML 文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSurveyXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetPointCodeXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 PointCode XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetPointCodeXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSensorXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Sensor XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSensorXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetMarkerXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Marker XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetMarkerXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetAuxXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Aux XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetAuxXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetDetourXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Detour XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetDetourXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetMuteXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Mute XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetMuteXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetBlastMachineXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 BlastMachine XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetBlastMachineXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetAbsoluteXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Absolute XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetAbsoluteXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetGenericXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Generic XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetGenericXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetLookXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Look XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetLookXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetInstrumentTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 InstrumentTestBase XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetInstrumentTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSensorTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 SensorTestBase XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSensorTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetInstrumentTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 InstrumentTestLimit XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetInstrumentTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSensorTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 SensorTestLimit XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSensorTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetInstrumentTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 InstrumentTest XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetInstrumentTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSensorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 SensorTest XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSensorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetMultipleTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 MultipleTest XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetMultipleTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSeisMonitorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 SeisMonitorTest XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSeisMonitorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetLAULeakageXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 LAULeakage XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetLAULeakageXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetFormLineXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 FormLine XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetFormLineXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	
	// 按区域查询仪器信息
	unsigned int QueryInstrumentInfoByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器噪声测试数据和测试结果
	unsigned int QueryInstrNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器失真测试数据和测试结果
	unsigned int QueryInstrDistortionTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器串扰测试数据和测试结果
	unsigned int QueryInstrCrosstalkTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器共模抑制比测试数据和测试结果
	unsigned int QueryInstrCMRRTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器增益相位测试数据和测试结果
	unsigned int QueryInstrGainPhaseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器阻抗测试数据和测试结果
	unsigned int QuerySensorResistanceTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器漏电测试数据和测试结果
	unsigned int QuerySensorLeakageTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器噪声测试数据和测试结果
	unsigned int QuerySensorNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度测试数据和测试结果
	unsigned int QuerySensorTiltTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度模式测试数据和测试结果
	unsigned int QuerySensorTiltModelTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域地震监测测试数据和测试结果
	unsigned int QuerySeisMonitorTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);

	// 从XML配置文件得到测试数据限制值
	float QueryTestDataLimitFromXML(bool bInstrument, string str);
	// 计算测试数据的算术均方根
	float CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument);

	// 由线号和点号得到区域位置
	void GetAreaFromPoint(int iLineIndex, int iPointIndex, m_oAreaStruct* pAreaStruct);
	// 由区域号得到线号及点号范围
	void GetPointRangeFromArea(int* iLineIndex, int* iPointMinIndex, int* iPointMaxIndex, 
		m_oAreaStruct* pAreaStruct);
	// 处理仪器设备表更新
	void OnProcInstrumentTableUpdate(CCommRecThread* pRecThread);

	// 判断仪器更新区域是否已加入索引表
	BOOL IfAreaExistInMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct);
	// 增加对象到索引表
	void AddAreaToMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct);
	// 处理仪器信息查询
	void OnProcQueryByArea(CCommRecThread* pRecThread, char* pChar, unsigned int uiSize, 
		unsigned int(CCommLineDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 处理全部信息查询
	void OnProcQueryInfoAll(CCommRecThread* pRecThread, 
		unsigned int(CCommLineDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 按区域查询仪器信息
	unsigned int QueryByArea(CCommRecThread* pRecThread, m_oAreaStruct* pArea, 
		unsigned int uiStartPos, unsigned int(CCommLineDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
};

