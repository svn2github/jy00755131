#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
#include "CommProtocol.h"
class CMatrixDllCall
{
public:
	CMatrixDllCall(void);
	~CMatrixDllCall(void);
public:
	HMODULE m_hDllMod;
	m_oEnvironmentStruct* m_pEnv;
	// Field On
	bool m_bFieldOn;
	// Field Off
	bool m_bFieldOff;
	// 上次Field On的时间
	unsigned int m_uiFieldOnTimeOld;
public:
	// DLL创建实例
	void Dll_Create_Instance(void);
	// DLL初始化
	void Dll_Init_Instance(void);
	// DLL关闭实例
	void Dll_Close_Instance(void);
	// DLL释放实例
	void Dll_Free_Instance(void);
	// DLL开始AD数据采集
	void Dll_StartSample(int iSampleRate);
	// DLL停止AD数据采集
	void Dll_StopSample(void);
	// DLL开始工作
	int Dll_Work(void);
	// DLL停止工作
	void Dll_Stop(void);
	// 载入MatrixServerDll动态链接库
	void LoadMatrixServerDll(CString strPath);
	// 释放MatrixServerDll动态链接库
	void FreeMatrixServerDll(void);
	// 初始化
	void OnInit(CString strPath);
	// 关闭
	void OnClose(void);
	// Dll_开始AD参数设置
	void Dll_ADCSet(void);
	// DLL按照路由地址设置部分ADC参数
	void Dll_ADCSetPart(int iLineIndex, int iPointIndex, int iRoutDirection, int iOpt, 
		int iSampleRate = 0, bool bOnly = true, bool bRout = true);
	// DLL手动设置ADC参数
	void Dll_ADCSet_ByHand(void);
	// DLL手动打开交叉站某一路由方向的电源
	void Dll_OpenLAUXRoutPower_ByHand(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet);
	// DLL得到路由方向上仪器个数
	unsigned int Dll_GetRoutInstrumentNum(int iLineIndex, int iPointIndex, int iDirection);
	// DLL手动发送ADC参数设置帧
	void Dll_OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout, 
		char* cpADCSet, int iADCSetNum);
	// DLL得到采样数据处理的回调函数
	void Dll_GetProSampleData_CallBack(void);
	// ADC参数设置线程开始工作
	void Dll_ADCSetThreadWork(int iOpt);
	// 判断仪器位置索引号是否已加入索引表
	BOOL Dll_IfLocationExistInMap(int iLineIndex, int iPointIndex, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
	// 增加对象到索引表
	void Dll_AddLocationToMap(int iLineIndex, int iPointIndex, m_oInstrumentStruct* pInstrument, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
	// 根据输入索引号，由索引表得到仪器指针
	m_oInstrumentStruct* Dll_GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
	// 从索引表删除索引号指向的仪器指针
	BOOL Dll_DeleteInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
		map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
	// 加载Survery设置数据
	void Dll_LoadSurverySetupData(void);
	// 加载Point Code设置数据
	void Dll_LoadPointCodeSetupData(void);
	// 加载Sensor设置数据
	void Dll_LoadSensorSetupData(void);
	// 加载Marker设置数据
	void Dll_LoadMarkerSetupData(void);
	// 加载Aux设置数据
	void Dll_LoadAuxSetupData(void);
	// 加载Detour设置数据
	void Dll_LoadDetourSetupData(void);
	// 加载Mute设置数据
	void Dll_LoadMuteSetupData(void);
	// 加载BlastMachine设置数据
	void Dll_LoadBlastMachineSetupData(void);
	// 加载Absolute设置数据
	void Dll_LoadAbsoluteSetupData(void);
	// 加载Generic设置数据
	void Dll_LoadGenericSetupData(void);
	// 加载Look设置数据
	void Dll_LoadLookSetupData(void);
	// 加载LAULeakage设置数据
	void Dll_LoadLAULeakageSetupData(void);
	// 加载FormLine设置数据
	void Dll_LoadFormLineSetupData(void);
	// 加载Instrument_SensorTestBase设置数据
	void Dll_LoadInstrument_SensorTestBaseSetupData(bool bInstrument);
	// 加载Instrument_SensorTestLimit设置数据
	void Dll_LoadInstrument_SensorTestLimitSetupData(bool bInstrument);
	// 加载Instrument Test设置数据
	void Dll_LoadInstrumentTestSetupData(void);
	// 加载Sensor Test设置数据
	void Dll_LoadSensorTestSetupData(void);
	// 加载Multiple Test设置数据
	void Dll_LoadMultipleTestSetupData(void);
	// 加载SeisMonitor设置数据
	void Dll_LoadSeisMonitorSetupData(void);
	// 设置Survery设置数据
	void Dll_SetSurverySetupData(char* pChar, unsigned int uiSize);
	// 设置Point Code设置数据
	void Dll_SetPointCodeSetupData(char* pChar, unsigned int uiSize);
	// 设置Sensor设置数据
	void Dll_SetSensorSetupData(char* pChar, unsigned int uiSize);
	// 设置Marker设置数据
	void Dll_SetMarkerSetupData(char* pChar, unsigned int uiSize);
	// 设置Aux设置数据
	void Dll_SetAuxSetupData(char* pChar, unsigned int uiSize);
	// 设置Detour设置数据
	void Dll_SetDetourSetupData(char* pChar, unsigned int uiSize);
	// 设置Mute设置数据
	void Dll_SetMuteSetupData(char* pChar, unsigned int uiSize);
	// 设置BlastMachine设置数据
	void Dll_SetBlastMachineSetupData(char* pChar, unsigned int uiSize);
	// 设置Absolute设置数据
	void Dll_SetAbsoluteSetupData(char* pChar, unsigned int uiSize);
	// 设置Generic设置数据
	void Dll_SetGenericSetupData(char* pChar, unsigned int uiSize);
	// 设置Look设置数据
	void Dll_SetLookSetupData(char* pChar, unsigned int uiSize);
	// 设置LAULeakage设置数据
	void Dll_SetLAULeakageSetupData(char* pChar, unsigned int uiSize);
	// 设置FormLine设置数据
	void Dll_SetFormLineSetupData(char* pChar, unsigned int uiSize);
	// 设置Instrument_SensorTestBase设置数据
	void Dll_SetInstrument_SensorTestBaseSetupData(char* pChar, unsigned int uiSize, bool bInstrument);
	// 设置Instrument_SensorTestLimit设置数据
	void Dll_SetInstrument_SensorTestLimitSetupData(char* pChar, unsigned int uiSize, bool bInstrument);
	// 设置Instrument Test设置数据
	void Dll_SetInstrumentTestSetupData(char* pChar, unsigned int uiSize);
	// 设置Sensor Test设置数据
	void Dll_SetSensorTestSetupData(char* pChar, unsigned int uiSize);
	// 设置Multiple Test设置数据
	void Dll_SetMultipleTestSetupData(char* pChar, unsigned int uiSize);
	// 设置SeisMonitor设置数据
	void Dll_SetSeisMonitorSetupData(char* pChar, unsigned int uiSize);

};

