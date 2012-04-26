#include "StdAfx.h"
#include "MatrixDllCall.h"

// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 初始化实例
typedef void (*Init_Instance)(m_oEnvironmentStruct* pEnv);
// Field On
typedef void (*On_Work)(m_oEnvironmentStruct* pEnv);
// Field Off
typedef void (*On_Stop)(m_oEnvironmentStruct* pEnv);
// 关闭实例
typedef void (*Close_Instance)(m_oEnvironmentStruct* pEnv);
// 开始采样
typedef void (*On_StartSample_All)(m_oEnvironmentStruct* pEnv);
// 停止采样
typedef void (*On_StopSample_All)(m_oEnvironmentStruct* pEnv);
// AD参数设置
typedef void (*On_ADCSet_All)(m_oEnvironmentStruct* pEnv);
// ADC参数设置改为手动设置
typedef void (*On_ADCSet_ByHand)(m_oInstrumentListStruct* pInstrumentList);
// 按照路由地址重置ADC参数设置标志位
typedef void (*On_ADCSet_Part)(int iLineIndex, int iPointIndex, int iRoutDirection, 
	int iOpt, m_oEnvironmentStruct* pEnv, bool bOnly, bool bRout);
// ADC参数设置线程开始工作
typedef void (*On_ADCSetThread_Work)(int iOpt, m_oADCSetThreadStruct* pADCSetThread);
// 打开交叉站某一路由方向的电源
typedef bool (*On_OpenLAUXRoutPower)(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet, 
	m_oEnvironmentStruct* pEnv);
// 得到路由方向上仪器个数
typedef bool (*On_GetRoutInstrumentNum)(int iLineIndex, int iPointIndex, int iDirection, 
	m_oEnvironmentStruct* pEnv, unsigned int& uiInstrumentNum);
// 采样数据回调函数
typedef void (*Get_ProSampleDateCallBack)(m_oADCDataRecThreadStruct* pADCDataRecThread, 
	ProSampleDateCallBack pCallBack);
// 手动发送ADC参数设置帧
typedef bool (*On_SetADCSetFrameByHand)(int iLineIndex, int iPointIndex, int iDirection, bool bRout, 
	char* cpADCSet, int iADCSetNum, m_oEnvironmentStruct* pEnv);
// 通过位置得到设备指针
typedef unsigned int (*Get_InstrumentSnByLocation)(int iLineIndex, int iPointIndex, 
	m_oInstrumentListStruct* pInstrumentList);
void CALLBACK ProSampleDate(int _iLineIndex, int _iPointIndex, int *_piData, int _iSize, unsigned int _uiSN)
{

}

CMatrixDllCall::CMatrixDllCall(void)
{
}


CMatrixDllCall::~CMatrixDllCall(void)
{
}

// DLL创建实例
void CMatrixDllCall::Dll_Create_Instance(void)
{
	Create_Instance Dll_On_Create = NULL;
	Dll_On_Create = (Create_Instance)GetProcAddress(m_hDllMod, "OnCreateInstance");
	if (!Dll_On_Create)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		m_pEnv = (*Dll_On_Create)();
	}
}


// DLL初始化
void CMatrixDllCall::Dll_Init_Instance(void)
{
	Init_Instance Dll_On_Init = NULL;
	Dll_On_Init = (Init_Instance)GetProcAddress(m_hDllMod, "OnInit");
	if (!Dll_On_Init)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Init)(m_pEnv);
	}
}

// DLL关闭实例
void CMatrixDllCall::Dll_Close_Instance(void)
{
	Close_Instance Dll_On_Close = NULL;
	Dll_On_Close = (Close_Instance)GetProcAddress(m_hDllMod, "OnClose");
	if (!Dll_On_Close)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Close)(m_pEnv);
	}
}


