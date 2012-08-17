#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
class CMatrixLineDllCall
{
public:
	CMatrixLineDllCall(void);
	~CMatrixLineDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
	/** Dll资源创建指针*/
	m_oEnvironmentStruct* m_pEnv;
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
	// 加载Survery设置数据
	void Dll_QuerySurverySetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Point Code设置数据
	void Dll_QueryPointCodeSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Sensor设置数据
	void Dll_QuerySensorSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Marker设置数据
	void Dll_QueryMarkerSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Aux设置数据
	void Dll_QueryAuxSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Detour设置数据
	void Dll_QueryDetourSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Mute设置数据
	void Dll_QueryMuteSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载BlastMachine设置数据
	void Dll_QueryBlastMachineSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Absolute设置数据
	void Dll_QueryAbsoluteSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Generic设置数据
	void Dll_QueryGenericSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Look设置数据
	void Dll_QueryLookSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载LAULeakage设置数据
	void Dll_QueryLAULeakageSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载FormLine设置数据
	void Dll_QueryFormLineSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Instrument_SensorTestBase设置数据
	void Dll_QueryInstrument_SensorTestBaseSetupData(char* cProcBuf, int& iPos, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Instrument_SensorTestLimit设置数据
	void Dll_QueryInstrument_SensorTestLimitSetupData(char* cProcBuf, int& iPos, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Instrument Test设置数据
	void Dll_QueryInstrumentTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Sensor Test设置数据
	void Dll_QuerySensorTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载Multiple Test设置数据
	void Dll_QueryMultipleTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 加载SeisMonitor设置数据
	void Dll_QuerySeisMonitorSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
	// 从XML配置文件得到测试数据限制值
	float Dll_QueryTestDataLimit(bool bInstrument, string str, m_oLineSetupDataStruct* pLineSetupData);
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
	// 得到测线接收区域
	void Dll_GetLineRevSection(unsigned int& uiLineNum, unsigned int& uiColumnNum, m_oLineSetupDataStruct* pLineSetupData);
	// 计算测试数据的算术均方根
	double Dll_CalMeanSquare(m_oInstrumentStruct* pInstrument);
	// 得到在线仪器位置
	void Dll_QueryInstrumentLocation(char* pChar, int& iPos);
	// 判断仪器位置索引号是否已加入索引表
	BOOL Dll_IfLocationExistInMap(int iLineIndex, int iPointIndex, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
	// 根据输入索引号，由索引表得到仪器指针
	m_oInstrumentStruct* Dll_GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
};

