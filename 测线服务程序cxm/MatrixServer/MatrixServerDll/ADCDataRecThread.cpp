#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建ADC数据接收线程
m_oADCDataRecThreadStruct* OnCreateADCDataRecThread(void)
{
	m_oADCDataRecThreadStruct* pADCDataRecThread = NULL;
	pADCDataRecThread = new m_oADCDataRecThreadStruct;
	pADCDataRecThread->m_pThread = new m_oThreadStruct;
	pADCDataRecThread->m_pLogOutPutADCFrameTime = NULL;
	pADCDataRecThread->m_pADCDataBufArray = NULL;
	pADCDataRecThread->m_pADCDataFrame = NULL;
	pADCDataRecThread->m_pLineList = NULL;
	InitializeCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	return pADCDataRecThread;
}
// 线程等待函数
void WaitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCDataRecThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		bClose = pADCDataRecThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataRecSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 采样数据回调函数
void GetProSampleDateCallBack(m_oADCDataRecThreadStruct* pADCDataRecThread, ProSampleDateCallBack pCallBack)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	pADCDataRecThread->m_oProSampleDataCallBack = pCallBack;
	LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
}
// 将ADC数据加入到任务列表
void AddToADCDataWriteFileList(int iLineIndex, int iPointIndex, unsigned int uiFrameNb,
	unsigned int uiSN, unsigned int uiSysTime, m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	m_oADCDataBufStruct* pADCDataBuf = NULL;
	pADCDataBuf = GetFreeADCDataBuf(pADCDataRecThread->m_pADCDataBufArray);
	pADCDataBuf->m_uiFrameNb = uiFrameNb;
	pADCDataBuf->m_uiSN = uiSN;
	pADCDataBuf->m_uiSysTime = uiSysTime;
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	memcpy(pADCDataBuf->m_pADCDataBuf, pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_pADCData,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * sizeof(int));
	// ADC数据处理的回调函数
	(*pADCDataRecThread->m_oProSampleDataCallBack)(iLineIndex, iPointIndex, 
		pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_pADCData,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum,
		uiSN);
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	pADCDataRecThread->m_pADCDataBufArray->m_olsADCDataToWrite.push_back(pADCDataBuf);
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
}
// 处理单个ADC数据帧
void ProcADCDataRecFrameOne(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOut = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned short usADCDataFramePointNow = 0;
	unsigned int uiADCDataFramePointMove = 0;
	unsigned int uiADCDataFrameSysTimeNow = 0;
	unsigned int uiADCDataFrameSysTimeMove = 0;
	unsigned int uiLostFrameNum = 0;
	int iADCDataInOneFrameNum = 0;
	int iADCFrameSaveInOneFileNum = 0;
	unsigned short usADCFramePointLimit = 0;
	ADCLostFrame_Struct* pADCLostFrame = NULL;
	// 帧序号
	unsigned int uiFrameNb = 0;
	// 测试数据
	int iTestData = 0;

	iADCDataInOneFrameNum = pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum;
	usADCFramePointLimit = pADCDataRecThread->m_pThread->m_pConstVar->m_usADCFramePointLimit;
	iADCFrameSaveInOneFileNum = pADCDataRecThread->m_pThread->m_pConstVar->m_iADCFrameSaveInOneFileNum;
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	// 得到仪器IP
	uiIPInstrument = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 得到指针偏移量
	usADCDataFramePointNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_usADCDataPoint;
	// 得到本地系统时间
	uiADCDataFrameSysTimeNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiADCSampleSysTime;
	// 测试数据
	iTestData = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_pADCData[0];
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	// 仪器不在索引表中
	if (FALSE == IfIndexExistInMap(uiIPInstrument, &pADCDataRecThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap))
	{
		str.Format(_T("收到IP地址 = 0x%x的ADC数据帧"), uiIPInstrument);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
			strConv, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
		return;
	}
	pInstrument = GetInstrumentFromMap(uiIPInstrument, 
		&pADCDataRecThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	// 在丢帧索引表中找到
	if (TRUE == IfIndexExistInADCFrameLostMap(uiIPInstrument, usADCDataFramePointNow, 
		&pADCDataRecThread->m_oADCLostFrameMap))
	{
		str.Format(_T("收到SN = 0x%x，IP地址 = 0x%x的ADC数据重发帧，指针偏移量 = %d"), 
			pInstrument->m_uiSN, pInstrument->m_uiIP, usADCDataFramePointNow);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
			strConv);

		pADCLostFrame = GetFromADCFrameLostMap(uiIPInstrument, usADCDataFramePointNow, 
			&pADCDataRecThread->m_oADCLostFrameMap);
		LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		if (pADCLostFrame->m_bReturnOk == false)
		{
			pInstrument->m_uiADCDataActualRecFrameNum++;
			pInstrument->m_uiADCDataRetransmissionFrameNum++;
			pADCLostFrame->m_bReturnOk = true;
			// 将该应答帧数据写入文件
			uiFrameNb = pADCLostFrame->m_uiFrameNb;
			AddToADCDataWriteFileList(pInstrument->m_iLineIndex, pInstrument->m_iPointIndex, 
				uiFrameNb, pInstrument->m_uiSN, pADCLostFrame->m_uiSysTime, pADCDataRecThread);
			if (pInstrument->m_olsADCDataSave.size() == pADCDataRecThread->m_pThread->m_pConstVar->m_uiSaveTestDataNum)
			{
				pInstrument->m_olsADCDataSave.pop_front();
			}
			pInstrument->m_olsADCDataSave.push_back(iTestData);
		}
		return;
	}
	LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	// 		// 调试用
	// 		if (usADCDataFramePointNow == 288)
	// 		{
	// 			return;
	// 		}
	if (pInstrument->m_uiADCDataActualRecFrameNum > 0)
	{
		if (usADCDataFramePointNow > usADCFramePointLimit)
		{
			EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			GetFrameInfo(pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData, 
				pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
				strFrameData, ErrorType, IDS_ERR_FRAMEPOINT_LIMITOVER);
			return;
		}
		if (usADCDataFramePointNow < pInstrument->m_usADCDataFramePoint)
		{
			uiADCDataFramePointMove = usADCFramePointLimit - pInstrument->m_usADCDataFramePoint 
				+ usADCDataFramePointNow;
		}
		else
		{
			uiADCDataFramePointMove = usADCDataFramePointNow - pInstrument->m_usADCDataFramePoint;
		}

		if (uiADCDataFrameSysTimeNow < pInstrument->m_uiADCDataFrameSysTime)
		{
			uiADCDataFrameSysTimeMove = 0xffffffff - pInstrument->m_uiADCDataFrameSysTime 
				+ uiADCDataFrameSysTimeNow;
		}
		else
		{
			uiADCDataFrameSysTimeMove = uiADCDataFrameSysTimeNow - pInstrument->m_uiADCDataFrameSysTime;
		}
		if (uiADCDataFramePointMove % iADCDataInOneFrameNum != 0)
		{
			EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			GetFrameInfo(pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData,
				pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
			LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
				strFrameData, ErrorType, IDS_ERR_FRAMEPOINT_ERROR);
			str.Format(_T("指针偏移量出错，偏移量差值为%d"), uiADCDataFramePointMove);
			strConv = (CStringA)str;
			AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", 
				strConv, ErrorType, IDS_ERR_FRAMEPOINT_ERROR);
			return;
		}
		else
		{
			EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
			if (uiADCDataFrameSysTimeNow != pADCDataRecThread->m_uiADCDataFrameSysTime)
			{
				pADCDataRecThread->m_iADCFrameCount++;
			}
			if (pInstrument->m_uiADCDataActualRecFrameNum == 1)
			{
				pInstrument->m_iADCDataFrameStartNum = pADCDataRecThread->m_iADCFrameCount - 1 - 1;
			}
			LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);

			uiLostFrameNum = uiADCDataFramePointMove / iADCDataInOneFrameNum - 1;
			if (uiLostFrameNum > 0)
			{
				// 加入到丢帧索引表
				for (unsigned int i=0; i<uiLostFrameNum; i++)
				{
					m_oADCLostFrameStruct ADCLostFrame;
					unsigned int uiIP = pInstrument->m_uiIP;
					unsigned short usADCFramePointNb = static_cast<unsigned short>(pInstrument->m_usADCDataFramePoint 
						+ (i + 1) * iADCDataInOneFrameNum);
					if (usADCFramePointNb >= usADCFramePointLimit)
					{
						usADCFramePointNb = usADCFramePointNb - usADCFramePointLimit;
					}
					ADCLostFrame.m_uiCount = 0;
					ADCLostFrame.m_bReturnOk = false;
					ADCLostFrame.m_uiFrameNb = pInstrument->m_uiADCDataShouldRecFrameNum 
						+ pInstrument->m_iADCDataFrameStartNum;
					// @@@@1K采样率的时间差为288
					ADCLostFrame.m_uiSysTime = pInstrument->m_uiADCDataFrameSysTime + (i + 1) * 288;
					EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
					AddToADCFrameLostMap(uiIP, usADCFramePointNb, ADCLostFrame, &pADCDataRecThread->m_oADCLostFrameMap);
					LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
					pInstrument->m_uiADCDataShouldRecFrameNum++;
					// 在丢帧的位置写当前帧的内容
					AddToADCDataWriteFileList(pInstrument->m_iLineIndex, pInstrument->m_iPointIndex,
						ADCLostFrame.m_uiFrameNb, pInstrument->m_uiSN, ADCLostFrame.m_uiSysTime, 
						pADCDataRecThread);
					if (pInstrument->m_olsADCDataSave.size() == pADCDataRecThread->m_pThread->m_pConstVar->m_uiSaveTestDataNum)
					{
						pInstrument->m_olsADCDataSave.pop_front();
					}
					pInstrument->m_olsADCDataSave.push_back(iTestData);
				}
				str.Format(_T("仪器SN = 0x%x，IP = 0x%x，丢失帧数为%d"), pInstrument->m_uiSN, 
					pInstrument->m_uiIP, uiLostFrameNum);
				strConv = (CStringA)str;
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne",
					strConv, WarningType);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", 
					strConv, WarningType);
			}
			uiFrameNb = pInstrument->m_uiADCDataShouldRecFrameNum + pInstrument->m_iADCDataFrameStartNum;
			AddToADCDataWriteFileList(pInstrument->m_iLineIndex, pInstrument->m_iPointIndex, 
				uiFrameNb, pInstrument->m_uiSN, uiADCDataFrameSysTimeNow, pADCDataRecThread);
			if (pInstrument->m_olsADCDataSave.size() == pADCDataRecThread->m_pThread->m_pConstVar->m_uiSaveTestDataNum)
			{
				pInstrument->m_olsADCDataSave.pop_front();
			}
			pInstrument->m_olsADCDataSave.push_back(iTestData);
		}
	}
	EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	pADCDataRecThread->m_uiADCDataFrameSysTime = uiADCDataFrameSysTimeNow;
	LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	pInstrument->m_uiADCDataActualRecFrameNum++;
	pInstrument->m_uiADCDataShouldRecFrameNum++;
	pInstrument->m_usADCDataFramePoint = usADCDataFramePointNow;
	pInstrument->m_uiADCDataFrameSysTime = uiADCDataFrameSysTimeNow;
	str.Format(_T("仪器SN = 0x%x，IP = 0x%x；"), pInstrument->m_uiSN, pInstrument->m_uiIP);
	strOut += str;
	str.Format(_T("接收帧的指针偏移量为 %d，差值为 %d；"), 
		pInstrument->m_usADCDataFramePoint, uiADCDataFramePointMove);
	strOut += str;
	str.Format(_T("本地时间为 %d，差值为%d；"), 
		pInstrument->m_uiADCDataFrameSysTime, uiADCDataFrameSysTimeMove);
	strOut += str;
	str.Format(_T("应收帧数为 %d, 起始的帧数为%d"), 
		pInstrument->m_uiADCDataShouldRecFrameNum, pInstrument->m_iADCDataFrameStartNum);
	strOut += str;
	strConv = (CStringA)strOut;
	AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", strConv);
}
// 处理ADC数据接收帧
void ProcADCDataRecFrame(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	iFrameCount = GetFrameCount(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pADCDataRecThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			// 得到帧数据
			if (false == GetFrameData(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
				pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData, 
				pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCDataRecThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
				continue;
			}
			if (false == ParseInstrumentADCDataRecFrame(pADCDataRecThread->m_pADCDataFrame, 
					pADCDataRecThread->m_pThread->m_pConstVar))
			{
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCDataRecFrame",
					"", ErrorType, IDS_ERR_PARSE_ADCDATARECFRAME);
				LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
				continue;
			}
			LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
			// 处理单个ADC数据帧
			ProcADCDataRecFrameOne(pADCDataRecThread);
		}		
	}
}
// 处理ADC数据重发
void ProcADCRetransmission(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct> ::iterator iter;
	m_oADCLostFrameStruct* pLostFrame = NULL;
	CString str = _T("");
	string strConv = "";
	for (iter = pADCDataRecThread->m_oADCLostFrameMap.begin(); 
		iter != pADCDataRecThread->m_oADCLostFrameMap.end();)
	{
		pLostFrame = &iter->second;
		pLostFrame->m_uiCount++;

		// 重发次数小于指定次数且没有收到重发帧应答
		if ((pLostFrame->m_uiCount < 4)
			&& (pLostFrame->m_bReturnOk == false))
		{
			// 生成ADC数据重发帧
			MakeInstrumentADCDataFrame(pADCDataRecThread->m_pADCDataFrame, 
				pADCDataRecThread->m_pThread->m_pConstVar, iter->first.m_uiIP, iter->first.m_usADCFramePointNb);
			SendInstrumentADCDataFrame(pADCDataRecThread->m_pADCDataFrame, 
				pADCDataRecThread->m_pThread->m_pConstVar);
			str.Format(_T("向仪器IP = 0x%x的仪器发送ADC数据重发帧，重发的指针偏移量 = %d"), iter->first.m_uiIP,
				iter->first.m_usADCFramePointNb);
			strConv = (CStringA)str;
			AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCRetransmission", 
				strConv);
			iter++;
		}
		// 重发次数大于指定次数则从索引中删除
		else if (pLostFrame->m_uiCount > 6)
		{
			// 没有收到重发帧应答
			if (pLostFrame->m_bReturnOk == false)
			{
				// 没有收到重发帧
				str.Format(_T("仪器IP = 0x%x的仪器没有收到ADC数据重发帧应答，重发的指针偏移量 = %d"), 
					iter->first.m_uiIP, iter->first.m_usADCFramePointNb);
				strConv = (CStringA)str;
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCRetransmission", 
					strConv);
			}
			pADCDataRecThread->m_oADCLostFrameMap.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}
