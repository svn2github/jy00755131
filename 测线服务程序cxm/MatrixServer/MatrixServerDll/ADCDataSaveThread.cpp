#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建施工放炮数据存储线程
m_oADCDataSaveThreadStruct* OnCreateADCDataSaveThread(void)
{
	m_oADCDataSaveThreadStruct* pADCDataSaveThread = NULL;
	pADCDataSaveThread = new m_oADCDataSaveThreadStruct;
	pADCDataSaveThread->m_pThread = new m_oThreadStruct;
	pADCDataSaveThread->m_pADCDataBufArray = NULL;
	InitializeCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	return pADCDataSaveThread;
}
// 线程等待函数
void WaitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	ASSERT(pADCDataSaveThread != NULL);
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCDataSaveThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		bClose = pADCDataSaveThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pADCDataSaveThread->m_pThread->m_pConstVar->m_iADCDataSaveSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}

// 关闭所有的施工文件
// void CloseAllADCDataSaveInFile(m_oOptTaskArrayStruct* pOptTaskArray)
// {
// 	ASSERT(pOptTaskArray != NULL);
// 	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter;
// 	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
// 	for (iter = pOptTaskArray->m_oOptTaskWorkMap.begin();
// 		iter != pOptTaskArray->m_oOptTaskWorkMap.end(); iter++)
// 	{
// 		if (iter->second->m_pPreviousFile != NULL)
// 		{
// 			fclose(iter->second->m_pPreviousFile);
// 		}
// 		if (iter->second->m_pFile != NULL)
// 		{
// 			fclose(iter->second->m_pFile);
// 		}
// 	}
// 	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
// }
// 保存ADC数据到施工文件
void ProcADCDataSaveInFile(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	ASSERT(pADCDataSaveThread != NULL);
	bool bSave = false;
	CString str = _T("");
	m_oADCDataBufStruct* pADCDataBuf = NULL;
	hash_map<unsigned int, m_oADCDataBufStruct*>::iterator iter;

	EnterCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	for (iter = pADCDataSaveThread->m_pADCDataBufArray->m_oADCDataBufWorkMap.begin();
		iter != pADCDataSaveThread->m_pADCDataBufArray->m_oADCDataBufWorkMap.end(); iter++)
	{
		pADCDataBuf = iter->second;
		// 可以保存到Segd文件
		if ((pADCDataBuf->m_bInUsed == true) && (pADCDataBuf->m_bSaveInSegd == true))
		{
			bSave = true;
			break;
		}
	}
	LeaveCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	if (bSave == true)
	{
		m_oSegdFileStruct oSegdFile;
		OnInitSegdSH(&oSegdFile.m_oSegdSH);
		oSegdFile.m_strPath = pADCDataBuf->m_SaveFilePath;
		oSegdFile.m_oSegdSH.m_uiSampleRate = pADCDataBuf->m_iSampleRate;
		oSegdFile.m_oSegdSH.m_uiShotNo = pADCDataBuf->m_uiOptNo;
		oSegdFile.m_oSegdSH.m_iSampleLength = pADCDataBuf->m_uiSavePointNum;
		oSegdFile.m_oSegdSH.m_uiSampleTime = pADCDataBuf->m_uiSampleTime;
		oSegdFile.m_iAcqTraceNum = pADCDataBuf->m_uiAcqTraceNum;
		oSegdFile.m_iAuxTraceNum = pADCDataBuf->m_uiAuxTraceNum;
		oSegdFile.m_iTotalTraceNum = pADCDataBuf->m_uiAcqTraceNum + pADCDataBuf->m_uiAuxTraceNum;
		oSegdFile.m_uiSegdDataBufLength = oSegdFile.m_oSegdSH.m_iSampleLength 
			* (oSegdFile.m_iAcqTraceNum + oSegdFile.m_iAuxTraceNum) 
			* pADCDataSaveThread->m_pThread->m_pConstVar->m_iADCDataSize3B;
		oSegdFile.m_pSegdDataBuf = pADCDataBuf->m_pADCDataBuf;
		oSegdFile.m_pSegdDHList = &pADCDataBuf->m_olsSegdDataHeader;
		// 保存文件
		SaveSegdFile(&oSegdFile);
		str.Format(_T("保存文件炮号为 %d"), oSegdFile.m_oSegdSH.m_uiShotNo);
		OutputDebugString(str);
		EnterCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
		AddFreeADCDataBuf(pADCDataBuf, pADCDataSaveThread->m_pADCDataBufArray);
		DeleteADCDataBufFromMap(pADCDataBuf->m_uiIndex, &pADCDataSaveThread->m_pADCDataBufArray->m_oADCDataBufWorkMap);
		LeaveCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	}
// 	// 重新产生施工仪器索引
// 	GenOptInstrMap(pEnv->m_pLineList, pEnv->m_pOptTaskArray);
}
// 线程函数
DWORD WINAPI RunADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	ASSERT(pADCDataSaveThread != NULL);
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		bClose = pADCDataSaveThread->m_pThread->m_bClose;
		bWork = pADCDataSaveThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			// 保存ADC数据到施工文件
			ProcADCDataSaveInFile(pADCDataSaveThread);
		}
		EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		bClose = pADCDataSaveThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		if (bClose == true)
		{
			break;
		}
		WaitADCDataSaveThread(pADCDataSaveThread);
	}
	EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pADCDataSaveThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	return 1;
}
// 初始化施工放炮数据存储线程
bool OnInitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	ASSERT(pADCDataSaveThread != NULL);
	EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	if (false == OnInitThread(pADCDataSaveThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pADCDataSaveThread->m_pThread->m_hThreadClose);
	// 创建线程
	pADCDataSaveThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCDataSaveThread,
		pADCDataSaveThread, 
		0, 
		&pADCDataSaveThread->m_pThread->m_dwThreadID);
	if (pADCDataSaveThread->m_pThread->m_hThread == NULL)
	{
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnInitADCDataSaveThread", 
			"pADCDataSaveThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnInitADCDataSaveThread", 
		"ADC数据存储线程创建成功");
	return true;
}
// 初始化施工放炮数据存储线程
bool OnInit_ADCDataSaveThread(m_oEnvironmentStruct* pEnv)
{
	ASSERT(pEnv != NULL);
	pEnv->m_pADCDataSaveThread->m_pADCDataBufArray = pEnv->m_pADCDataBufArray;
	return OnInitADCDataSaveThread(pEnv->m_pADCDataSaveThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭施工放炮数据存储线程
bool OnCloseADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	ASSERT(pADCDataSaveThread != NULL);
	EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	if (false == OnCloseThread(pADCDataSaveThread->m_pThread))
	{
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnCloseADCDataSaveThread", 
			"ADC数据存储线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnCloseADCDataSaveThread", 
		"ADC数据存储线程成功关闭");
	return true;
}
// 释放施工放炮数据存储线程
void OnFreeADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	ASSERT(pADCDataSaveThread != NULL);
	if (pADCDataSaveThread->m_pThread != NULL)
	{
		delete pADCDataSaveThread->m_pThread;
		pADCDataSaveThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	delete pADCDataSaveThread;
	pADCDataSaveThread = NULL;
}
