#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建时统线程
m_oTimeDelayThreadStruct* OnCreateTimeDelayThread(void)
{
	m_oTimeDelayThreadStruct* pTimeDelayThread = NULL;
	pTimeDelayThread = new m_oTimeDelayThreadStruct;
	pTimeDelayThread->m_pThread = new m_oThreadStruct;
	pTimeDelayThread->m_pLogOutPutTimeDelay = NULL;
	pTimeDelayThread->m_pInstrumentList = NULL;
	pTimeDelayThread->m_pRoutList = NULL;
	pTimeDelayThread->m_pTailTimeFrame = NULL;
	pTimeDelayThread->m_pTimeDelayFrame = NULL;
	InitializeCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	return pTimeDelayThread;
}
// 线程等待函数
void WaitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pTimeDelayThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		bClose = pTimeDelayThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelaySleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 尾包时刻查询准备工作
void PrepareTailTimeFrame(m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "PrepareTailTimeFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}

	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	pInstrument->m_bTailTimeQueryOK = false;
	pInstrument->m_iTailTimeQueryCount++;
	do 
	{
		pInstrument = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		/** 尾包时刻查询计数*/
		pInstrument->m_iTailTimeQueryCount++;
		/** 尾包时刻查询是否成功*/
		pInstrument->m_bTailTimeQueryOK = false;
	} while (pInstrument != pRout->m_pTail);
}
// 处理尾包时刻查询
void ProcTailTimeFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return;
	}
	CString str = _T("");
	string strConv = "";
	// 准备工作
	PrepareTailTimeFrame(pRout, pTimeDelayThread->m_pThread->m_pConstVar);
	// 按IP查询路由头仪器的尾包时刻
	MakeInstrumentTailTimeQueryFramebyIP(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pHead->m_uiIP);
	SendInstrumentTailTimeQueryFrame(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送尾包时刻查询帧"), pRout->m_pHead->m_uiIP);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	// 广播查询路由仪器的尾包时刻
	MakeInstrumentTailTimeQueryFramebyBroadCast(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort);
	SendInstrumentTailTimeQueryFrame(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar);
	str.Format(_T("向广播端口 = 0x%x 的仪器广播发送尾包时刻查询帧"), pRout->m_pTail->m_uiBroadCastPort);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
}
// 处理单个尾包时刻查询应答帧
void ProcTailTimeReturnFrameOne(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOutPut = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiSrcIP = 0;
	unsigned short usLAUXTailRecTimeLAUXLineA = 0;
	unsigned short usLAUXTailRecTimeLAUXLineB = 0;
	unsigned short usLAUXTailRecTimeLineA = 0;
	unsigned short usLAUXTailRecTimeLineB = 0;
	unsigned short usTailSndTime = 0;
	unsigned short usTailRecTime = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	uiSrcIP = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSrcIP;
	usLAUXTailRecTimeLAUXLineA = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineA;
	usLAUXTailRecTimeLAUXLineB = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineB;
	usLAUXTailRecTimeLineA = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineA;
	usLAUXTailRecTimeLineB = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineB;
	usTailRecTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailRecTime;
	usTailSndTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailSndTime;
	LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	// 判断仪器IP是否在SN索引表中
	if (TRUE == IfIndexExistInMap(uiSrcIP, &pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiSrcIP, &pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap);
		// @@@@@@@暂不判断尾包时刻过期的情况
		// 接收到尾包时刻查询应答标志位设为true
		pInstrument->m_bTailTimeQueryOK = true;
		pInstrument->m_iTailTimeReturnCount++;
		str.Format(_T("接收到IP地址 = 0x%x 仪器的尾包时刻查询应答帧,	"), pInstrument->m_uiIP);
		strOutPut += str;
		// 如果仪器类型为交叉站或者LCI
		if ((pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 如果为交叉线方向路由
			if (pRout->m_bRoutLaux == true)
			{
				pInstrument->m_usCrossTopReceiveTime = usLAUXTailRecTimeLAUXLineA;
				pInstrument->m_usCrossDownReceiveTime = usLAUXTailRecTimeLAUXLineB;
				str.Format(_T("交叉站交叉线A尾包接收时刻 = 0x%x，交叉站交叉线B尾包接收时刻 = 0x%x,	"), 
					pInstrument->m_usCrossTopReceiveTime, pInstrument->m_usCrossDownReceiveTime);
				strOutPut += str;
			}
			// 大线方向
			else
			{
				pInstrument->m_usLineLeftReceiveTime = usLAUXTailRecTimeLineA;
				pInstrument->m_usLineRightReceiveTime = usLAUXTailRecTimeLineB;
				str.Format(_T("交叉站大线A尾包接收时刻 = 0x%x，交叉站大线B尾包接收时刻 = 0x%x,	"), 
					pInstrument->m_usLineLeftReceiveTime, pInstrument->m_usLineRightReceiveTime);
				strOutPut += str;
			}
		}
		else
		{
			// 采集站或电源站尾包接收时刻
			pInstrument->m_usReceiveTime = usTailRecTime;
			str.Format(_T("采集站尾包接收时刻 = 0x%x,	"), pInstrument->m_usReceiveTime);
			strOutPut += str;
		}
		// 尾包发送时刻
		pInstrument->m_usSendTime = usTailSndTime;
		str.Format(_T("尾包发送时刻 = 0x%x"), pInstrument->m_usSendTime);
		strOutPut += str;
		ConvertCStrToStr(strOutPut, &strConv);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	}
	else
	{
		GetFrameInfo(pTimeDelayThread->m_pTailTimeFrame->m_cpRcvFrameData,
			pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTailTimeReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 处理尾包时刻查询应答
void ProcTailTimeReturnFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
				pTimeDelayThread->m_pTailTimeFrame->m_cpRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTailTimeReturnFrame(pTimeDelayThread->m_pTailTimeFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ParseInstrumentTailTimeReturnFrame", 
						"", ErrorType, IDS_ERR_PARSE_TAILTIMERETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
					// 处理单个尾包时刻查询应答帧
					ProcTailTimeReturnFrameOne(pRout, pTimeDelayThread);
					LeaveCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 检查尾包时刻查询结果是否接收完全
bool CheckTailTimeReturn(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return false;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strConv = "";
	pInstrument = pRout->m_pHead;
	if (pInstrument->m_bTailTimeQueryOK == false)
	{
		str.Format(_T("没有收到路由IP = 0x%x的路由头仪器IP = 0x%x的尾包时刻查询帧"), 
			pRout->m_uiRoutIP, pInstrument->m_uiIP);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", 
			strConv, WarningType);
		return false;
	}
	do 
	{
		pInstrument = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
			pTimeDelayThread->m_pThread->m_pConstVar);
		if (pInstrument == NULL)
		{
			break;
		}
		/** 尾包时刻查询是否成功*/
		if (pInstrument->m_bTailTimeQueryOK == false)
		{
			str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收不完全"), pRout->m_uiRoutIP);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", 
				strConv, WarningType);
			return false;
		}
	} while (pInstrument != pRout->m_pTail);
	str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收完全"), pRout->m_uiRoutIP);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "CheckTailTimeReturn", 
		strConv);
	return true;
}
// 处理时统设置
void ProcTimeDelayFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if ((pTimeDelayThread == NULL) || (pRout == NULL))
	{
		return;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 临时变量
	int itmp1 = 0;
	int itmp2 = 0;
	CString str = _T("");
	CString strOutPut = _T("");
	string strConv = "";
	bool bADCStartSample = false;
	pInstrument = pRout->m_pHead;
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	bADCStartSample = pTimeDelayThread->m_bADCStartSample;
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
			pTimeDelayThread->m_pThread->m_pConstVar);
		if (pInstrumentNext == NULL)
		{
			break;
		}
		// 仪器类型为LCI或交叉站
		if ((pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 如果路由方向为上方
			if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionTop)
			{
				itmp1 = pInstrument->m_usCrossTopReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为下方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionDown)
			{
				itmp1 = pInstrument->m_usCrossDownReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为左方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionLeft)
			{
				itmp1 = pInstrument->m_usLineLeftReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为右方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionRight)
			{
				itmp1 = pInstrument->m_usLineRightReceiveTime - pInstrumentNext->m_usSendTime;
			}
		}
		else
		{
			itmp1 = pInstrument->m_usReceiveTime - pInstrumentNext->m_usSendTime;
		}
		if (itmp1 < 0)
		{
			itmp1 += 0x3fff;
		}
		str.Format(_T("IP地址 = 0x%x 的仪器的尾包时刻差值为 %d,	"), pInstrumentNext->m_uiIP, itmp1);
		strOutPut = str;
		itmp1 = itmp1 / 2;
		itmp2 += itmp1;
		// 时间修正低位
		pInstrumentNext->m_uiTimeLow = itmp2 & 0x3fff;
		// 时间修正高位
		pInstrumentNext->m_uiTimeHigh = (pInstrumentNext->m_uiNetTime - pInstrumentNext->m_uiSystemTime) & 0xffffffff;
		// 在数据采集期间只针对未时统的仪器进行时统设置	
		if (bADCStartSample == true)
		{
			if (pInstrumentNext->m_iTimeSetReturnCount == 0)
			{
				// 时统设置次数加一
				pInstrumentNext->m_iTimeSetCount++;
				// 生成并发送时统设置帧
				MakeInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar, pInstrumentNext);
				SendInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar);
				str.Format(_T("发送时统设置帧时统修正高位为 0x%x，时统修正低位为 0x%x"), 
					pInstrumentNext->m_uiTimeHigh, pInstrumentNext->m_uiTimeLow);
				strOutPut += str;
				ConvertCStrToStr(strOutPut, &strConv);
				AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
			}
		}
		else
		{
			// 时统设置次数加一
			pInstrumentNext->m_iTimeSetCount++;
			// 生成并发送时统设置帧
			MakeInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, 
				pTimeDelayThread->m_pThread->m_pConstVar, pInstrumentNext);
			SendInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, 
				pTimeDelayThread->m_pThread->m_pConstVar);
			str.Format(_T("发送时统设置帧时统修正高位为 0x%x，时统修正低位为 0x%x"), 
				pInstrumentNext->m_uiTimeHigh, pInstrumentNext->m_uiTimeLow);
			strOutPut += str;
			ConvertCStrToStr(strOutPut, &strConv);
			AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);
}
// 处理单个时统设置应答
void ProcTimeDelayReturnFrameOne(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiSrcIP = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	uiSrcIP = pTimeDelayThread->m_pTimeDelayFrame->m_pCommandStructReturn->m_uiSrcIP;
	LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 判断仪器IP是否在SN索引表中
	if (TRUE == IfIndexExistInMap(uiSrcIP, &pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiSrcIP, &pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap);
		// 接收到时统设置应答标志位
		pInstrument->m_bTimeSetOK = true;
		pInstrument->m_iTimeSetReturnCount++;
		str.Format(_T("接收到IP地址 = 0x%x 仪器的时统设置应答帧"), pInstrument->m_uiIP);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	}
	else
	{
		GetFrameInfo(pTimeDelayThread->m_pTimeDelayFrame->m_cpRcvFrameData,
			pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTimeDelayReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 处理时统设置应答
void ProcTimeDelayReturnFrame(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
				pTimeDelayThread->m_pTimeDelayFrame->m_cpRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTimeDelayReturnFrame(pTimeDelayThread->m_pTimeDelayFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
						"ParseInstrumentTimeDelayReturnFrame", "", 
						ErrorType, IDS_ERR_PARSE_TIMEDELAYRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
					// 处理单个时统设置应答帧
					ProcTimeDelayReturnFrameOne(pTimeDelayThread);
					LeaveCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 线程函数
DWORD WINAPI RunTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return 1;
	}
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	string strConv = "";
	unsigned int uiProcRoutIP = 0;
	while(true)
	{
		EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		if (pTimeDelayThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
			break;
		}
		if (pTimeDelayThread->m_pThread->m_bWork == true)
		{
			if (pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.size() > 0)
			{
				pTimeDelayThread->m_uiCounter++;
				if (pTimeDelayThread->m_uiCounter == 1)
				{
					// 得到时统队列头的路由IP地址作为当前任务处理的路由IP
					uiProcRoutIP = *pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.begin();
					// 当前操作的路由IP在路由索引表中
					if (TRUE == IfIndexExistInRoutMap(uiProcRoutIP, &pTimeDelayThread->m_pRoutList->m_oRoutMap))
					{
						// 得到当前操作的路由指针
						pRout = GetRout(uiProcRoutIP, &pTimeDelayThread->m_pRoutList->m_oRoutMap);
					}
					else
					{
						str.Format(_T("路由IP = 0x%x"), pRout->m_uiRoutIP);
						ConvertCStrToStr(str, &strConv);
						AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "RunTimeDelayThread", 
							strConv, ErrorType, IDS_ERR_ROUT_NOTEXIT);
						// 删除该路由时统任务
						pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.pop_front();
						pTimeDelayThread->m_uiCounter = 0;
						LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
						continue;
					}
					// 处理尾包时刻查询
					ProcTailTimeFrame(pRout, pTimeDelayThread);
				}
				else if (pTimeDelayThread->m_uiCounter == 5)
				{
					// 处理尾包时刻查询应答
					ProcTailTimeReturnFrame(pRout, pTimeDelayThread);
					// 检查尾包时刻查询结果是否接收完全
					if (false == CheckTailTimeReturn(pRout, pTimeDelayThread))
					{
						pTimeDelayThread->m_uiCounter = 0;
					}
					else
					{
						// 处理时统设置
						ProcTimeDelayFrame(pRout, pTimeDelayThread);
					}
				}
				else if (pTimeDelayThread->m_uiCounter == 10)
				{
					// 处理时统设置应答
					ProcTimeDelayReturnFrame(pTimeDelayThread);
					// 判断该路由方向是否完成时统
					// 检查时统设置应答是否接收完全
					if (true == CheckTimeDelayReturnByRout(pRout, pTimeDelayThread, false))
					{
						// 时统任务移到队列尾
						pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.pop_front();
						pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.push_back(uiProcRoutIP);
					}
				}
				else if (pTimeDelayThread->m_uiCounter == 15)
				{
					pTimeDelayThread->m_uiCounter = 0;
				}
			}
		}
		if (pTimeDelayThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
			break;
		}
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		WaitTimeDelayThread(pTimeDelayThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pTimeDelayThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化时统设置线程
bool OnInitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	pTimeDelayThread->m_bADCStartSample = false;
	pTimeDelayThread->m_uiCounter = 0;
	if (false == OnInitThread(pTimeDelayThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pTimeDelayThread->m_pThread->m_hThreadClose);
	// 创建线程
	pTimeDelayThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunTimeDelayThread,
		pTimeDelayThread, 
		0, 
		&pTimeDelayThread->m_pThread->m_dwThreadID);
	if (pTimeDelayThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", 
			"pTimeDelayThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		return false;
	}
	AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", 
		"时统设置线程创建成功");
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	return true;
}
// 初始化时统设置线程
bool OnInit_TimeDelayThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pTimeDelayThread->m_pLogOutPutTimeDelay = pEnv->m_pLogOutPutTimeDelay;
	pEnv->m_pTimeDelayThread->m_pTailTimeFrame = pEnv->m_pTailTimeFrame;
	pEnv->m_pTimeDelayThread->m_pTimeDelayFrame = pEnv->m_pTimeDelayFrame;
	pEnv->m_pTimeDelayThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pTimeDelayThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitTimeDelayThread(pEnv->m_pTimeDelayThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭时统设置线程
bool OnCloseTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pTimeDelayThread->m_pThread))
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", 
			"时统设置线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", 
		"时统设置线程成功关闭");
	return true;
}
// 释放时统设置线程
void OnFreeTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	if (pTimeDelayThread->m_pThread != NULL)
	{
		delete pTimeDelayThread->m_pThread;
	}
	DeleteCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	delete pTimeDelayThread;
}