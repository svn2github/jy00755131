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
	void Dll_StartSample(int iSampleRate);
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
	void Dll_ADCSetPart(unsigned int uiSN, int iRoutDirection, int iOpt, int iSampleRate = 0);
	// DLL手动设置ADC参数
	void Dll_ADCSet_ByHand(void);
	// DLL手动打开交叉站某一路由方向的电源
	void Dll_OpenLAUXRoutPower_ByHand(unsigned int uiSN, unsigned char ucLAUXRoutOpenSet);
	// DLL得到路由方向上仪器个数
	unsigned int Dll_GetRoutInstrumentNum(unsigned int uiSN, int iDirection);
	// DLL手动发送ADC参数设置帧
	void Dll_OnSetADCSetFrameByHand(unsigned int uiSN, int iDirection, bool bRout, 
		char* cpADCSet, int iADCSetNum);
	// DLL得到采样数据处理的回调函数
	void Dll_GetProSampleData_CallBack(void);
};

