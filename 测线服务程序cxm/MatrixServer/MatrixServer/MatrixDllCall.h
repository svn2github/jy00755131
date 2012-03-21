#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
class CMatrixDllCall
{
public:
	CMatrixDllCall(void);
	~CMatrixDllCall(void);
protected:
	HMODULE m_hDllMod;
	m_oEnvironmentStruct* m_pEnv;
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
	void Dll_StartSample(void);
	// DLL停止AD数据采集
	void Dll_StopSample(void);
	// DLL开始工作
	void Dll_Work(void);
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
	void Dll_ADCSet_Part(unsigned int uiRoutIP, int iOpt);
	// DLL手动设置ADC参数
	void Dll_ADCSet_ByHand(void);
	// DLL得到采样数据处理的回调函数
	void Dll_GetProSampleData_CallBack(void);
};

