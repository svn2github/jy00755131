#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建误码查询线程
m_oErrorCodeThreadStruct* OnCreateErrorCodeThread(void)
{
	m_oErrorCodeThreadStruct* pErrorCodeThread = NULL;
	pErrorCodeThread = new m_oErrorCodeThreadStruct;
	pErrorCodeThread->m_pThread = new m_oThreadStruct;
	pErrorCodeThread->m_pLogOutPutErrorCode = NULL;
	pErrorCodeThread->m_pErrorCodeFrame = NULL;
	pErrorCodeThread->m_pLineList = NULL;
	InitializeCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	return pErrorCodeThread;
}
// 线程等待函数
void WaitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pErrorCodeThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		bClose = pErrorCodeThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pErrorCodeThread->m_pThread->m_pConstVar->m_iErrorCodeSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 处理单个误码查询应答帧
void ProcErrorCodeReturnFrameOne(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	CString strOutPut = _T("");
	CString strDebug = _T("");
	bool bStrDebug = false;
	string strFrameData = "";
	string strConv = "";
	char cLAUXErrorCodeDataLineACount = 0;
	char cLAUXErrorCodeDataLineBCount = 0;
	char cLAUXErrorCodeDataLAUXLineACount = 0;
	char cLAUXErrorCodeDataLAUXLineBCount = 0;
	char cLAUXErrorCodeCmdCount = 0;
	char cFDUErrorCodeDataCount = 0;
	char cFDUErrorCodeCmdCount = 0;
	unsigned int uiSysTime = 0;
	int iTemp = 0;
	EnterCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	// 得到仪器IP
	uiIPInstrument = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_uiSrcIP;
	cLAUXErrorCodeDataLineACount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLineACount;
	cLAUXErrorCodeDataLineBCount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLineBCount;
	cLAUXErrorCodeDataLAUXLineACount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLAUXLineACount;
	cLAUXErrorCodeDataLAUXLineBCount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLAUXLineBCount;
	cLAUXErrorCodeCmdCount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeCmdCount;
	cFDUErrorCodeDataCount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cFDUErrorCodeDataCount;
	cFDUErrorCodeCmdCount = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cFDUErrorCodeCmdCount;
	uiSysTime = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_uiSysTime;
	LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	EnterCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
	// 仪器在索引表中
	if (FALSE == IfIndexExistInMap(uiIPInstrument, &pErrorCodeThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap))
	{
		LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
		GetFrameInfo(pErrorCodeThread->m_pErrorCodeFrame->m_cpRcvFrameData,
			pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "ProcErrorCodeReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPSETMAP_NOTEXIT);
		return;
	}
	pInstrument = GetInstrumentFromMap(uiIPInstrument, 
		&pErrorCodeThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	if (FALSE == GetRoutByRoutIP(pInstrument->m_uiRoutIP, pErrorCodeThread->m_pLineList->m_pRoutList, &pRout))
	{
		LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
		GetFrameInfo(pErrorCodeThread->m_pErrorCodeFrame->m_cpRcvFrameData,
			pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "ProcErrorCodeReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_ROUT_NOTEXIT);
		return;
	}
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument);
	if (NULL == GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection))
	{
		LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
		return;
	}
	str.Format(_T("仪器SN = 0x%x，IP = %d，仪器位置 = %d "), pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_iPointIndex);
	strOutPut += str;
	strDebug += str;
	// 仪器类型为LCI或者交叉站
	if ((pInstrument->m_iInstrumentType == InstrumentTypeLCI)
		|| (pInstrument->m_iInstrumentType == InstrumentTypeLAUX))
	{
		// 交叉站大线A数据故障
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cLAUXErrorCodeDataLineACountOld > cLAUXErrorCodeDataLineACount)
			{
				iTemp = 256 + cLAUXErrorCodeDataLineACount - pInstrument->m_cLAUXErrorCodeDataLineACountOld;
			}
			else
			{
				iTemp = cLAUXErrorCodeDataLineACount - pInstrument->m_cLAUXErrorCodeDataLineACountOld;
			}
			pInstrument->m_iLAUXErrorCodeDataLineACount += iTemp;
		}
		pInstrument->m_cLAUXErrorCodeDataLineACountOld = cLAUXErrorCodeDataLineACount;
		str.Format(_T("大线A数据故障数=%d，"),iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
		// 交叉站大线B数据故障
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cLAUXErrorCodeDataLineBCountOld > cLAUXErrorCodeDataLineBCount)
			{
				iTemp = 256 + cLAUXErrorCodeDataLineBCount - pInstrument->m_cLAUXErrorCodeDataLineBCountOld;
			}
			else
			{
				iTemp = cLAUXErrorCodeDataLineBCount - pInstrument->m_cLAUXErrorCodeDataLineBCountOld;
			}
			pInstrument->m_iLAUXErrorCodeDataLineBCount += iTemp;
		}
		pInstrument->m_cLAUXErrorCodeDataLineBCountOld = cLAUXErrorCodeDataLineBCount;
		str.Format(_T("大线B数据故障数=%d，"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
		// 交叉站交叉线A数据故障
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld > cLAUXErrorCodeDataLAUXLineACount)
			{
				iTemp = 256 + cLAUXErrorCodeDataLAUXLineACount - pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld;
			}
			else
			{
				iTemp = cLAUXErrorCodeDataLAUXLineACount - pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld;
			}
			pInstrument->m_iLAUXErrorCodeDataLAUXLineACount += iTemp;
		}
		pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld = cLAUXErrorCodeDataLAUXLineACount;
		str.Format(_T("交叉线A数据故障数=%d，"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
		// 交叉站交叉线B数据故障
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld > cLAUXErrorCodeDataLAUXLineBCount)
			{
				iTemp = 256 + cLAUXErrorCodeDataLAUXLineBCount - pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld;
			}
			else
			{
				iTemp = cLAUXErrorCodeDataLAUXLineBCount - pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld;
			}
			pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount += iTemp;
		}
		pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld = cLAUXErrorCodeDataLAUXLineBCount;
		str.Format(_T("交叉线B数据故障数=%d，"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
		// 交叉站命令口故障
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cLAUXErrorCodeCmdCountOld > cLAUXErrorCodeCmdCount)
			{
				iTemp = 256 + cLAUXErrorCodeCmdCount - pInstrument->m_cLAUXErrorCodeCmdCountOld;
			}
			else
			{
				iTemp = cLAUXErrorCodeCmdCount - pInstrument->m_cLAUXErrorCodeCmdCountOld;
			}
			// LCI不计算命令误码
			if (pInstrument->m_iInstrumentType == InstrumentTypeLCI)
			{
				iTemp = 0;
			}
			pInstrument->m_iLAUXErrorCodeCmdCount += iTemp;
		}
		pInstrument->m_cLAUXErrorCodeCmdCountOld = cLAUXErrorCodeCmdCount;
		str.Format(_T("命令口故障数=%d；"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
	}
	// 仪器类型为采集站或者电源站
	else
	{
		// 采集站和电源站网络数据错误计数
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cFDUErrorCodeDataCountOld > cFDUErrorCodeDataCount)
			{
				iTemp = 256 + cFDUErrorCodeDataCount - pInstrument->m_cFDUErrorCodeDataCountOld;
			}
			else
			{
				iTemp = cFDUErrorCodeDataCount - pInstrument->m_cFDUErrorCodeDataCountOld;
			}
			pInstrument->m_iFDUErrorCodeDataCount += iTemp;
		}
		pInstrument->m_cFDUErrorCodeDataCountOld = cFDUErrorCodeDataCount;
		str.Format(_T("网络数据错误计数=%d，"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
		// 采集站和电源站命令错误计数
		if (pInstrument->m_uiErrorCodeReturnNum != 0)
		{
			if (pInstrument->m_cFDUErrorCodeCmdCountOld > cFDUErrorCodeCmdCount)
			{
				iTemp = 256 + cFDUErrorCodeCmdCount - pInstrument->m_cFDUErrorCodeCmdCountOld;
			}
			else
			{
				iTemp = cFDUErrorCodeCmdCount - pInstrument->m_cFDUErrorCodeCmdCountOld;
			}
			pInstrument->m_iFDUErrorCodeCmdCount += iTemp;
		}
		pInstrument->m_cFDUErrorCodeCmdCountOld = cFDUErrorCodeCmdCount;
		str.Format(_T("命令错误计数=%d；"), iTemp);
		strOutPut += str;
		if (iTemp != 0)
		{
			strDebug += str;
		}
	}
	pInstrument->m_uiErrorCodeReturnNum++;
	str.Format(_T("接收帧数=%d "), pInstrument->m_uiErrorCodeReturnNum);
	strOutPut += str;
	if (pInstrument->m_iLAUXErrorCodeDataLineACount != 0)
	{
		str.Format(_T("大线A数据故障总数=%d，"),pInstrument->m_iLAUXErrorCodeDataLineACount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iLAUXErrorCodeDataLineBCount != 0)
	{
		str.Format(_T("大线B数据故障总数=%d，"),pInstrument->m_iLAUXErrorCodeDataLineBCount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iLAUXErrorCodeDataLAUXLineACount != 0)
	{
		str.Format(_T("交叉线A数据故障总数=%d，"),pInstrument->m_iLAUXErrorCodeDataLAUXLineACount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount != 0)
	{
		str.Format(_T("交叉线B数据故障总数=%d，"),pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iLAUXErrorCodeCmdCount != 0)
	{
		str.Format(_T("命令口故障总数=%d，"),pInstrument->m_iLAUXErrorCodeCmdCount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iFDUErrorCodeDataCount != 0)
	{
		str.Format(_T("网络数据错误计数总数=%d，"),pInstrument->m_iFDUErrorCodeDataCount);
		strDebug += str;
		bStrDebug = true;
	}
	if (pInstrument->m_iFDUErrorCodeCmdCount != 0)
	{
		str.Format(_T("命令错误计数总数=%d，"),pInstrument->m_iFDUErrorCodeCmdCount);
		strDebug += str;
		bStrDebug = true;
	}
	LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
	str.Format(_T("本地时间 = 0x%x"), uiSysTime);
	strOutPut += str;
	strConv = (CStringA)strOutPut;
	AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", strConv);
	if (bStrDebug == true)
	{
		OutputDebugString(strDebug);
	}
}
// 处理误码查询应答帧
void ProcErrorCodeReturnFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	EnterCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	iFrameCount = GetFrameCount(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
		pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pErrorCodeThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
			// 得到帧数据
			if (false == GetFrameData(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
				pErrorCodeThread->m_pErrorCodeFrame->m_cpRcvFrameData, 
				pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pErrorCodeThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
				continue;
			}
			LeaveCriticalSection(&pErrorCodeThread->m_pErrorCodeFrame->m_oSecErrorCodeFrame);
			if (false == ParseInstrErrorCodeReturnFrame(pErrorCodeThread->m_pErrorCodeFrame, 
				pErrorCodeThread->m_pThread->m_pConstVar, pErrorCodeThread->m_pThread->m_pLogOutPut))
			{
				AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, 
					"ParseInstrumentErrorCodeReturnFrame","", 
					ErrorType, IDS_ERR_PARSE_ERRORCODERETURNFRAME);
				continue;
			}
			// 处理单个IP地址设置应答帧
			ProcErrorCodeReturnFrameOne(pErrorCodeThread);		
		}		
	}
}
// 发送误码查询帧
void ProcErrorCodeQueryFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	m_oRoutStruct* pRout = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
	for (iter = pErrorCodeThread->m_pLineList->m_pRoutList->m_oRoutMap.begin();
		iter != pErrorCodeThread->m_pLineList->m_pRoutList->m_oRoutMap.end();
		iter++)
	{
		pRout = iter->second;
		if (pRout->m_pTail != NULL)
		{
			pInstrument = pRout->m_pHead;
			do 
			{
				pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection);
				if (pInstrument == NULL)
				{
					break;
				}
				pInstrument->m_uiErrorCodeQueryNum++;
			} while (pInstrument != pRout->m_pTail);
			// 广播发送误码查询帧
			MakeInstrErrorCodeQueryFrame(pErrorCodeThread->m_pErrorCodeFrame,
				pErrorCodeThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort,
				pErrorCodeThread->m_pThread->m_pLogOutPut);
			str.Format(_T("向路由IP = 0x%x广播发送误码查询帧"), pRout->m_uiRoutIP);
			strConv = (CStringA)str;
			AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", strConv);
		}
	}
	LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
}
// 线程函数
DWORD WINAPI RunErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		bClose = pErrorCodeThread->m_pThread->m_bClose;
		bWork = pErrorCodeThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			// 处理误码查询应答帧
			ProcErrorCodeReturnFrame(pErrorCodeThread);
			// 发送误码查询帧
			ProcErrorCodeQueryFrame(pErrorCodeThread);
		}
		EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		bClose = pErrorCodeThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		if (bClose == true)
		{
			break;
		}
		WaitErrorCodeThread(pErrorCodeThread);
	}
	EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pErrorCodeThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	return 1;
}
// 初始化误码查询线程
bool OnInitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	ASSERT(pErrorCodeThread != NULL);
	EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	if (false == OnInitThread(pErrorCodeThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pErrorCodeThread->m_pThread->m_hThreadClose);
	// 创建线程
	pErrorCodeThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunErrorCodeThread,
		pErrorCodeThread, 
		0, 
		&pErrorCodeThread->m_pThread->m_dwThreadID);
	if (pErrorCodeThread->m_pThread->m_hThread == NULL)
	{
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", 
			"pErrorCodeThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", 
		"误码查询线程创建成功");
	return true;
}
// 初始化误码查询线程
bool OnInit_ErrorCodeThread(m_oEnvironmentStruct* pEnv)
{
	ASSERT(pEnv != NULL);
	pEnv->m_pErrorCodeThread->m_pLogOutPutErrorCode = pEnv->m_pLogOutPutErrorCode;
	pEnv->m_pErrorCodeThread->m_pErrorCodeFrame = pEnv->m_pErrorCodeFrame;
	pEnv->m_pErrorCodeThread->m_pLineList = pEnv->m_pLineList;
	return OnInitErrorCodeThread(pEnv->m_pErrorCodeThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭误码查询线程
bool OnCloseErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	if (false == OnCloseThread(pErrorCodeThread->m_pThread))
	{
		LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", 
			"误码查询线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", 
		"误码查询线程成功关闭");
	return true;
}
// 释放误码查询线程
void OnFreeErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	ASSERT(pErrorCodeThread != NULL);
	if (pErrorCodeThread->m_pThread != NULL)
	{
		delete pErrorCodeThread->m_pThread;
		pErrorCodeThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	delete pErrorCodeThread;
	pErrorCodeThread = NULL;
}