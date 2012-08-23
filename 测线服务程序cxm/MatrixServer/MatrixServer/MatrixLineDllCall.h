#pragma once
#include "..\\MatrixServerDll\\ConfigLineXml.h"
/**
* @class CMatrixLineDllCall
* @detail 测线客户端接收数据调用Dll解析类
*/
class CMatrixLineDllCall
{
public:
	CMatrixLineDllCall(void);
	~CMatrixLineDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
public:
	// 初始化测线客户程序设置信息
	void Dll_Init_Instance(m_oLineSetupDataStruct* pLineSetupData);
	// DLL释放实例
	void Dll_Free_Instance(m_oLineSetupDataStruct* pLineSetupData);
	// 载入MatrixServerDll动态链接库
	void LoadMatrixServerDll(CString strPath);
	// 释放MatrixServerDll动态链接库
	void FreeMatrixServerDll(void);
public:
	// 设置Survery设置数据
	void Dll_SetSurverySetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Point Code设置数据
	void Dll_SetPointCodeSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Sensor设置数据
	void Dll_SetSensorSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Marker设置数据
	void Dll_SetMarkerSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Aux设置数据
	void Dll_SetAuxSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Detour设置数据
	void Dll_SetDetourSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Mute设置数据
	void Dll_SetMuteSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置BlastMachine设置数据
	void Dll_SetBlastMachineSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Absolute设置数据
	void Dll_SetAbsoluteSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Generic设置数据
	void Dll_SetGenericSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Look设置数据
	void Dll_SetLookSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置LAULeakage设置数据
	void Dll_SetLAULeakageSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置FormLine设置数据
	void Dll_SetFormLineSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Instrument_SensorTestBase设置数据
	void Dll_SetInstrument_SensorTestBaseSetupData(char* pChar, unsigned int uiSize, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Instrument_SensorTestLimit设置数据
	void Dll_SetInstrument_SensorTestLimitSetupData(char* pChar, unsigned int uiSize, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Instrument Test设置数据
	void Dll_SetInstrumentTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Sensor Test设置数据
	void Dll_SetSensorTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置Multiple Test设置数据
	void Dll_SetMultipleTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
	// 设置SeisMonitor设置数据
	void Dll_SetSeisMonitorSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);

};

