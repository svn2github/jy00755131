#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建尾包帧信息结构体
m_oTailFrameStruct* OnCreateInstrTailFrame(void)
{
	m_oTailFrameStruct* pTailFrame = NULL;
	pTailFrame = new m_oTailFrameStruct;
	InitializeCriticalSection(&pTailFrame->m_oSecTailFrame);
	pTailFrame->m_cpRcvFrameData = NULL;
	pTailFrame->m_oTailFrameSocket = INVALID_SOCKET;
	pTailFrame->m_pCommandStruct = NULL;
	pTailFrame->m_usPortMove = 0;
	return pTailFrame;
}
// 初始化尾包
void OnInitInstrTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	ASSERT(pTailFrame != NULL);
	ASSERT(pCommInfo != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	if (pTailFrame->m_pCommandStruct != NULL)
	{
		delete pTailFrame->m_pCommandStruct;
		pTailFrame->m_pCommandStruct = NULL;
	}
	pTailFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pTailFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pTailFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 尾包接收缓冲区帧数设定为仪器个数
	pTailFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 接收端口
	pTailFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usTailFramePort;
	// 尾包接收端口偏移量
	pTailFrame->m_usPortMove = pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 重置帧内容解析变量
	ResetInstrFramePacket(pTailFrame->m_pCommandStruct);
	// 清空接收帧缓冲区
	if (pTailFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTailFrame->m_cpRcvFrameData;
		pTailFrame->m_cpRcvFrameData = NULL;
	}
	pTailFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTailFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
}
// 关闭尾包帧信息结构体
void OnCloseInstrTailFrame(m_oTailFrameStruct* pTailFrame)
{
	ASSERT(pTailFrame != NULL);
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	if (pTailFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTailFrame->m_cpRcvFrameData;
		pTailFrame->m_cpRcvFrameData = NULL;
	}
	if (pTailFrame->m_pCommandStruct != NULL)
	{
		delete pTailFrame->m_pCommandStruct;
		pTailFrame->m_pCommandStruct = NULL;
	}
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
}
// 释放尾包帧信息结构体
void OnFreeInstrTailFrame(m_oTailFrameStruct* pTailFrame)
{
	ASSERT(pTailFrame != NULL);
	DeleteCriticalSection(&pTailFrame->m_oSecTailFrame);
	delete pTailFrame;
	pTailFrame = NULL;
}
// 创建并设置尾包端口
void OnCreateAndSetTailFrameSocket(m_oTailFrameStruct* pTailFrame, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pTailFrame != NULL);
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	// 创建套接字
	pTailFrame->m_oTailFrameSocket = CreateInstrSocket(pTailFrame->m_pCommandStruct->m_usReturnPort + pTailFrame->m_usPortMove, 
		pTailFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pTailFrame->m_oTailFrameSocket, pTailFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTailFrameSocket", "创建并设置尾包端口！");
}
// 解析尾包帧
bool ParseInstrTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pTailFrame != NULL);
	ASSERT(pConstVar != NULL);
	bool bReturn = false;
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	bReturn = ParseInstrFrame(pTailFrame->m_pCommandStruct, 
		pTailFrame->m_cpRcvFrameData, pConstVar, pLogOutPut);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
	return bReturn;
}