// 线程函数
DWORD WINAPI RunADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		if (pADCDataRecThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
			break;
		}
		if (pADCDataRecThread->m_pThread->m_bWork == true)
		{
			// 处理ADC数据接收帧
			ProcADCDataRecFrame(pADCDataRecThread);
			// 处理ADC数据重发
			ProcADCRetransmission(pADCDataRecThread);
		}
		if (pADCDataRecThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
			break;
		}
		LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		WaitADCDataRecThread(pADCDataRecThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pADCDataRecThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化ADC数据接收线程
bool OnInitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pADCDataRecThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	if (false == OnInitThread(pADCDataRecThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		return false;
	}
	pADCDataRecThread->m_uiADCDataFrameSysTime = 0;
	pADCDataRecThread->m_iADCFrameCount = 0;
	pADCDataRecThread->m_oADCLostFrameMap.clear();
	// 设置事件对象为无信号状态
	ResetEvent(pADCDataRecThread->m_pThread->m_hThreadClose);
	// 创建线程
	pADCDataRecThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCDataRecThread,
		pADCDataRecThread, 
		0, 
		&pADCDataRecThread->m_pThread->m_dwThreadID);
	if (pADCDataRecThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnInitADCDataRecThread", 
			"pADCDataRecThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
		return false;
	}
	AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnInitADCDataRecThread", 
		"ADC数据接收线程创建成功");
	LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	return true;
}
// 初始化ADC数据接收线程
bool OnInit_ADCDataRecThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pADCDataRecThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	pEnv->m_pADCDataRecThread->m_pADCDataBufArray = pEnv->m_pADCDataBufArray;
	pEnv->m_pADCDataRecThread->m_pADCDataFrame = pEnv->m_pADCDataFrame;
	pEnv->m_pADCDataRecThread->m_pLineList = pEnv->m_pLineList;
	return OnInitADCDataRecThread(pEnv->m_pADCDataRecThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC数据接收线程
bool OnCloseADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	// 清空丢帧索引
	pADCDataRecThread->m_oADCLostFrameMap.clear();
	LeaveCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	if (false == OnCloseThread(pADCDataRecThread->m_pThread))
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", 
			"ADC数据接收线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", 
		"ADC数据接收线程成功关闭");
	return true;
}
// 释放ADC数据接收线程
void OnFreeADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	if (pADCDataRecThread->m_pThread != NULL)
	{
		delete pADCDataRecThread->m_pThread;
	}
	DeleteCriticalSection(&pADCDataRecThread->m_oSecADCDataRecThread);
	delete pADCDataRecThread;
}