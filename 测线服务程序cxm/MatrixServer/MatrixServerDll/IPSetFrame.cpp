#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建IP地址设置帧信息结构体
m_oIPSetFrameStruct* OnCreateInstrumentIPSetFrame(void)
{
	m_oIPSetFrameStruct* pIPSetFrame = NULL;
	pIPSetFrame = new m_oIPSetFrameStruct;
	InitializeCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	pIPSetFrame->m_cpSndFrameData = NULL;
	pIPSetFrame->m_cpCommandWord = NULL;
	pIPSetFrame->m_cpRcvFrameData = NULL;
	pIPSetFrame->m_oIPSetFrameSocket = INVALID_SOCKET;
	pIPSetFrame->m_pCommandStructSet = NULL;
	pIPSetFrame->m_pCommandStructReturn = NULL;
	return pIPSetFrame;
}
// 初始化IP地址设置
void OnInitInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if ((pIPSetFrame == NULL) || (pCommInfo == NULL))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentIPSetFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	if (pIPSetFrame->m_pCommandStructSet != NULL)
	{
		delete pIPSetFrame->m_pCommandStructSet;
	}
	pIPSetFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pIPSetFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_oXMLIPSetupData.m_uiSrcIP;
	// 目的地址
	pIPSetFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_oXMLIPSetupData.m_uiAimIP;
	// 目标IP地址端口号
	pIPSetFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_oXMLPortSetupData.m_usAimPort;
	// IP地址设置发送缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiSndBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iSndFrameSize;
	// IP地址设置应答接收缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiRcvBufferSize = pConstVar->m_iInstrumentNum * pConstVar->m_iRcvFrameSize;
	// IP地址设置返回端口
	pIPSetFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_oXMLPortSetupData.m_usIPSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pIPSetFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pIPSetFrame->m_cpSndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_cpSndFrameData;
	}
	pIPSetFrame->m_cpSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pIPSetFrame->m_cpSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空IP地址设置命令字集合
	if (pIPSetFrame->m_cpCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_cpCommandWord;
	}
	pIPSetFrame->m_cpCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pIPSetFrame->m_cpCommandWord, pConstVar->m_cSndFrameBufInit, 
		pConstVar->m_iCommandWordMaxNum);
	// IP地址设置命令字个数
	pIPSetFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pIPSetFrame->m_pCommandStructReturn != NULL)
	{
		delete pIPSetFrame->m_pCommandStructReturn;
	}
	pIPSetFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pIPSetFrame->m_pCommandStructReturn);
	// 清空接收帧缓冲区
	if (pIPSetFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_cpRcvFrameData;
	}
	pIPSetFrame->m_cpRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pIPSetFrame->m_cpRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 关闭IP地址设置帧信息结构体
void OnCloseInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	if (pIPSetFrame->m_cpSndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_cpSndFrameData;
		pIPSetFrame->m_cpSndFrameData = NULL;
	}
	if (pIPSetFrame->m_cpCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_cpCommandWord;
		pIPSetFrame->m_cpCommandWord = NULL;
	}
	if (pIPSetFrame->m_cpRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_cpRcvFrameData;
		pIPSetFrame->m_cpRcvFrameData = NULL;
	}
	if (pIPSetFrame->m_pCommandStructSet != NULL)
	{
		delete pIPSetFrame->m_pCommandStructSet;
		pIPSetFrame->m_pCommandStructSet = NULL;
	}
	if (pIPSetFrame->m_pCommandStructReturn != NULL)
	{
		delete pIPSetFrame->m_pCommandStructReturn;
		pIPSetFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 释放IP地址设置帧信息结构体
void OnFreeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	delete pIPSetFrame;
}
// 创建并设置IP地址设置端口
void OnCreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame, m_oLogOutPutStruct* pLogOutPut)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 创建套接字
	pIPSetFrame->m_oIPSetFrameSocket = CreateInstrumentSocket(pIPSetFrame->m_pCommandStructSet->m_usReturnPort, 
		pIPSetFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pIPSetFrame->m_oIPSetFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetIPSetFrameSocket", "创建并设置IP地址设置端口！");
}
// 解析IP地址设置应答帧
bool ParseInstrumentIPSetReturnFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pIPSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentIPSetReturnFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	bool bReturn = false;
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	bReturn = ParseInstrumentFrame(pIPSetFrame->m_pCommandStructReturn, 
		pIPSetFrame->m_cpRcvFrameData, pConstVar);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	return bReturn;
}
// 生成IP地址查询帧
void MakeInstrumentIPQueryFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pIPSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPQueryFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 仪器IP地址
	pIPSetFrame->m_pCommandStructSet->m_uiDstIP = uiInstrumentIP;
	// IP地址查询命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	pIPSetFrame->m_cpCommandWord[0] = pConstVar->m_cCmdLocalIPAddr;
	// 查询命令字个数
	pIPSetFrame->m_usCommandWordNum = 1;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_cpSndFrameData, 
		pIPSetFrame->m_cpCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送IP地址查询帧"), 
		pIPSetFrame->m_pCommandStructSet->m_uiDstIP);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPQueryFrame", strConv);
}
// 打开交叉站某一路由方向的电源
bool OpenLAUXRoutPower(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet, 
	m_oEnvironmentStruct* pEnv)
{
	m_oInstrumentStruct* pInstrument = NULL;
	unsigned int uiIP = 0;
	EnterCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
	if (FALSE == IfLocationExistInMap(iLineIndex, iPointIndex, &pEnv->m_pInstrumentList->m_oInstrumentLocationMap))
	{
		LeaveCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
		return false;
	}
	pInstrument = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, &pEnv->m_pInstrumentList->m_oInstrumentLocationMap);
	if (pInstrument->m_bIPSetOK == false)
	{
		LeaveCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
		return false;
	}
	pInstrument->m_ucLAUXRoutOpenSet = ucLAUXRoutOpenSet;
	uiIP = pInstrument->m_uiIP;
	LeaveCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	// 仪器IP地址
	pEnv->m_pIPSetFrame->m_pCommandStructSet->m_uiDstIP = uiIP;
	// 设置命令
	pEnv->m_pIPSetFrame->m_pCommandStructSet->m_usCommand = pEnv->m_pConstVar->m_usSendSetCmd;
	// 路由开关打开
	pEnv->m_pIPSetFrame->m_pCommandStructSet->m_cLAUXRoutOpenSet = ucLAUXRoutOpenSet;
	// 命令字内容
	pEnv->m_pIPSetFrame->m_cpCommandWord[0] = pEnv->m_pConstVar->m_cCmdLAUXRoutOpenSet;
	// 命令字个数
	pEnv->m_pIPSetFrame->m_usCommandWordNum = 1;
	MakeInstrumentFrame(pEnv->m_pIPSetFrame->m_pCommandStructSet, pEnv->m_pConstVar, pEnv->m_pIPSetFrame->m_cpSndFrameData, 
		pEnv->m_pIPSetFrame->m_cpCommandWord, pEnv->m_pIPSetFrame->m_usCommandWordNum);
	SendInstrumentIPSetFrame(pEnv->m_pIPSetFrame, pEnv->m_pConstVar);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	return true;
}
// 生成IP地址设置帧
void MakeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pIPSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPSetFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	string strConv = "";
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 仪器SN号
	pIPSetFrame->m_pCommandStructSet->m_uiSN = pInstrument->m_uiSN;
	// 仪器本地IP
	pIPSetFrame->m_pCommandStructSet->m_uiInstrumentIP = pInstrument->m_uiIP;
	// 设置广播端口
	pIPSetFrame->m_pCommandStructSet->m_uiBroadCastPortSet = pInstrument->m_uiBroadCastPort;
	// 路由IP地址，路由方向 1-上
	pIPSetFrame->m_pCommandStructSet->m_uiRoutIPTop = pInstrument->m_uiRoutIPTop;
	// 路由IP地址，路由方向 2-下
	pIPSetFrame->m_pCommandStructSet->m_uiRoutIPDown = pInstrument->m_uiRoutIPDown;
	// 路由IP地址，路由方向 3-左
	pIPSetFrame->m_pCommandStructSet->m_uiRoutIPLeft = pInstrument->m_uiRoutIPLeft;
	// 路由IP地址，路由方向 4-右
	pIPSetFrame->m_pCommandStructSet->m_uiRoutIPRight = pInstrument->m_uiRoutIPRight;
	// 路由开关打开
	pIPSetFrame->m_pCommandStructSet->m_cLAUXRoutOpenSet = pInstrument->m_ucLAUXRoutOpenSet;
	// 广播IP地址
	pIPSetFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_uiIPBroadcastAddr;
	// 时间修正高位
	pIPSetFrame->m_pCommandStructSet->m_uiLocalTimeFixedHigh = pInstrument->m_uiTimeHigh;
	// 时间修正低位
	pIPSetFrame->m_pCommandStructSet->m_uiLocalTimeFixedLow = pInstrument->m_uiTimeLow;

	// 仪器SN命令字
	pIPSetFrame->m_cpCommandWord[0] = pConstVar->m_cCmdSn;
	// 仪器IP命令字
	pIPSetFrame->m_cpCommandWord[1] = pConstVar->m_cCmdLocalIPAddr;
	// 时间修正高位命令字
	pIPSetFrame->m_cpCommandWord[2] = pConstVar->m_cCmdLocalTimeFixedHigh;
	// 时间修正低位命令字
	pIPSetFrame->m_cpCommandWord[3] = pConstVar->m_cCmdLocalTimeFixedLow;
	// 仪器广播端口命令字
	pIPSetFrame->m_cpCommandWord[4] = pConstVar->m_cCmdBroadCastPortSet;

	// 生成IP地址设置帧
	if((pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
		|| (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUL))
	{
		// 命令字个数
		pIPSetFrame->m_usCommandWordNum = 5;
	}
	// LCI和交叉站需要设置路由IP地址
	else
	{
		// 仪器路由IP命令字
		pIPSetFrame->m_cpCommandWord[5] = pConstVar->m_cCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_cpCommandWord[6] = pConstVar->m_cCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_cpCommandWord[7] = pConstVar->m_cCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_cpCommandWord[8] = pConstVar->m_cCmdLAUXSetRout;
		// 打开仪器路由命令字
		pIPSetFrame->m_cpCommandWord[9] = pConstVar->m_cCmdLAUXRoutOpenSet;
		// 命令字个数
		pIPSetFrame->m_usCommandWordNum = 10;
	}
	// IP地址设置命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_cpSndFrameData, 
		pIPSetFrame->m_cpCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器SN = 0x%x，IP地址 = 0x%x 的仪器发送IP地址设置帧"), 
		pIPSetFrame->m_pCommandStructSet->m_uiSN, pIPSetFrame->m_pCommandStructSet->m_uiInstrumentIP);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPSetFrame", strConv);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 发送IP地址设置帧
void SendInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pIPSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SendInstrumentIPSetFrame", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	SendFrame(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_cpSndFrameData, 
		pConstVar->m_iSndFrameSize, pIPSetFrame->m_pCommandStructSet->m_usAimPort,
		pIPSetFrame->m_pCommandStructSet->m_uiAimIP, pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}