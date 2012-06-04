#include "StdAfx.h"
#include "MatrixDllCall.h"

// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 初始化实例
typedef void (*Init_Instance)(m_oEnvironmentStruct* pEnv);
// Field On
typedef unsigned int (*On_Work)(m_oEnvironmentStruct* pEnv);
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
// 判断仪器位置索引号是否已加入索引表
typedef BOOL (*IfLocation_ExistInMap)(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 增加对象到索引表
typedef void (*Add_LocationToMap)(int iLineIndex, int iPointIndex, m_oInstrumentStruct* pInstrument, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 根据输入索引号，由索引表得到仪器指针
typedef m_oInstrumentStruct* (*Get_InstrumentFromLocationMap)(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 从索引表删除索引号指向的仪器指针
typedef BOOL (*Delete_InstrumentFromLocationMap)(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 载入配置文件
// 加载Survery设置数据
typedef void (*Query_SurverySetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Point Code设置数据
typedef void (*Query_PointCodeSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Sensor设置数据
typedef void (*Query_SensorSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Marker设置数据
typedef void (*Query_MarkerSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Aux设置数据
typedef void (*Query_AuxSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Detour设置数据
typedef void (*Query_DetourSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Mute设置数据
typedef void (*Query_MuteSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载BlastMachine设置数据
typedef void (*Query_BlastMachineSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Absolute设置数据
typedef void (*Query_AbsoluteSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Generic设置数据
typedef void (*Query_GenericSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Look设置数据
typedef void (*Query_LookSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载LAULeakage设置数据
typedef void (*Query_LAULeakageSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载FormLine设置数据
typedef void (*Query_FormLineSetupData)(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Instrument_SensorTestBase设置数据
typedef void (*Load_Instrument_SensorTestBaseSetupData)(bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Instrument_SensorTestLimit设置数据
typedef void (*Load_Instrument_SensorTestLimitSetupData)(bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Instrument Test设置数据
typedef void (*Load_InstrumentTestSetupData)(m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Sensor Test设置数据
typedef void (*Load_SensorTestSetupData)(m_oInstrumentCommInfoStruct* pCommInfo);
// 加载Multiple Test设置数据
typedef void (*Load_MultipleTestSetupData)(m_oInstrumentCommInfoStruct* pCommInfo);
// 加载SeisMonitor设置数据
typedef void (*Load_SeisMonitorSetupData)(m_oInstrumentCommInfoStruct* pCommInfo);
// 写入配置文件
// 设置Survery设置数据
typedef void (*Set_SurverySetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Point Code设置数据
typedef void (*Set_PointCodeSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Sensor设置数据
typedef void (*Set_SensorSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Marker设置数据
typedef void (*Set_MarkerSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Aux设置数据
typedef void (*Set_AuxSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Detour设置数据
typedef void (*Set_DetourSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Mute设置数据
typedef void (*Set_MuteSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置BlastMachine设置数据
typedef void (*Set_BlastMachineSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Absolute设置数据
typedef void (*Set_AbsoluteSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Generic设置数据
typedef void (*Set_GenericSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Look设置数据
typedef void (*Set_LookSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置LAULeakage设置数据
typedef void (*Set_LAULeakageSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置FormLine设置数据
typedef void (*Set_FormLineSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Instrument_SensorTestBase设置数据
typedef void (*Set_Instrument_SensorTestBaseSetupData)(char* pChar, unsigned int uiSize, bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Instrument_SensorTestLimit设置数据
typedef void (*Set_Instrument_SensorTestLimitSetupData)(char* pChar, unsigned int uiSize, bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Instrument Test设置数据
typedef void (*Set_InstrumentTestSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Sensor Test设置数据
typedef void (*Set_SensorTestSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置Multiple Test设置数据
typedef void (*Set_MultipleTestSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 设置SeisMonitor设置数据
typedef void (*Set_SeisMonitorSetupData)(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo);
// 得到测线接收区域
typedef void (*Get_LineRevSection)(unsigned int& uiLineNum, unsigned int& uiColumnNum, m_oInstrumentCommInfoStruct* pCommInfo);
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
void CMatrixDllCall::Dll_StartSample(int iSampleRate, bool bHPFOpen)
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
		m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_iSampleRate = iSampleRate;
		m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_bHPFOpen = bHPFOpen;
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
unsigned int CMatrixDllCall::Dll_Work(void)
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
		return (*Dll_On_Work)(m_pEnv);
	}
	return 0;
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
	int iSampleRate, bool bOnly, bool bRout, bool bHPFOpen)
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
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_iSampleRate = iSampleRate;
			m_pEnv->m_pInstrumentCommInfo->m_oXMLADCSetupData.m_bHPFOpen = bHPFOpen;
			LeaveCriticalSection(&m_pEnv->m_pInstrumentCommInfo->m_oSecCommInfo);
			EnterCriticalSection(&m_pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
			m_pEnv->m_pADCDataRecThread->m_iADCSampleRate = iSampleRate;
			LeaveCriticalSection(&m_pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
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
		EnterCriticalSection(&m_pEnv->m_pLineList->m_oSecLineList);
		// call the function
		(*Dll_On_ADCSetByHand)(m_pEnv->m_pLineList->m_pInstrumentList);
		LeaveCriticalSection(&m_pEnv->m_pLineList->m_oSecLineList);
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
// 判断仪器位置索引号是否已加入索引表
BOOL CMatrixDllCall::Dll_IfLocationExistInMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	BOOL bReturn = FALSE;
	IfLocation_ExistInMap Dll_IfLocation_ExistInMap = NULL;
	Dll_IfLocation_ExistInMap = (IfLocation_ExistInMap)GetProcAddress(m_hDllMod, "IfLocationExistInMap");
	if (!Dll_IfLocation_ExistInMap)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		bReturn = (*Dll_IfLocation_ExistInMap)(iLineIndex, iPointIndex, pMap);
	}
	return bReturn;
}
// 增加对象到索引表
void CMatrixDllCall::Dll_AddLocationToMap(int iLineIndex, int iPointIndex, m_oInstrumentStruct* pInstrument, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	Add_LocationToMap Dll_Add_LocationToMap = NULL;
	Dll_Add_LocationToMap = (Add_LocationToMap)GetProcAddress(m_hDllMod, "AddLocationToMap");
	if (!Dll_Add_LocationToMap)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Add_LocationToMap)(iLineIndex, iPointIndex, pInstrument, pMap);
	}
}
// 根据输入索引号，由索引表得到仪器指针
m_oInstrumentStruct* CMatrixDllCall::Dll_GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	m_oInstrumentStruct* pInstrument = NULL;
	Get_InstrumentFromLocationMap Dll_Get_InstrumentFromLocationMap = NULL;
	Dll_Get_InstrumentFromLocationMap = (Get_InstrumentFromLocationMap)GetProcAddress(m_hDllMod, "GetInstrumentFromLocationMap");
	if (!Dll_Get_InstrumentFromLocationMap)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		pInstrument = (*Dll_Get_InstrumentFromLocationMap)(iLineIndex, iPointIndex, pMap);
	}
	return pInstrument;
}
// 从索引表删除索引号指向的仪器指针
BOOL CMatrixDllCall::Dll_DeleteInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	BOOL bReturn = FALSE;
	Delete_InstrumentFromLocationMap Dll_Delete_InstrumentFromLocationMap = NULL;
	Dll_Delete_InstrumentFromLocationMap = (Delete_InstrumentFromLocationMap)GetProcAddress(m_hDllMod, "DeleteInstrumentFromLocationMap");
	if (!Dll_Delete_InstrumentFromLocationMap)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		bReturn = (*Dll_Delete_InstrumentFromLocationMap)(iLineIndex, iPointIndex, pMap);
	}
	return bReturn;
}
// 加载Survery设置数据
void CMatrixDllCall::Dll_QuerySurverySetupData(char* cProcBuf, int& iPos)
{
	Query_SurverySetupData Dll_Query_SurverySetupData = NULL;
	Dll_Query_SurverySetupData = (Query_SurverySetupData)GetProcAddress(m_hDllMod, "QuerySurverySetupData");
	if (!Dll_Query_SurverySetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_SurverySetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Point Code设置数据
void CMatrixDllCall::Dll_QueryPointCodeSetupData(char* cProcBuf, int& iPos)
{
	Query_PointCodeSetupData Dll_Query_PointCodeSetupData = NULL;
	Dll_Query_PointCodeSetupData = (Query_PointCodeSetupData)GetProcAddress(m_hDllMod, "QueryPointCodeSetupData");
	if (!Dll_Query_PointCodeSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_PointCodeSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Sensor设置数据
void CMatrixDllCall::Dll_QuerySensorSetupData(char* cProcBuf, int& iPos)
{
	Query_SensorSetupData Dll_Query_SensorSetupData = NULL;
	Dll_Query_SensorSetupData = (Query_SensorSetupData)GetProcAddress(m_hDllMod, "QuerySensorSetupData");
	if (!Dll_Query_SensorSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_SensorSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Marker设置数据
void CMatrixDllCall::Dll_QueryMarkerSetupData(char* cProcBuf, int& iPos)
{
	Query_MarkerSetupData Dll_Query_MarkerSetupData = NULL;
	Dll_Query_MarkerSetupData = (Query_MarkerSetupData)GetProcAddress(m_hDllMod, "QueryMarkerSetupData");
	if (!Dll_Query_MarkerSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_MarkerSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Aux设置数据
void CMatrixDllCall::Dll_QueryAuxSetupData(char* cProcBuf, int& iPos)
{
	Query_AuxSetupData Dll_Query_AuxSetupData = NULL;
	Dll_Query_AuxSetupData = (Query_AuxSetupData)GetProcAddress(m_hDllMod, "QueryAuxSetupData");
	if (!Dll_Query_AuxSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_AuxSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Detour设置数据
void CMatrixDllCall::Dll_QueryDetourSetupData(char* cProcBuf, int& iPos)
{
	Query_DetourSetupData Dll_Query_DetourSetupData = NULL;
	Dll_Query_DetourSetupData = (Query_DetourSetupData)GetProcAddress(m_hDllMod, "QueryDetourSetupData");
	if (!Dll_Query_DetourSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_DetourSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Mute设置数据
void CMatrixDllCall::Dll_QueryMuteSetupData(char* cProcBuf, int& iPos)
{
	Query_MuteSetupData Dll_Query_MuteSetupData = NULL;
	Dll_Query_MuteSetupData = (Query_MuteSetupData)GetProcAddress(m_hDllMod, "QueryMuteSetupData");
	if (!Dll_Query_MuteSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_MuteSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载BlastMachine设置数据
void CMatrixDllCall::Dll_QueryBlastMachineSetupData(char* cProcBuf, int& iPos)
{
	Query_BlastMachineSetupData Dll_Query_BlastMachineSetupData = NULL;
	Dll_Query_BlastMachineSetupData = (Query_BlastMachineSetupData)GetProcAddress(m_hDllMod, "QueryBlastMachineSetupData");
	if (!Dll_Query_BlastMachineSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_BlastMachineSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Absolute设置数据
void CMatrixDllCall::Dll_QueryAbsoluteSetupData(char* cProcBuf, int& iPos)
{
	Query_AbsoluteSetupData Dll_Query_AbsoluteSetupData = NULL;
	Dll_Query_AbsoluteSetupData = (Query_AbsoluteSetupData)GetProcAddress(m_hDllMod, "QueryAbsoluteSetupData");
	if (!Dll_Query_AbsoluteSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_AbsoluteSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Generic设置数据
void CMatrixDllCall::Dll_QueryGenericSetupData(char* cProcBuf, int& iPos)
{
	Query_GenericSetupData Dll_Query_GenericSetupData = NULL;
	Dll_Query_GenericSetupData = (Query_GenericSetupData)GetProcAddress(m_hDllMod, "QueryGenericSetupData");
	if (!Dll_Query_GenericSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_GenericSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Look设置数据
void CMatrixDllCall::Dll_QueryLookSetupData(char* cProcBuf, int& iPos)
{
	Query_LookSetupData Dll_Query_LookSetupData = NULL;
	Dll_Query_LookSetupData = (Query_LookSetupData)GetProcAddress(m_hDllMod, "QueryLookSetupData");
	if (!Dll_Query_LookSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_LookSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载LAULeakage设置数据
void CMatrixDllCall::Dll_QueryLAULeakageSetupData(char* cProcBuf, int& iPos)
{
	Query_LAULeakageSetupData Dll_Query_LAULeakageSetupData = NULL;
	Dll_Query_LAULeakageSetupData = (Query_LAULeakageSetupData)GetProcAddress(m_hDllMod, "QueryLAULeakageSetupData");
	if (!Dll_Query_LAULeakageSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_LAULeakageSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载FormLine设置数据
void CMatrixDllCall::Dll_QueryFormLineSetupData(char* cProcBuf, int& iPos)
{
	Query_FormLineSetupData Dll_Query_FormLineSetupData = NULL;
	Dll_Query_FormLineSetupData = (Query_FormLineSetupData)GetProcAddress(m_hDllMod, "QueryFormLineSetupData");
	if (!Dll_Query_FormLineSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_FormLineSetupData)(cProcBuf, iPos, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Instrument_SensorTestBase设置数据
void CMatrixDllCall::Dll_LoadInstrument_SensorTestBaseSetupData(bool bInstrument)
{
	Load_Instrument_SensorTestBaseSetupData Dll_Load_Instrument_SensorTestBaseSetupData = NULL;
	Dll_Load_Instrument_SensorTestBaseSetupData = 
		(Load_Instrument_SensorTestBaseSetupData)GetProcAddress(m_hDllMod, "LoadInstrument_SensorTestBaseSetupData");
	if (!Dll_Load_Instrument_SensorTestBaseSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_Instrument_SensorTestBaseSetupData)(bInstrument, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Instrument_SensorTestLimit设置数据
void CMatrixDllCall::Dll_LoadInstrument_SensorTestLimitSetupData(bool bInstrument)
{
	Load_Instrument_SensorTestLimitSetupData Dll_Load_Instrument_SensorTestLimitSetupData = NULL;
	Dll_Load_Instrument_SensorTestLimitSetupData = 
		(Load_Instrument_SensorTestLimitSetupData)GetProcAddress(m_hDllMod, "LoadInstrument_SensorTestLimitSetupData");
	if (!Dll_Load_Instrument_SensorTestLimitSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_Instrument_SensorTestLimitSetupData)(bInstrument, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Instrument Test设置数据
void CMatrixDllCall::Dll_LoadInstrumentTestSetupData(void)
{
	Load_InstrumentTestSetupData Dll_Load_InstrumentTestSetupData = NULL;
	Dll_Load_InstrumentTestSetupData = (Load_InstrumentTestSetupData)GetProcAddress(m_hDllMod, "LoadInstrumentTestSetupData");
	if (!Dll_Load_InstrumentTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_InstrumentTestSetupData)(m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Sensor Test设置数据
void CMatrixDllCall::Dll_LoadSensorTestSetupData(void)
{
	Load_SensorTestSetupData Dll_Load_SensorTestSetupData = NULL;
	Dll_Load_SensorTestSetupData = (Load_SensorTestSetupData)GetProcAddress(m_hDllMod, "LoadSensorTestSetupData");
	if (!Dll_Load_SensorTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_SensorTestSetupData)(m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载Multiple Test设置数据
void CMatrixDllCall::Dll_LoadMultipleTestSetupData(void)
{
	Load_MultipleTestSetupData Dll_Load_MultipleTestSetupData = NULL;
	Dll_Load_MultipleTestSetupData = (Load_MultipleTestSetupData)GetProcAddress(m_hDllMod, "LoadMultipleTestSetupData");
	if (!Dll_Load_MultipleTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_MultipleTestSetupData)(m_pEnv->m_pInstrumentCommInfo);
	}
}
// 加载SeisMonitor设置数据
void CMatrixDllCall::Dll_LoadSeisMonitorSetupData(void)
{
	Load_SeisMonitorSetupData Dll_Load_SeisMonitorSetupData = NULL;
	Dll_Load_SeisMonitorSetupData = (Load_SeisMonitorSetupData)GetProcAddress(m_hDllMod, "LoadSeisMonitorSetupData");
	if (!Dll_Load_SeisMonitorSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Load_SeisMonitorSetupData)(m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Survery设置数据
void CMatrixDllCall::Dll_SetSurverySetupData(char* pChar, unsigned int uiSize)
{
	Set_SurverySetupData Dll_Set_SurverySetupData = NULL;
	Dll_Set_SurverySetupData = (Set_SurverySetupData)GetProcAddress(m_hDllMod, "SetSurverySetupData");
	if (!Dll_Set_SurverySetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_SurverySetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Point Code设置数据
void CMatrixDllCall::Dll_SetPointCodeSetupData(char* pChar, unsigned int uiSize)
{
	Set_PointCodeSetupData Dll_Set_PointCodeSetupData = NULL;
	Dll_Set_PointCodeSetupData = (Set_PointCodeSetupData)GetProcAddress(m_hDllMod, "SetPointCodeSetupData");
	if (!Dll_Set_PointCodeSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_PointCodeSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Sensor设置数据
void CMatrixDllCall::Dll_SetSensorSetupData(char* pChar, unsigned int uiSize)
{
	Set_SensorSetupData Dll_Set_SensorSetupData = NULL;
	Dll_Set_SensorSetupData = (Set_SensorSetupData)GetProcAddress(m_hDllMod, "SetSensorSetupData");
	if (!Dll_Set_SensorSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_SensorSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Marker设置数据
void CMatrixDllCall::Dll_SetMarkerSetupData(char* pChar, unsigned int uiSize)
{
	Set_MarkerSetupData Dll_Set_MarkerSetupData = NULL;
	Dll_Set_MarkerSetupData = (Set_MarkerSetupData)GetProcAddress(m_hDllMod, "SetMarkerSetupData");
	if (!Dll_Set_MarkerSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_MarkerSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Aux设置数据
void CMatrixDllCall::Dll_SetAuxSetupData(char* pChar, unsigned int uiSize)
{
	Set_AuxSetupData Dll_Set_AuxSetupData = NULL;
	Dll_Set_AuxSetupData = (Set_AuxSetupData)GetProcAddress(m_hDllMod, "SetAuxSetupData");
	if (!Dll_Set_AuxSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_AuxSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Detour设置数据
void CMatrixDllCall::Dll_SetDetourSetupData(char* pChar, unsigned int uiSize)
{
	Set_DetourSetupData Dll_Set_DetourSetupData = NULL;
	Dll_Set_DetourSetupData = (Set_DetourSetupData)GetProcAddress(m_hDllMod, "SetDetourSetupData");
	if (!Dll_Set_DetourSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_DetourSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Mute设置数据
void CMatrixDllCall::Dll_SetMuteSetupData(char* pChar, unsigned int uiSize)
{
	Set_MuteSetupData Dll_Set_MuteSetupData = NULL;
	Dll_Set_MuteSetupData = (Set_MuteSetupData)GetProcAddress(m_hDllMod, "SetMuteSetupData");
	if (!Dll_Set_MuteSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_MuteSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置BlastMachine设置数据
void CMatrixDllCall::Dll_SetBlastMachineSetupData(char* pChar, unsigned int uiSize)
{
	Set_BlastMachineSetupData Dll_Set_BlastMachineSetupData = NULL;
	Dll_Set_BlastMachineSetupData = (Set_BlastMachineSetupData)GetProcAddress(m_hDllMod, "SetBlastMachineSetupData");
	if (!Dll_Set_BlastMachineSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_BlastMachineSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Absolute设置数据
void CMatrixDllCall::Dll_SetAbsoluteSetupData(char* pChar, unsigned int uiSize)
{
	Set_AbsoluteSetupData Dll_Set_AbsoluteSetupData = NULL;
	Dll_Set_AbsoluteSetupData = (Set_AbsoluteSetupData)GetProcAddress(m_hDllMod, "SetAbsoluteSetupData");
	if (!Dll_Set_AbsoluteSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_AbsoluteSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Generic设置数据
void CMatrixDllCall::Dll_SetGenericSetupData(char* pChar, unsigned int uiSize)
{
	Set_GenericSetupData Dll_Set_GenericSetupData = NULL;
	Dll_Set_GenericSetupData = (Set_GenericSetupData)GetProcAddress(m_hDllMod, "SetGenericSetupData");
	if (!Dll_Set_GenericSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_GenericSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Look设置数据
void CMatrixDllCall::Dll_SetLookSetupData(char* pChar, unsigned int uiSize)
{
	Set_LookSetupData Dll_Set_LookSetupData = NULL;
	Dll_Set_LookSetupData = (Set_LookSetupData)GetProcAddress(m_hDllMod, "SetLookSetupData");
	if (!Dll_Set_LookSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_LookSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置LAULeakage设置数据
void CMatrixDllCall::Dll_SetLAULeakageSetupData(char* pChar, unsigned int uiSize)
{
	Set_LAULeakageSetupData Dll_Set_LAULeakageSetupData = NULL;
	Dll_Set_LAULeakageSetupData = (Set_LAULeakageSetupData)GetProcAddress(m_hDllMod, "SetLAULeakageSetupData");
	if (!Dll_Set_LAULeakageSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_LAULeakageSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置FormLine设置数据
void CMatrixDllCall::Dll_SetFormLineSetupData(char* pChar, unsigned int uiSize)
{
	Set_FormLineSetupData Dll_Set_FormLineSetupData = NULL;
	Dll_Set_FormLineSetupData = (Set_FormLineSetupData)GetProcAddress(m_hDllMod, "SetFormLineSetupData");
	if (!Dll_Set_FormLineSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_FormLineSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Instrument_SensorTestBase设置数据
void CMatrixDllCall::Dll_SetInstrument_SensorTestBaseSetupData(char* pChar, unsigned int uiSize, bool bInstrument)
{
	Set_Instrument_SensorTestBaseSetupData Dll_Set_Instrument_SensorTestBaseSetupData = NULL;
	Dll_Set_Instrument_SensorTestBaseSetupData = (Set_Instrument_SensorTestBaseSetupData)GetProcAddress(m_hDllMod, "SetInstrument_SensorTestBaseSetupData");
	if (!Dll_Set_Instrument_SensorTestBaseSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_Instrument_SensorTestBaseSetupData)(pChar, uiSize, bInstrument, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Instrument_SensorTestLimit设置数据
void CMatrixDllCall::Dll_SetInstrument_SensorTestLimitSetupData(char* pChar, unsigned int uiSize, bool bInstrument)
{
	Set_Instrument_SensorTestLimitSetupData Dll_Set_Instrument_SensorTestLimitSetupData = NULL;
	Dll_Set_Instrument_SensorTestLimitSetupData = (Set_Instrument_SensorTestLimitSetupData)GetProcAddress(m_hDllMod, "SetInstrument_SensorTestLimitSetupData");
	if (!Dll_Set_Instrument_SensorTestLimitSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_Instrument_SensorTestLimitSetupData)(pChar, uiSize, bInstrument, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Instrument Test设置数据
void CMatrixDllCall::Dll_SetInstrumentTestSetupData(char* pChar, unsigned int uiSize)
{
	Set_InstrumentTestSetupData Dll_Set_InstrumentTestSetupData = NULL;
	Dll_Set_InstrumentTestSetupData = (Set_InstrumentTestSetupData)GetProcAddress(m_hDllMod, "SetInstrumentTestSetupData");
	if (!Dll_Set_InstrumentTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_InstrumentTestSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Sensor Test设置数据
void CMatrixDllCall::Dll_SetSensorTestSetupData(char* pChar, unsigned int uiSize)
{
	Set_SensorTestSetupData Dll_Set_SensorTestSetupData = NULL;
	Dll_Set_SensorTestSetupData = (Set_SensorTestSetupData)GetProcAddress(m_hDllMod, "SetSensorTestSetupData");
	if (!Dll_Set_SensorTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_SensorTestSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置Multiple Test设置数据
void CMatrixDllCall::Dll_SetMultipleTestSetupData(char* pChar, unsigned int uiSize)
{
	Set_MultipleTestSetupData Dll_Set_MultipleTestSetupData = NULL;
	Dll_Set_MultipleTestSetupData = (Set_MultipleTestSetupData)GetProcAddress(m_hDllMod, "SetMultipleTestSetupData");
	if (!Dll_Set_MultipleTestSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_MultipleTestSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 设置SeisMonitor设置数据
void CMatrixDllCall::Dll_SetSeisMonitorSetupData(char* pChar, unsigned int uiSize)
{
	Set_SeisMonitorSetupData Dll_Set_SeisMonitorSetupData = NULL;
	Dll_Set_SeisMonitorSetupData = (Set_SeisMonitorSetupData)GetProcAddress(m_hDllMod, "SetSeisMonitorSetupData");
	if (!Dll_Set_SeisMonitorSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_SeisMonitorSetupData)(pChar, uiSize, m_pEnv->m_pInstrumentCommInfo);
	}
}
// 得到测线接收区域
void CMatrixDllCall::Dll_GetLineRevSection(unsigned int& uiLineNum, unsigned int& uiColumnNum)
{
	Get_LineRevSection Dll_Get_LineRevSection = NULL;
	Dll_Get_LineRevSection = (Get_LineRevSection)GetProcAddress(m_hDllMod, "GetLineRevSection");
	if (!Dll_Get_LineRevSection)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Get_LineRevSection)(uiLineNum, uiColumnNum, m_pEnv->m_pInstrumentCommInfo);
	}
}
