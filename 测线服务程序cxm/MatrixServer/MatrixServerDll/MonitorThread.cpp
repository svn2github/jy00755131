#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建路由监视线程
m_oMonitorThreadStruct* OnCreateMonitorThread(void)
{
	m_oMonitorThreadStruct* pMonitorThread = NULL;
	pMonitorThread = new m_oMonitorThreadStruct;
	pMonitorThread->m_pThread = new m_oThreadStruct;
	pMonitorThread->m_pTimeDelayThread = NULL;
	pMonitorThread->m_pADCSetThread = NULL;
	pMonitorThread->m_pErrorCodeThread = NULL;
	pMonitorThread->m_pInstrumentList = NULL;
	pMonitorThread->m_pRoutList = NULL;
	InitializeCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	return pMonitorThread;
}
// 线程等待函数
void WaitMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pMonitorThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		bClose = pMonitorThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pMonitorThread->m_pThread->m_pConstVar->m_iMonitorSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 沿着路由方向得到时统任务
void GetTimeDelayTaskAlongRout(m_oRoutStruct* pRout, 
	m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pRoutList == NULL) || (pRout == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTaskAlongRout", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		pInstrument = pInstrumentNext;
		// 如果仪器为交叉站
		if ((pInstrumentNext->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrumentNext->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
		{
			// 将仪器四个方向的路由加入临时队列
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPTop);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPDown);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPLeft);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPRight);
		}
	} while (pInstrumentNext != pRout->m_pTail);
}
// 得到时统任务
void GetTimeDelayTask(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTask", "", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oRoutStruct* pRout = NULL;
	unsigned int uiRoutIP = 0;
	CString str = _T("");
	string strConv = "";
	while(false == pRoutList->m_olsTimeDelayTemp.empty())
	{
		uiRoutIP = *pRoutList->m_olsTimeDelayTemp.begin();
		pRoutList->m_olsTimeDelayTemp.pop_front();
		// 判断该路由是否在路由索引表中
		if (TRUE == IfIndexExistInRoutMap(uiRoutIP, &pRoutList->m_oRoutMap))
		{
			// 得到路由指针
			pRout = GetRout(uiRoutIP, &pRoutList->m_oRoutMap);
			if (pRout->m_pTail != NULL)
			{
				// 不是LCI
				if (uiRoutIP != 0)
				{
					// 将该路由IP加入任务队列
					pRoutList->m_olsTimeDelayTaskQueue.push_back(uiRoutIP);
					str.Format(_T("将路由IP = 0x%x 加入时统任务队列"), uiRoutIP);
					ConvertCStrToStr(str, &strConv);
					AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTask", strConv);
				}
				// 沿着路由方向得到时统任务
				GetTimeDelayTaskAlongRout(pRout, pRoutList, pConstVar);
			}
		}
		else
		{
			// 不是LCI路由
			if (uiRoutIP != 0)
			{
				str.Format(_T("路由IP = 0x%x"), uiRoutIP);
				ConvertCStrToStr(str, &strConv);
				AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTask", strConv,
					ErrorType, IDS_ERR_ROUT_NOTEXIT);
			}
		}
	}
}
// 生成时统任务队列
void GenTimeDelayTaskQueue(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GenTimeDelayTaskQueue", "", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 清空任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 将LCI路由加入临时队列
	pRoutList->m_olsTimeDelayTemp.push_back(0);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GenTimeDelayTaskQueue", "当前系统稳定，生成时统任务队列");
	// 得到时统任务
	GetTimeDelayTask(pRoutList, pConstVar);
}
// 判断路由方向上是否有采集站
void GetADCTaskQueueByRout(m_oADCSetThreadStruct* pADCSetThread, 
	m_oRoutStruct* pRout, int iOpt)
{
	if ((pADCSetThread == NULL) || (pRout == NULL) || (pRout->m_pTail == NULL))
	{
		return;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	bool bRoutADCSet = true;
	bool bADCSet = true;
	bool bADCStartSample = false;
	bool bADCStopSample = false;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	bADCStartSample = pADCSetThread->m_bADCStartSample;
	bADCStopSample = pADCSetThread->m_bADCStopSample;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
			pADCSetThread->m_pThread->m_pConstVar);
		if (pInstrumentNext->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
		{
			if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb)
			{
				if (false == pInstrumentNext->m_bADCSet)
				{
					bADCSet = false;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb)
			{
				if ((true == bADCStartSample)
					&& (false == pInstrumentNext->m_bADCStartSample))
				{
					bADCSet = false;
					// 实际接收ADC数据帧数
					pInstrumentNext->m_uiADCDataActualRecFrameNum = 0;
					// 重发查询帧得到的应答帧数
					pInstrumentNext->m_uiADCDataRetransmissionFrameNum = 0;
					// 应该接收ADC数据帧数（含丢帧）
					pInstrumentNext->m_uiADCDataShouldRecFrameNum = 0;
					// ADC数据帧的指针偏移量
					pInstrumentNext->m_usADCDataFramePoint = 0;
					// ADC数据帧发送时的本地时间
					pInstrumentNext->m_uiADCDataFrameSysTime = 0;
					// ADC数据帧起始帧数
					pInstrumentNext->m_iADCDataFrameStartNum = 0;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb)
			{
				if ((true == bADCStopSample)
					&& (false == pInstrumentNext->m_bADCStopSample))
				{
					bADCSet = false;
				}
				else
				{
					bADCSet = true;
				}
			}
			if (bADCSet == false)
			{
				pInstrumentNext->m_bADCSetReturn = false;
				AddInstrumentToMap(pInstrumentNext->m_uiIP, pInstrumentNext, 
					&pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
			else
			{
				DeleteInstrumentFromMap(pInstrumentNext->m_uiIP, 
					&pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
				bRoutADCSet = false;
			}
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);
	if (bRoutADCSet == true)
	{
		// 将路由加入索引
		pRout->m_bADCSetReturn = false;
		pRout->m_bADCSetRout = true;
		AddRout(pRout->m_uiRoutIP, pRout, &pADCSetThread->m_pRoutList->m_oADCSetRoutMap);
		// 从仪器索引表删除该路由的仪器
		pInstrument = pRout->m_pHead;
		do 
		{
			pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
				pADCSetThread->m_pThread->m_pConstVar);
			if (pInstrumentNext->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
			{
				DeleteInstrumentFromMap(pInstrumentNext->m_uiIP, 
					&pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
			pInstrument = pInstrumentNext;
		} while (pInstrumentNext != pRout->m_pTail);
	}
	else
	{
		DeleteRout(pRout->m_uiRoutIP, &pADCSetThread->m_pRoutList->m_oADCSetRoutMap);
	}
}
// 生成ADC参数设置任务队列
void GetADCTaskQueue(m_oADCSetThreadStruct* pADCSetThread, int iOpt)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	for (iter = pADCSetThread->m_pRoutList->m_oRoutMap.begin(); 
		iter != pADCSetThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		// 将路由索引表中的大线方向路由加入到ADC参数设置任务索引
		if (iter->second->m_bRoutLaux == false)
		{
			GetADCTaskQueueByRout(pADCSetThread, iter->second, iOpt);
		}
	}
}
// 自动开始ADC参数设置
void OnADCCmdAuto(m_oADCSetThreadStruct* pADCSetThread, int iOpt)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 生成ADC参数设置任务队列
	GetADCTaskQueue(pADCSetThread, iOpt);
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	if ((false == pADCSetThread->m_pRoutList->m_oADCSetRoutMap.empty())
		|| (false == pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.empty()))
	{
		// 进行ADC参数设置
		pADCSetThread->m_uiCounter = 0;
		if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 1;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 12;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 19;
		}
		// ADC参数设置线程开始工作
		pADCSetThread->m_pThread->m_bWork = true;
	}
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
}
// ADC参数设置
void OnADCSetAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb);
}
// ADC开始采集命令
void OnADCStartSampleAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb);
}
// ADC停止采集命令
void OnADCStopSampleAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb);
}
// 监视过期路由及仪器
void MonitorRoutAndInstrument(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	unsigned int uiTimeNow = 0;
	uiTimeNow = GetTickCount();
	// hash_map迭代器
	hash_map<unsigned int, m_oRoutStruct*>::iterator  iter;
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pMonitorThread->m_pInstrumentList->m_oSecInstrumentList);
	EnterCriticalSection(&pMonitorThread->m_pRoutList->m_oSecRoutList);
	// 删除过期仪器，将过期路由加入路由删除索引表
	for(iter = pMonitorThread->m_pRoutList->m_oRoutMap.begin();
		iter != pMonitorThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		if ((uiTimeNow > (iter->second->m_uiRoutTime + pMonitorThread->m_pThread->m_pConstVar->m_iMonitorStableTime))
			&& (iter->second->m_pTail != NULL))
		{
			str.Format(_T("路由IP = 0x%x的路由时间过期，删除该路由方向上的仪器"), iter->second->m_uiRoutIP);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "ProcMonitor", strConv);
			// 如果路由尾仪器为LCI则表明要删除的是LCI本身
			if (iter->second->m_pTail->m_iInstrumentType == pMonitorThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			{
				// 回收一个仪器
				if (iter->second->m_olsRoutInstrument.size() > 0)
				{
					iter->second->m_olsRoutInstrument.pop_back();
				}
				FreeInstrumentFromMap(iter->second->m_pHead, pMonitorThread->m_pInstrumentList, 
					pMonitorThread->m_pRoutList, pMonitorThread->m_pThread->m_pConstVar);
				iter->second->m_uiInstrumentNum = 0;
			}
			else
			{
				DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, 
					pMonitorThread->m_pInstrumentList, pMonitorThread->m_pRoutList, 
					pMonitorThread->m_pThread->m_pConstVar);
			}
		}
	}
	// 删除路由删除索引表中的仪器
	while(1)
	{
		if (pMonitorThread->m_pRoutList->m_oRoutDeleteMap.empty() == true)
		{
			break;
		}
		iter = pMonitorThread->m_pRoutList->m_oRoutDeleteMap.begin();
		DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, pMonitorThread->m_pInstrumentList, 
			pMonitorThread->m_pRoutList, pMonitorThread->m_pThread->m_pConstVar);
		// 路由索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oRoutMap);
		str.Format(_T("回收路由IP = 0x%x的过期路由"), iter->first);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "ProcMonitor", strConv);
		// 将过期路由回收到空闲路由队列
		AddFreeRout(iter->second, pMonitorThread->m_pRoutList);
		// ADC参数设置索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oADCSetRoutMap);
		// 路由删除索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oRoutDeleteMap);
	}
	LeaveCriticalSection(&pMonitorThread->m_pRoutList->m_oSecRoutList);
	LeaveCriticalSection(&pMonitorThread->m_pInstrumentList->m_oSecInstrumentList);
}
// 检查时统设置应答是否接收完全
bool CheckTimeDelayReturnByRout(m_oRoutStruct* pRout, 
	m_oTimeDelayThreadStruct* pTimeDelayThread, bool bSample)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return false;
	}
	if ((pRout->m_pTail == NULL) || (pRout->m_pTail == pRout->m_pHead))
	{
		return true;
	}
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strConv = "";
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
			pTimeDelayThread->m_pThread->m_pConstVar);
		pInstrument = pInstrumentNext;
		if (bSample == true)
		{
			if (pInstrumentNext->m_iTimeSetReturnCount == 0)
			{
				str.Format(_T("数据采样过程中，路由IP = 0x%x的仪器的没有全部实现时统"), 
					pRout->m_uiRoutIP);
				ConvertCStrToStr(str, &strConv);
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
					"CheckTimeDelayReturnByRout", strConv, WarningType);
				return false;
			}
		}
		else
		{
			/** 时统设置是否成功*/
			if (pInstrumentNext->m_bTimeSetOK == false)
			{
				str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收不完全"), pRout->m_uiRoutIP);
				ConvertCStrToStr(str,&strConv);
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
					"CheckTimeDelayReturnByRout", strConv);
				return false;
			}
		}
	} while (pInstrumentNext != pRout->m_pTail);
	if (bSample == false)
	{
		str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收完全"), pRout->m_uiRoutIP);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, 
			"CheckTimeDelayReturnByRout", strConv);
	}
	return true;
}
// 检查仪器是否都完成了时统
bool CheckTimeDelayReturn(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	// hash_map迭代器
	hash_map<unsigned int, m_oRoutStruct*>::iterator  iter;
	for(iter = pTimeDelayThread->m_pRoutList->m_oRoutMap.begin(); 
		iter != pTimeDelayThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		if (false == CheckTimeDelayReturnByRout(iter->second, pTimeDelayThread, true))
		{
			return false;
		}
	}
	return true;
}
// 监视时统
void MonitorTimeDelay(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	unsigned int uiTimeNow = 0;
	unsigned int uiLineChangeTime = 0;
	bool bLineStableChange = false;
	uiTimeNow = GetTickCount();
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	EnterCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
	EnterCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
	uiLineChangeTime = pTimeDelayThread->m_pInstrumentList->m_uiLineChangeTime;
	bLineStableChange = pTimeDelayThread->m_pInstrumentList->m_bLineStableChange;
	// 判断系统稳定
	if (uiTimeNow > (uiLineChangeTime + pTimeDelayThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 测网状态由不稳定变为稳定
		if (bLineStableChange == false)
		{
			pTimeDelayThread->m_pInstrumentList->m_bLineStableChange = true;
			// 生成时统任务队列
			GenTimeDelayTaskQueue(pTimeDelayThread->m_pRoutList, pTimeDelayThread->m_pThread->m_pConstVar);
			// 清空尾包时刻查询帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize);
			// 清空时统设置应答帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize);
			pTimeDelayThread->m_uiCounter = 0;
			// 时统设置线程开始工作
			pTimeDelayThread->m_pThread->m_bWork = true;
		}
		else
		{
			// 采集过程中，如果仪器都完成了时统则停止时统线程
			if (pTimeDelayThread->m_bADCStartSample == true)
			{
				if (true == CheckTimeDelayReturn(pTimeDelayThread))
				{
					// 时统设置线程停止工作
					pTimeDelayThread->m_pThread->m_bWork = false;
				}
			}
		}
	} 
	else
	{
		// 时统设置线程停止工作
		pTimeDelayThread->m_pThread->m_bWork = false;
		// 测网状态为不稳定
		pTimeDelayThread->m_pInstrumentList->m_bLineStableChange = false;
	}
	LeaveCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
	LeaveCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
}
// 监视仪器的ADC参数设置
void MonitorADCSet(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	unsigned int uiTimeNow = 0;
	unsigned int uiLineChangeTime = 0;
	uiTimeNow = GetTickCount();
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
	EnterCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
	uiLineChangeTime = pADCSetThread->m_pInstrumentList->m_uiLineChangeTime;
	// 判断系统稳定
	if (uiTimeNow > (uiLineChangeTime + pADCSetThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 自动进行未完成的ADC参数设置
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCSetAuto(pADCSetThread);
		}
		// 自动进行未完成的开始ADC数据采集
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCStartSampleAuto(pADCSetThread);
		}
		// 自动进行未完成的停止ADC数据采集
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCStopSampleAuto(pADCSetThread);
		}
	}
	LeaveCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
	LeaveCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
}
// 监视仪器的误码
void MonitorErrorCode(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	unsigned int uiTimeNow = 0;
	unsigned int uiLineChangeTime = 0;
	uiTimeNow = GetTickCount();
	EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
	EnterCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
	uiLineChangeTime = pErrorCodeThread->m_pInstrumentList->m_uiLineChangeTime;
	// 判断系统稳定
	if (uiTimeNow > (uiLineChangeTime + pErrorCodeThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 误码查询线程开始工作
		pErrorCodeThread->m_pThread->m_bWork = true;
	}
	else
	{
		// 误码查询线程停止工作
		pErrorCodeThread->m_pThread->m_bWork = false;
	}
	LeaveCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
	LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
}
// 处理路由监视
void ProcMonitor(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	
	// 监视过期路由及仪器
	MonitorRoutAndInstrument(pMonitorThread);
	// 系统稳定且不进行ADC数据采集时进行时统
	MonitorTimeDelay(pMonitorThread->m_pTimeDelayThread);
 	// 系统稳定则对仪器进行ADC参数设置
 	MonitorADCSet(pMonitorThread->m_pADCSetThread);
	// 系统稳定则进行误码查询
	MonitorErrorCode(pMonitorThread->m_pErrorCodeThread);
}
// 线程函数
DWORD WINAPI RunMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		if (pMonitorThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
			break;
		}
		if (pMonitorThread->m_pThread->m_bWork == true)
		{
			// 处理路由监视
			ProcMonitor(pMonitorThread);
		}
		if (pMonitorThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
			break;
		}
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		WaitMonitorThread(pMonitorThread);
	}
	EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pMonitorThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	return 1;
}
// 初始化路由监视线程
bool OnInitMonitorThread(m_oMonitorThreadStruct* pMonitorThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pMonitorThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	if (false == OnInitThread(pMonitorThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pMonitorThread->m_pThread->m_hThreadClose);
	// 创建线程
	pMonitorThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunMonitorThread,
		pMonitorThread, 
		0, 
		&pMonitorThread->m_pThread->m_dwThreadID);
	if (pMonitorThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", 
			"pMonitorThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		return false;
	}
	AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", 
		"路由监视线程创建成功");
	LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	return true;
}
// 初始化路由监视线程
bool OnInit_MonitorThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pMonitorThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pMonitorThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pMonitorThread->m_pTimeDelayThread = pEnv->m_pTimeDelayThread;
	pEnv->m_pMonitorThread->m_pADCSetThread = pEnv->m_pADCSetThread;
	pEnv->m_pMonitorThread->m_pErrorCodeThread = pEnv->m_pErrorCodeThread;
	return OnInitMonitorThread(pEnv->m_pMonitorThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭路由监视线程
bool OnCloseMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	if (false == OnCloseThread(pMonitorThread->m_pThread))
	{
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", 
			"路由监视线程强制关闭", WarningType);
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		return false;
	}
	AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", 
		"路由监视线程成功关闭");
	LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	return true;
}
// 释放路由监视线程
void OnFreeMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	if (pMonitorThread->m_pThread != NULL)
	{
		delete pMonitorThread->m_pThread;
	}
	DeleteCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	delete pMonitorThread;
}