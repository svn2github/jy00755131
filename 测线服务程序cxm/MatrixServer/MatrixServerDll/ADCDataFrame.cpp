#include "stdafx.h"
#include "MatrixServerDll.h"
// 创建ADC数据帧信息结构体
m_oADCDataFrameStruct* OnCreateInstrumentADCDataFrame(void)
{
	m_oADCDataFrameStruct* pADCDataFrame = NULL;
	pADCDataFrame = new m_oADCDataFrameStruct;
	InitializeCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	pADCDataFrame->m_cpRcvFrameData = NULL;
	pADCDataFrame->m_cpSndFrameData = NULL;
	pADCDataFrame->m_oADCDataFrameSocket = INVALID_SOCKET;
	pADCDataFrame->m_pCommandStructSet = NULL;
	pADCDataFrame->m_pCommandStructReturn = NULL;
	return pADCDataFrame;
}
// 初始化ADC数据帧
void OnInitInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pADCDataFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentADCDataFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	if (pADCDataFrame->m_pCommandStructSet != NULL)
	{
		delete pADCDataFrame->m_pCommandStructSet;
	}
	pADCDataFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pADCDataFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pADCDataFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pADCDataFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
	// ADC数据帧发送缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// ADC数据帧接收缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// ADC数据帧返回端口
	pADCDataFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usADCDataReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pADCDataFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendADCCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pADCDataFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pADCDataFrame->m_cpSndFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpSndFrameData;
	}
	pADCDataFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pADCDataFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);

	// 重置帧内容解析变量
	if (pADCDataFrame->m_pCommandStructReturn != NULL)
	{
		delete pADCDataFrame->m_pCommandStructReturn;
	}
	pADCDataFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pADCDataFrame->m_pCommandStructReturn);
	pADCDataFrame->m_pCommandStructReturn->m_pADCData = new int[pConstVar->m_iADCDataInOneFrameNum];
	// 清空接收帧缓冲区
	if (pADCDataFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpRcvFrameData;
	}
	pADCDataFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pADCDataFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 关闭ADC数据帧信息结构体
void OnCloseInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	if (pADCDataFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	if (pADCDataFrame->m_cpSndFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpSndFrameData;
		pADCDataFrame->m_cpSndFrameData = NULL;
	}
	if (pADCDataFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpRcvFrameData;
		pADCDataFrame->m_cpRcvFrameData = NULL;
	}
	if (pADCDataFrame->m_pCommandStructSet != NULL)
	{
		delete pADCDataFrame->m_pCommandStructSet;
		pADCDataFrame->m_pCommandStructSet = NULL;
	}
	if (pADCDataFrame->m_pCommandStructReturn != NULL)
	{
		if (pADCDataFrame->m_pCommandStructReturn->m_pADCData != NULL)
		{
			delete[] pADCDataFrame->m_pCommandStructReturn->m_pADCData;
		}
		delete pADCDataFrame->m_pCommandStructReturn;
		pADCDataFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 释放ADC数据帧信息结构体
void OnFreeInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	if (pADCDataFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	delete pADCDataFrame;
}
// 创建并设置ADC数据帧端口
void OnCreateAndSetADCDataFrameSocket(m_oADCDataFrameStruct* pADCDataFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pADCDataFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	// 创建套接字
	pADCDataFrame->m_oADCDataFrameSocket = CreateInstrumentSocket(pADCDataFrame->m_pCommandStructSet->m_usReturnPort, 
		pADCDataFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pADCDataFrame->m_oADCDataFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetADCDataFrameSocket", "创建并设置ADC数据帧端口！");
}
// 解析ADC数据接收帧
bool ParseInstrumentADCDataRecFrame(m_oADCDataFrameStruct* pADCDataFrame, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pADCDataFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentADCDataRecFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	bReturn = ParseInstrumentFrame(pADCDataFrame->m_pCommandStructReturn, 
		pADCDataFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	return bReturn;
}
// 生成ADC数据查询帧
void MakeInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiIP, unsigned short usDataPoint)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pADCDataFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentADCDataFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	// 仪器IP地址
	pADCDataFrame->m_pCommandStructSet->m_uiDstIP = uiIP;
	pADCDataFrame->m_pCommandStructSet->m_usADCDataPoint = usDataPoint;
	MakeInstrumentFrame(pADCDataFrame->m_pCommandStructSet, pConstVar, pADCDataFrame->m_cpSndFrameData);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 发送ADC数据查询帧
void SendInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pADCDataFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SendInstrumentADCDataFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	SendFrame(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pADCDataFrame->m_pCommandStructSet->m_usAimPort, 
		pADCDataFrame->m_pCommandStructSet->m_uiAimIP, pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}