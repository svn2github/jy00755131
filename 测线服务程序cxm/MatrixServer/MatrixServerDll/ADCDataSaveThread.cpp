#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建施工放炮数据存储线程
m_oADCDataSaveThreadStruct* OnCreateADCDataSaveThread(void)
{
	m_oADCDataSaveThreadStruct* pADCDataSaveThread = NULL;
	pADCDataSaveThread = new m_oADCDataSaveThreadStruct;
	pADCDataSaveThread->m_pThread = new m_oThreadStruct;
	pADCDataSaveThread->m_pADCDataBufArray = NULL;
	pADCDataSaveThread->m_pOptTaskArray = NULL;
	InitializeCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	return pADCDataSaveThread;
}
// 线程等待函数
void WaitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	if (pADCDataSaveThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCDataSaveThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		bClose = pADCDataSaveThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pADCDataSaveThread->m_pThread->m_pConstVar->m_iADCDataSaveSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 保存ADC数据到施工文件
void WriteADCDataInOptTaskFile(m_oADCDataBufStruct* pADCDataBuf, 
	m_oOptTaskStruct* pOptTask, unsigned int uiLineIndex, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pOptTask == NULL) || (pADCDataBuf == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "WriteADCDataInOptTaskFile", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	unsigned int uiFileNb = 0;
	unsigned int uiFrameInFileNb = 0;
	CString strPath = _T("");
	CString str = _T("");
	string strOut = "";
	string strConv = "";
	FILE* pFile = NULL;
	char pdata[100];
	size_t strOutSize = 0;
	long lOffSet = 0;
	// 一行的长度（字符为单位）
	unsigned int uiLineLength = 0;
	// 行号
	unsigned int uiLineNb = 0;
	// 列号（字符为单位）
	unsigned int uiRowNb = 0;
	errno_t err;
	uiFileNb = (pADCDataBuf->m_uiFrameNb - pOptTask->m_uiStartFrame) / pConstVar->m_iADCFrameSaveInOneFileNum;
	if (uiFileNb > pOptTask->m_uiFileSaveNb)
	{
		pOptTask->m_uiFileSaveNb = uiFileNb;
	}
	uiFrameInFileNb = (pADCDataBuf->m_uiFrameNb - pOptTask->m_uiStartFrame) % pConstVar->m_iADCFrameSaveInOneFileNum;
	// 得到文件路径
	strPath.Format(_T("\\%d.txt"), uiFileNb);
	str = pOptTask->m_SaveLogFilePath.c_str();
	strPath = str + strPath;
	// 一行的长度=左侧信息长度+（间隔长度+数据长度）*数据帧数*一帧数据个数 + 换行符长度
	uiLineLength = pConstVar->m_iADCSaveLeftInfoBytes 
		+ (pConstVar->m_iADCSaveDataIntervalBytes + pConstVar->m_iADCSaveDataBytes)
		* pConstVar->m_iADCFrameSaveInOneFileNum * pConstVar->m_iADCDataInOneFrameNum + 2;
	uiLineNb = pConstVar->m_iADCSaveHeadLineNum + uiLineIndex;
	uiRowNb = pConstVar->m_iADCSaveLeftInfoBytes 
		+ (pConstVar->m_iADCSaveDataIntervalBytes + pConstVar->m_iADCSaveDataBytes) 
		* uiFrameInFileNb * pConstVar->m_iADCDataInOneFrameNum;
	// 如果文件不存在则创建
	if (false == IfFileExist(strPath))
	{
		pOptTask->m_pPreviousFile = pOptTask->m_pFile;
		strConv = (CStringA)strPath;
		err = fopen_s(&pOptTask->m_pFile, strConv.c_str(), "w+t, ccs=UNICODE");
		if (pOptTask->m_pFile == NULL)
		{
			return;
		}
		lOffSet = uiLineLength - 2;
		strOut = '\n';
		strOutSize = strOut.length();
		for (unsigned int i=0; i<(pOptTask->m_oSNMap.size()+pConstVar->m_iADCSaveHeadLineNum); i++)
		{
			fseek(pOptTask->m_pFile, lOffSet, SEEK_CUR);
			fwrite(strOut.c_str(), sizeof(char), strOutSize, pOptTask->m_pFile);
		}
		// 文件刚创建，写前三行文件头信息：
		// 第一行
		str.Format(_T("施工任务数据存储从 %d 帧开始"), pOptTask->m_uiStartFrame);
		fseek(pOptTask->m_pFile, 0, SEEK_SET);
		strOut = (CStringA)str;
		strOutSize = strOut.length();
		fwrite(strOut.c_str(), sizeof(char), strOutSize, pOptTask->m_pFile);
		// 第三行左侧信息
		lOffSet = uiLineLength * (pConstVar->m_iADCSaveHeadLineNum - 1);
		fseek(pOptTask->m_pFile, lOffSet, SEEK_SET);
		strOut = "";
		for (int i=0; i<(pConstVar->m_iADCSaveLeftInfoBytes - 1); i++)
		{
			strOut += '-';
		}
		strOut += ' ';
		strOutSize = strOut.length();
		fwrite(strOut.c_str(), sizeof(char), strOutSize, pOptTask->m_pFile);
	}
	// 表明存的是前一个文件
	if (uiFileNb < pOptTask->m_uiFileSaveNb)
	{
		pFile = pOptTask->m_pPreviousFile;
	}
	// 存的是最新的文件
	else
	{
		if ((pOptTask->m_pPreviousFile != NULL) 
			&& (uiFrameInFileNb > (static_cast<unsigned int>(pConstVar->m_iADCDataBufSize / pConstVar->m_iADCDataInOneFrameNum))))
		{
			fclose(pOptTask->m_pPreviousFile);
			pOptTask->m_pPreviousFile = NULL;
		}
		pFile = pOptTask->m_pFile;
	}
	if (pFile == NULL)
	{
		return;
	}

	lOffSet = uiLineLength * uiLineNb + uiRowNb;
	fseek(pFile, lOffSet, SEEK_SET);
	strOut = "";
	for (int i=0; i<pConstVar->m_iADCDataInOneFrameNum; i++)
	{
		sprintf_s(pdata, 100, "%*d ", pConstVar->m_iADCSaveDataBytes, pADCDataBuf->m_pADCDataBuf[i]);
		strOut += pdata;
	}
	strOutSize = strOut.length();
	fwrite(strOut.c_str(), sizeof(char), strOutSize, pFile);

	// 写第一个仪器的采样数据时写入采样帧的本地时间作为第三行
	if (uiLineIndex == 0)
	{
		lOffSet = uiLineLength * (pConstVar->m_iADCSaveHeadLineNum - 1) + uiRowNb;
		fseek(pFile, lOffSet, SEEK_SET);
		str.Format(_T("%*d "), pConstVar->m_iADCSaveDataBytes, pADCDataBuf->m_uiSysTime);
		strOut = (CStringA)str;
		for (int i=0; i<(pConstVar->m_iADCDataInOneFrameNum - 1); i++)
		{
			for (int j=0; j<pConstVar->m_iADCSaveDataBytes; j++)
			{
				strOut += '-';
			}
			strOut += ' ';
		}
		strOutSize = strOut.length();
		fwrite(strOut.c_str(), sizeof(char), strOutSize, pFile);
	}
	// 记录文件中每个设备的最后一个数据帧时将SN写入左侧信息区
/*	if ((uiFrameInFileNb + 1) == static_cast<unsigned int>(pConstVar->m_iADCFrameSaveInOneFileNum))*/
	if (uiFrameInFileNb == 0)
	{
		lOffSet = uiLineLength * uiLineNb;
		fseek(pFile, lOffSet, SEEK_SET);
		str.Format(_T("SN=0x%x "), pADCDataBuf->m_uiSN);
		strOut = (CStringA)str;
		strOutSize = strOut.length();
		fwrite(strOut.c_str(), sizeof(char), strOutSize, pFile);
	}
}
// 关闭所有的施工文件
void CloseAllADCDataSaveInFile(m_oOptTaskArrayStruct* pOptTaskArray)
{
	if (pOptTaskArray == NULL)
	{
		return;
	}
	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter;
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	for (iter = pOptTaskArray->m_oOptTaskWorkMap.begin();
		iter != pOptTaskArray->m_oOptTaskWorkMap.end(); iter++)
	{
		if (iter->second->m_pPreviousFile != NULL)
		{
			fclose(iter->second->m_pPreviousFile);
		}
		if (iter->second->m_pFile != NULL)
		{
			fclose(iter->second->m_pFile);
		}
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
// 保存ADC数据到施工文件
void ProcADCDataSaveInFile(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	if (pADCDataSaveThread == NULL)
	{
		return;
	}
	list<m_oADCDataBufStruct*>::iterator iter;
	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter2;
	// 行号
	unsigned int uiLineIndex = 0;
	m_oADCDataBufStruct* pADCDataBuf = NULL;
	EnterCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
	EnterCriticalSection(&pADCDataSaveThread->m_pOptTaskArray->m_oSecOptTaskArray);
	while(pADCDataSaveThread->m_pADCDataBufArray->m_olsADCDataToWrite.empty() == false)
	{
		iter = pADCDataSaveThread->m_pADCDataBufArray->m_olsADCDataToWrite.begin();
		pADCDataBuf = *iter;
		pADCDataSaveThread->m_pADCDataBufArray->m_olsADCDataToWrite.pop_front();
		// 将数据按照施工任务写入文件
		for (iter2 = pADCDataSaveThread->m_pOptTaskArray->m_oOptTaskWorkMap.begin();
			iter2 != pADCDataSaveThread->m_pOptTaskArray->m_oOptTaskWorkMap.end(); iter2++)
		{
			// 仪器在施工任务仪器索引表中
			if (TRUE == IfIndexExistInOptTaskSNMap(pADCDataBuf->m_uiSN, &iter2->second->m_oSNMap))
			{
				uiLineIndex = GetLineNbFromOptTaskSNMap(pADCDataBuf->m_uiSN, &iter2->second->m_oSNMap);
				// 将数据写入文件
				WriteADCDataInOptTaskFile(pADCDataBuf, iter2->second, uiLineIndex, 
					pADCDataSaveThread->m_pThread->m_pConstVar);
			}
		}
		// 将该数据缓冲加入空闲任务队列
		AddFreeADCDataBuf(pADCDataBuf, pADCDataSaveThread->m_pADCDataBufArray);
	}
	LeaveCriticalSection(&pADCDataSaveThread->m_pOptTaskArray->m_oSecOptTaskArray);
	LeaveCriticalSection(&pADCDataSaveThread->m_pADCDataBufArray->m_oSecADCDataBufArray);
}
// 线程函数
DWORD WINAPI RunADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	if (pADCDataSaveThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		if (pADCDataSaveThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
			break;
		}
		if (pADCDataSaveThread->m_pThread->m_bWork == true)
		{
			// 保存ADC数据到施工文件
			ProcADCDataSaveInFile(pADCDataSaveThread);
		}
		if (pADCDataSaveThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
			break;
		}
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		WaitADCDataSaveThread(pADCDataSaveThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pADCDataSaveThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化施工放炮数据存储线程
bool OnInitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pADCDataSaveThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	if (false == OnInitThread(pADCDataSaveThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pADCDataSaveThread->m_pThread->m_hThreadClose);
	// 创建线程
	pADCDataSaveThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCDataSaveThread,
		pADCDataSaveThread, 
		0, 
		&pADCDataSaveThread->m_pThread->m_dwThreadID);
	if (pADCDataSaveThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnInitADCDataSaveThread", 
			"pADCDataSaveThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
		return false;
	}
	AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnInitADCDataSaveThread", 
		"ADC数据存储线程创建成功");
	LeaveCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	return true;
}
// 初始化施工放炮数据存储线程
bool OnInit_ADCDataSaveThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pADCDataSaveThread->m_pADCDataBufArray = pEnv->m_pADCDataBufArray;
	pEnv->m_pADCDataSaveThread->m_pOptTaskArray = pEnv->m_pOptTaskArray;
	return OnInitADCDataSaveThread(pEnv->m_pADCDataSaveThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭施工放炮数据存储线程
bool OnCloseADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	if (pADCDataSaveThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pADCDataSaveThread->m_pThread))
	{
		AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnCloseADCDataSaveThread", 
			"ADC数据存储线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pADCDataSaveThread->m_pThread->m_pLogOutPut, "OnCloseADCDataSaveThread", 
		"ADC数据存储线程成功关闭");
	return true;
}
// 释放施工放炮数据存储线程
void OnFreeADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread)
{
	if (pADCDataSaveThread == NULL)
	{
		return;
	}
	if (pADCDataSaveThread->m_pThread != NULL)
	{
		delete pADCDataSaveThread->m_pThread;
	}
	DeleteCriticalSection(&pADCDataSaveThread->m_oSecADCDataSaveThread);
	delete pADCDataSaveThread;
}
