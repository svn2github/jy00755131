#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建首包帧信息结构体
m_oHeadFrameStruct* OnCreateInstrumentHeadFrame(void)
{
	m_oHeadFrameStruct* pHeadFrame = NULL;
	pHeadFrame = new m_oHeadFrameStruct;
	InitializeCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	pHeadFrame->m_cpRcvFrameData = NULL;
	pHeadFrame->m_oHeadFrameSocket = INVALID_SOCKET;
	pHeadFrame->m_pCommandStruct = NULL;
	return pHeadFrame;
}
// 初始化首包
void OnInitInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pHeadFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentHeadFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_pCommandStruct != NULL)
	{
		delete pHeadFrame->m_pCommandStruct;
		pHeadFrame->m_pCommandStruct = NULL;
	}
	pHeadFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pHeadFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pHeadFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 首包接收缓冲区帧数设定为仪器个数
	pHeadFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 接收端口
	pHeadFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pHeadFrame->m_pCommandStruct);
	// 清空接收帧缓冲区
	if (pHeadFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_cpRcvFrameData;
		pHeadFrame->m_cpRcvFrameData = NULL;
	}
	pHeadFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pHeadFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 关闭首包帧信息结构体
void OnCloseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_cpRcvFrameData;
		pHeadFrame->m_cpRcvFrameData = NULL;
	}
	if (pHeadFrame->m_pCommandStruct != NULL)
	{
		delete pHeadFrame->m_pCommandStruct;
		pHeadFrame->m_pCommandStruct = NULL;
	}
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 释放首包帧信息结构体
void OnFreeInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	delete pHeadFrame;
	pHeadFrame = NULL;
}
// 创建并设置首包端口
void OnCreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	// 创建套接字
	pHeadFrame->m_oHeadFrameSocket = CreateInstrumentSocket(pHeadFrame->m_pCommandStruct->m_usReturnPort + NetedPortMove, 
		pHeadFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pHeadFrame->m_oHeadFrameSocket, pHeadFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetHeadFrameSocket", "创建并设置首包端口！");
}
// 解析首包帧
bool ParseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pHeadFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentHeadFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	bReturn = ParseInstrumentFrame(pHeadFrame->m_pCommandStruct, 
		pHeadFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	return bReturn;
}