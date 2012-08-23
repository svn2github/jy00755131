#pragma once
#include "MatrixLineDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
/**
* @class CCommLineDll
* @detail 网络接收命令的命令响应函数类
*/
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

