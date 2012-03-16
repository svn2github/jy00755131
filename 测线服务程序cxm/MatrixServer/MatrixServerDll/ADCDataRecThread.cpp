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
	pADCDataRecThread->m_pInstrumentList = NULL;
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
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCDataRecThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pADCDataRecThread->m_pThread->m_bClose == true)
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
// 将ADC数据加入到任务列表
void AddToADCDataWriteFileList(unsigned int uiFrameNb, unsigned int uiSN, 
	unsigned int uiSysTime, m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// @@@@调试用
	// 	CString str = _T("");
	// 	str.Format(_T("仪器IP = 0x%x，写入文件序号 = %d，帧序号 = %d"), uiIP, uiFileNb, uiFrameInFileNb);
	// 	AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", ConvertCStrToStr(str));
	m_oADCDataBufStruct* pADCDataBuf = NULL;
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
	EnterCriticalSection(&pADCDataRecThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	pADCDataBuf = GetFreeADCDataBuf(pADCDataRecThread->m_pADCDataBufArray);
	pADCDataBuf->m_uiFrameNb = uiFrameNb;
	pADCDataBuf->m_uiSN = uiSN;
	pADCDataBuf->m_uiSysTime = uiSysTime;
	memcpy(pADCDataBuf->m_pADCDataBuf, pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_pADCData,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * sizeof(int));
	pADCDataRecThread->m_pADCDataBufArray->m_olsADCDataToWrite.push_back(pADCDataBuf);
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	LeaveCriticalSection(&pADCDataRecThread->m_pADCDataFrame->m_oSecADCDataFrame);
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
	m_oADCLostFrameKeyStruct oLostFrameKey;
	ADCLostFrame_Struct* pADCLostFrame = NULL;
	// 帧序号
	unsigned int uiFrameNb = 0;

	iADCDataInOneFrameNum = pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum;
	usADCFramePointLimit = pADCDataRecThread->m_pThread->m_pConstVar->m_usADCFramePointLimit;
	iADCFrameSaveInOneFileNum = pADCDataRecThread->m_pThread->m_pConstVar->m_iADCFrameSaveInOneFileNum;
	// 得到仪器IP
	uiIPInstrument = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 得到指针偏移量
	usADCDataFramePointNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_usADCDataPoint;
	// 得到本地系统时间
	uiADCDataFrameSysTimeNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiADCSampleSysTime;

	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pADCDataRecThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, 
			&pADCDataRecThread->m_pInstrumentList->m_oIPInstrumentMap);
		// 判断是否为重发帧
		oLostFrameKey.m_uiIP = uiIPInstrument;
		oLostFrameKey.m_usADCFramePointNb = usADCDataFramePointNow;
		// 在丢帧索引表中找到
		if (TRUE == IfIndexExistInADCFrameLostMap(oLostFrameKey, &pADCDataRecThread->m_oADCLostFrameMap))
		{
			str.Format(_T("收到SN = 0x%x，IP地址 = 0x%x的ADC数据重发帧，指针偏移量 = %d"), 
				pInstrument->m_uiSN, pInstrument->m_uiIP, usADCDataFramePointNow);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
				strConv);

			pADCLostFrame = GetFromADCFrameLostMap(oLostFrameKey, &pADCDataRecThread->m_oADCLostFrameMap);
			if (pADCLostFrame->m_bReturnOk == false)
			{
				pInstrument->m_uiADCDataActualRecFrameNum++;
				pInstrument->m_uiADCDataRetransmissionFrameNum++;
				pADCLostFrame->m_bReturnOk = true;
				// 将该应答帧数据写入文件
				uiFrameNb = pADCLostFrame->m_uiFrameNb;
				AddToADCDataWriteFileList(uiFrameNb, pInstrument->m_uiSN, pADCLostFrame->m_uiSysTime, 
					pADCDataRecThread);
			}
			return;
		}
		// 		// 调试用
		// 		if (usADCDataFramePointNow == 288)
		// 		{
		// 			return;
		// 		}
		if (pInstrument->m_uiADCDataActualRecFrameNum > 0)
		{
			if (usADCDataFramePointNow > usADCFramePointLimit)
			{
				GetFrameInfo(pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData, 
					pADCDataRecThread->m_pADCDataFrame->m_uiRcvBufferSize, &strFrameData);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
					strFrameData, ErrorType, IDS_ERR_FRAMEPOINT_LIMITOVER);
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
				GetFrameInfo(pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData,
					pADCDataRecThread->m_pADCDataFrame->m_uiRcvBufferSize, &strFrameData);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", 
					strFrameData, ErrorType, IDS_ERR_FRAMEPOINT_ERROR);
				str.Format(_T("指针偏移量出错，偏移量差值为%d"), uiADCDataFramePointMove);
				ConvertCStrToStr(str, &strConv);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", 
					strConv, ErrorType, IDS_ERR_FRAMEPOINT_ERROR);
			}
			else
			{
				if (uiADCDataFrameSysTimeNow != pADCDataRecThread->m_uiADCDataFrameSysTime)
				{
					pADCDataRecThread->m_iADCFrameCount++;
				}
				if (pInstrument->m_uiADCDataActualRecFrameNum == 1)
				{
					pInstrument->m_iADCDataFrameStartNum = pADCDataRecThread->m_iADCFrameCount - 1 - 1;
				}

				uiLostFrameNum = uiADCDataFramePointMove / iADCDataInOneFrameNum - 1;
				if (uiLostFrameNum > 0)
				{
					// 加入到丢帧索引表
					for (unsigned int i=0; i<uiLostFrameNum; i++)
					{
						m_oADCLostFrameKeyStruct Key;
						m_oADCLostFrameStruct ADCLostFrame;
						Key.m_uiIP = pInstrument->m_uiIP;
						Key.m_usADCFramePointNb = static_cast<unsigned short>(pInstrument->m_usADCDataFramePoint 
							+ (i + 1) * iADCDataInOneFrameNum);
						if (Key.m_usADCFramePointNb >= usADCFramePointLimit)
						{
							Key.m_usADCFramePointNb = Key.m_usADCFramePointNb - usADCFramePointLimit;
						}
						ADCLostFrame.m_uiCount = 0;
						ADCLostFrame.m_bReturnOk = false;
						ADCLostFrame.m_uiFrameNb = pInstrument->m_uiADCDataShouldRecFrameNum 
							+ pInstrument->m_iADCDataFrameStartNum;
						// @@@@1K采样率的时间差为288
						ADCLostFrame.m_uiSysTime = pInstrument->m_uiADCDataFrameSysTime + (i + 1) * 288;
						AddToADCFrameLostMap(Key, ADCLostFrame, &pADCDataRecThread->m_oADCLostFrameMap);
						pInstrument->m_uiADCDataShouldRecFrameNum++;
						// 在丢帧的位置写当前帧的内容
						AddToADCDataWriteFileList(ADCLostFrame.m_uiFrameNb, pInstrument->m_uiSN, 
							ADCLostFrame.m_uiSysTime, pADCDataRecThread);
					}
					str.Format(_T("仪器SN = 0x%x，IP = 0x%x，丢失帧数为%d"), pInstrument->m_uiSN, 
						pInstrument->m_uiIP, uiLostFrameNum);
					ConvertCStrToStr(str, &strConv);
					AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne",
						strConv, WarningType);
					AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", 
						strConv, WarningType);
				}
				uiFrameNb = pInstrument->m_uiADCDataShouldRecFrameNum + pInstrument->m_iADCDataFrameStartNum;
				AddToADCDataWriteFileList(uiFrameNb, pInstrument->m_uiSN, uiADCDataFrameSysTimeNow, pADCDataRecThread);
			}
		}

		pADCDataRecThread->m_uiADCDataFrameSysTime = uiADCDataFrameSysTimeNow;
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
		ConvertCStrToStr(strOut, &strConv);
		AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", strConv);
	}
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
	iFrameCount = GetFrameCount(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pADCDataRecThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
				pADCDataRecThread->m_pADCDataFrame->m_cpRcvFrameData, 
				pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCDataRecThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentADCDataRecFrame(pADCDataRecThread->m_pADCDataFrame, 
					pADCDataRecThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCDataRecFrame",
						"", ErrorType, IDS_ERR_PARSE_ADCDATARECFRAME);
				}
				else
				{
					EnterCriticalSection(&pADCDataRecThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个ADC数据帧
					ProcADCDataRecFrameOne(pADCDataRecThread);
					LeaveCriticalSection(&pADCDataRecThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
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
			ConvertCStrToStr(str, &strConv);
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
				ConvertCStrToStr(str, &strConv);
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
		if (pADCDataRecThread->m_pThread->m_bClose == true)
		{
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
			break;
		}
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
	if (false == OnInitThread(pADCDataRecThread->m_pThread, pLogOutPut, pConstVar))
	{
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
		return false;
	}
	AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnInitADCDataRecThread", 
		"ADC数据接收线程创建成功");
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
	pEnv->m_pADCDataRecThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	return OnInitADCDataRecThread(pEnv->m_pADCDataRecThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC数据接收线程
bool OnCloseADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return false;
	}
	// 清空丢帧索引
	pADCDataRecThread->m_oADCLostFrameMap.clear();
	if (false == OnCloseThread(pADCDataRecThread->m_pThread))
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", 
			"ADC数据接收线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", 
			"ADC数据接收线程成功关闭");
		return true;
	}
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
	delete pADCDataRecThread;
}