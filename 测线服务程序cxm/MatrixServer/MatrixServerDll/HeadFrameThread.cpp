#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建首包接收线程
m_oHeadFrameThreadStruct* OnCreateHeadFrameThread(void)
{
	m_oHeadFrameThreadStruct* pHeadFrameThread = NULL;
	pHeadFrameThread = new m_oHeadFrameThreadStruct;
	pHeadFrameThread->m_pThread = new m_oThreadStruct;
	pHeadFrameThread->m_pHeadFrame = NULL;
	pHeadFrameThread->m_pInstrumentList = NULL;
	pHeadFrameThread->m_pRoutList = NULL;
	InitializeCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	return pHeadFrameThread;
}
bool CompareHeadFrameTime(m_oInstrumentStruct* pInstrumentFirst, m_oInstrumentStruct* pInstrumentSecond)
{
	return (pInstrumentFirst->m_uiTimeHeadFrame < pInstrumentSecond->m_uiTimeHeadFrame);
}
// 仪器位置按照首包时刻排序
void InstrumentLocationSort(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pInstrument == NULL) || (pRout == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InstrumentLocationSort", "", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	list<m_oInstrumentStruct*>::iterator iter;
	list<m_oInstrumentStruct*>::iterator iterOld;
	list<m_oInstrumentStruct*>::iterator iterNew;
	// 上方或下方
	if ((pInstrument->m_iRoutDirection == pConstVar->m_iDirectionTop)
		|| (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionDown))
	{
		pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex;
	}
	// 大线左方或右方
	else if ((pInstrument->m_iRoutDirection == pConstVar->m_iDirectionLeft)
		|| (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionRight))
	{
		pInstrument->m_iLineIndex = pRout->m_pHead->m_iLineIndex;
	}
	// LCI
	else
	{
		pInstrument->m_iHeadFrameStableNum++;
		return;
	}

	pRout->m_olsRoutInstrument.sort(CompareHeadFrameTime);
	pRout->m_pTail = &(**pRout->m_olsRoutInstrument.rbegin());
	for (iter = pRout->m_olsRoutInstrument.begin(); ; )
	{
		iterOld = iter;
		iterNew = ++iter;
		if (iterNew == pRout->m_olsRoutInstrument.end())
		{
			break;
		}
		// 上方
		if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionTop)
		{
			(*iterOld)->m_pInstrumentTop = *(iterNew);
			(*iterNew)->m_pInstrumentDown = *(iterOld);
			if ((*iterNew)->m_iLineIndex == ((*iterOld)->m_iLineIndex - 1))
			{
				if (pInstrument == (*iterNew))
				{
					(*iterNew)->m_iHeadFrameStableNum++;
				}
			}
			else
			{
				(*iterNew)->m_iLineIndex = (*iterOld)->m_iLineIndex - 1;
				(*iterNew)->m_iHeadFrameStableNum = 0;
			}
		}
		// 下方
		else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionDown)
		{
			(*iterOld)->m_pInstrumentDown = *(iterNew);
			(*iterNew)->m_pInstrumentTop = *(iterOld);
			if ((*iterNew)->m_iLineIndex == ((*iterOld)->m_iLineIndex + 1))
			{
				if (pInstrument == (*iterNew))
				{
					(*iterNew)->m_iHeadFrameStableNum++;
				}
			}
			else
			{
				(*iterNew)->m_iLineIndex = (*iterOld)->m_iLineIndex + 1;
				(*iterNew)->m_iHeadFrameStableNum = 0;
			}
		}
		// 左方
		else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionLeft)
		{
			(*iterOld)->m_pInstrumentLeft = *(iterNew);
			(*iterNew)->m_pInstrumentRight = *(iterOld);
			if ((*iterNew)->m_iPointIndex == ((*iterOld)->m_iPointIndex - 1))
			{
				if (pInstrument == (*iterNew))
				{
					(*iterNew)->m_iHeadFrameStableNum++;
				}
			}
			else
			{
				(*iterNew)->m_iPointIndex = (*iterOld)->m_iPointIndex - 1;
				(*iterNew)->m_iHeadFrameStableNum = 0;
			}
		}
		// 右方
		else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionRight)
		{
			(*iterOld)->m_pInstrumentRight = *(iterNew);
			(*iterNew)->m_pInstrumentLeft = *(iterOld);
			if ((*iterNew)->m_iPointIndex == ((*iterOld)->m_iPointIndex + 1))
			{
				if (pInstrument == (*iterNew))
				{
					(*iterNew)->m_iHeadFrameStableNum++;
				}
			}
			else
			{
				(*iterNew)->m_iPointIndex = (*iterOld)->m_iPointIndex + 1;
				(*iterNew)->m_iHeadFrameStableNum = 0;
			}
		}
	}
}
/**
* 设置交叉站某个方向的路由
* @param CInstrument* &pInstrument 仪器指针
* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
void SetCrossRout(m_oInstrumentStruct* pInstrument, int iRoutDirection, 
	m_oConstVarStruct* pConstVar, m_oRoutListStruct* pRoutList)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pInstrument == NULL) || (pRoutList == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SetCrossRout", "", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oRoutStruct* pRout = NULL;
	pRout = GetFreeRout(pRoutList);
	// 判断方向，设置仪器路由地址
	if (iRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrument->m_uiRoutIPTop = pRout->m_uiRoutIP;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrument->m_uiRoutIPDown = pRout->m_uiRoutIP;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrument->m_uiRoutIPLeft = pRout->m_uiRoutIP;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrument->m_uiRoutIPRight = pRout->m_uiRoutIP;
	}
	// 设置交叉站路由方向
	pRout->m_iRoutDirection = iRoutDirection;
	// 把仪器加入路由仪器队列
	pRout->m_olsRoutInstrument.push_back(pInstrument);
	// 设置路由头
	pRout->m_pHead = pInstrument;
	if (iRoutDirection == pConstVar->m_iDirectionCenter)
	{
		// 设置LCI路由
		pRout->m_uiRoutIP = 0;
		// 设置路由尾
		pRout->m_pTail = pInstrument;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
	}
	else
	{
		// 设置路由尾
		pRout->m_pTail = NULL;
	}
	// 更新路由对象的路由时间
	UpdateRoutTime(pRout);
	// 路由对象加入路由索引表
	AddRout(pRout->m_uiRoutIP, pRout,&pRoutList->m_oRoutMap);
}
// 处理单个首包帧
void ProcHeadFrameOne(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
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
	unsigned int uiTimeHeadFrame = 0;
	unsigned int uiRoutIP = 0;
	EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	uiSN = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiSN;
	uiTimeHeadFrame = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiTimeHeadFrame;
	uiRoutIP = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP;
	LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	// 判断仪器SN是否在SN索引表中
	if(FALSE == IfIndexExistInMap(uiSN, &pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap))
	{
		int iDirection = 0;
		// 得到新仪器
		pInstrument = GetFreeInstrument(pHeadFrameThread->m_pInstrumentList);
		//设置新仪器的SN
		pInstrument->m_uiSN = uiSN;
		// 仪器类型
		pInstrument->m_iInstrumentType = pInstrument->m_uiSN & 0xff;
		//设置新仪器的路由IP地址
		pInstrument->m_uiRoutIP = uiRoutIP;
		// 设置新仪器的首包时刻
		pInstrument->m_uiTimeHeadFrame = uiTimeHeadFrame;
		// 设置新仪器的仪器类型
		// 路由地址为0为LCI
		if (pInstrument->m_uiRoutIP == 0)
		{
			pInstrument->m_iInstrumentType = pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI;
			pInstrument->m_iRoutDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			// 得到空闲路由对象
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, 
				pHeadFrameThread->m_pRoutList);
		}
		else
		{
			if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP,
				&pHeadFrameThread->m_pRoutList->m_oRoutMap))
			{
				pRout = GetRout(uiRoutIP, &pHeadFrameThread->m_pRoutList->m_oRoutMap);
				pInstrument->m_iRoutDirection = pRout->m_iRoutDirection;
				// 把仪器加入路由仪器队列
				pRout->m_olsRoutInstrument.push_back(pInstrument);
			}
		}

		if ((pInstrument->m_iInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionTop;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionDown;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionLeft;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionRight;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
		}
		pInstrument->m_uiBroadCastPort = pHeadFrameThread->m_pThread->m_pConstVar->m_iBroadcastPortStart + pInstrument->m_uiRoutIP;
		// 新仪器加入SN索引表
		AddInstrumentToMap(pInstrument->m_uiSN, pInstrument, &pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
	}

	// 在索引表中则找到该仪器,得到该仪器指针
	pInstrument = GetInstrumentFromMap(uiSN, &pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
	// 判断仪器是否已经设置IP
	if (pInstrument->m_bIPSetOK == true)
	{
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_EXPIREDHEADFRAME);
		return;
	}
	// 判断首包时刻是否发生变化
	if (pInstrument->m_uiTimeHeadFrame != uiTimeHeadFrame)
	{
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_HEADFRAMETIME);
		pInstrument ->m_uiTimeHeadFrame = uiTimeHeadFrame;
	}
	if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP, 
		&pHeadFrameThread->m_pRoutList->m_oRoutMap))
	{
		pRout = GetRout(uiRoutIP, &pHeadFrameThread->m_pRoutList->m_oRoutMap);
		// 更新路由对象的路由时间
		UpdateRoutTime(pRout);
		// 仪器位置按照首包时刻排序
		InstrumentLocationSort(pInstrument, pRout, pHeadFrameThread->m_pThread->m_pConstVar);
	}
	else
	{
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_ROUT_NOTEXIT);
	}
	// 如果仪器在其路由方向上位置稳定次数超过设定次数
	// 则将该仪器加入IP地址设置队列
	if (pInstrument->m_iHeadFrameStableNum >= pHeadFrameThread->m_pThread->m_pConstVar->m_iHeadFrameStableTimes)
	{
		AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, &pHeadFrameThread->m_pInstrumentList->m_oIPSetMap);
	}
	str.Format(_T("接收到SN = 0x%x的仪器首包帧，首包时刻 = 0x%x，路由IP = 0x%x, 测线号 = %d，测点序号 = %d"), 
		pInstrument->m_uiSN, pInstrument->m_uiTimeHeadFrame, pInstrument->m_uiRoutIP, 
		pInstrument->m_iLineIndex, pInstrument->m_iPointIndex);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", strConv);
}
// 处理首包帧
void ProcHeadFrame(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
		pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pHeadFrameThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
				pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
				pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pHeadFrameThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentHeadFrame(pHeadFrameThread->m_pHeadFrame, 
					pHeadFrameThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ParseInstrumentHeadFrame", 
						"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				}
				else
				{
					EnterCriticalSection(&pHeadFrameThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pHeadFrameThread->m_pRoutList->m_oSecRoutList);
					// 处理单个首包帧
					ProcHeadFrameOne(pHeadFrameThread);
					// 系统发生变化的时间
					UpdateLineChangeTime(pHeadFrameThread->m_pInstrumentList);
					LeaveCriticalSection(&pHeadFrameThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pHeadFrameThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 线程等待函数
void WaitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pHeadFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		bClose = pHeadFrameThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pHeadFrameThread->m_pThread->m_pConstVar->m_iHeadFrameSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		if (pHeadFrameThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
			break;
		}
		if (pHeadFrameThread->m_pThread->m_bWork == true)
		{
			// 处理首包帧
			ProcHeadFrame(pHeadFrameThread);
		}
		if (pHeadFrameThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
			break;
		}
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		WaitHeadFrameThread(pHeadFrameThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pHeadFrameThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化首包接收线程
bool OnInitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	if (false == OnInitThread(pHeadFrameThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pHeadFrameThread->m_pThread->m_hThreadClose);
	// 创建线程
	pHeadFrameThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunHeadFrameThread,
		pHeadFrameThread, 
		0, 
		&pHeadFrameThread->m_pThread->m_dwThreadID);
	if (pHeadFrameThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
			"pHeadFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		return false;
	}
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
		"首包接收线程创建成功");
	LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	return true;
}
// 初始化首包接收线程
bool OnInit_HeadFrameThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pHeadFrameThread->m_pHeadFrame = pEnv->m_pHeadFrame;
	pEnv->m_pHeadFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pHeadFrameThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitHeadFrameThread(pEnv->m_pHeadFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭首包接收线程
bool OnCloseHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pHeadFrameThread->m_pThread))
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, 
			"OnCloseHeadFrameThread", "首包线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, 
		"OnCloseHeadFrameThread", "首包线程成功关闭");
	return true;
}
// 释放首包接收线程
void OnFreeHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	if (pHeadFrameThread->m_pThread != NULL)
	{
		delete pHeadFrameThread->m_pThread;
	}
	DeleteCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	delete pHeadFrameThread;
}