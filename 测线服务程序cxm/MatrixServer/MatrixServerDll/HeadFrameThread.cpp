#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建首包接收线程
m_oHeadFrameThreadStruct* OnCreateHeadFrameThread(void)
{
	m_oHeadFrameThreadStruct* pHeadFrameThread = NULL;
	pHeadFrameThread = new m_oHeadFrameThreadStruct;
	pHeadFrameThread->m_pThread = new m_oThreadStruct;
	pHeadFrameThread->m_pHeadFrame = NULL;
	pHeadFrameThread->m_pLineList = NULL;
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
	// 路由对象加入路由索引表
	AddRout(pRout->m_uiRoutIP, pRout,&pRoutList->m_oRoutMap);
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument, pConstVar);
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
	unsigned int uiVersion = 0;
	unsigned int uiHeadFrameCount = 0;
	EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	pHeadFrameThread->m_uiHeadFrameCount++;
	uiHeadFrameCount = pHeadFrameThread->m_uiHeadFrameCount;
	LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);

	EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	uiSN = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiSN;
	uiTimeHeadFrame = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiTimeHeadFrame;
	uiRoutIP = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP;
	uiVersion = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiVersion;
	LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	EnterCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
	// 判断仪器SN是否在SN索引表中
	if(FALSE == IfIndexExistInMap(uiSN, &pHeadFrameThread->m_pLineList->m_pInstrumentList->m_oSNInstrumentMap))
	{
		int iDirection = 0;
		// 得到新仪器
		pInstrument = GetFreeInstrument(pHeadFrameThread->m_pLineList->m_pInstrumentList);
		//设置新仪器的SN
		pInstrument->m_uiSN = uiSN;
		// 仪器类型
		pInstrument->m_iInstrumentType = pInstrument->m_uiSN & 0xff;
		//设置新仪器的路由IP地址
		pInstrument->m_uiRoutIP = uiRoutIP;
		// 设置新仪器的首包时刻
		pInstrument->m_uiTimeHeadFrame = uiTimeHeadFrame;
		// 仪器软件版本号
		pInstrument->m_uiVersion = uiVersion;
		// 设置新仪器的仪器类型
		// 路由地址为0为LCI
		if (pInstrument->m_uiRoutIP == 0)
		{
			pInstrument->m_iInstrumentType = pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI;
			pInstrument->m_iRoutDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			// 得到空闲路由对象
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, 
				pHeadFrameThread->m_pLineList->m_pRoutList);
		}
		else
		{
			if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP,
				&pHeadFrameThread->m_pLineList->m_pRoutList->m_oRoutMap))
			{
				pRout = GetRout(uiRoutIP, &pHeadFrameThread->m_pLineList->m_pRoutList->m_oRoutMap);
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
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pLineList->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionDown;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pLineList->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionLeft;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pLineList->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionRight;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pLineList->m_pRoutList);
		}
		pInstrument->m_uiBroadCastPort = pHeadFrameThread->m_pThread->m_pConstVar->m_iBroadcastPortStart + pInstrument->m_uiRoutIP;
		// 新仪器加入SN索引表
		AddInstrumentToMap(pInstrument->m_uiSN, pInstrument, &pHeadFrameThread->m_pLineList->m_pInstrumentList->m_oSNInstrumentMap);
	}

	// 在索引表中则找到该仪器,得到该仪器指针
	pInstrument = GetInstrumentFromMap(uiSN, &pHeadFrameThread->m_pLineList->m_pInstrumentList->m_oSNInstrumentMap);

	// 判断仪器是否已经设置IP
	if (pInstrument->m_bIPSetOK == true)
	{
		LeaveCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_EXPIREDHEADFRAME);
		return;
	}
	// 判断首包时刻是否发生变化
	if (pInstrument->m_uiTimeHeadFrame != uiTimeHeadFrame)
	{
		LeaveCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_HEADFRAMETIME);
		EnterCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
		pInstrument ->m_uiTimeHeadFrame = uiTimeHeadFrame;
	}
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument, pHeadFrameThread->m_pThread->m_pConstVar);
	if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP, 
		&pHeadFrameThread->m_pLineList->m_pRoutList->m_oRoutMap))
	{
		pRout = GetRout(uiRoutIP, &pHeadFrameThread->m_pLineList->m_pRoutList->m_oRoutMap);
		// 仪器位置按照首包时刻排序
		InstrumentLocationSort(pInstrument, pRout, pHeadFrameThread->m_pThread->m_pConstVar);
	}
	else
	{
		LeaveCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		GetFrameInfo(pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
			pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
			strFrameData, ErrorType, IDS_ERR_ROUT_NOTEXIT);
		return;
	}
	// 如果仪器在其路由方向上位置稳定次数超过设定次数
	// 则将该仪器加入IP地址设置队列
	if (pInstrument->m_iHeadFrameStableNum >= pHeadFrameThread->m_pThread->m_pConstVar->m_iHeadFrameStableTimes)
	{
		AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, &pHeadFrameThread->m_pLineList->m_pInstrumentList->m_oIPSetInstrumentMap);
	}
	str.Format(_T("接收到SN = 0x%x的仪器首包帧，首包时刻 = 0x%x，路由IP = 0x%x, 软件版本 = 0x%x，测线号 = %d，测点序号 = %d，首包计数 = %d"), 
		pInstrument->m_uiSN, pInstrument->m_uiTimeHeadFrame, pInstrument->m_uiRoutIP, uiVersion, 
		pInstrument->m_iLineIndex, pInstrument->m_iPointIndex, uiHeadFrameCount);
	LeaveCriticalSection(&pHeadFrameThread->m_pLineList->m_oSecLineList);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", strConv);
	OutputDebugString(str);
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
	EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
		pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pHeadFrameThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
			// 得到帧数据
			if (false == GetFrameData(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
				pHeadFrameThread->m_pHeadFrame->m_cpRcvFrameData, 
				pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pHeadFrameThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
				continue;
			}
			LeaveCriticalSection(&pHeadFrameThread->m_pHeadFrame->m_oSecHeadFrame);
			if (false == ParseInstrHeadFrame(pHeadFrameThread->m_pHeadFrame, 
				pHeadFrameThread->m_pThread->m_pConstVar))
			{
				AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ParseInstrumentHeadFrame", 
					"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				continue;
			}
			// 处理单个首包帧
			ProcHeadFrameOne(pHeadFrameThread);
			// 系统发生变化的时间
			UpdateLineChangeTime(pHeadFrameThread->m_pLineList);	
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
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		bClose = pHeadFrameThread->m_pThread->m_bClose;
		bWork = pHeadFrameThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			// 处理首包帧
			ProcHeadFrame(pHeadFrameThread);
		}
		EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		bClose = pHeadFrameThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		if (bClose == true)
		{
			break;
		}
		WaitHeadFrameThread(pHeadFrameThread);
	}
	EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pHeadFrameThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
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
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
			"pHeadFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
		"首包接收线程创建成功");
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
	pEnv->m_pHeadFrameThread->m_pLineList = pEnv->m_pLineList;
	return OnInitHeadFrameThread(pEnv->m_pHeadFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭首包接收线程
bool OnCloseHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	if (false == OnCloseThread(pHeadFrameThread->m_pThread))
	{
		LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, 
			"OnCloseHeadFrameThread", "首包线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
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
		pHeadFrameThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pHeadFrameThread->m_oSecHeadFrameThread);
	delete pHeadFrameThread;
	pHeadFrameThread = NULL;
}