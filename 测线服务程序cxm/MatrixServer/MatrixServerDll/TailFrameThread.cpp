#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建尾包接收线程
m_oTailFrameThreadStruct* OnCreateTailFrameThread(void)
{
	m_oTailFrameThreadStruct* pTailFrameThread = NULL;
	pTailFrameThread = new m_oTailFrameThreadStruct;
	pTailFrameThread->m_pThread = new m_oThreadStruct;
	pTailFrameThread->m_pLineList = NULL;
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
	m_oLineListStruct* pLineList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pLineList == NULL) || (pInstrument == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "FreeInstrumentFromMap", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pLineList->m_oSecLineList);
	// 从SN索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiSN, &pLineList->m_pInstrumentList->m_oSNInstrumentMap);
	// 从IP地址设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pLineList->m_pInstrumentList->m_oIPSetInstrumentMap);
	// 从IP地址索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	// 从ADC参数设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
	// 从仪器位置索引表中删除该仪器指针
	DeleteInstrumentFromLocationMap(pInstrument->m_iLineIndex, pInstrument->m_iPointIndex, 
		&pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
	str.Format(_T("删除仪器的SN = 0x%x，路由 = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "FreeInstrumentFromMap", strConv);

	// 删除仪器的类型为LCI或交叉站
	if ((pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		|| (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
	{
		if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		{
			FreeRoutFromMap(pInstrument->m_uiRoutIP, pLineList->m_pRoutList);
		}
		// 仪器上方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPTop, pLineList->m_pRoutList);
		// 仪器下方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPDown, pLineList->m_pRoutList);
		// 仪器左方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPLeft, pLineList->m_pRoutList);
		// 仪器右方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPRight, pLineList->m_pRoutList);
	}
	// 将仪器加入到空闲仪器队列
	AddFreeInstrument(pInstrument, pLineList->m_pInstrumentList);
	// 系统发生变化的时间
	UpdateLineChangeTime(pLineList);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 删除该路由方向指定仪器之后的仪器
void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, 
	m_oLineListStruct* pLineList, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pRout == NULL) || (pInstrument == NULL) || (pLineList == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "DeleteInstrumentAlongRout", 
			"", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 前一个仪器指针为路由尾仪器
	m_oInstrumentStruct* pInstrumentPrevious = NULL;
	m_oInstrumentStruct* pInstrumentDelete = NULL;
	CString str = _T("");
	EnterCriticalSection(&pLineList->m_oSecLineList);
	pInstrumentPrevious = pRout->m_pTail;
	if (pInstrument == pRout->m_pHead)
	{
		// 不是LCI路由
		if (pRout->m_pHead != pRout->m_pTail)
		{
			pRout->m_pTail = NULL;
		}
	}
	else
	{
		pRout->m_pTail = pInstrument;
	}
	do 
	{
		if (pInstrumentPrevious == NULL)
		{
			break;
		}
		pInstrumentDelete = pInstrumentPrevious;
		// 得到要删除仪器沿着路由方向的前一个仪器的指针
		pInstrumentPrevious = GetPreviousInstr(pInstrumentDelete, pConstVar);
		if (pInstrumentDelete->m_bIPSetOK == true)
		{
			pRout->m_uiInstrumentNum--;
		}
		// 回收一个仪器
		if (pRout->m_olsRoutInstrument.size() > 0)
		{
			pRout->m_olsRoutInstrument.pop_back();
		}
		// 回收一个仪器
		FreeInstrumentFromMap(pInstrumentDelete, pLineList, pConstVar);
	} while (pInstrument != pInstrumentPrevious);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 在路由方向上删除该仪器之后的全部仪器
void DeleteAllInstrumentAlongRout(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, 
	m_oLineListStruct* pLineList, 
	m_oConstVarStruct* pConstVar,
	m_oLogOutPutStruct* pLogOutPut)
{
	hash_map <unsigned int, m_oRoutStruct*>  ::iterator iter;
	CString str = _T("");
	string strConv = "";
	unsigned int uiRoutIP = 0;
	m_oRoutStruct* pRoutDelete = NULL;
	if (pRout->m_pTail == NULL)
	{
		return;
	}
	DeleteInstrumentAlongRout(pInstrument, pRout, pLineList, pConstVar);
	EnterCriticalSection(&pLineList->m_oSecLineList);
	while(true)
	{
		if (pLineList->m_pRoutList->m_oRoutDeleteMap.empty() == true)
		{
			break;
		}
		iter = pLineList->m_pRoutList->m_oRoutDeleteMap.begin();
		uiRoutIP = iter->first;
		pRoutDelete = iter->second;
		DeleteInstrumentAlongRout(pRoutDelete->m_pHead, pRoutDelete, pLineList, pConstVar);
		// 路由索引表回收路由
		DeleteRout(uiRoutIP, &pLineList->m_pRoutList->m_oRoutMap);
		str.Format(_T("回收路由IP = 0x%x的过期路由"), uiRoutIP);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pLogOutPut, "DeleteAllInstrumentAlongRout", strConv);
		// ADC参数设置索引表回收路由
		DeleteRout(uiRoutIP, &pLineList->m_pRoutList->m_oADCSetRoutMap);
		// 路由删除索引表回收路由
		DeleteRout(uiRoutIP, &pLineList->m_pRoutList->m_oRoutDeleteMap);
		// 将过期路由回收到空闲路由队列
		AddFreeRout(pRoutDelete, pLineList->m_pRoutList);
	}
	LeaveCriticalSection(&pLineList->m_oSecLineList);
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
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiSN = 0;
	unsigned int uiRoutIP = 0;
	unsigned int uiTailFrameCount = 0;
	EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	pTailFrameThread->m_uiTailFrameCount++;
	uiTailFrameCount = pTailFrameThread->m_uiTailFrameCount;
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	uiSN = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSN;
	uiRoutIP = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP;
	LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	EnterCriticalSection(&pTailFrameThread->m_pLineList->m_oSecLineList);
	// 判断仪器SN是否在SN索引表中
	if(TRUE == IfIndexExistInMap(uiSN, &pTailFrameThread->m_pLineList->m_pInstrumentList->m_oSNInstrumentMap))
	{
		// 在索引表中则找到该仪器,得到该仪器指针
		pInstrument = GetInstrumentFromMap(uiSN, &pTailFrameThread->m_pLineList->m_pInstrumentList->m_oSNInstrumentMap);
		// 仪器设置IP成功且路由地址发生变化
		if ((pInstrument->m_bIPSetOK == true) && (pInstrument->m_uiRoutIP != uiRoutIP))
		{
			LeaveCriticalSection(&pTailFrameThread->m_pLineList->m_oSecLineList);
			EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			GetFrameInfo(pTailFrameThread->m_pTailFrame->m_cpRcvFrameData,
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne",
				strFrameData, ErrorType, IDS_ERR_ROUT_CHANGE);
			return;
		}
		str.Format(_T("接收到SN = 0x%x，路由 = 0x%x 的仪器的尾包，仪器位置= %d，尾包计数 = %d"), 
			uiSN, uiRoutIP, pInstrument->m_iPointIndex, uiTailFrameCount);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", strConv);
		OutputDebugString(str);
		// 在路由索引表中找到该尾包所在的路由
		// 更新仪器的存活时间
		UpdateInstrActiveTime(pInstrument, pTailFrameThread->m_pThread->m_pConstVar);
		if (FALSE == IfIndexExistInRoutMap(uiRoutIP, &pTailFrameThread->m_pLineList->m_pRoutList->m_oRoutMap))
		{
			LeaveCriticalSection(&pTailFrameThread->m_pLineList->m_oSecLineList);
			EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			GetFrameInfo(pTailFrameThread->m_pTailFrame->m_cpRcvFrameData,
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			EnterCriticalSection(&pTailFrameThread->m_pLineList->m_oSecLineList);
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne",
				strFrameData,	ErrorType, IDS_ERR_ROUT_NOTEXIT);
		}
	}
	LeaveCriticalSection(&pTailFrameThread->m_pLineList->m_oSecLineList);
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
	EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
		pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTailFrameThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			// 得到帧数据
			if (false == GetFrameData(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
				pTailFrameThread->m_pTailFrame->m_cpRcvFrameData, 
				pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTailFrameThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
				continue;
			}
			LeaveCriticalSection(&pTailFrameThread->m_pTailFrame->m_oSecTailFrame);
			if (false == ParseInstrTailFrame(pTailFrameThread->m_pTailFrame, 
				pTailFrameThread->m_pThread->m_pConstVar))
			{
				AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, 
					"ParseInstrumentTailFrame", "", ErrorType, IDS_ERR_PARSE_TAILFRAME);
				continue;
			}
			// 处理单个尾包帧
			ProcTailFrameOne(pTailFrameThread);			
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
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		bClose = pTailFrameThread->m_pThread->m_bClose;
		bWork = pTailFrameThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			// 处理尾包接收
			ProcTailFrame(pTailFrameThread);
		}
		EnterCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		bClose = pTailFrameThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		if (bClose == true)
		{
			break;
		}
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
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
			"pTailFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
		"尾包接收线程创建成功");
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
	pEnv->m_pTailFrameThread->m_pLineList = pEnv->m_pLineList;
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
		LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
			"尾包接收线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
		"尾包接收线程成功关闭");
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
		pTailFrameThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pTailFrameThread->m_oSecTailFrameThread);
	delete pTailFrameThread;
	pTailFrameThread = NULL;
}