// DLL释放实例
void CMatrixDllCall::Dll_Free_Instance(void)
{
	Free_Instance Dll_On_Free = NULL;
	Dll_On_Free = (Free_Instance)GetProcAddress(m_hDllMod, "OnFreeInstance");
	if (!Dll_On_Free)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Free)(m_pEnv);
	}
}
// DLL开始AD数据采集
void CMatrixDllCall::Dll_StartSample(int iSampleRate)
{
	On_StartSample_All Dll_On_StartSample = NULL;
	Dll_On_StartSample = (On_StartSample_All)GetProcAddress(m_hDllMod, "OnADCStartSample");
	if (!Dll_On_StartSample)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		EnterCriticalSection(&m_pEnv->m_pInstrumentCommInfo->m_oSecCommInfo);
// 		// 经过高通滤波器
// 		if (iSampleRate == 250)
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 67;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 86;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 19;
// 		}
// 		else if (iSampleRate == 500)
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 75;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -89;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 9;
// 		}
// 		else if (iSampleRate == 1000)
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 83;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -45;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 4;
// 		}
// 		else if (iSampleRate == 2000)
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 91;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 105;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 2;
// 		}
// 		else if (iSampleRate == 4000)
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 99;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 52;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 1;
// 		}
// 		// 如果不在所选采样率则按照1000采样率采样
// 		else
// 		{
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 83;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -45;
// 			m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 4;
// 		}
		// 不经过高通滤波
		if (iSampleRate == 250)
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 66;
		}
		else if (iSampleRate == 500)
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 74;
		}
		else if (iSampleRate == 1000)
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
		}
		else if (iSampleRate == 2000)
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 90;
		}
		else if (iSampleRate == 4000)
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 98;
		}
		// 如果不在所选采样率则按照1000采样率采样
		else
		{
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
		}
		LeaveCriticalSection(&m_pEnv->m_pInstrumentCommInfo->m_oSecCommInfo);
		// call the function
		(*Dll_On_StartSample)(m_pEnv);
	}
}


// DLL停止AD数据采集
void CMatrixDllCall::Dll_StopSample(void)
{
	On_StopSample_All Dll_On_StopSample = NULL;
	Dll_On_StopSample = (On_StopSample_All)GetProcAddress(m_hDllMod, "OnADCStopSample");
	if (!Dll_On_StopSample)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_StopSample)(m_pEnv);
	}
}

// DLL开始工作
void CMatrixDllCall::Dll_Work(void)
{
	On_Work Dll_On_Work = NULL;
	Dll_On_Work = (On_Work)GetProcAddress(m_hDllMod, "OnWork");
	if (!Dll_On_Work)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Work)(m_pEnv);
	}
}


// DLL停止工作
void CMatrixDllCall::Dll_Stop(void)
{
	On_Stop Dll_On_Stop = NULL;
	Dll_On_Stop = (On_Stop)GetProcAddress(m_hDllMod, "OnStop");
	if (!Dll_On_Stop)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Stop)(m_pEnv);
	}
}


// 载入MatrixServerDll动态链接库
void CMatrixDllCall::LoadMatrixServerDll(CString strPath)
{
	CString str = _T("");
	m_hDllMod = ::LoadLibrary(strPath);
	if (m_hDllMod == NULL)
	{
		str.Format(_T("载入失败！错误码为%d。"), GetLastError());
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
}


// 释放MatrixServerDll动态链接库
void CMatrixDllCall::FreeMatrixServerDll(void)
{
	::FreeLibrary(m_hDllMod);
}


// 初始化
void CMatrixDllCall::OnInit(CString strPath)
{
	// 载入动态链接库
	LoadMatrixServerDll(strPath);
	// DLL创建实例
	Dll_Create_Instance();
	// DLL手动设置AD
//	Dll_ADCSet_ByHand();
	// DLL得到采样数据处理的回调函数
	Dll_GetProSampleData_CallBack();
	// DLL初始化实例
	Dll_Init_Instance();
}


// 关闭
void CMatrixDllCall::OnClose(void)
{
	// DLL关闭实例
	Dll_Close_Instance();
	// DLL释放实例
	Dll_Free_Instance();
	// 释放MatrixServerDll动态链接库
	FreeMatrixServerDll();
}


// Dll_开始AD参数设置
void CMatrixDllCall::Dll_ADCSet(void)
{
	On_ADCSet_All Dll_On_ADCSet = NULL;
	Dll_On_ADCSet = (On_ADCSet_All)GetProcAddress(m_hDllMod, "OnADCSet");
	if (!Dll_On_ADCSet)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_ADCSet)(m_pEnv);
	}
}

