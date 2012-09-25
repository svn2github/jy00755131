#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建时统设置帧信息结构体
m_oTimeDelayFrameStruct* OnCreateInstrumentTimeDelayFrame(void)
{
	m_oTimeDelayFrameStruct* pTimeDelayFrame = NULL;
	pTimeDelayFrame = new m_oTimeDelayFrameStruct;
	InitializeCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	pTimeDelayFrame->m_cpRcvFrameData = NULL;
	pTimeDelayFrame->m_cpSndFrameData = NULL;
	pTimeDelayFrame->m_cpCommandWord = NULL;
	pTimeDelayFrame->m_oTimeDelayFrameSocket = INVALID_SOCKET;
	pTimeDelayFrame->m_pCommandStructSet = NULL;
	pTimeDelayFrame->m_pCommandStructReturn = NULL;
	pTimeDelayFrame->m_usPortMove = 0;
	return pTimeDelayFrame;
}
// 初始化时统设置帧
void OnInitInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pTimeDelayFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentTimeDelayFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	if (pTimeDelayFrame->m_pCommandStructSet != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructSet;
		pTimeDelayFrame->m_pCommandStructSet = NULL;
	}
	pTimeDelayFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pTimeDelayFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usAimPort;
	// 时统设置发送缓冲区帧数设定为仪器个数
	pTimeDelayFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// 时统设置应答接收缓冲区帧数设定为仪器个数
	pTimeDelayFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 时统设置返回端口
	pTimeDelayFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usTimeDelayReturnPort;
	// 时统设置接收端口偏移量
	pTimeDelayFrame->m_usPortMove = pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pTimeDelayFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pTimeDelayFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pTimeDelayFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_cpSndFrameData;
		pTimeDelayFrame->m_cpSndFrameData = NULL;
	}
	pTimeDelayFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pTimeDelayFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iSndFrameSize);
	// 清空时统设置命令字集合
	if (pTimeDelayFrame->m_cpCommandWord != NULL)
	{
		delete[] pTimeDelayFrame->m_cpCommandWord;
		pTimeDelayFrame->m_cpCommandWord = NULL;
	}
	pTimeDelayFrame->m_cpCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pTimeDelayFrame->m_cpCommandWord, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iCommandWordMaxNum);
	// 时统设置命令字个数
	pTimeDelayFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pTimeDelayFrame->m_pCommandStructReturn != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructReturn;
		pTimeDelayFrame->m_pCommandStructReturn = NULL;
	}
	pTimeDelayFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pTimeDelayFrame->m_pCommandStructReturn);
	// 清空接收帧缓冲区
	if (pTimeDelayFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_cpRcvFrameData;
		pTimeDelayFrame->m_cpRcvFrameData = NULL;
	}
	pTimeDelayFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTimeDelayFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 关闭时统设置帧信息结构体
void OnCloseInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame)
{
	if (pTimeDelayFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	if (pTimeDelayFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_cpSndFrameData;
		pTimeDelayFrame->m_cpSndFrameData = NULL;
	}
	if (pTimeDelayFrame->m_cpCommandWord != NULL)
	{
		delete[] pTimeDelayFrame->m_cpCommandWord;
		pTimeDelayFrame->m_cpCommandWord = NULL;
	}
	if (pTimeDelayFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_cpRcvFrameData;
		pTimeDelayFrame->m_cpRcvFrameData = NULL;
	}
	if (pTimeDelayFrame->m_pCommandStructSet != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructSet;
		pTimeDelayFrame->m_pCommandStructSet = NULL;
	}
	if (pTimeDelayFrame->m_pCommandStructReturn != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructReturn;
		pTimeDelayFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 释放时统设置帧信息结构体
void OnFreeInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame)
{
	if (pTimeDelayFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	delete pTimeDelayFrame;
	pTimeDelayFrame = NULL;
}
// 创建并设置时统设置端口
void OnCreateAndSetTimeDelayFrameSocket(m_oTimeDelayFrameStruct* pTimeDelayFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pTimeDelayFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 创建套接字
	pTimeDelayFrame->m_oTimeDelayFrameSocket = CreateInstrumentSocket(pTimeDelayFrame->m_pCommandStructSet->m_usReturnPort + pTimeDelayFrame->m_usPortMove, 
		pTimeDelayFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pTimeDelayFrame->m_oTimeDelayFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pTimeDelayFrame->m_oTimeDelayFrameSocket, 
		pTimeDelayFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pTimeDelayFrame->m_oTimeDelayFrameSocket, 
		pTimeDelayFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTimeDelayFrameSocket", "创建并设置时统设置帧端口！");
}
// 解析时统设置应答帧
bool ParseInstrumentTimeDelayReturnFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pTimeDelayFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentTimeDelayReturnFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	bReturn = ParseInstrumentFrame(pTimeDelayFrame->m_pCommandStructReturn, 
		pTimeDelayFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	return bReturn;
}
// 生成时统设置帧
void MakeInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pTimeDelayFrame == NULL) || (pInstrument == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentDelayTimeFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	unsigned short usPos = 0;
	// 接收到时统设置应答标志位
	pInstrument->m_bTimeSetOK = false;
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 仪器IP地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiDstIP = pInstrument->m_uiIP;
	// 设置命令
	pTimeDelayFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 时统设置高位
	pTimeDelayFrame->m_pCommandStructSet->m_uiLocalTimeFixedHigh = pInstrument->m_uiTimeHigh;
	// 时统设置低位
	pTimeDelayFrame->m_pCommandStructSet->m_uiLocalTimeFixedLow = pInstrument->m_uiTimeLow;
	// 设置命令字内容
	// 时统设置高位
	pTimeDelayFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLocalTimeFixedHigh;
	usPos ++;
	// 时统设置低位
	pTimeDelayFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLocalTimeFixedLow;
	usPos ++;
	// 设置命令字个数
	pTimeDelayFrame->m_usCommandWordNum = usPos;
	MakeInstrumentFrame(pTimeDelayFrame->m_pCommandStructSet, pConstVar, pTimeDelayFrame->m_cpSndFrameData, 
		pTimeDelayFrame->m_cpCommandWord, pTimeDelayFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 发送时统设置帧
void SendInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTimeDelayFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SendInstrumentDelayTimeFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	SendFrame(pTimeDelayFrame->m_oTimeDelayFrameSocket, pTimeDelayFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pTimeDelayFrame->m_pCommandStructSet->m_usAimPort, 
		pTimeDelayFrame->m_pCommandStructSet->m_uiAimIP, pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}