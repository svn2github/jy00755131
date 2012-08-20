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
	/** 接收区域——行数*/
	unsigned int m_uiLineNum;
	/** 接收区域——列数*/
	unsigned int m_uiColumnNum;
public:
	/** 初始化*/
	void OnInit(CString strPath);
	/** 关闭*/
	void OnClose();
public:
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
};

