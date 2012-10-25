#include "stdafx.h"
#include "MatrixServerDll.h"
// 创建ADC数据帧信息结构体
m_oADCDataFrameStruct* OnCreateInstrADCDataFrame(void)
{
	m_oADCDataFrameStruct* pADCDataFrame = NULL;
	pADCDataFrame = new m_oADCDataFrameStruct;
	InitializeCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	pADCDataFrame->m_cpRcvFrameData = NULL;
	pADCDataFrame->m_cpSndFrameData = NULL;
	pADCDataFrame->m_oADCDataFrameSocket = INVALID_SOCKET;
	pADCDataFrame->m_pCommandStructSet = NULL;
	pADCDataFrame->m_pCommandStructReturn = NULL;
	pADCDataFrame->m_usPortMove = 0;
	return pADCDataFrame;
}
// 初始化ADC数据帧
void OnInitInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	ASSERT(pADCDataFrame != NULL);
	ASSERT(pCommInfo != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	int iBufLen = 0;
	if (pADCDataFrame->m_pCommandStructSet != NULL)
	{
		delete pADCDataFrame->m_pCommandStructSet;
		pADCDataFrame->m_pCommandStructSet = NULL;
	}
	pADCDataFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pADCDataFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pADCDataFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pADCDataFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usAimPort
		+ pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// ADC数据帧发送缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// ADC数据帧接收缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// ADC数据帧返回端口
	pADCDataFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usADCDataReturnPort;
	// ADC数据帧接收端口偏移量
	pADCDataFrame->m_usPortMove = pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pADCDataFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendADCCmd;
	// 重置帧内容解析变量
	ResetInstrFramePacket(pADCDataFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pADCDataFrame->m_cpSndFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpSndFrameData;
		pADCDataFrame->m_cpSndFrameData = NULL;
	}
	pADCDataFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pADCDataFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);

	// 重置帧内容解析变量
	if (pADCDataFrame->m_pCommandStructReturn != NULL)
	{
		delete pADCDataFrame->m_pCommandStructReturn;
		pADCDataFrame->m_pCommandStructReturn = NULL;
	}
	pADCDataFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrFramePacket(pADCDataFrame->m_pCommandStructReturn);
	iBufLen = pConstVar->m_iADCDataInOneFrameNum * pConstVar->m_iADCDataSize3B;
	pADCDataFrame->m_pCommandStructReturn->m_pADCDataBuf = new char[iBufLen];
	// 清空接收帧缓冲区
	if (pADCDataFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pADCDataFrame->m_cpRcvFrameData;
		pADCDataFrame->m_cpRcvFrameData = NULL;
	}
	pADCDataFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pADCDataFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 关闭ADC数据帧信息结构体
void OnCloseInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	ASSERT(pADCDataFrame != NULL);
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
		if (pADCDataFrame->m_pCommandStructReturn->m_pADCDataBuf != NULL)
		{
			delete[] pADCDataFrame->m_pCommandStructReturn->m_pADCDataBuf;
			pADCDataFrame->m_pCommandStructReturn->m_pADCDataBuf = NULL;
		}
		delete pADCDataFrame->m_pCommandStructReturn;
		pADCDataFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 释放ADC数据帧信息结构体
void OnFreeInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	ASSERT(pADCDataFrame != NULL);
	DeleteCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	delete pADCDataFrame;
	pADCDataFrame = NULL;
}
// 创建并设置ADC数据帧端口
void OnCreateAndSetADCDataFrameSocket(m_oADCDataFrameStruct* pADCDataFrame, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pADCDataFrame != NULL);
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	// 创建套接字
	pADCDataFrame->m_oADCDataFrameSocket = CreateInstrSocket(pADCDataFrame->m_pCommandStructSet->m_usReturnPort + pADCDataFrame->m_usPortMove, 
		pADCDataFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrSocketBroadCast(pADCDataFrame->m_oADCDataFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetADCDataFrameSocket", "创建并设置ADC数据帧端口！");
}
// 解析ADC数据接收帧
bool ParseInstrADCDataRecFrame(m_oADCDataFrameStruct* pADCDataFrame, m_oConstVarStruct* pConstVar, 
	m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pADCDataFrame != NULL);
	ASSERT(pConstVar != NULL);
	bool bReturn = false;
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	bReturn = ParseInstrFrame(pADCDataFrame->m_pCommandStructReturn, 
		pADCDataFrame->m_cpRcvFrameData, pConstVar, pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	return bReturn;
}
// 生成ADC数据查询帧
void MakeInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiIP, 
	unsigned short usDataPoint, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pADCDataFrame != NULL);
	ASSERT(pConstVar != NULL);
	CString str = _T("");
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	// 仪器IP地址
	pADCDataFrame->m_pCommandStructSet->m_uiDstIP = uiIP;
	pADCDataFrame->m_pCommandStructSet->m_usADCDataPoint = usDataPoint;
	MakeInstrFrame(pADCDataFrame->m_pCommandStructSet, pConstVar, 
		pADCDataFrame->m_cpSndFrameData);
	SendFrame(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pADCDataFrame->m_pCommandStructSet->m_usAimPort, 
		pADCDataFrame->m_pCommandStructSet->m_uiAimIP, pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}