// ADC参数设置线程开始工作
void CMatrixDllCall::Dll_ADCSetThreadWork(int iOpt)
{
	On_ADCSetThread_Work Dll_On_ADCSetThread_Work = NULL;
	Dll_On_ADCSetThread_Work = (On_ADCSetThread_Work)GetProcAddress(m_hDllMod, "OnADCSetThreadWork");
	if (!Dll_On_ADCSetThread_Work)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_ADCSetThread_Work)(iOpt, m_pEnv->m_pADCSetThread);
	}
}

// DLL按照路由地址设置部分ADC参数, 路由方向为1上、2下、3左、4右
void CMatrixDllCall::Dll_ADCSetPart(int iLineIndex, int iPointIndex, int iRoutDirection, int iOpt, 
	int iSampleRate, bool bOnly, bool bRout)
{
	int iOperation = 0;
	On_ADCSet_Part Dll_On_ADCSet_Part = NULL;
	Dll_On_ADCSet_Part = (On_ADCSet_Part)GetProcAddress(m_hDllMod, "OnSetADCByLAUXSN");
	if (!Dll_On_ADCSet_Part)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		if (iOpt == 1)
		{
			iOperation = m_pEnv->m_pConstVar->m_iADCSetOptNb;
		}
		else if (iOpt == 2)
		{
			EnterCriticalSection(&m_pEnv->m_pInstrumentCommInfo->m_oSecCommInfo);
// 			// 经过高通滤波器
// 			if (iSampleRate == 250)
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 67;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 86;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 19;
// 			}
// 			else if (iSampleRate == 500)
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 75;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -89;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 9;
// 			}
// 			else if (iSampleRate == 1000)
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 83;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -45;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 4;
// 			}
// 			else if (iSampleRate == 2000)
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 91;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 105;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 2;
// 			}
// 			else if (iSampleRate == 4000)
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 99;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = 52;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 1;
// 			}
// 			// 如果不在所选采样率则按照1000采样率采样
// 			else
// 			{
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[4] = 83;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[6] = -45;
// 				m_pEnv->m_pInstrumentCommInfo->m_cpSetADCSample[7] = 4;
// 			}
			// 不经过高通滤波
			if (iSampleRate == 250)
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 66;
			}
			else if (iSampleRate == 500)
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 74;
			}
			else if (iSampleRate == 1000)
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
			}
			else if (iSampleRate == 2000)
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 90;
			}
			else if (iSampleRate == 4000)
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 98;
			}
			// 如果不在所选采样率则按照1000采样率采样
			else
			{
				m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
			}
			LeaveCriticalSection(&m_pEnv->m_pInstrumentCommInfo->m_oSecCommInfo);
			EnterCriticalSection(&m_pEnv->m_pADCSetThread->m_oSecADCSetThread);
			m_pEnv->m_pADCSetThread->m_bADCStartSample = true;
			m_pEnv->m_pADCSetThread->m_bADCStopSample = false;
			LeaveCriticalSection(&m_pEnv->m_pADCSetThread->m_oSecADCSetThread);
			EnterCriticalSection(&m_pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
			m_pEnv->m_pTimeDelayThread->m_bADCStartSample = true;
			LeaveCriticalSection(&m_pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
			iOperation = m_pEnv->m_pConstVar->m_iADCStartSampleOptNb;
		}
		else if (iOpt == 3)
		{
			EnterCriticalSection(&m_pEnv->m_pADCSetThread->m_oSecADCSetThread);
			m_pEnv->m_pADCSetThread->m_bADCStartSample = false;
			m_pEnv->m_pADCSetThread->m_bADCStopSample = true;
			LeaveCriticalSection(&m_pEnv->m_pADCSetThread->m_oSecADCSetThread);
			EnterCriticalSection(&m_pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
			m_pEnv->m_pTimeDelayThread->m_bADCStartSample = false;
			LeaveCriticalSection(&m_pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
			iOperation = m_pEnv->m_pConstVar->m_iADCStopSampleOptNb;
		}
		else
		{
			return;
		}
		// call the function
		(*Dll_On_ADCSet_Part)(iLineIndex, iPointIndex, iRoutDirection, iOperation, m_pEnv, bOnly, bRout);
		Dll_ADCSetThreadWork(iOperation);
	}
}


// DLL手动设置ADC参数
void CMatrixDllCall::Dll_ADCSet_ByHand(void)
{
	On_ADCSet_ByHand Dll_On_ADCSetByHand = NULL;
	Dll_On_ADCSetByHand = (On_ADCSet_ByHand)GetProcAddress(m_hDllMod, "OnSetADCSetByHand");
	if (!Dll_On_ADCSetByHand)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_ADCSetByHand)(m_pEnv->m_pInstrumentList);
	}
}

