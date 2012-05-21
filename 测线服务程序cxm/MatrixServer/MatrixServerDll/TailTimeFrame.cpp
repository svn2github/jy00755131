#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建尾包时刻帧信息结构体
m_oTailTimeFrameStruct* OnCreateInstrumentTailTimeFrame(void)
{
	m_oTailTimeFrameStruct* pTailTimeFrame = NULL;
	pTailTimeFrame = new m_oTailTimeFrameStruct;
	InitializeCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	pTailTimeFrame->m_cpRcvFrameData = NULL;
	pTailTimeFrame->m_cpSndFrameData = NULL;
	pTailTimeFrame->m_cpCommandWord = NULL;
	pTailTimeFrame->m_oTailTimeFrameSocket = INVALID_SOCKET;
	pTailTimeFrame->m_pCommandStructSet = NULL;
	pTailTimeFrame->m_pCommandStructReturn = NULL;
	return pTailTimeFrame;
}
// 初始化尾包时刻帧
void OnInitInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pTailTimeFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentTailTimeFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_pCommandStructSet != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructSet;
	}
	pTailTimeFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pTailTimeFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pTailTimeFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_oXMLPortSetupData.m_usAimPort;
	// 尾包时刻发送缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// 尾包时刻应答接收缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 尾包时刻查询返回端口
	pTailTimeFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_oXMLPortSetupData.m_usTailTimeReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pTailTimeFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pTailTimeFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpSndFrameData;
	}
	pTailTimeFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pTailTimeFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iSndFrameSize);
	// 清空尾包时刻查询命令字集合
	if (pTailTimeFrame->m_cpCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_cpCommandWord;
	}
	pTailTimeFrame->m_cpCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pTailTimeFrame->m_cpCommandWord, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iCommandWordMaxNum);
	// 尾包时刻查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pTailTimeFrame->m_pCommandStructReturn != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructReturn;
	}
	pTailTimeFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pTailTimeFrame->m_pCommandStructReturn);
	// 清空接收帧缓冲区
	if (pTailTimeFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpRcvFrameData;
	}
	pTailTimeFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTailTimeFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 关闭尾包时刻帧信息结构体
void OnCloseInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	if (pTailTimeFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpSndFrameData;
		pTailTimeFrame->m_cpSndFrameData = NULL;
	}
	if (pTailTimeFrame->m_cpCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_cpCommandWord;
		pTailTimeFrame->m_cpCommandWord = NULL;
	}
	if (pTailTimeFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpRcvFrameData;
		pTailTimeFrame->m_cpRcvFrameData = NULL;
	}
	if (pTailTimeFrame->m_pCommandStructSet != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructSet;
		pTailTimeFrame->m_pCommandStructSet = NULL;
	}
	if (pTailTimeFrame->m_pCommandStructReturn != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructReturn;
		pTailTimeFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 释放尾包时刻帧信息结构体
void OnFreeInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	if (pTailTimeFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	delete pTailTimeFrame;
}
// 创建并设置尾包时刻端口
void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pTailTimeFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 创建套接字
	pTailTimeFrame->m_oTailTimeFrameSocket = CreateInstrumentSocket(pTailTimeFrame->m_pCommandStructSet->m_usReturnPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pTailTimeFrame->m_oTailTimeFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, 
		pTailTimeFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, 
		pTailTimeFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTailTimeFrameSocket", "创建并设置尾包时刻查询帧端口！");
}
// 解析尾包时刻查询帧
bool ParseInstrumentTailTimeReturnFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pTailTimeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentTailTimeReturnFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	bReturn = ParseInstrumentFrame(pTailTimeFrame->m_pCommandStructReturn, 
		pTailTimeFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	return bReturn;
}
// 按IP地址查询尾包时刻帧
void MakeInstrumentTailTimeQueryFramebyIP(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTailTimeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentTailTimeQueryFramebyIP", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	unsigned short usPos = 0;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 仪器IP地址
	pTailTimeFrame->m_pCommandStructSet->m_uiDstIP = uiInstrumentIP;
	// 查询命令
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLAUTailRecTimeLAUX;
	usPos ++;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLineTailRecTimeLAUX;
	usPos ++;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdTailRecSndTime;
	usPos ++;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = usPos;
	MakeInstrumentFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_cpSndFrameData, 
		pTailTimeFrame->m_cpCommandWord, pTailTimeFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 广播查询尾包时刻帧
void MakeInstrumentTailTimeQueryFramebyBroadCast(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTailTimeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentTailTimeQueryFramebyBroadCast", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	unsigned short usPos = 0;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 仪器IP地址
	pTailTimeFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_uiIPBroadcastAddr;
	// 查询命令
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 广播端口
	pTailTimeFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiBroadCastPort;
	// 查询命令字内容
	// 广播端口
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdBroadCastPortSeted;
	usPos ++;
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLAUTailRecTimeLAUX;
	usPos ++;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdLineTailRecTimeLAUX;
	usPos ++;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_cpCommandWord[usPos] = pConstVar->m_cCmdTailRecSndTime;
	usPos ++;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = usPos;
	MakeInstrumentFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_cpSndFrameData, 
		pTailTimeFrame->m_cpCommandWord, pTailTimeFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 发送尾包时刻查询帧
void SendInstrumentTailTimeQueryFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTailTimeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SendInstrumentTailTimeQueryFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	SendFrame(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pTailTimeFrame->m_pCommandStructSet->m_usAimPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiAimIP, pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}