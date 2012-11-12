#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建尾包时刻帧信息结构体
m_oTailTimeFrameStruct* OnCreateInstrTailTimeFrame(void)
{
	m_oTailTimeFrameStruct* pTailTimeFrame = NULL;
	pTailTimeFrame = new m_oTailTimeFrameStruct;
	InitializeCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	pTailTimeFrame->m_cpRcvFrameData = NULL;
	pTailTimeFrame->m_cpSndFrameData = NULL;
	pTailTimeFrame->m_pbyCommandWord = NULL;
	pTailTimeFrame->m_oTailTimeFrameSocket = INVALID_SOCKET;
	pTailTimeFrame->m_pCommandStructSet = NULL;
	pTailTimeFrame->m_pCommandStructReturn = NULL;
	pTailTimeFrame->m_usPortMove = 0;
	return pTailTimeFrame;
}
// 初始化尾包时刻帧
void OnInitInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	ASSERT(pTailTimeFrame != NULL);
	ASSERT(pCommInfo != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_pCommandStructSet != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructSet;
		pTailTimeFrame->m_pCommandStructSet = NULL;
	}
	pTailTimeFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pTailTimeFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_pServerSetupData->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pTailTimeFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usAimPort
		+ pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 尾包时刻发送缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// 尾包时刻应答接收缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 尾包时刻查询返回端口
	pTailTimeFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_pServerSetupData->m_oXMLPortSetupData.m_usTailTimeReturnPort;
	// 尾包时刻查询接收端口偏移量
	pTailTimeFrame->m_usPortMove = pCommInfo->m_pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrFramePacket(pTailTimeFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pTailTimeFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpSndFrameData;
		pTailTimeFrame->m_cpSndFrameData = NULL;
	}
	pTailTimeFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pTailTimeFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iSndFrameSize);
	// 清空尾包时刻查询命令字集合
	if (pTailTimeFrame->m_pbyCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pbyCommandWord;
		pTailTimeFrame->m_pbyCommandWord = NULL;
	}
	pTailTimeFrame->m_pbyCommandWord = new BYTE[pConstVar->m_iCommandWordMaxNum];
	memset(pTailTimeFrame->m_pbyCommandWord, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iCommandWordMaxNum);
	// 尾包时刻查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pTailTimeFrame->m_pCommandStructReturn != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructReturn;
		pTailTimeFrame->m_pCommandStructReturn = NULL;
	}
	pTailTimeFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrFramePacket(pTailTimeFrame->m_pCommandStructReturn);
	// 清空接收帧缓冲区
	if (pTailTimeFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpRcvFrameData;
		pTailTimeFrame->m_cpRcvFrameData = NULL;
	}
	pTailTimeFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTailTimeFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 关闭尾包时刻帧信息结构体
void OnCloseInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	ASSERT(pTailTimeFrame != NULL);
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_cpSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_cpSndFrameData;
		pTailTimeFrame->m_cpSndFrameData = NULL;
	}
	if (pTailTimeFrame->m_pbyCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pbyCommandWord;
		pTailTimeFrame->m_pbyCommandWord = NULL;
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
void OnFreeInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	ASSERT(pTailTimeFrame != NULL);
	DeleteCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	delete pTailTimeFrame;
	pTailTimeFrame = NULL;
}
// 创建并设置尾包时刻端口
void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pTailTimeFrame != NULL);
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 创建套接字
	pTailTimeFrame->m_oTailTimeFrameSocket = CreateInstrSocket(pTailTimeFrame->m_pCommandStructSet->m_usReturnPort + pTailTimeFrame->m_usPortMove, 
		pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrSocketBroadCast(pTailTimeFrame->m_oTailTimeFrameSocket, pLogOutPut);
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
bool ParseInstrTailTimeReturnFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pTailTimeFrame != NULL);
	ASSERT(pConstVar != NULL);
	bool bReturn = false;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	bReturn = ParseInstrFrame(pTailTimeFrame->m_pCommandStructReturn, 
		pTailTimeFrame->m_cpRcvFrameData, pConstVar, pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	return bReturn;
}
// 按IP地址查询尾包时刻帧
void MakeInstrTailTimeQueryFramebyIP(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pTailTimeFrame != NULL);
	ASSERT(pConstVar != NULL);
	unsigned short usPos = 0;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 仪器IP地址
	pTailTimeFrame->m_pCommandStructSet->m_uiDstIP = uiInstrumentIP;
	// 查询命令
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdLAUTailRecTimeLAUX;
	usPos ++;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdLineTailRecTimeLAUX;
	usPos ++;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdTailRecSndTime1;
	usPos ++;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = usPos;
	MakeInstrFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_cpSndFrameData, 
		pTailTimeFrame->m_pbyCommandWord, pTailTimeFrame->m_usCommandWordNum);
	SendFrame(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pTailTimeFrame->m_pCommandStructSet->m_usAimPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiAimIP, pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 广播查询尾包时刻帧
void MakeInstrTailTimeQueryFramebyBroadCast(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort, m_oLogOutPutStruct* pLogOutPut)
{
	ASSERT(pLogOutPut != NULL);
	ASSERT(pTailTimeFrame != NULL);
	ASSERT(pConstVar != NULL);
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
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdBroadCastPortSeted;
	usPos ++;
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdLAUTailRecTimeLAUX;
	usPos ++;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdLineTailRecTimeLAUX;
	usPos ++;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdTailRecSndTime1;
	usPos ++;
	// 本地时间
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdLocalSysTime1;
	usPos ++;
	// 网络时间
	pTailTimeFrame->m_pbyCommandWord[usPos] = pConstVar->m_byCmdNetTime;
	usPos ++;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = usPos;
	MakeInstrFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_cpSndFrameData, 
		pTailTimeFrame->m_pbyCommandWord, pTailTimeFrame->m_usCommandWordNum);
	SendFrame(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pTailTimeFrame->m_pCommandStructSet->m_usAimPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiAimIP, pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}