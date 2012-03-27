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
	pErrorCodeThread->m_pRoutList = NULL;
	pErrorCodeThread->m_pInstrumentList = NULL;
	return pErrorCodeThread;
}
// 线程等待函数
void WaitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pErrorCodeThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pErrorCodeThread->m_pThread->m_bClose == true)
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
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOutPut = _T("");
	string strFrameData = "";
	string strConv = "";
	char byTemp = 0;
	int iTemp = 0;
	// 得到仪器IP
	uiIPInstrument = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pErrorCodeThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, 
			&pErrorCodeThread->m_pInstrumentList->m_oIPInstrumentMap);
		str.Format(_T("仪器SN = 0x%x，IP = 0x%x；"), pInstrument->m_uiSN, pInstrument->m_uiIP);
		strOutPut += str;
		// 仪器类型为LCI或者交叉站
		if ((pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 交叉站大线A数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLineACount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cLAUXErrorCodeDataLineACountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cLAUXErrorCodeDataLineACountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cLAUXErrorCodeDataLineACountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLineACount += iTemp;
			}
			pInstrument->m_cLAUXErrorCodeDataLineACountOld = byTemp;
			str.Format(_T("大线A数据故障数=%d，"),iTemp);
			strOutPut += str;
			// 交叉站大线B数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLineBCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cLAUXErrorCodeDataLineBCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cLAUXErrorCodeDataLineBCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cLAUXErrorCodeDataLineBCountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLineBCount += iTemp;
			}
			pInstrument->m_cLAUXErrorCodeDataLineBCountOld = byTemp;
			str.Format(_T("大线B数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站交叉线A数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLAUXLineACount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLAUXLineACount += iTemp;
			}
			pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld = byTemp;
			str.Format(_T("交叉线A数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站交叉线B数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeDataLAUXLineBCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount += iTemp;
			}
			pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld = byTemp;
			str.Format(_T("交叉线B数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站命令口故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cLAUXErrorCodeCmdCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cLAUXErrorCodeCmdCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cLAUXErrorCodeCmdCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cLAUXErrorCodeCmdCountOld;
				}
				// LCI不计算命令误码
				if (pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
				{
					iTemp = 0;
				}
				pInstrument->m_iLAUXErrorCodeCmdCount += iTemp;
			}
			pInstrument->m_cLAUXErrorCodeCmdCountOld = byTemp;
			str.Format(_T("命令口故障数=%d；"), iTemp);
			strOutPut += str;
		}
		// 仪器类型为采集站或者电源站
		else
		{
			// 采集站和电源站网络数据错误计数
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cFDUErrorCodeDataCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cFDUErrorCodeDataCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cFDUErrorCodeDataCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cFDUErrorCodeDataCountOld;
				}
				pInstrument->m_iFDUErrorCodeDataCount += iTemp;
			}
			pInstrument->m_cFDUErrorCodeDataCountOld = byTemp;
			str.Format(_T("网络数据错误计数=%d，"), iTemp);
			strOutPut += str;
			// 采集站和电源站命令错误计数
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_cFDUErrorCodeCmdCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_cFDUErrorCodeCmdCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_cFDUErrorCodeCmdCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_cFDUErrorCodeCmdCountOld;
				}
				pInstrument->m_iFDUErrorCodeCmdCount += iTemp;
			}
			pInstrument->m_cFDUErrorCodeCmdCountOld = byTemp;
			str.Format(_T("命令错误计数=%d；"), iTemp);
			strOutPut += str;
		}
		pInstrument->m_uiErrorCodeReturnNum++;
		str.Format(_T("接收帧数=%d"), pInstrument->m_uiErrorCodeReturnNum);
		strOutPut += str;
		ConvertCStrToStr(strOutPut, &strConv);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", strConv);
	}
	else
	{
		GetFrameInfo(pErrorCodeThread->m_pErrorCodeFrame->m_cpRcvFrameData,
			pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "ProcErrorCodeReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPSETMAP_NOTEXIT);
	}
}
// 处理误码查询应答帧
void ProcErrorCodeReturnFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
		pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pErrorCodeThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
				pErrorCodeThread->m_pErrorCodeFrame->m_cpRcvFrameData, 
				pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pErrorCodeThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentErrorCodeReturnFrame(pErrorCodeThread->m_pErrorCodeFrame, 
					pErrorCodeThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, 
						"ParseInstrumentErrorCodeReturnFrame","", 
						ErrorType, IDS_ERR_PARSE_ERRORCODERETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个IP地址设置应答帧
					ProcErrorCodeReturnFrameOne(pErrorCodeThread);
					LeaveCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 发送误码查询帧
void ProcErrorCodeQueryFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	m_oRoutStruct* pRout = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pErrorCodeThread->m_pRoutList->m_oSecRoutList);
	for (iter = pErrorCodeThread->m_pRoutList->m_oRoutMap.begin();
		iter != pErrorCodeThread->m_pRoutList->m_oRoutMap.end();
		iter++)
	{
		pRout = iter->second;
		if (pRout->m_pTail != NULL)
		{
			pInstrument = pRout->m_pHead;
			do 
			{
				pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
					pErrorCodeThread->m_pThread->m_pConstVar);
				pInstrumentNext->m_uiErrorCodeQueryNum++;
				pInstrument = pInstrumentNext;
			} while (pInstrumentNext != pRout->m_pTail);
			// 广播发送误码查询帧
			MakeInstrumentErrorCodeQueryFrame(pErrorCodeThread->m_pErrorCodeFrame,
				pErrorCodeThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort);
			SendInstrumentErrorCodeFrame(pErrorCodeThread->m_pErrorCodeFrame,
				pErrorCodeThread->m_pThread->m_pConstVar);
			str.Format(_T("向路由IP = 0x%x广播发送误码查询帧"), pRout->m_uiRoutIP);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", strConv);
		}
	}
	LeaveCriticalSection(&pErrorCodeThread->m_pRoutList->m_oSecRoutList);
}
// 线程函数
DWORD WINAPI RunErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pErrorCodeThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pErrorCodeThread->m_pThread->m_bWork == true)
		{
			// 处理误码查询应答帧
			ProcErrorCodeReturnFrame(pErrorCodeThread);
			// 发送误码查询帧
			ProcErrorCodeQueryFrame(pErrorCodeThread);
		}
		if (pErrorCodeThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitErrorCodeThread(pErrorCodeThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pErrorCodeThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化误码查询线程
bool OnInitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pErrorCodeThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pErrorCodeThread->m_pThread, pLogOutPut, pConstVar))
	{
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
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", 
			"pErrorCodeThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", 
		"误码查询线程创建成功");
	return true;
}
// 初始化误码查询线程
bool OnInit_ErrorCodeThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pErrorCodeThread->m_pLogOutPutErrorCode = pEnv->m_pLogOutPutErrorCode;
	pEnv->m_pErrorCodeThread->m_pErrorCodeFrame = pEnv->m_pErrorCodeFrame;
	pEnv->m_pErrorCodeThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pErrorCodeThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	return OnInitErrorCodeThread(pEnv->m_pErrorCodeThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭误码查询线程
bool OnCloseErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pErrorCodeThread->m_pThread))
	{
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", 
			"误码查询线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", 
			"误码查询线程成功关闭");
		return true;
	}
}
// 释放误码查询线程
void OnFreeErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	if (pErrorCodeThread->m_pThread != NULL)
	{
		delete pErrorCodeThread->m_pThread;
	}
	delete pErrorCodeThread;
}