#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建心跳帧信息结构体
m_oHeartBeatFrameStruct* OnCreateInstrHeartBeat(void)
{
	m_oHeartBeatFrameStruct* pHeartBeatFrame = NULL;
	pHeartBeatFrame = new m_oHeartBeatFrameStruct;
	InitializeCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_cpSndFrameData = NULL;
	pHeartBeatFrame->m_cpCommandWord = NULL;
	pHeartBeatFrame->m_oHeartBeatSocket = INVALID_SOCKET;
	pHeartBeatFrame->m_pCommandStruct = NULL;
	pHeartBeatFrame->m_usPortMove = 0;
	return pHeartBeatFrame;
}
// 初始化心跳
void OnInitInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	ASSERT(pHeartBeatFrame != NULL);
	ASSERT(pCommInfo != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	if (pHeartBeatFrame->m_pCommandStruct != NULL)
	{
		delete pHeartBeatFrame->m_pCommandStruct;
		pHeartBeatFrame->m_pCommandStruct = NULL;
	}
	pHeartBeatFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pHeartBeatFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pHeartBeatFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pHeartBeatFrame->m_pCommandStruct->m_usAimPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usAimPort
		+ pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 心跳返回端口
	pHeartBeatFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usHeartBeatReturnPort;
	// 心跳接收端口偏移量
	pHeartBeatFrame->m_usPortMove = pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pHeartBeatFrame->m_pCommandStruct->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrFramePacket(pHeartBeatFrame->m_pCommandStruct);
	// 清空发送帧缓冲区
	if (pHeartBeatFrame->m_cpSndFrameData != NULL)
	{
		delete[] pHeartBeatFrame->m_cpSndFrameData;
		pHeartBeatFrame->m_cpSndFrameData = NULL;
	}
	pHeartBeatFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pHeartBeatFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iSndFrameSize);
	// 清空心跳命令字集合
	if (pHeartBeatFrame->m_cpCommandWord != NULL)
	{
		delete[] pHeartBeatFrame->m_cpCommandWord;
		pHeartBeatFrame->m_cpCommandWord = NULL;
	}
	pHeartBeatFrame->m_cpCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pHeartBeatFrame->m_cpCommandWord, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iCommandWordMaxNum);
	// 心跳命令字个数
	pHeartBeatFrame->m_usCommandWordNum = 0;
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 关闭心跳帧信息结构体
void OnCloseInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	ASSERT(pHeartBeatFrame != NULL);
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	if (pHeartBeatFrame->m_cpSndFrameData != NULL)
	{
		delete[] pHeartBeatFrame->m_cpSndFrameData;
		pHeartBeatFrame->m_cpSndFrameData = NULL;
	}
	if (pHeartBeatFrame->m_cpCommandWord != NULL)
	{
		delete[] pHeartBeatFrame->m_cpCommandWord;
		pHeartBeatFrame->m_cpCommandWord = NULL;
	}
	if (pHeartBeatFrame->m_pCommandStruct != NULL)
	{
		delete pHeartBeatFrame->m_pCommandStruct;
		pHeartBeatFrame->m_pCommandStruct = NULL;
	}
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 释放心跳帧信息结构体
void OnFreeInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	ASSERT(pHeartBeatFrame != NULL);
	DeleteCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	delete pHeartBeatFrame;
	pHeartBeatFrame = NULL;
}
// 创建并设置心跳端口
void OnCreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pHeartBeatFrame != NULL);
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	// 创建套接字
	pHeartBeatFrame->m_oHeartBeatSocket = CreateInstrSocket(pHeartBeatFrame->m_pCommandStruct->m_usReturnPort + pHeartBeatFrame->m_usPortMove, 
		pHeartBeatFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrSocketBroadCast(pHeartBeatFrame->m_oHeartBeatSocket, pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetHeartBeatSocket", "创建并设置心跳端口！");
}
// 生成心跳帧
void MakeInstrHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pHeartBeatFrame != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_pCommandStruct->m_uiDstIP = pConstVar->m_uiIPBroadcastAddr;
	MakeInstrFrame(pHeartBeatFrame->m_pCommandStruct,  pConstVar, pHeartBeatFrame->m_cpSndFrameData,
		pHeartBeatFrame->m_cpCommandWord, pHeartBeatFrame->m_usCommandWordNum);
	SendFrame(pHeartBeatFrame->m_oHeartBeatSocket, pHeartBeatFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize,pHeartBeatFrame->m_pCommandStruct->m_usAimPort, 
		pHeartBeatFrame->m_pCommandStruct->m_uiAimIP, pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
