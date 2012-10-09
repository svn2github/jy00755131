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
	pMonitorThread->m_pIPSetFrame = NULL;
	pMonitorThread->m_pLineList = NULL;
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
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection, pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		// 如果仪器为交叉站
		if ((pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
		{
			// 将仪器四个方向的路由加入临时队列
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrument->m_uiRoutIPTop);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrument->m_uiRoutIPDown);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrument->m_uiRoutIPLeft);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrument->m_uiRoutIPRight);
		}
	} while (pInstrument != pRout->m_pTail);
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
					strConv = (CStringA)str;
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
				strConv = (CStringA)str;
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
// ADC参数设置线程开始工作
void OnADCSetThreadWork(int iOpt, m_oADCSetThreadStruct* pADCSetThread)
{
	bool bSetWork = false;
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	if ((false == pADCSetThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap.empty())
		|| (false == pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap.empty()))
	{
		bSetWork = true;
	}
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	if (bSetWork == true)
	{
		EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		// 进行ADC参数设置
		pADCSetThread->m_uiCounter = 0;
		if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb)
		{
			pADCSetThread->m_iADCSetOperationNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCSetCmdBeginNb;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb)
		{
			pADCSetThread->m_iADCSetOperationNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleBeginNb;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb)
		{
			pADCSetThread->m_iADCSetOperationNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleBeginNb;
		}
		// ADC参数设置线程开始工作
		pADCSetThread->m_pThread->m_bWork = true;
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	}
}
// 清除ADC参数设置任务索引
void OnClearADCSetMap(m_oLineListStruct* pLineLIst)
{
	if (pLineLIst == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLineLIst->m_oSecLineList);
	pLineLIst->m_pInstrumentList->m_oADCSetInstrumentMap.clear();
	pLineLIst->m_pRoutList->m_oADCSetRoutMap.clear();
	LeaveCriticalSection(&pLineLIst->m_oSecLineList);
}
// 将仪器加入ADC参数设置索引表
void GetADCTaskQueueBySN(bool bADCStartSample, bool bADCStopSample, m_oLineListStruct* pLineList, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument, int iOpt)
{
	if ((pLineList == NULL) || (pConstVar == NULL) || (pInstrument == NULL))
	{
		return;
	}
	CString str = _T("");
	string strConv = "";
	bool bAdd = false;
	EnterCriticalSection(&pLineList->m_oSecLineList);
	if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
	{
		if (iOpt == pConstVar->m_iADCSetOptNb)
		{
			if (false == pInstrument->m_bADCSet)
			{
				bAdd = true;
			}
		}
		else if (iOpt == pConstVar->m_iADCStartSampleOptNb)
		{
			if ((true == bADCStartSample)
				&& (false == pInstrument->m_bADCStartSample))
			{
				// 实际接收ADC数据帧数
				pInstrument->m_uiADCDataActualRecFrameNum = 0;
				// 重发查询帧得到的应答帧数
				pInstrument->m_uiADCDataRetransmissionFrameNum = 0;
				// 应该接收ADC数据帧数（含丢帧）
				pInstrument->m_uiADCDataShouldRecFrameNum = 0;
				// ADC数据帧的指针偏移量
				pInstrument->m_usADCDataFramePoint = 0;
				// ADC数据帧发送时的本地时间
				pInstrument->m_uiADCDataFrameSysTime = 0;
				// ADC数据帧起始帧数
				pInstrument->m_iADCDataFrameStartNum = 0;
				bAdd = true;
			}
		}
		else if (iOpt == pConstVar->m_iADCStopSampleOptNb)
		{
			if ((true == bADCStopSample)
				&& (false == pInstrument->m_bADCStopSample))
			{
				bAdd = true;
			}
		}
	}
	if (bAdd == true)
	{
		pInstrument->m_bADCSetReturn = false;
		AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, 
			&pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
		str.Format(_T("将仪器SN = 0x%x 加入ADC参数设置任务索引表"), pInstrument->m_uiSN);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetADCTaskQueueBySN", strConv);
	}
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 判断路由方向上是否有采集站
void GetADCTaskQueueByRout(bool bADCStartSample, bool bADCStopSample, m_oLineListStruct* pLineList, 
	m_oConstVarStruct* pConstVar, m_oRoutStruct* pRout, int iOpt)
{
	if ((pLineList == NULL) || (pConstVar == NULL)|| (pRout == NULL) || (pRout->m_pTail == NULL))
	{
		return;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	bool bRoutADCSet = true;
	bool bADCSet = true;
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pLineList->m_oSecLineList);
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection, pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
		{
			if (iOpt == pConstVar->m_iADCSetOptNb)
			{
				if (false == pInstrument->m_bADCSet)
				{
					bADCSet = false;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pConstVar->m_iADCStartSampleOptNb)
			{
				if ((true == bADCStartSample)
					&& (false == pInstrument->m_bADCStartSample))
				{
					bADCSet = false;
					// 实际接收ADC数据帧数
					pInstrument->m_uiADCDataActualRecFrameNum = 0;
					// 重发查询帧得到的应答帧数
					pInstrument->m_uiADCDataRetransmissionFrameNum = 0;
					// 应该接收ADC数据帧数（含丢帧）
					pInstrument->m_uiADCDataShouldRecFrameNum = 0;
					// ADC数据帧的指针偏移量
					pInstrument->m_usADCDataFramePoint = 0;
					// ADC数据帧发送时的本地时间
					pInstrument->m_uiADCDataFrameSysTime = 0;
					// ADC数据帧起始帧数
					pInstrument->m_iADCDataFrameStartNum = 0;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pConstVar->m_iADCStopSampleOptNb)
			{
				if ((true == bADCStopSample)
					&& (false == pInstrument->m_bADCStopSample))
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
				pInstrument->m_bADCSetReturn = false;
				AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, 
					&pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
			else
			{
				DeleteInstrumentFromMap(pInstrument->m_uiIP, 
					&pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
				bRoutADCSet = false;
			}
		}
	} while (pInstrument != pRout->m_pTail);
	if (bRoutADCSet == true)
	{
		// 将路由加入索引
		pRout->m_bADCSetReturn = false;
		pRout->m_bADCSetRout = true;
		str.Format(_T("将路由IP = 0x%x 加入ADC参数设置任务索引表"), pRout->m_uiRoutIP);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetADCTaskQueueByRout", strConv);
		AddRout(pRout->m_uiRoutIP, pRout, &pLineList->m_pRoutList->m_oADCSetRoutMap);

		// 从仪器索引表删除该路由的仪器
		pInstrument = pRout->m_pHead;
		do 
		{
			pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection, pConstVar);
			if (pInstrument == NULL)
			{
				break;
			}
			if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
			{
				DeleteInstrumentFromMap(pInstrument->m_uiIP, 
					&pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
		} while (pInstrument != pRout->m_pTail);
	}
	else
	{
		DeleteRout(pRout->m_uiRoutIP, &pLineList->m_pRoutList->m_oADCSetRoutMap);
	}
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 生成ADC参数设置任务队列
void GetADCTaskQueue(m_oADCSetThreadStruct* pADCSetThread, int iOpt)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	bool bStartSample = false;
	bool bStopSample = false;
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	bStartSample = pADCSetThread->m_bADCStartSample;
	bStopSample = pADCSetThread->m_bADCStopSample;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	for (iter = pADCSetThread->m_pLineList->m_pRoutList->m_oRoutMap.begin(); 
		iter != pADCSetThread->m_pLineList->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		// 将路由索引表中的大线方向路由加入到ADC参数设置任务索引
		if ((iter->second->m_bRoutLaux == false)
			&& (iter->second->m_pTail != NULL)) 
		{
			GetADCTaskQueueByRout(bStartSample, bStopSample, 
				pADCSetThread->m_pLineList, pADCSetThread->m_pThread->m_pConstVar, iter->second, iOpt);
		}
	}
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
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
	OnADCSetThreadWork(iOpt, pADCSetThread);
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
	unsigned int uiRoutIP = 0;
	m_oRoutStruct* pRoutDelete = NULL;
	m_oRoutStruct* pRout = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	EnterCriticalSection(&pMonitorThread->m_pLineList->m_oSecLineList);
	// 删除过期仪器，将过期路由加入路由删除索引表
	for(iter = pMonitorThread->m_pLineList->m_pRoutList->m_oRoutMap.begin();
		iter != pMonitorThread->m_pLineList->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		pRout = iter->second;
		if (pRout->m_pTail == NULL)
		{
			continue;
		}
		pInstrument = pRout->m_pHead;
		if (pRout->m_pHead->m_bInUsed == false)
		{
			if (pRout->m_iRoutDirection == pMonitorThread->m_pThread->m_pConstVar->m_iDirectionTop)
			{
				pInstrument->m_pInstrumentTop = *(++pRout->m_olsRoutInstrument.begin());
			}
			else if (pRout->m_iRoutDirection == pMonitorThread->m_pThread->m_pConstVar->m_iDirectionDown)
			{
				pInstrument->m_pInstrumentDown = *(++pRout->m_olsRoutInstrument.begin());
			}
			else if (pRout->m_iRoutDirection == pMonitorThread->m_pThread->m_pConstVar->m_iDirectionLeft)
			{
				pInstrument->m_pInstrumentLeft = *(++pRout->m_olsRoutInstrument.begin());
			}
			else if (pRout->m_iRoutDirection == pMonitorThread->m_pThread->m_pConstVar->m_iDirectionRight)
			{
				pInstrument->m_pInstrumentRight = *(++pRout->m_olsRoutInstrument.begin());
			}
			else
			{
				continue;
			}
		}
		do 
		{
			pInstrumentNext = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection, pMonitorThread->m_pThread->m_pConstVar);
			// 判断尾包计数器达到设定值则删除相同路由之后的仪器
			if (pInstrumentNext->m_iTailFrameCount > pMonitorThread->m_pThread->m_pConstVar->m_iTailFrameStableTimes)
			{
				DeleteInstrumentAlongRout(pInstrument, pRout, 
					pMonitorThread->m_pLineList, pMonitorThread->m_pThread->m_pConstVar);
				break;
			}
			if (uiTimeNow > (pInstrumentNext->m_uiActiveTime + pMonitorThread->m_pThread->m_pConstVar->m_iMonitorStableTime))
			{
				// 生成IP地址查询帧
				MakeInstrIPQueryFrame(pMonitorThread->m_pIPSetFrame, 
					pMonitorThread->m_pThread->m_pConstVar, pInstrumentNext->m_uiIP);
				// 尾包计数器加一
				pInstrumentNext->m_iTailFrameCount++;
				break;
			}
			pInstrument = pInstrumentNext;
		} while (pInstrument != pRout->m_pTail);
		if (pRout->m_pHead->m_bInUsed == false)
		{
			pRout->m_pHead->m_pInstrumentTop = NULL;
			pRout->m_pHead->m_pInstrumentDown = NULL;
			pRout->m_pHead->m_pInstrumentLeft = NULL;
			pRout->m_pHead->m_pInstrumentRight = NULL;
		}
	}
	// 删除路由删除索引表中的仪器
	while(1)
	{
		if (pMonitorThread->m_pLineList->m_pRoutList->m_oRoutDeleteMap.empty() == true)
		{
			break;
		}
		iter = pMonitorThread->m_pLineList->m_pRoutList->m_oRoutDeleteMap.begin();
		uiRoutIP = iter->first;
		pRoutDelete = iter->second;
		// 路由索引表回收路由
		DeleteRout(uiRoutIP, &pMonitorThread->m_pLineList->m_pRoutList->m_oRoutMap);
		str.Format(_T("回收路由IP = 0x%x的过期路由"), uiRoutIP);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "ProcMonitor", strConv);
		// ADC参数设置索引表回收路由
		DeleteRout(uiRoutIP, &pMonitorThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap);
		// 路由删除索引表回收路由
		DeleteRout(uiRoutIP, &pMonitorThread->m_pLineList->m_pRoutList->m_oRoutDeleteMap);
		// 将过期路由回收到空闲路由队列
		AddFreeRout(pRoutDelete, pMonitorThread->m_pLineList->m_pRoutList);
	}
	LeaveCriticalSection(&pMonitorThread->m_pLineList->m_oSecLineList);
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
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strConv = "";
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection, pTimeDelayThread->m_pThread->m_pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		if (bSample == true)
		{
			if (pInstrument->m_iTimeSetReturnCount == 0)
			{
				str.Format(_T("数据采样过程中，路由IP = 0x%x的仪器的没有全部实现时统"), 
					pRout->m_uiRoutIP);
				strConv = (CStringA)str;
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
					"CheckTimeDelayReturnByRout", strConv, WarningType);
				return false;
			}
		}
		else
		{
			/** 时统设置是否成功*/
			if (pInstrument->m_bTimeSetOK == false)
			{
				str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收不完全"), pRout->m_uiRoutIP);
				strConv = (CStringA)str;
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
					"CheckTimeDelayReturnByRout", strConv);
				return false;
			}
		}
	} while (pInstrument != pRout->m_pTail);
	if (bSample == false)
	{
		str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收完全"), pRout->m_uiRoutIP);
		strConv = (CStringA)str;
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
	EnterCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
	for(iter = pTimeDelayThread->m_pLineList->m_pRoutList->m_oRoutMap.begin(); 
		iter != pTimeDelayThread->m_pLineList->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		if (false == CheckTimeDelayReturnByRout(iter->second, pTimeDelayThread, true))
		{
			LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
			return false;
		}
	}
	LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
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
	bool bStartSample = false;
	bool bSet = false;
	bool bWork = false;
	uiTimeNow = GetTickCount();
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	bStartSample = pTimeDelayThread->m_bADCStartSample;
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);

	EnterCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
	uiLineChangeTime = pTimeDelayThread->m_pLineList->m_uiLineChangeTime;
	bLineStableChange = pTimeDelayThread->m_pLineList->m_bLineStableChange;
	// 判断系统稳定
	if (uiTimeNow > (uiLineChangeTime + pTimeDelayThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 测网状态由不稳定变为稳定
		if (bLineStableChange == false)
		{
			pTimeDelayThread->m_pLineList->m_bLineStableChange = true;
			// 生成时统任务队列
			GenTimeDelayTaskQueue(pTimeDelayThread->m_pLineList->m_pRoutList, pTimeDelayThread->m_pThread->m_pConstVar);
			EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
			// 清空尾包时刻查询帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize);
			LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
			EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
			// 清空时统设置应答帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize);
			LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
			bSet = true;
			bWork = true;
		}
		else
		{
			// 采集过程中，如果仪器都完成了时统则停止时统线程
			if (bStartSample == true)
			{
				if (true == CheckTimeDelayReturn(pTimeDelayThread))
				{
					bSet = true;
					bWork = false;
				}
			}
		}
	} 
	else
	{
		bSet = true;
		bWork = false;
		// 测网状态为不稳定
		pTimeDelayThread->m_pLineList->m_bLineStableChange = false;
	}
	LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
	if (bSet == true)
	{
		EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		if (bWork == true)
		{
			pTimeDelayThread->m_uiCounter = 0;
			// 时统设置线程开始工作
			pTimeDelayThread->m_pThread->m_bWork = true;
		}
		else
		{
			// 时统设置线程停止工作
			pTimeDelayThread->m_pThread->m_bWork = false;
		}
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	}
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
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	uiLineChangeTime = pADCSetThread->m_pLineList->m_uiLineChangeTime;
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	// 判断系统稳定
	if (uiTimeNow > (uiLineChangeTime + pADCSetThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 自动进行未完成的ADC参数设置
		if (pADCSetThread->m_iADCSetOperationNb == 0)
		{
			OnADCSetAuto(pADCSetThread);
		}
		// 自动进行未完成的开始ADC数据采集
		if (pADCSetThread->m_iADCSetOperationNb == 0)
		{
			OnADCStartSampleAuto(pADCSetThread);
		}
		// 自动进行未完成的停止ADC数据采集
		if (pADCSetThread->m_iADCSetOperationNb == 0)
		{
			OnADCStopSampleAuto(pADCSetThread);
		}
	}
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
	EnterCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
	uiLineChangeTime = pErrorCodeThread->m_pLineList->m_uiLineChangeTime;
	LeaveCriticalSection(&pErrorCodeThread->m_pLineList->m_oSecLineList);
	EnterCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
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
	LeaveCriticalSection(&pErrorCodeThread->m_oSecErrorCodeThread);
}
// 处理路由监视
void ProcMonitor(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	bool bSetByHand = false;
	// 监视过期路由及仪器
	MonitorRoutAndInstrument(pMonitorThread);
	// 系统稳定且不进行ADC数据采集时进行时统
	MonitorTimeDelay(pMonitorThread->m_pTimeDelayThread);
	EnterCriticalSection(&pMonitorThread->m_pLineList->m_oSecLineList);
	bSetByHand = pMonitorThread->m_pLineList->m_pInstrumentList->m_bSetByHand;
	LeaveCriticalSection(&pMonitorThread->m_pLineList->m_oSecLineList);
	if (bSetByHand == false)
	{
		// 系统稳定则对仪器进行ADC参数设置
		MonitorADCSet(pMonitorThread->m_pADCSetThread);
	}
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
	bool bWork = false;
	bool bClose = false;
	while(true)
	{
		EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		bClose = pMonitorThread->m_pThread->m_bClose;
		bWork = pMonitorThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			// 处理路由监视
			ProcMonitor(pMonitorThread);
		}
		EnterCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		bClose = pMonitorThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		if (bClose == true)
		{
			break;
		}
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
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", 
			"pMonitorThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", 
		"路由监视线程创建成功");
	return true;
}
// 初始化路由监视线程
bool OnInit_MonitorThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pMonitorThread->m_pLineList = pEnv->m_pLineList;
	pEnv->m_pMonitorThread->m_pTimeDelayThread = pEnv->m_pTimeDelayThread;
	pEnv->m_pMonitorThread->m_pADCSetThread = pEnv->m_pADCSetThread;
	pEnv->m_pMonitorThread->m_pErrorCodeThread = pEnv->m_pErrorCodeThread;
	pEnv->m_pMonitorThread->m_pIPSetFrame = pEnv->m_pIPSetFrame;
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
		LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", 
			"路由监视线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", 
		"路由监视线程成功关闭");
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
		pMonitorThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pMonitorThread->m_oSecMonitorThread);
	delete pMonitorThread;
	pMonitorThread = NULL;
}