#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建ADC参数设置帧信息结构体
m_oADCSetFrameStruct* OnCreateInstrumentADCSetFrame(void)
{
	m_oADCSetFrameStruct* pADCSetFrame = NULL;
	pADCSetFrame = new m_oADCSetFrameStruct;
	InitializeCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	pADCSetFrame->m_cpRcvFrameData = NULL;
	pADCSetFrame->m_cpSndFrameData = NULL;
	pADCSetFrame->m_cpCommandWord = NULL;
	pADCSetFrame->m_oADCSetFrameSocket = INVALID_SOCKET;
	pADCSetFrame->m_pCommandStructSet = NULL;
	pADCSetFrame->m_pCommandStructReturn = NULL;
	return pADCSetFrame;
}
// 初始化ADC参数设置帧
void OnInitInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pADCSetFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentADCSetFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	if (pADCSetFrame->m_pCommandStructSet != NULL)
	{
		delete pADCSetFrame->m_pCommandStructSet;
		pADCSetFrame->m_pCommandStructSet = NULL;
	}
	pADCSetFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pADCSetFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pADCSetFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pADCSetFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_oXMLPortSetupData.m_usAimPort;
	// ADC参数设置发送缓冲区帧数设定为仪器个数
	pADCSetFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// ADC参数设置应答接收缓冲区帧数设定为仪器个数
	pADCSetFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// ADC参数设置返回端口
	pADCSetFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_oXMLPortSetupData.m_usADCSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pADCSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pADCSetFrame->m_pCommandStructSet);
	// ADC数据返回地址
	pADCSetFrame->m_pCommandStructSet->m_uiADCDataReturnAddr = pCommInfo->m_oXMLIPSetupData.m_uiADCDataReturnAddr;
	// ADC数据返回端口
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPort = pCommInfo->m_oXMLPortSetupData.m_usADCDataReturnPort;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnCmd = pConstVar->m_usSendADCCmd;
	// 端口递增下限
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPortLimitLow = pCommInfo->m_oXMLPortSetupData.m_usADCDataReturnPort;
	// 端口递增上限
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPortLimitHigh = pCommInfo->m_oXMLPortSetupData.m_usADCDataReturnPort;
	// 清空发送帧缓冲区
	if (pADCSetFrame->m_cpSndFrameData != NULL)
	{
		delete[] pADCSetFrame->m_cpSndFrameData;
		pADCSetFrame->m_cpSndFrameData = NULL;
	}
	pADCSetFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pADCSetFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空时统设置命令字集合
	if (pADCSetFrame->m_cpCommandWord != NULL)
	{
		delete[] pADCSetFrame->m_cpCommandWord;
		pADCSetFrame->m_cpCommandWord = NULL;
	}
	pADCSetFrame->m_cpCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pADCSetFrame->m_cpCommandWord, pConstVar->m_cSndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
	// ADC参数设置命令字个数
	pADCSetFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pADCSetFrame->m_pCommandStructReturn != NULL)
	{
		delete pADCSetFrame->m_pCommandStructReturn;
		pADCSetFrame->m_pCommandStructReturn = NULL;
	}
	pADCSetFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pADCSetFrame->m_pCommandStructReturn);
	pADCSetFrame->m_pCommandStructReturn->m_cpADCSet = new char[pConstVar->m_iSndFrameSize];
	// 清空接收帧缓冲区
	if (pADCSetFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pADCSetFrame->m_cpRcvFrameData;
		pADCSetFrame->m_cpRcvFrameData = NULL;
	}
	pADCSetFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pADCSetFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}
// 关闭ADC参数设置帧信息结构体
void OnCloseInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame)
{
	if (pADCSetFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	if (pADCSetFrame->m_cpSndFrameData != NULL)
	{
		delete[] pADCSetFrame->m_cpSndFrameData;
		pADCSetFrame->m_cpSndFrameData = NULL;
	}
	if (pADCSetFrame->m_cpCommandWord != NULL)
	{
		delete[] pADCSetFrame->m_cpCommandWord;
		pADCSetFrame->m_cpCommandWord = NULL;
	}
	if (pADCSetFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pADCSetFrame->m_cpRcvFrameData;
		pADCSetFrame->m_cpRcvFrameData = NULL;
	}
	if (pADCSetFrame->m_pCommandStructSet != NULL)
	{
		delete pADCSetFrame->m_pCommandStructSet;
		pADCSetFrame->m_pCommandStructSet = NULL;
	}
	if (pADCSetFrame->m_pCommandStructReturn != NULL)
	{
		if (pADCSetFrame->m_pCommandStructReturn->m_cpADCSet != NULL)
		{
			delete[] pADCSetFrame->m_pCommandStructReturn->m_cpADCSet;
			pADCSetFrame->m_pCommandStructReturn->m_cpADCSet = NULL;
		}
		delete pADCSetFrame->m_pCommandStructReturn;
		pADCSetFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}
// 释放ADC参数设置帧信息结构体
void OnFreeInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame)
{
	if (pADCSetFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	delete pADCSetFrame;
	pADCSetFrame = NULL;
}
// 创建并设置ADC参数设置端口
void OnCreateAndSetADCSetFrameSocket(m_oADCSetFrameStruct* pADCSetFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pADCSetFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	// 创建套接字
	pADCSetFrame->m_oADCSetFrameSocket = CreateInstrumentSocket(pADCSetFrame->m_pCommandStructSet->m_usReturnPort + NetedPortMove, 
		pADCSetFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pADCSetFrame->m_oADCSetFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pADCSetFrame->m_oADCSetFrameSocket, pADCSetFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pADCSetFrame->m_oADCSetFrameSocket, pADCSetFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetADCSetFrameSocket", "创建并设置ADC参数设置帧端口！");
}
// 解析ADC参数设置应答帧
bool ParseInstrumentADCSetReturnFrame(m_oADCSetFrameStruct* pADCSetFrame, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pADCSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentADCSetReturnFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	bReturn = ParseInstrumentFrame(pADCSetFrame->m_pCommandStructReturn, 
		pADCSetFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	return bReturn;
}
// 发送ADC参数设置帧
void SendInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pADCSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SendInstrumentADCSetFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	SendFrame(pADCSetFrame->m_oADCSetFrameSocket, pADCSetFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pADCSetFrame->m_pCommandStructSet->m_usAimPort, 
		pADCSetFrame->m_pCommandStructSet->m_uiAimIP, pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}