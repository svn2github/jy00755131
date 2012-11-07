#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
class CMatrixServerDllCall
{
public:
	CMatrixServerDllCall(void);
	~CMatrixServerDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
	/** Dll资源创建指针*/
	m_oEnvironmentStruct* m_pEnv;
public:
	// DLL开始AD数据采集
	void Dll_StartSample(int iSampleRate, bool bHPFOpen = true);
	// DLL停止AD数据采集
	void Dll_StopSample(void);
	// DLL开始工作
	unsigned int Dll_Work(void);
	// DLL停止工作
	void Dll_Stop(void);
	// Dll_开始AD参数设置
	void Dll_ADCSet(void);
	// DLL按照路由地址设置部分ADC参数
	void Dll_ADCSetPart(int iLineIndex, int iPointIndex, int iRoutDirection, int iOpt, 
		int iSampleRate = 0, bool bOnly = true, bool bRout = true, bool bHPFOpen = true);
	// DLL手动设置ADC参数
	void Dll_ADCSet_ByHand(void);
	// DLL手动打开交叉站某一路由方向的电源
	void Dll_OpenLAUXRoutPower_ByHand(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet);
	// DLL得到路由方向上仪器个数
	unsigned int Dll_GetRoutInstrumentNum(int iLineIndex, int iPointIndex, int iDirection);
	// DLL手动发送ADC参数设置帧
	void Dll_OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout, 
		char* cpADCSet, int iADCSetNum);
// 	// DLL得到采样数据处理的回调函数
// 	void Dll_GetProSampleData_CallBack(void);
	// ADC参数设置线程开始工作
	void Dll_ADCSetThreadWork(int iOpt);

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
	// 根据输入索引号，由索引表得到仪器指针
	m_oInstrumentStruct* Dll_GetInstrumentFromMap(unsigned int uiIndex, 
		hash_map<unsigned int, m_oInstrumentStruct*>* pMap);
};

