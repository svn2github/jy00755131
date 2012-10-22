#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建时统线程
m_oTimeDelayThreadStruct* OnCreateTimeDelayThread(void)
{
	m_oTimeDelayThreadStruct* pTimeDelayThread = NULL;
	pTimeDelayThread = new m_oTimeDelayThreadStruct;
	pTimeDelayThread->m_pThread = new m_oThreadStruct;
	pTimeDelayThread->m_pLogOutPutTimeDelay = NULL;
	pTimeDelayThread->m_pLineList = NULL;
	pTimeDelayThread->m_pTailTimeFrame = NULL;
	pTimeDelayThread->m_pTimeDelayFrame = NULL;
	InitializeCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	return pTimeDelayThread;
}
// 线程等待函数
void WaitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
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
void PrepareTailTimeFrame(m_oRoutStruct* pRout)
{
	ASSERT(pRout != NULL);
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	pInstrument->m_bTailTimeQueryOK = false;
	pInstrument->m_iTailTimeQueryCount++;
	do 
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection);
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
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pRout != NULL);
	if (pRout->m_pTail == NULL)
	{
		return;
	}
	CString str = _T("");
	string strConv = "";
	// 准备工作
	PrepareTailTimeFrame(pRout);
	// 按IP查询路由头仪器的尾包时刻
	MakeInstrTailTimeQueryFramebyIP(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pHead->m_uiIP,
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送尾包时刻查询帧"), pRout->m_pHead->m_uiIP);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	// 广播查询路由仪器的尾包时刻
	MakeInstrTailTimeQueryFramebyBroadCast(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort,
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	str.Format(_T("向广播端口 = 0x%x 的仪器广播发送尾包时刻查询帧"), pRout->m_pTail->m_uiBroadCastPort);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
}
// 处理单个尾包时刻查询应答帧
void ProcTailTimeReturnFrameOne(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pRout != NULL);
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
	unsigned int uiSysTime = 0;
	unsigned int uiNetTime = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	uiSrcIP = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSrcIP;
	usLAUXTailRecTimeLAUXLineA = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineA;
	usLAUXTailRecTimeLAUXLineB = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineB;
	usLAUXTailRecTimeLineA = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineA;
	usLAUXTailRecTimeLineB = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineB;
	usTailRecTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailRecTime;
	usTailSndTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailSndTime;
	uiSysTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSysTime;
	uiNetTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiNetTime;
	LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	EnterCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
	// 判断仪器IP是否在SN索引表中
	if (FALSE == IfIndexExistInMap(uiSrcIP, &pTimeDelayThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap))
	{
		LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
		GetFrameInfo(pTimeDelayThread->m_pTailTimeFrame->m_cpRcvFrameData,
			pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTailTimeReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
		return;
	}
	pInstrument = GetInstrumentFromMap(uiSrcIP, &pTimeDelayThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument);
	// @@@@@@@暂不判断尾包时刻过期的情况
	// 接收到尾包时刻查询应答标志位设为true
	pInstrument->m_bTailTimeQueryOK = true;
	pInstrument->m_iTailTimeReturnCount++;
	str.Format(_T("接收到IP地址 = 0x%x 仪器的尾包时刻查询应答帧,	"), pInstrument->m_uiIP);
	strOutPut += str;
	// 如果仪器类型为交叉站或者LCI
	if ((pInstrument->m_iInstrumentType == InstrumentTypeLCI)
		|| (pInstrument->m_iInstrumentType == InstrumentTypeLAUX))
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
	pInstrument->m_uiSystemTime = uiSysTime;
	pInstrument->m_uiNetTime = uiNetTime;
	str.Format(_T("尾包发送时刻 = 0x%x"), pInstrument->m_usSendTime);
	strOutPut += str;
	strConv = (CStringA)strOutPut;
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
}
// 处理尾包时刻查询应答
void ProcTailTimeReturnFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pRout != NULL);
	// 帧数量设置为0
	int iFrameCount = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
			// 得到帧数据
			if (false == GetFrameData(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
				pTimeDelayThread->m_pTailTimeFrame->m_cpRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
				continue;
			}
			LeaveCriticalSection(&pTimeDelayThread->m_pTailTimeFrame->m_oSecTailTimeFrame);
			if (false == ParseInstrTailTimeReturnFrame(pTimeDelayThread->m_pTailTimeFrame, 
				pTimeDelayThread->m_pThread->m_pConstVar, pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ParseInstrumentTailTimeReturnFrame", 
					"", ErrorType, IDS_ERR_PARSE_TAILTIMERETURNFRAME);
				continue;
			}
			// 处理单个尾包时刻查询应答帧
			ProcTailTimeReturnFrameOne(pRout, pTimeDelayThread);			
		}		
	}
}
// 检查尾包时刻查询结果是否接收完全
bool CheckTailTimeReturn(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pRout != NULL);
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strConv = "";
	pInstrument = pRout->m_pHead;
	if (pInstrument->m_bTailTimeQueryOK == false)
	{
		str.Format(_T("没有收到路由IP = 0x%x的路由头仪器IP = 0x%x的尾包时刻查询帧"), 
			pRout->m_uiRoutIP, pInstrument->m_uiIP);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", 
			strConv, WarningType);
		return false;
	}
	do 
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection);
		if (pInstrument == NULL)
		{
			break;
		}
		/** 尾包时刻查询是否成功*/
		if (pInstrument->m_bTailTimeQueryOK == false)
		{
			str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收不完全"), pRout->m_uiRoutIP);
			strConv = (CStringA)str;
			AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", 
				strConv, WarningType);
			return false;
		}
	} while (pInstrument != pRout->m_pTail);
	str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收完全"), pRout->m_uiRoutIP);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "CheckTailTimeReturn", strConv);
	return true;
}
// 处理时统设置
void ProcTimeDelayFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pRout != NULL);
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 临时变量
	int itmp1 = 0;
	int itmp2 = 0;
	CString str = _T("");
	CString strOutPut = _T("");
	string strConv = "";
	bool bADCStartSample = false;

	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	bADCStartSample = pTimeDelayThread->m_bADCStartSample;
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection);
		if (pInstrumentNext == NULL)
		{
			break;
		}
		// 仪器类型为LCI或交叉站
		if ((pInstrument->m_iInstrumentType == InstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == InstrumentTypeLAUX))
		{
			// 如果路由方向为上方
			if (pRout->m_iRoutDirection == DirectionTop)
			{
				itmp1 = pInstrument->m_usCrossTopReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为下方
			else if (pRout->m_iRoutDirection == DirectionDown)
			{
				itmp1 = pInstrument->m_usCrossDownReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为左方
			else if (pRout->m_iRoutDirection == DirectionLeft)
			{
				itmp1 = pInstrument->m_usLineLeftReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为右方
			else if (pRout->m_iRoutDirection == DirectionRight)
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
		if ((pInstrument->m_iInstrumentType == InstrumentTypeFDU)
			&& (pInstrumentNext->m_iInstrumentType == InstrumentTypeFDU))
		{
			itmp1 += pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelayFDUToFDU;
		}
		else if ((pInstrument->m_iInstrumentType == InstrumentTypeLCI)
			&& (pInstrumentNext->m_iInstrumentType == InstrumentTypeFDU))
		{
			itmp1 += pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelayLCIToFDU;
		}
		else if ((pInstrument->m_iInstrumentType == InstrumentTypeFDU)
			&& (pInstrumentNext->m_iInstrumentType == InstrumentTypeLAUL))
		{
			itmp1 += pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelayFDUToLAUL;
		}
		else if ((pInstrument->m_iInstrumentType == InstrumentTypeLAUL)
			&& (pInstrumentNext->m_iInstrumentType == InstrumentTypeFDU))
		{
			itmp1 += pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelayLAULToFDU;
		}
		itmp1 >>= 1;
		if (itmp1 > TimeDelayLowLimit)
		{
			itmp1 = TimeDelayDefault;
		}
		itmp2 += itmp1;
		str.Format(_T("IP地址 = 0x%x 的仪器的尾包时刻差值为 %d,	"), pInstrumentNext->m_uiIP, itmp1);
		strOutPut = str;

		// 时间修正低位
		pInstrumentNext->m_uiTimeLow = itmp2 & 0x3fff;
		// 时间修正高位
		/*pInstrumentNext->m_uiTimeHigh = (pInstrumentNext->m_uiNetTime - pInstrumentNext->m_uiSystemTime) & 0xffffffff;*/
		pInstrumentNext->m_uiTimeHigh = 0;
		// 在数据采集期间只针对未时统的仪器进行时统设置
		if (((bADCStartSample == true) && (pInstrumentNext->m_iTimeSetReturnCount == 0))
			|| (bADCStartSample == false))
		{
			// 时统设置次数加一
			pInstrumentNext->m_iTimeSetCount++;
			// 生成并发送时统设置帧
			MakeInstrDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, 
				pTimeDelayThread->m_pThread->m_pConstVar, pInstrumentNext,
				pTimeDelayThread->m_pThread->m_pLogOutPut);
			str.Format(_T("发送时统设置帧时统修正高位为 0x%x，时统修正低位为 0x%x，本地时间为 0x%x，网络时间为0x%x"), 
				pInstrumentNext->m_uiTimeHigh, pInstrumentNext->m_uiTimeLow, 
				pInstrumentNext->m_uiSystemTime, pInstrumentNext->m_uiNetTime);
			strOutPut += str;
			strConv = (CStringA)strOutPut;
			AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
/*			OutputDebugString(strOutPut);*/
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);
}
// 处理单个时统设置应答
void ProcTimeDelayReturnFrameOne(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiSrcIP = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	uiSrcIP = pTimeDelayThread->m_pTimeDelayFrame->m_pCommandStructReturn->m_uiSrcIP;
	LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	EnterCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
	// 判断仪器IP是否在SN索引表中
	if (FALSE == IfIndexExistInMap(uiSrcIP, &pTimeDelayThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap))
	{
		LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
		GetFrameInfo(pTimeDelayThread->m_pTimeDelayFrame->m_cpRcvFrameData,
			pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTimeDelayReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
		return;
	}
	pInstrument = GetInstrumentFromMap(uiSrcIP, &pTimeDelayThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument);
	// 接收到时统设置应答标志位
	pInstrument->m_bTimeSetOK = true;
	pInstrument->m_iTimeSetReturnCount++;
	str.Format(_T("接收到IP地址 = 0x%x 仪器的时统设置应答帧"), pInstrument->m_uiIP);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", strConv);
	LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
}
// 处理时统设置应答
void ProcTimeDelayReturnFrame(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	// 帧数量设置为0
	int iFrameCount = 0;
	EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
			// 得到帧数据
			if (false == GetFrameData(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
				pTimeDelayThread->m_pTimeDelayFrame->m_cpRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
				continue;
			}
			LeaveCriticalSection(&pTimeDelayThread->m_pTimeDelayFrame->m_oSecTimeDelayFrame);
			if (false == ParseInstrTimeDelayReturnFrame(pTimeDelayThread->m_pTimeDelayFrame, 
				pTimeDelayThread->m_pThread->m_pConstVar, pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, 
					"ParseInstrumentTimeDelayReturnFrame", "", 
					ErrorType, IDS_ERR_PARSE_TIMEDELAYRETURNFRAME);
				continue;
			}
			// 处理单个时统设置应答帧
			ProcTimeDelayReturnFrameOne(pTimeDelayThread);
		}		
	}
}
// 线程函数
DWORD WINAPI RunTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	string strConv = "";
	unsigned int uiProcRoutIP = 0;
	unsigned int uiCounter = 0;
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		bClose = pTimeDelayThread->m_pThread->m_bClose;
		bWork = pTimeDelayThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
			uiCounter = pTimeDelayThread->m_uiCounter;
			LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
			EnterCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
			if (pTimeDelayThread->m_pLineList->m_pRoutList->m_olsTimeDelayTaskQueue.size() > 0)
			{
				uiCounter++;
				EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
				pTimeDelayThread->m_uiCounter = uiCounter;
				LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
				if (uiCounter == 1)
				{
					// 得到时统队列头的路由IP地址作为当前任务处理的路由IP
					uiProcRoutIP = *pTimeDelayThread->m_pLineList->m_pRoutList->m_olsTimeDelayTaskQueue.begin();
					// 当前操作的路由IP在路由索引表中
					if (TRUE == IfIndexExistInRoutMap(uiProcRoutIP, &pTimeDelayThread->m_pLineList->m_pRoutList->m_oRoutMap))
					{
						// 得到当前操作的路由指针
						pRout = GetRout(uiProcRoutIP, &pTimeDelayThread->m_pLineList->m_pRoutList->m_oRoutMap);
					}
					else
					{
						str.Format(_T("路由IP = 0x%x"), uiProcRoutIP);
						strConv = (CStringA)str;
						AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "RunTimeDelayThread", 
							strConv, ErrorType, IDS_ERR_ROUT_NOTEXIT);
						// 删除该路由时统任务
						pTimeDelayThread->m_pLineList->m_pRoutList->m_olsTimeDelayTaskQueue.pop_front();
						LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
						uiCounter = 0;
						EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
						pTimeDelayThread->m_uiCounter = uiCounter;
						LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
						continue;
					}
					// 处理尾包时刻查询
					ProcTailTimeFrame(pRout, pTimeDelayThread);
				}
				else if (uiCounter == 5)
				{
					// 处理尾包时刻查询应答
					ProcTailTimeReturnFrame(pRout, pTimeDelayThread);
					// 检查尾包时刻查询结果是否接收完全
					if (false == CheckTailTimeReturn(pRout, pTimeDelayThread))
					{
						uiCounter = 0;
						EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
						pTimeDelayThread->m_uiCounter = uiCounter;
						LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
					}
					else
					{
						// 处理时统设置
						ProcTimeDelayFrame(pRout, pTimeDelayThread);
					}
				}
				else if (uiCounter == 10)
				{
					// 处理时统设置应答
					ProcTimeDelayReturnFrame(pTimeDelayThread);
					// 判断该路由方向是否完成时统
					// 检查时统设置应答是否接收完全
					if (true == CheckTimeDelayReturnByRout(pRout, pTimeDelayThread, false))
					{
						// 时统任务移到队列尾
						pTimeDelayThread->m_pLineList->m_pRoutList->m_olsTimeDelayTaskQueue.pop_front();
						pTimeDelayThread->m_pLineList->m_pRoutList->m_olsTimeDelayTaskQueue.push_back(uiProcRoutIP);
					}
				}
				else if (uiCounter == 15)
				{
					uiCounter = 0;
					EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
					pTimeDelayThread->m_uiCounter = uiCounter;
					LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
				}
			}
			LeaveCriticalSection(&pTimeDelayThread->m_pLineList->m_oSecLineList);
		}
		EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		bClose = pTimeDelayThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		if (bClose == true)
		{
			break;
		}
		WaitTimeDelayThread(pTimeDelayThread);
	}
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pTimeDelayThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	return 1;
}
// 初始化时统设置线程
bool OnInitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	ASSERT(pTimeDelayThread != NULL);
	ASSERT(pLogOutPut != NULL);
	ASSERT(pConstVar != NULL);
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
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", 
			"pTimeDelayThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", 
		"时统设置线程创建成功");
	return true;
}
// 初始化时统设置线程
bool OnInit_TimeDelayThread(m_oEnvironmentStruct* pEnv)
{
	ASSERT(pEnv != NULL);
	pEnv->m_pTimeDelayThread->m_pLogOutPutTimeDelay = pEnv->m_pLogOutPutTimeDelay;
	pEnv->m_pTimeDelayThread->m_pTailTimeFrame = pEnv->m_pTailTimeFrame;
	pEnv->m_pTimeDelayThread->m_pTimeDelayFrame = pEnv->m_pTimeDelayFrame;
	pEnv->m_pTimeDelayThread->m_pLineList = pEnv->m_pLineList;
	return OnInitTimeDelayThread(pEnv->m_pTimeDelayThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭时统设置线程
bool OnCloseTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	EnterCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	if (false == OnCloseThread(pTimeDelayThread->m_pThread))
	{
		LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", 
			"时统设置线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", 
		"时统设置线程成功关闭");
	return true;
}
// 释放时统设置线程
void OnFreeTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	ASSERT(pTimeDelayThread != NULL);
	if (pTimeDelayThread->m_pThread != NULL)
	{
		delete pTimeDelayThread->m_pThread;
		pTimeDelayThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pTimeDelayThread->m_oSecTimeDelayThread);
	delete pTimeDelayThread;
	pTimeDelayThread = NULL;
}