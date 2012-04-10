#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建尾包接收线程
m_oTailFrameThreadStruct* OnCreateTailFrameThread(void)
{
	m_oTailFrameThreadStruct* pTailFrameThread = NULL;
	pTailFrameThread = new m_oTailFrameThreadStruct;
	pTailFrameThread->m_pThread = new m_oThreadStruct;
	pTailFrameThread->m_pInstrumentList = NULL;
	pTailFrameThread->m_pRoutList = NULL;
	pTailFrameThread->m_pTailFrame = NULL;
	InitializeCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	return pTailFrameThread;
}
// 线程等待函数
void WaitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pTailFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		bClose = pTailFrameThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pTailFrameThread->m_pThread->m_pConstVar->m_iTailFrameSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 清除与该仪器路由方向相同的之前仪器的尾包计数
void OnClearSameRoutTailCount(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pInstrument == NULL) || (pRout == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnClearSameRoutTailCount", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 下一个仪器指针为空
	m_oInstrumentStruct* pInstrumentNext = NULL;
	pInstrumentNext = GetNextInstrument(pInstrument->m_iRoutDirection, pRout->m_pHead, pConstVar);
	while(pInstrument != pInstrumentNext)
	{
		if (pInstrumentNext == NULL)
		{
			break;
		}
		pInstrumentNext->m_iTailFrameCount = 0;
		pInstrumentNext = GetNextInstrument(pInstrumentNext->m_iRoutDirection, pInstrumentNext, pConstVar);
	}
}
// 回收一个路由
void FreeRoutFromMap(unsigned int uiRoutIP, m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return;
	}
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	pRout = GetRout(uiRoutIP, &pRoutList->m_oRoutMap);
	AddRout(uiRoutIP, pRout, &pRoutList->m_oRoutDeleteMap);
}
// 回收一个仪器
void FreeInstrumentFromMap(m_oInstrumentStruct* pInstrument, 
	m_oInstrumentListStruct* pInstrumentList, 
	m_oRoutListStruct* pRoutList, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pInstrumentList == NULL) || (pRoutList == NULL) || (pInstrument == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "FreeInstrumentFromMap", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	string strConv = "";
	// 从SN索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiSN, &pInstrumentList->m_oSNInstrumentMap);
	// 从IP地址设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPSetMap);
	// 从IP地址索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPInstrumentMap);
	// 从ADC参数设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oADCSetInstrumentMap);
	str.Format(_T("删除仪器的SN = 0x%x，路由 = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "FreeInstrumentFromMap", strConv);

	// 删除仪器的类型为LCI或交叉站
	if ((pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		|| (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
	{
		if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		{
			FreeRoutFromMap(pInstrument->m_uiRoutIP, pRoutList);
		}
		// 仪器上方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPTop, pRoutList);
		// 仪器下方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPDown, pRoutList);
		// 仪器左方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPLeft, pRoutList);
		// 仪器右方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPRight, pRoutList);
	}
	// 将仪器加入到空闲仪器队列
	AddFreeInstrument(pInstrument, pInstrumentList);
	// 系统发生变化的时间
	UpdateLineChangeTime(pInstrumentList);
}
// 删除该路由方向指定仪器之后的仪器
void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, 
	m_oInstrumentListStruct* pInstrumentList, 
	m_oRoutListStruct* pRoutList, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pRout == NULL) || (pInstrument == NULL)
		|| (pInstrumentList == NULL) || (pRoutList == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "DeleteInstrumentAlongRout", 
			"", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 前一个仪器指针为路由尾仪器
	m_oInstrumentStruct* pInstrumentPrevious = pRout->m_pTail;
	m_oInstrumentStruct* pInstrumentDelete = NULL;
	CString str = _T("");
	while (pInstrument != pInstrumentPrevious)
	{
		if (pInstrumentPrevious == NULL)
		{
			break;
		}
		pInstrumentDelete = pInstrumentPrevious;
		// 得到要删除仪器沿着路由方向的前一个仪器的指针
		pInstrumentPrevious = GetPreviousInstrument(pInstrumentDelete->m_iRoutDirection, 
			pInstrumentDelete, pConstVar);
		pRout->m_pTail = pInstrumentPrevious;
		if (pRout->m_pTail == pRout->m_pHead)
		{
			pRout->m_pTail = NULL;
		}
		if (pInstrumentDelete->m_bIPSetOK == true)
		{
			pRout->m_uiInstrumentNum--;
		}
		// 回收一个仪器
		FreeInstrumentFromMap(pInstrumentDelete, pInstrumentList, pRoutList, pConstVar);
		if (pRout->m_olsRoutInstrument.size() > 0)
		{
			pRout->m_olsRoutInstrument.pop_back();
		}
	}
}
// 处理单个尾包帧
void ProcTailFrameOne(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	m_oRoutStruct* pRout = NULL;
	hash_map <unsigned int, m_oRoutStruct*>  ::iterator iter;
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiSN = 0;
	unsigned int uiRoutIP = 0;
	EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	uiSN = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSN;
	uiRoutIP = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP;
	LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	str.Format(_T("接收到SN = 0x%x，路由 = 0x%x 的仪器的尾包"), uiSN, uiRoutIP);
	// 判断仪器SN是否在SN索引表中
	if(TRUE == IfIndexExistInMap(uiSN, &pTailFrameThread->m_pInstrumentList->m_oSNInstrumentMap))
	{
		// 在索引表中则找到该仪器,得到该仪器指针
		pInstrument = GetInstrumentFromMap(uiSN, &pTailFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
		// 仪器设置IP成功且路由地址发生变化
		if ((pInstrument->m_bIPSetOK == true) && (pInstrument->m_uiRoutIP != uiRoutIP))
		{
			GetFrameInfo(pTailFrameThread->m_pTailFrame->m_cpRcvFrameData,
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne",
				strFrameData, ErrorType, IDS_ERR_ROUT_CHANGE);
			return;
		}
		// 尾包计数器加一
		pInstrument->m_iTailFrameCount++;
		// 		// 更新尾包仪器的尾包时刻
		// 		pInstrument->m_uiTailSysTime = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSysTime;
		// 在路由索引表中找到该尾包所在的路由
		if (TRUE == IfIndexExistInRoutMap(uiRoutIP, &pTailFrameThread->m_pRoutList->m_oRoutMap))
		{
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", 
				strConv);
			// 由路由IP得到路由对象
			pRout = GetRout(uiRoutIP, &pTailFrameThread->m_pRoutList->m_oRoutMap);
			// 更新路由对象的路由时间
			UpdateRoutTime(pRout);
			// 清除与该仪器路由方向相同的之前仪器的尾包计数
			OnClearSameRoutTailCount(pInstrument, pRout, pTailFrameThread->m_pThread->m_pConstVar);
			// 判断尾包计数器达到设定值则删除相同路由之后的仪器
			if (pInstrument->m_iTailFrameCount > pTailFrameThread->m_pThread->m_pConstVar->m_iTailFrameStableTimes)
			{
				if (pRout->m_pTail != NULL)
				{
					DeleteInstrumentAlongRout(pInstrument, pRout, pTailFrameThread->m_pInstrumentList, 
						pTailFrameThread->m_pRoutList, pTailFrameThread->m_pThread->m_pConstVar);
					while(true)
					{
						if (pTailFrameThread->m_pRoutList->m_oRoutDeleteMap.empty() == true)
						{
							break;
						}
						iter = pTailFrameThread->m_pRoutList->m_oRoutDeleteMap.begin();
						DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, 
							pTailFrameThread->m_pInstrumentList, pTailFrameThread->m_pRoutList, 
							pTailFrameThread->m_pThread->m_pConstVar);
						// 路由索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oRoutMap);
						str.Format(_T("回收路由IP = 0x%x的过期路由"), iter->first);
						ConvertCStrToStr(str, &strConv);
						AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, 
							"ProcTailFrameOne", strConv);
						// 将过期路由回收到空闲路由队列
						AddFreeRout(iter->second, pTailFrameThread->m_pRoutList);
						// 路由删除索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oRoutDeleteMap);
						// ADC参数设置索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oADCSetRoutMap);
					}
				}
				pInstrument->m_iTailFrameCount = 0;
			}
		}
		else
		{
			GetFrameInfo(pTailFrameThread->m_pTailFrame->m_cpRcvFrameData,
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne",
				strFrameData,	ErrorType, IDS_ERR_ROUT_NOTEXIT);
		}
	}
	else
	{
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, 
			"ProcTailFrameOne", strConv);
	}
}
// 处理尾包帧
void ProcTailFrame(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
		pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTailFrameThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
				pTailFrameThread->m_pTailFrame->m_cpRcvFrameData, 
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTailFrameThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTailFrame(pTailFrameThread->m_pTailFrame, 
					pTailFrameThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, 
						"ParseInstrumentTailFrame", "", ErrorType, IDS_ERR_PARSE_TAILFRAME);
				}
				else
				{
					EnterCriticalSection(&pTailFrameThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pTailFrameThread->m_pRoutList->m_oSecRoutList);
					// 处理单个尾包帧
					ProcTailFrameOne(pTailFrameThread);
					LeaveCriticalSection(&pTailFrameThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pTailFrameThread->m_pInstrumentList->m_oSecInstrumentList);	
				}	
			}		
		}		
	}
}
// 线程函数
DWORD WINAPI RunTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		if (pTailFrameThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
			break;
		}
		if (pTailFrameThread->m_pThread->m_bWork == true)
		{
			// 处理尾包接收
			ProcTailFrame(pTailFrameThread);
		}
		if (pTailFrameThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
			break;
		}
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		WaitTailFrameThread(pTailFrameThread);
	}
	EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pTailFrameThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	return 1;
}
// 初始化尾包接收线程
bool OnInitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	if (false == OnInitThread(pTailFrameThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pTailFrameThread->m_pThread->m_hThreadClose);
	// 创建线程
	pTailFrameThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunTailFrameThread,
		pTailFrameThread, 
		0, 
		&pTailFrameThread->m_pThread->m_dwThreadID);
	if (pTailFrameThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
			"pTailFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		return false;
	}
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
		"尾包接收线程创建成功");
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	return true;
}
// 初始化尾包接收线程
bool OnInit_TailFrameThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pTailFrameThread->m_pTailFrame = pEnv->m_pTailFrame;
	pEnv->m_pTailFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pTailFrameThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitTailFrameThread(pEnv->m_pTailFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭尾包接收线程
bool OnCloseTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	if (false == OnCloseThread(pTailFrameThread->m_pThread))
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
			"尾包接收线程强制关闭", WarningType);
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		return false;
	}
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
		"尾包接收线程成功关闭");
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	return true;
}
// 释放尾包接收线程
void OnFreeTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	if (pTailFrameThread->m_pThread != NULL)
	{
		delete pTailFrameThread->m_pThread;
	}
	DeleteCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	delete pTailFrameThread;
}