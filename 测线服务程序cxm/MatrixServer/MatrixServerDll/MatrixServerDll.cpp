// MatrixServerDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "MatrixServerDll.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
// 产生一个施工任务
void GenOneOptTask(unsigned int uiIndex, unsigned int uiStartFrame, 
	m_oOptTaskArrayStruct* pOptTaskArray, m_oLineListStruct* pLineList,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pOptTaskArray == NULL) || (pLineList == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GenOneOptTask", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oOptTaskStruct* pOptTask = NULL;
	CString str = _T("");
	CString str2 = _T("");
	unsigned int uiLineNb = 0;
	hash_map <unsigned int, m_oInstrumentStruct*>::iterator iter;
	list<m_oOptInstrumentStruct>::iterator list_iter;
	list<m_oOptInstrumentStruct> oOptInstrumentList;
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	pOptTaskArray->m_uiADCDataFolderNb++;
	// 得到一个空闲施工任务
	pOptTask = GetFreeOptTask(pOptTaskArray);
	// 加入施工任务索引
	AddOptTaskToMap(uiIndex, pOptTask, &pOptTaskArray->m_oOptTaskWorkMap);
	// 得到施工任务，如果已经有该任务则重置其仪器位置序号索引
	pOptTask = GetOptTaskFromMap(uiIndex, &pOptTaskArray->m_oOptTaskWorkMap);
	// 记录施工任务开始时ADC数据采样帧数
	pOptTask->m_uiStartFrame = uiStartFrame;
	str.Format(_T("\\施工任务%d"), pOptTaskArray->m_uiADCDataFolderNb);
	str2 = pOptTaskArray->m_SaveLogFolderPath.c_str();
	str2 += str;
	// 创建施工任务数据文件夹
	CreateDirectory(str2, NULL);
	// 创建施工任务数据文件夹
	pOptTask->m_SaveLogFilePath = (CStringA)str2;
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	
	// @@@@参与施工任务的采集站排序后加入施工任务的仪器索引表
	oOptInstrumentList.clear();
	EnterCriticalSection(&pLineList->m_oSecLineList);
	for (iter = pLineList->m_pInstrumentList->m_oIPInstrumentMap.begin(); 
		iter != pLineList->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		if (iter->second->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
		{
			m_oOptInstrumentStruct oOptInstrument;
			oOptInstrument.m_uiSN = iter->second->m_uiSN;
			oOptInstrument.m_iLineIndex = iter->second->m_iLineIndex;
			oOptInstrument.m_iPointIndex = iter->second->m_iPointIndex;
			oOptInstrumentList.push_back(oOptInstrument);
		}
	}
	LeaveCriticalSection(&pLineList->m_oSecLineList);
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	for (list_iter = oOptInstrumentList.begin();
		list_iter != oOptInstrumentList.end();
		list_iter++)
	{
		pOptTask->m_olsOptInstrument.push_back(*list_iter);
	}
	pOptTask->m_olsOptInstrument.sort();
	for (list_iter = pOptTask->m_olsOptInstrument.begin();
		list_iter != pOptTask->m_olsOptInstrument.end();
		list_iter++)
	{
		AddToOptTaskSNMap(list_iter->m_uiSN, uiLineNb, &pOptTask->m_oSNMap);
		uiLineNb++;
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	oOptInstrumentList.clear();
}
// 释放一个施工任务
void FreeOneOptTask(unsigned int uiIndex, m_oOptTaskArrayStruct* pOptTaskArray)
{
	if (pOptTaskArray == NULL)
	{
		return;
	}
	m_oOptTaskStruct* pOptTask = NULL;
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	// 得到施工任务
	pOptTask = GetOptTaskFromMap(uiIndex, &pOptTaskArray->m_oOptTaskWorkMap);
	if (pOptTask->m_pPreviousFile != NULL)
	{
		fclose(pOptTask->m_pPreviousFile);
	}
	if (pOptTask->m_pFile != NULL)
	{
		fclose(pOptTask->m_pFile);
	}
	// 将施工任务加入到空闲列表
	AddFreeOptTask(pOptTask, pOptTaskArray);
	// 从施工任务索引表中删除
	DeleteOptTaskFromMap(uiIndex, &pOptTaskArray->m_oOptTaskWorkMap);
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
// 按SN重置ADC参数设置标志位
void OnResetADCSetLableBySN(m_oInstrumentStruct* pInstrument, int iOpt, m_oConstVarStruct* pConstVar)
{
	if ((pInstrument == NULL) || (pConstVar == NULL))
	{
		return;
	}
	if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
	{
		if (iOpt == pConstVar->m_iADCSetOptNb)
		{
			pInstrument->m_bADCSet = false;
		}
		else if (iOpt == pConstVar->m_iADCStartSampleOptNb)
		{
			pInstrument->m_bADCStartSample = false;
		}
		else if (iOpt == pConstVar->m_iADCStopSampleOptNb)
		{
			pInstrument->m_bADCStopSample = false;
		}
	}
}
// 按路由重置ADC参数设置标志位
void OnResetADCSetLableByRout(m_oRoutStruct* pRout, int iOpt, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnResetADCSetLableByRout", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRout->m_pTail == NULL)
	{
		return;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrument = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
		{
			if (iOpt == pConstVar->m_iADCSetOptNb)
			{
				pInstrument->m_bADCSet = false;
			}
			else if (iOpt == pConstVar->m_iADCStartSampleOptNb)
			{
				pInstrument->m_bADCStartSample = false;
			}
			else if (iOpt == pConstVar->m_iADCStopSampleOptNb)
			{
				pInstrument->m_bADCStopSample = false;
			}
		}
	} while (pInstrument != pRout->m_pTail);

}
// 重置ADC参数设置标志位
void OnResetADCSetLable(m_oLineListStruct* pLineList, int iOpt, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pLineList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnResetADCSetLable", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	EnterCriticalSection(&pLineList->m_oSecLineList);
	for (iter = pLineList->m_pRoutList->m_oRoutMap.begin(); iter != pLineList->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		// 将路由索引表中的大线方向路由加入到ADC参数设置任务索引
		if (iter->second->m_bRoutLaux == false)
		{
			OnResetADCSetLableByRout(iter->second, iOpt, pConstVar);
		}
	}
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 按照路由地址重置ADC参数设置标志位
void OnSetADCByLAUXSN(int iLineIndex, int iPointIndex, int iDirection, int iOpt, 
	m_oEnvironmentStruct* pEnv, bool bOnly, bool bRout)
{
	if (pEnv == NULL)
	{
		return;
	}
	m_oRoutStruct* pRout = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	unsigned int uiRoutIP = 0;
	bool bStartSample = false;
	bool bStopSample = false;
	if (bOnly == true)
	{
		OnClearADCSetMap(pEnv->m_pLineList);
	}

	EnterCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	bStartSample = pEnv->m_pADCSetThread->m_bADCStartSample;
	bStopSample = pEnv->m_pADCSetThread->m_bADCStopSample;
	LeaveCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	pInstrument = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, 
		&pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
	if (pInstrument == NULL)
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return;
	}
	// 按照路由设置
	if (bRout == true)
	{
		if (false == GetRoutIPBySn(pInstrument->m_uiSN, iDirection, pEnv->m_pLineList->m_pInstrumentList, 
			pEnv->m_pConstVar, uiRoutIP))
		{
			LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
			return;
		}
		if (false == GetRoutByRoutIP(uiRoutIP, pEnv->m_pLineList->m_pRoutList, &pRout))
		{
			LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
			return;
		}
		OnResetADCSetLableByRout(pRout, iOpt, pEnv->m_pConstVar);
		GetADCTaskQueueByRout(bStartSample, bStopSample, pEnv->m_pLineList, pEnv->m_pConstVar, pRout, iOpt);
	}
	// 按照SN设置单个仪器的ADC
	else
	{
		OnResetADCSetLableBySN(pInstrument, iOpt, pEnv->m_pConstVar);
		GetADCTaskQueueBySN(bStartSample, bStopSample, pEnv->m_pLineList, pEnv->m_pConstVar, pInstrument, iOpt);
	}
	LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
}
// ADC参数设置
void OnADCSet(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 重置ADC参数设置成功的标志位
	OnResetADCSetLable(pEnv->m_pLineList, pEnv->m_pConstVar->m_iADCSetOptNb, pEnv->m_pConstVar);
}
// ADC开始采集命令
void OnADCStartSample(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	int iSampleRate = 0;
	EnterCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	pEnv->m_pADCSetThread->m_bADCStartSample = true;
	pEnv->m_pADCSetThread->m_bADCStopSample = false;
	LeaveCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	// 重置ADC开始采集命令成功的标志位
	OnResetADCSetLable(pEnv->m_pLineList, pEnv->m_pConstVar->m_iADCStartSampleOptNb, pEnv->m_pConstVar);
	EnterCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	pEnv->m_pTimeDelayThread->m_bADCStartSample = true;
	LeaveCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	EnterCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	iSampleRate = pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate;
	LeaveCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	EnterCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	pEnv->m_pADCDataRecThread->m_uiADCDataFrameSysTime = 0;
	pEnv->m_pADCDataRecThread->m_iADCFrameCount = 0;
	pEnv->m_pADCDataRecThread->m_iADCSampleRate = iSampleRate;
	LeaveCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	EnterCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	// 清空丢帧索引
	pEnv->m_pLineList->m_pInstrumentList->m_oADCLostFrameMap.clear();
	LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	// @@@@调试用
	// 产生一个施工任务
	GenOneOptTask(1, 0, pEnv->m_pOptTaskArray, pEnv->m_pLineList, pEnv->m_pConstVar);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnADCStartSample", "开始ADC数据采集");
}
// ADC停止采集命令
void OnADCStopSample(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// @@@@调试用
	// 释放一个施工任务
	FreeOneOptTask(1, pEnv->m_pOptTaskArray);
	EnterCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	pEnv->m_pADCSetThread->m_bADCStartSample = false;
	pEnv->m_pADCSetThread->m_bADCStopSample = true;
	LeaveCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	// 重置ADC停止采集命令成功的标志位
	OnResetADCSetLable(pEnv->m_pLineList, pEnv->m_pConstVar->m_iADCStopSampleOptNb, pEnv->m_pConstVar);
	EnterCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	pEnv->m_pTimeDelayThread->m_bADCStartSample = false;
	pEnv->m_pTimeDelayThread->m_uiCounter = 0;
	// 时统设置线程开始工作
	pEnv->m_pTimeDelayThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	EnterCriticalSection(&pEnv->m_pTailTimeFrame->m_oSecTailTimeFrame);
	// 清空尾包时刻查询帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pTailTimeFrame->m_oSecTailTimeFrame);
	EnterCriticalSection(&pEnv->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 清空时统设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnADCStopSample", "停止ADC数据采集");
}

// 输出接收和发送帧的统计结果
void OnOutPutResult(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	CString str = _T("");
	CString strOutError = _T("");
	string strConv = "";
	m_oInstrumentStruct* pInstrument = NULL;
	// 尾包查询帧查询的仪器计数
	int iTailTimeQueryNum = 0;
	// 尾包查询帧应答计数
	int iTailTimeReturnNum = 0;
	// 时统设置帧计数
	int iTimeDelaySetNum = 0;
	// 时统设置应答帧计数
	int iTimeDelayReturnNum = 0;

	// 网络数据错误计数
	int iErrorCodeDataNum = 0;
	// 命令错误计数
	int iErrorCodeCmdNum = 0;
	// 发送误码查询帧数
	int iErrorCodeQueryNum = 0;
	// 接收到误码查询帧数
	int iErrorCodeReturnNum = 0;
	EnterCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	for (iter = pEnv->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap.begin();
		iter != pEnv->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		pInstrument = iter->second;
		// 尾包时刻查询和时统
		str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，发送尾包时刻查询帧次数 %d，应答次数 %d，发送时统次数 %d，应答次数%d"),
			pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_iTailTimeQueryCount, 
			pInstrument->m_iTailTimeReturnCount, pInstrument->m_iTimeSetCount, 
			pInstrument->m_iTimeSetReturnCount);
		// 尾包时刻查询和时统
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pEnv->m_pLogOutPutTimeDelay, "", strConv);
		iTailTimeQueryNum += pInstrument->m_iTailTimeQueryCount;
		iTailTimeReturnNum += pInstrument->m_iTailTimeReturnCount;
		iTimeDelaySetNum += pInstrument->m_iTimeSetCount;
		iTimeDelayReturnNum += pInstrument->m_iTimeSetReturnCount;

		// 误码查询帧数
		str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，发送误码查询帧数 %d，应答次数 %d；"),
			pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_uiErrorCodeQueryNum, 
			pInstrument->m_uiErrorCodeReturnNum);
		strOutError = str;
		if ((pInstrument->m_iInstrumentType == pEnv->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pEnv->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			str.Format(_T("大线A数据误码 = %d，大线B数据误码 = %d，交叉线A数据误码 = %d，交叉线B数据误码 = %d，命令误码 = %d"), 
				pInstrument->m_iLAUXErrorCodeDataLineACount, pInstrument->m_iLAUXErrorCodeDataLineBCount, 
				pInstrument->m_iLAUXErrorCodeDataLAUXLineACount, pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount,
				pInstrument->m_iLAUXErrorCodeCmdCount);
			strOutError += str;
		}
		else
		{
			str.Format(_T("数据误码 = %d，命令误码 = %d"), pInstrument->m_iFDUErrorCodeDataCount, 
				pInstrument->m_iFDUErrorCodeCmdCount);
			strOutError += str;
		}
		// 误码查询
		strConv = (CStringA)strOutError;
		AddMsgToLogOutPutList(pEnv->m_pLogOutPutErrorCode, "", strConv);
		iErrorCodeQueryNum += pInstrument->m_uiErrorCodeQueryNum;
		iErrorCodeReturnNum += pInstrument->m_uiErrorCodeReturnNum;
		// 网络数据错误计数
		iErrorCodeDataNum += pInstrument->m_iFDUErrorCodeDataCount 
			+ pInstrument->m_iLAUXErrorCodeDataLineACount
			+ pInstrument->m_iLAUXErrorCodeDataLineBCount 
			+ pInstrument->m_iLAUXErrorCodeDataLAUXLineACount
			+ pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount;
		// 命令错误计数
		iErrorCodeCmdNum += pInstrument->m_iFDUErrorCodeCmdCount + pInstrument->m_iLAUXErrorCodeCmdCount;
	}
	LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	// 尾包时刻
	str.Format(_T("尾包时刻查询仪器的总数%d， 应答帧总数%d"), iTailTimeQueryNum, iTailTimeReturnNum);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", strConv);
	// 时统
	str.Format(_T("时统设置仪器的总数%d， 应答帧总数%d"), iTimeDelaySetNum, iTimeDelayReturnNum);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", strConv);
	// 误码查询
	str.Format(_T("误码查询仪器的总数%d， 应答帧总数%d"), iErrorCodeQueryNum, iErrorCodeReturnNum);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", strConv);
	// 误码查询结果统计
	str.Format(_T("数据误码总数%d， 命令误码总数%d"), iErrorCodeDataNum, iErrorCodeCmdNum);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", strConv);
}
// 初始化套接字库
void OnInitSocketLib(void)
{
	WSADATA wsaData;
	CString str = _T("");
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		str.Format(_T("WSAStartup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
}
// 释放套接字库
void OnCloseSocketLib(void)
{
	CString str = _T("");
	// 释放套接字库
	if (WSACleanup() != 0)
	{
		str.Format(_T("WSACleanup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
}

// 创建实例，并返回实例指针
m_oEnvironmentStruct* OnCreateInstance(void)
{
	m_oEnvironmentStruct* pEnv = new m_oEnvironmentStruct;
	pEnv->m_pConstVar = NULL;
	pEnv->m_pInstrumentCommInfo = NULL;
	pEnv->m_pHeartBeatFrame = NULL;
	pEnv->m_pHeadFrame = NULL;
	pEnv->m_pIPSetFrame = NULL;
	pEnv->m_pTailFrame = NULL;
	pEnv->m_pTailTimeFrame = NULL;
	pEnv->m_pTimeDelayFrame = NULL;
	pEnv->m_pADCSetFrame = NULL;
	pEnv->m_pErrorCodeFrame = NULL;
	pEnv->m_pADCDataFrame = NULL;
	pEnv->m_pLogOutPutOpt = NULL;
	pEnv->m_pLogOutPutTimeDelay = NULL;
	pEnv->m_pLogOutPutErrorCode = NULL;
	pEnv->m_pLogOutPutADCFrameTime = NULL;
	pEnv->m_pLineList = NULL;
	pEnv->m_pADCDataBufArray = NULL;
	pEnv->m_pOptTaskArray = NULL;
	pEnv->m_pLogOutPutThread = NULL;
	pEnv->m_pHeartBeatThread = NULL;
	pEnv->m_pHeadFrameThread = NULL;
	pEnv->m_pIPSetFrameThread = NULL;
	pEnv->m_pTailFrameThread = NULL;
	pEnv->m_pMonitorThread = NULL;
	pEnv->m_pTimeDelayThread = NULL;
	pEnv->m_pADCSetThread = NULL;
	pEnv->m_pErrorCodeThread = NULL;
	pEnv->m_pADCDataRecThread = NULL;
	pEnv->m_pADCDataSaveThread = NULL;
	// 创建操作日志输出结构体
	pEnv->m_pLogOutPutOpt = OnCreateLogOutPut();
	// 创建时统日志输出结构体
	pEnv->m_pLogOutPutTimeDelay = OnCreateLogOutPut();
	// 创建误码查询日志输出结构体
	pEnv->m_pLogOutPutErrorCode = OnCreateLogOutPut();
	// 创建ADC数据帧时间日志输出结构体
	pEnv->m_pLogOutPutADCFrameTime = OnCreateLogOutPut();
	// 创建常量信息结构体
	pEnv->m_pConstVar = OnCreateConstVar();
	// 创建仪器通讯信息结构体
	pEnv->m_pInstrumentCommInfo = OnCreateInstrumentCommInfo();
	// 创建心跳帧信息结构体
	pEnv->m_pHeartBeatFrame = OnCreateInstrumentHeartBeat();
	// 创建首包帧信息结构体
	pEnv->m_pHeadFrame = OnCreateInstrumentHeadFrame();
	// 创建IP地址设置帧信息结构体
	pEnv->m_pIPSetFrame = OnCreateInstrumentIPSetFrame();
	// 创建尾包帧信息结构体
	pEnv->m_pTailFrame = OnCreateInstrumentTailFrame();
	// 创建尾包时刻帧信息结构体
	pEnv->m_pTailTimeFrame = OnCreateInstrumentTailTimeFrame();
	// 创建时统设置帧信息结构体
	pEnv->m_pTimeDelayFrame = OnCreateInstrumentTimeDelayFrame();
	// 创建ADC参数设置帧信息结构体
	pEnv->m_pADCSetFrame = OnCreateInstrumentADCSetFrame();
	// 创建误码查询帧信息结构体
	pEnv->m_pErrorCodeFrame = OnCreateInstrumentErrorCodeFrame();
	// 创建ADC数据帧信息结构体
	pEnv->m_pADCDataFrame = OnCreateInstrumentADCDataFrame();
	// 创建测线队列结构体
	pEnv->m_pLineList = OnCreateLineList();
	// 创建数据存储缓冲区结构体
	pEnv->m_pADCDataBufArray = OnCreateADCDataBufArray();
	// 创建施工任务数组结构体
	pEnv->m_pOptTaskArray = OnCreateOptTaskArray();

	// 创建日志输出线程
	pEnv->m_pLogOutPutThread = OnCreateLogOutPutThread();
	// 创建心跳线程
	pEnv->m_pHeartBeatThread = OnCreateHeartBeatThread();
	// 创建首包接收线程
	pEnv->m_pHeadFrameThread = OnCreateHeadFrameThread();
	// 创建IP地址设置线程
	pEnv->m_pIPSetFrameThread = OnCreateIPSetFrameThread();
	// 创建尾包接收线程
	pEnv->m_pTailFrameThread = OnCreateTailFrameThread();
	// 创建时统线程
	pEnv->m_pTimeDelayThread = OnCreateTimeDelayThread();
	// 创建路由监视线程
	pEnv->m_pMonitorThread = OnCreateMonitorThread();
	// 创建ADC参数设置线程
	pEnv->m_pADCSetThread = OnCreateADCSetThread();
	// 创建误码查询线程
	pEnv->m_pErrorCodeThread = OnCreateErrorCodeThread();
	// 创建ADC数据接收线程
	pEnv->m_pADCDataRecThread = OnCreateADCDataRecThread();
	// 创建ADC数据存储线程
	pEnv->m_pADCDataSaveThread = OnCreateADCDataSaveThread();
	// Field On
	pEnv->m_bFieldOn = false;
	// Field Off
	pEnv->m_bFieldOff = true;
	// Netd程序的进程信息
	memset(&pEnv->m_piNetd, 0 ,sizeof(PROCESS_INFORMATION));
	return pEnv;
}
// 调用netd程序
void OnCreateNetdProcess(m_oEnvironmentStruct* pEnv)
{
	if (_taccess(NetdExePath, 0) == -1)
	{
		AfxMessageBox(_T("The pcap exe file is not exist!"));
		return;
	}
	TCHAR szCommandLine[] = _T("NetWinPcap.exe NetCardId=2 DownStreamRcvSndPort=36666_36866 UpStreamRcvSndPort=28672_28722,32768_32818,36864_36914,37120_37170,37376_37426,37632_37682,37888_37938,38144_38194,38400_38450 NetDownStreamSrcPort=39320 NetUpStreamSrcPort=39321 WinpcapBufSize=26214400 UpStreamIP=192.168.100.252 DownStreamIP=192.168.100.22 NetIP=192.168.100.22 UpStreamMacAddr=0,10,53,0,1,2 DownStreamMacAddr=0,48,103,107,228,202 NetMacAddr=0,48,103,107,228,202 MaxPackageSize=512 PcapTimeOut=100 PcapSndWaitTime=10 PcapRcvWaitTime=1 PcapQueueSize=100000");
	STARTUPINFO si = {0};
	si.dwFlags = STARTF_USESHOWWINDOW; // 指定wShowWindow成员有效
	si.wShowWindow = SW_SHOW; // 此成员设定是否显示新建进程的主窗口
	BOOL bRet = CreateProcess (NULL,// 不在此指定可执行文件的文件名
		szCommandLine, // 命令行参数
		NULL, // 默认进程安全性
		NULL, // 默认进程安全性
		FALSE, // 指定当前进程内句柄不可以被子进程继承
		0, // 为新进程创建一个新的控制台窗口
		NULL, // 使用本进程的环境变量
		NULL, // 使用本进程的驱动器和目录
		&si,
		&pEnv->m_piNetd);
	if(!bRet)
	{
		AfxMessageBox(_T("CreateProcess NetWinPcap.exe failed!"));
	}
}
// 由进程ID获取窗口句柄
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{ 
	long id;
	HWND thwnd;
	id = GetWindowThreadProcessId(hwnd,NULL);
	if (id == lParam)
	{
		while((thwnd=GetParent(hwnd)) != NULL)
		{
			hwnd = thwnd;
		}
		PostMessage(hwnd, WM_CLOSE, NULL, NULL);
		return false;
	}
	return true;
}
// 关闭netd程序
void OnCloseNetdProcess(m_oEnvironmentStruct* pEnv)
{
	EnumWindows(EnumWindowsProc,pEnv->m_piNetd.dwThreadId);
}
// 初始化实例
void OnInit(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	CString str = _T("");
	CString strPath = _T("");
	SYSTEMTIME  sysTime;
	// 创建程序运行日志文件夹
	CreateDirectory(LogFolderPath, NULL);
	GetLocalTime(&sysTime);
	str.Format(_T("\\%04d年%02d月%02d日_Log"),sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	strPath = LogFolderPath + str;
	CreateDirectory(strPath, NULL);
	str.Format(_T("\\%02d时%02d分%02d秒_log"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strPath += str;
	CreateDirectory(strPath, NULL);

	// 初始化操作日志输出结构体
	CreateDirectory(strPath + SysOptLogFolderPath, NULL);
	EnterCriticalSection(&pEnv->m_pLogOutPutOpt->m_oSecLogFile);
	pEnv->m_pLogOutPutOpt->m_SaveLogFilePath = (CStringA)(strPath + SysOptLogFolderPath);
	pEnv->m_pLogOutPutOpt->m_cLogFileType = OptLogType;
	LeaveCriticalSection(&pEnv->m_pLogOutPutOpt->m_oSecLogFile);
	OnInitLogOutPut(pEnv->m_pLogOutPutOpt);
	// 初始化时统日志输出结构体
	CreateDirectory(strPath + TimeDelayLogFolderPath, NULL);
	EnterCriticalSection(&pEnv->m_pLogOutPutTimeDelay->m_oSecLogFile);
	pEnv->m_pLogOutPutTimeDelay->m_SaveLogFilePath = (CStringA)(strPath + TimeDelayLogFolderPath);
	pEnv->m_pLogOutPutTimeDelay->m_cLogFileType = TimeDelayLogType;
	LeaveCriticalSection(&pEnv->m_pLogOutPutTimeDelay->m_oSecLogFile);
	OnInitLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 初始化误码查询日志输出结构体
	CreateDirectory(strPath + ErrorCodeLogFolderPath, NULL);
	EnterCriticalSection(&pEnv->m_pLogOutPutErrorCode->m_oSecLogFile);
	pEnv->m_pLogOutPutErrorCode->m_SaveLogFilePath = (CStringA)(strPath + ErrorCodeLogFolderPath);
	pEnv->m_pLogOutPutErrorCode->m_cLogFileType = ErrorCodeLogType;
	LeaveCriticalSection(&pEnv->m_pLogOutPutErrorCode->m_oSecLogFile);
	OnInitLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 初始化ADC数据帧时间日志输出结构体
	CreateDirectory(strPath + ADCFrameTimeLogFolderPath, NULL);
	EnterCriticalSection(&pEnv->m_pLogOutPutADCFrameTime->m_oSecLogFile);
	pEnv->m_pLogOutPutADCFrameTime->m_SaveLogFilePath = (CStringA)(strPath + ADCFrameTimeLogFolderPath);
	pEnv->m_pLogOutPutADCFrameTime->m_cLogFileType = ADCFrameTimeLogType;
	LeaveCriticalSection(&pEnv->m_pLogOutPutADCFrameTime->m_oSecLogFile);
	OnInitLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
	// 创建ADC数据存储文件夹
	CreateDirectory(strPath + ADCDataLogFolderPath, NULL);
	EnterCriticalSection(&pEnv->m_pOptTaskArray->m_oSecOptTaskArray);
	pEnv->m_pOptTaskArray->m_SaveLogFolderPath = (CStringA)(strPath + ADCDataLogFolderPath);
	LeaveCriticalSection(&pEnv->m_pOptTaskArray->m_oSecOptTaskArray);

	// 初始化套接字库
	OnInitSocketLib();
	// 初始化常量信息结构体
	OnInitConstVar(pEnv->m_pConstVar, pEnv->m_pLogOutPutOpt);
	// 初始化仪器通讯信息结构体
	OnInitInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	// 调用netd程序
//	OnCreateNetdProcess(pEnv);
	// 初始化日志输出线程
	OnInit_LogOutPutThread(pEnv);
	// 初始化心跳线程
	OnInit_HeartBeatThread(pEnv);
	// 初始化首包接收线程
	OnInit_HeadFrameThread(pEnv);
	// 初始化IP地址设置线程
	OnInit_IPSetFrameThread(pEnv);
	// 初始化尾包接收线程
	OnInit_TailFrameThread(pEnv);
	// 初始化时统设置线程
	OnInit_TimeDelayThread(pEnv);
	// 初始化路由监视线程
	OnInit_MonitorThread(pEnv);
	// 初始化ADC参数设置线程
	OnInit_ADCSetThread(pEnv);
	// 初始化误码查询线程
	OnInit_ErrorCodeThread(pEnv);
	// 初始化ADC数据接收线程
	OnInit_ADCDataRecThread(pEnv);
	// 初始化ADC数据存储线程
	OnInit_ADCDataSaveThread(pEnv);

	// 初始化心跳
	OnInitInstrumentHeartBeat(pEnv->m_pHeartBeatFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化首包
	OnInitInstrumentHeadFrame(pEnv->m_pHeadFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化IP地址设置
	OnInitInstrumentIPSetFrame(pEnv->m_pIPSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包
	OnInitInstrumentTailFrame(pEnv->m_pTailFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包时刻
	OnInitInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化时统设置
	OnInitInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化ADC参数设置
	OnInitInstrumentADCSetFrame(pEnv->m_pADCSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化误码查询
	OnInitInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化ADC数据帧
	OnInitInstrumentADCDataFrame(pEnv->m_pADCDataFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化测线队列结构体
	OnInitLineList(pEnv->m_pLineList, pEnv->m_pConstVar);
	// 初始化数据存储缓冲区结构体
	OnInitADCDataBufArray(pEnv->m_pADCDataBufArray, pEnv->m_pConstVar);
	// 初始化施工任务数组结构体
	OnInitOptTaskArray(pEnv->m_pOptTaskArray, pEnv->m_pConstVar);

	// 关闭心跳Socket
	OnCloseSocket(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	// 创建并设置心跳端口
	OnCreateAndSetHeartBeatSocket(pEnv->m_pHeartBeatFrame, pEnv->m_pLogOutPutOpt);
	// 关闭首包Socket
	OnCloseSocket(pEnv->m_pHeadFrame->m_oHeadFrameSocket);
	// 创建并设置首包端口
	OnCreateAndSetHeadFrameSocket(pEnv->m_pHeadFrame, pEnv->m_pLogOutPutOpt);
	// 关闭IP地址设置Socket
	OnCloseSocket(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 创建并设置IP地址设置端口
	OnCreateAndSetIPSetFrameSocket(pEnv->m_pIPSetFrame, pEnv->m_pLogOutPutOpt);
	// 关闭尾包Socket
	OnCloseSocket(pEnv->m_pTailFrame->m_oTailFrameSocket);
	// 创建并设置尾包端口
	OnCreateAndSetTailFrameSocket(pEnv->m_pTailFrame, pEnv->m_pLogOutPutOpt);
	// 关闭尾包时刻Socket
	OnCloseSocket(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket);
	// 创建并设置尾包时刻查询帧端口
	OnCreateAndSetTailTimeFrameSocket(pEnv->m_pTailTimeFrame, pEnv->m_pLogOutPutOpt);
	// 关闭时统设置Socket
	OnCloseSocket(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket);
	// 创建并设置时统设置帧端口
	OnCreateAndSetTimeDelayFrameSocket(pEnv->m_pTimeDelayFrame, pEnv->m_pLogOutPutOpt);
	// 关闭ADC参数设置Socket
	OnCloseSocket(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket);
	// 创建并设置ADC参数设置帧端口
	OnCreateAndSetADCSetFrameSocket(pEnv->m_pADCSetFrame, pEnv->m_pLogOutPutOpt);
	// 关闭误码查询帧Socket
	OnCloseSocket(pEnv->m_pErrorCodeFrame->m_oErrorCodeFrameSocket);
	// 创建并设置误码查询帧端口
	OnCreateAndSetErrorCodeFrameSocket(pEnv->m_pErrorCodeFrame, pEnv->m_pLogOutPutOpt);
	// 关闭ADC数据帧Socket
	OnCloseSocket(pEnv->m_pADCDataFrame->m_oADCDataFrameSocket);
	// 创建并设置ADC数据帧端口
	OnCreateAndSetADCDataFrameSocket(pEnv->m_pADCDataFrame, pEnv->m_pLogOutPutOpt);
}
// 关闭
void OnClose(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	CString str = _T("");
	if (pEnv->m_bFieldOn == true)
	{
		// 将FieldOff时间写入配置文件
		EnterCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_oTimeFieldOff = CTime::GetCurrentTime();
		LeaveCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
		SaveServerParameterSetupData(pEnv->m_pInstrumentCommInfo->m_pServerSetupData);
	}
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	pEnv->m_pLogOutPutThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	pEnv->m_pHeartBeatThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	pEnv->m_pHeadFrameThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	pEnv->m_pIPSetFrameThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	pEnv->m_pTailFrameThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	pEnv->m_pMonitorThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	pEnv->m_pTimeDelayThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	pEnv->m_pADCSetThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pErrorCodeThread->m_oSecErrorCodeThread);
	pEnv->m_pErrorCodeThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pErrorCodeThread->m_oSecErrorCodeThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	pEnv->m_pADCDataRecThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	// 线程关闭标志位为true
	EnterCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	pEnv->m_pADCDataSaveThread->m_pThread->m_bClose = true;
	LeaveCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	// 关闭心跳线程
	OnCloseHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 关闭首包接收线程
	OnCloseHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 关闭IP地址设置线程
	OnCloseIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 关闭尾包接收线程
	OnCloseTailFrameThread(pEnv->m_pTailFrameThread);
	// 关闭路由监视线程
	OnCloseMonitorThread(pEnv->m_pMonitorThread);
	// 关闭时统设置线程
	OnCloseTimeDelayThread(pEnv->m_pTimeDelayThread);
	// 关闭ADC参数设置线程
	OnCloseADCSetThread(pEnv->m_pADCSetThread);
	// 关闭误码查询线程
	OnCloseErrorCodeThread(pEnv->m_pErrorCodeThread);
	// 关闭ADC数据接收线程
	OnCloseADCDataRecThread(pEnv->m_pADCDataRecThread);
	// 关闭ADC数据存储线程
	OnCloseADCDataSaveThread(pEnv->m_pADCDataSaveThread);
	// 关闭日志输出线程
	OnCloseLogOutPutThread(pEnv->m_pLogOutPutThread);

	// 输出接收和发送帧的统计结果
	OnOutPutResult(pEnv);

	// 关闭心跳Socket
	OnCloseSocket(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	// 关闭首包Socket
	OnCloseSocket(pEnv->m_pHeadFrame->m_oHeadFrameSocket);
	// 关闭IP地址设置Socket
	OnCloseSocket(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 关闭尾包Socket
	OnCloseSocket(pEnv->m_pTailFrame->m_oTailFrameSocket);
	// 关闭尾包时刻Socket
	OnCloseSocket(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket);
	// 关闭时统设置Socket
	OnCloseSocket(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket);
	// 关闭ADC参数设置Socket
	OnCloseSocket(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket);
	// 关闭误码查询帧Socket
	OnCloseSocket(pEnv->m_pErrorCodeFrame->m_oErrorCodeFrameSocket);
	// 关闭ADC数据帧Socket
	OnCloseSocket(pEnv->m_pADCDataFrame->m_oADCDataFrameSocket);
	// 释放常量资源
	OnCloseConstVar(pEnv->m_pConstVar);
	// 释放心跳资源
	OnCloseInstrumentHeartBeat(pEnv->m_pHeartBeatFrame);
	// 释放首包资源
	OnCloseInstrumentHeadFrame(pEnv->m_pHeadFrame);
	// 释放IP地址设置资源
	OnCloseInstrumentIPSetFrame(pEnv->m_pIPSetFrame);
	// 释放尾包资源
	OnCloseInstrumentTailFrame(pEnv->m_pTailFrame);
	// 释放尾包时刻资源
	OnCloseInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame);
	// 释放时统设置资源
	OnCloseInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame);
	// 释放ADC参数设置资源
	OnCloseInstrumentADCSetFrame(pEnv->m_pADCSetFrame);
	// 释放误码查询资源
	OnCloseInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame);
	// 释放ADC数据帧资源
	OnCloseInstrumentADCDataFrame(pEnv->m_pADCDataFrame);
	// 释放测线队列资源
	OnCloseLineList(pEnv->m_pLineList);
	// 关闭施工数据文件
	CloseAllADCDataSaveInFile(pEnv->m_pOptTaskArray);
	// 关闭数据存储缓冲区结构体
	OnCloseADCDataBufArray(pEnv->m_pADCDataBufArray);
	// 关闭施工任务数组结构体
	OnCloseOptTaskArray(pEnv->m_pOptTaskArray);
	// 释放套接字库
	OnCloseSocketLib();

	// 关闭操作日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutOpt);
	// 关闭时统日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 关闭误码查询日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 关闭ADC数据帧时间日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
	// 关闭Netd程序
//	OnCloseNetdProcess(pEnv);
}
// 工作
unsigned int OnWork(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return 0;
	}
	if (pEnv->m_bFieldOn == true)
	{
		return 0;
	}
	CTime timeFieldOn = CTime::GetCurrentTime();
	CTime timeFieldOff;
	CTimeSpan timeWait;
	int nDays, nHours, nMins, nSecs;
	// 从配置文件中读出上一次FieldOff时间
	LoadServerParameterSetupData(pEnv->m_pInstrumentCommInfo->m_pServerSetupData);
	EnterCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	timeFieldOff = pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_oTimeFieldOff;
	LeaveCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	nDays = pEnv->m_pConstVar->m_uiFieldOnWaitTimeLimit / (24 * 3600);
	nHours = pEnv->m_pConstVar->m_uiFieldOnWaitTimeLimit / 3600;
	nMins = pEnv->m_pConstVar->m_uiFieldOnWaitTimeLimit / 60;
	nSecs = pEnv->m_pConstVar->m_uiFieldOnWaitTimeLimit % 60;
	timeFieldOff += CTimeSpan(nDays, nHours, nMins, nSecs);
	if (timeFieldOn < timeFieldOff)
	{
		timeWait = timeFieldOff - timeFieldOn;
		return (unsigned int)timeWait.GetTotalSeconds();
	}
	pEnv->m_bFieldOn = true;
	// 重置测线队列结构体
	OnResetLineList(pEnv->m_pLineList);
	// 重置数据存储缓冲区结构体
	OnResetADCDataBufArray(pEnv->m_pADCDataBufArray);
	// 重置施工任务数组结构体
	OnResetOptTaskArray(pEnv->m_pOptTaskArray);
	// 日志输出线程开始工作
	EnterCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	pEnv->m_pLogOutPutThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	// 心跳线程开始工作
	EnterCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	pEnv->m_pHeartBeatThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	EnterCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	// 清空首包帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pHeadFrame->m_oHeadFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	// 首包接收线程开始工作
	EnterCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	pEnv->m_pHeadFrameThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	// 清空IP地址设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	// IP地址设置线程开始工作	
	EnterCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	pEnv->m_pIPSetFrameThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	EnterCriticalSection(&pEnv->m_pTailFrame->m_oSecTailFrame);
	// 清空尾包帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTailFrame->m_oTailFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pTailFrame->m_oSecTailFrame);
	// 尾包接收线程开始工作
	EnterCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	pEnv->m_pTailFrameThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	// 路由监视线程开始工作
	EnterCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	pEnv->m_pMonitorThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	EnterCriticalSection(&pEnv->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	// 清空误码查询接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pErrorCodeFrame->m_oErrorCodeFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	EnterCriticalSection(&pEnv->m_pADCSetFrame->m_oSecADCSetFrame);
	// 清空ADC参数设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pADCSetFrame->m_oSecADCSetFrame);
	EnterCriticalSection(&pEnv->m_pADCDataFrame->m_oSecADCDataFrame);
	// 清空ADC数据接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pADCDataFrame->m_oADCDataFrameSocket, pEnv->m_pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pADCDataFrame->m_oSecADCDataFrame);
	// ADC数据接收线程开始工作
	EnterCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	pEnv->m_pADCDataRecThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	// ADC数据存储线程开始工作
	EnterCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	pEnv->m_pADCDataSaveThread->m_pThread->m_bWork = true;
	LeaveCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnWork", "开始工作");
	pEnv->m_bFieldOff = false;
	return 0;
}
// 停止
void OnStop(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	if (pEnv->m_bFieldOff == true)
	{
		return;
	}
	// 将FieldOff时间写入配置文件
	EnterCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_oTimeFieldOff = CTime::GetCurrentTime();
	LeaveCriticalSection(&pEnv->m_pInstrumentCommInfo->m_pServerSetupData->m_oSecCommInfo);
	SaveServerParameterSetupData(pEnv->m_pInstrumentCommInfo->m_pServerSetupData);

	pEnv->m_bFieldOff = true;
	// 日志输出线程停止工作
	EnterCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	pEnv->m_pLogOutPutThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pLogOutPutThread->m_oSecLogOutPutThread);
	// 心跳线程停止工作
	EnterCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	pEnv->m_pHeartBeatThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pHeartBeatThread->m_oSecHeartBeatThread);
	// 首包接收线程停止工作
	EnterCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	pEnv->m_pHeadFrameThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pHeadFrameThread->m_oSecHeadFrameThread);
	// IP地址设置线程停止工作
	EnterCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	pEnv->m_pIPSetFrameThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pIPSetFrameThread->m_oSecIPSetFrameThread);
	// 尾包接收线程停止工作
	EnterCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	pEnv->m_pTailFrameThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pTailFrameThread->m_oSecTailFrameThread);
	// 路由监视线程停止工作
	EnterCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	pEnv->m_pMonitorThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pMonitorThread->m_oSecMonitorThread);
	// 时统设置线程停止工作
	EnterCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	pEnv->m_pTimeDelayThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pTimeDelayThread->m_oSecTimeDelayThread);
	// ADC参数设置线程停止工作
	EnterCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	pEnv->m_pADCSetThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pADCSetThread->m_oSecADCSetThread);
	// 误码查询线程停止工作
	EnterCriticalSection(&pEnv->m_pErrorCodeThread->m_oSecErrorCodeThread);
	pEnv->m_pErrorCodeThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pErrorCodeThread->m_oSecErrorCodeThread);
	// ADC数据接收线程停止工作
	EnterCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	pEnv->m_pADCDataRecThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pADCDataRecThread->m_oSecADCDataRecThread);
	// ADC数据存储线程停止工作
	EnterCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	pEnv->m_pADCDataSaveThread->m_pThread->m_bWork = false;
	LeaveCriticalSection(&pEnv->m_pADCDataSaveThread->m_oSecADCDataSaveThread);
	// 关闭施工数据文件
	CloseAllADCDataSaveInFile(pEnv->m_pOptTaskArray);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnStop", "停止工作");
	pEnv->m_bFieldOn = false;
}
// 释放实例资源
void OnFreeInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 释放操作日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutOpt);
	// 释放时统日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 释放误码查询日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 释放ADC数据帧时间日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
	// 释放INI文件导入常量结构体资源
	OnFreeConstVar(pEnv->m_pConstVar);
	// 释放XML文件导入的通讯信息设置结构体资源
	OnFreeInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	// 释放心跳帧结构体资源
	OnFreeInstrumentHeartBeat(pEnv->m_pHeartBeatFrame);
	// 释放首包帧结构体资源
	OnFreeInstrumentHeadFrame(pEnv->m_pHeadFrame);
	// 释放IP地址设置帧结构体资源
	OnFreeInstrumentIPSetFrame(pEnv->m_pIPSetFrame);
	// 释放尾包帧结构体资源
	OnFreeInstrumentTailFrame(pEnv->m_pTailFrame);
	// 释放尾包时刻帧结构体资源
	OnFreeInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame);
	// 释放时统设置帧结构体资源
	OnFreeInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame);
	// 释放ADC参数设置帧结构体资源
	OnFreeInstrumentADCSetFrame(pEnv->m_pADCSetFrame);
	// 释放误码帧结构体资源
	OnFreeInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame);
	// 释放ADC数据帧结构体资源
	OnFreeInstrumentADCDataFrame(pEnv->m_pADCDataFrame);
	// 释放测线队列结构体资源
	OnFreeLineList(pEnv->m_pLineList);
	// 释放数据存储缓冲区结构体
	OnFreeADCDataBufArray(pEnv->m_pADCDataBufArray);
	// 释放数据存储缓冲区结构体
	OnFreeOptTaskArray(pEnv->m_pOptTaskArray);

	// 释放日志输出线程
	OnFreeLogOutPutThread(pEnv->m_pLogOutPutThread);
	// 释放心跳线程
	OnFreeHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 释放首包接收线程
	OnFreeHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 释放IP地址设置线程
	OnFreeIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 释放尾包接收线程
	OnFreeTailFrameThread(pEnv->m_pTailFrameThread);
	// 释放路由监视线程
	OnFreeMonitorThread(pEnv->m_pMonitorThread);
	// 释放时统设置线程
	OnFreeTimeDelayThread(pEnv->m_pTimeDelayThread);
	// 释放ADC参数设置线程
	OnFreeADCSetThread(pEnv->m_pADCSetThread);
	// 释放误码查询线程
	OnFreeErrorCodeThread(pEnv->m_pErrorCodeThread);
	// 释放ADC数据接收线程
	OnFreeADCDataRecThread(pEnv->m_pADCDataRecThread);
	// 释放ADC数据接收线程
	OnFreeADCDataSaveThread(pEnv->m_pADCDataSaveThread);
	delete pEnv;
	pEnv = NULL;
}