// DLL手动打开交叉站某一路由方向的电源，方向为1上、2下、3左、4右
void CMatrixDllCall::Dll_OpenLAUXRoutPower_ByHand(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet)
{
	On_OpenLAUXRoutPower Dll_On_OpenLAUXRoutPower = NULL;
	Dll_On_OpenLAUXRoutPower = (On_OpenLAUXRoutPower)GetProcAddress(m_hDllMod, "OpenLAUXRoutPower");
	if (!Dll_On_OpenLAUXRoutPower)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		if (false == (*Dll_On_OpenLAUXRoutPower)(iLineIndex, iPointIndex, ucLAUXRoutOpenSet, m_pEnv))
		{
			AfxMessageBox(_T("该仪器路由开关没有成功打开！"));
		}
	}
}

// DLL得到路由方向上仪器个数
unsigned int CMatrixDllCall::Dll_GetRoutInstrumentNum(int iLineIndex, int iPointIndex, int iDirection)
{
	On_GetRoutInstrumentNum Dll_On_GetRoutInstrumentNum = NULL;
	unsigned int uiInstrumentNum = 0;
	Dll_On_GetRoutInstrumentNum = (On_GetRoutInstrumentNum)GetProcAddress(m_hDllMod, "OnGetRoutInstrumentNum");
	if (!Dll_On_GetRoutInstrumentNum)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		if (false == (*Dll_On_GetRoutInstrumentNum)(iLineIndex, iPointIndex, iDirection, m_pEnv, uiInstrumentNum))
		{
			AfxMessageBox(_T("该仪器路由开关没有成功打开！"));
		}
	}
	return uiInstrumentNum;
}

// DLL手动发送ADC参数设置帧
void CMatrixDllCall::Dll_OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout, 
	char* cpADCSet, int iADCSetNum)
{
	On_SetADCSetFrameByHand Dll_On_SetADCSetFrameByHand = NULL;
	Dll_On_SetADCSetFrameByHand = (On_SetADCSetFrameByHand)GetProcAddress(m_hDllMod, "OnSetADCSetFrameByHand");
	if (!Dll_On_SetADCSetFrameByHand)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		if (false == (*Dll_On_SetADCSetFrameByHand)(iLineIndex, iPointIndex, iDirection, bRout, cpADCSet, iADCSetNum, m_pEnv))
		{
			AfxMessageBox(_T("该参数未成功发送！"));
		}
	}
}
// DLL得到采样数据处理的回调函数
void CMatrixDllCall::Dll_GetProSampleData_CallBack(void)
{
	Get_ProSampleDateCallBack Dll_On_GetProSampleDataCallBack = NULL;
	Dll_On_GetProSampleDataCallBack = (Get_ProSampleDateCallBack)GetProcAddress(m_hDllMod, "GetProSampleDateCallBack");
	if (!Dll_On_GetProSampleDataCallBack)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_GetProSampleDataCallBack)(m_pEnv->m_pADCDataRecThread, ProSampleDate);
	}
}
// 通过位置得到设备SN
unsigned int CMatrixDllCall::Dll_GetInstrumentSnByLocation(int iLineIndex, int iPointIndex)
{
	unsigned int uiSn = 0;
	Get_InstrumentSnByLocation Dll_Get_InstrumentSnByLocation = NULL;
	Dll_Get_InstrumentSnByLocation = (Get_InstrumentSnByLocation)GetProcAddress(m_hDllMod, "GetInstrumentSnFromLocationMap");
	if (!Dll_Get_InstrumentSnByLocation)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		EnterCriticalSection(&m_pEnv->m_pInstrumentList->m_oSecInstrumentList);
		// call the function
		uiSn = (*Dll_Get_InstrumentSnByLocation)(iLineIndex, iPointIndex, m_pEnv->m_pInstrumentList);
		LeaveCriticalSection(&m_pEnv->m_pInstrumentList->m_oSecInstrumentList);
	}
	return uiSn;
}
