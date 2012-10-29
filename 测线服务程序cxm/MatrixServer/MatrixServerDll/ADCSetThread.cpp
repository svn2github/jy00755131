#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建ADC参数设置线程
m_oADCSetThreadStruct* OnCreateADCSetThread(void)
{
	m_oADCSetThreadStruct* pADCSetThread = NULL;
	pADCSetThread = new m_oADCSetThreadStruct;
	pADCSetThread->m_pThread = new m_oThreadStruct;
	pADCSetThread->m_pADCSetFrame = NULL;
	pADCSetThread->m_pLineList = NULL;
	pADCSetThread->m_pCommInfo = NULL;
	pADCSetThread->m_pLogOutPutADCFrameTime = NULL;
	InitializeCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	return pADCSetThread;
}
// 发送ADC命令设置帧
void OnSelectADCSetCmd(m_oADCSetThreadStruct* pADCSetThread, bool bRout, 
	unsigned int uiDstIP, unsigned int uiTBHigh)
{
	ASSERT(pADCSetThread != NULL);
	CString str = _T("");
	string strConv = "";
	unsigned int uiADCSetOperationNb = 0;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	uiADCSetOperationNb = pADCSetThread->m_iADCSetOperationNb;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	// 查询命令字个数
	pADCSetThread->m_pADCSetFrame->m_usCommandWordNum = 0;
	// 按照路由的广播端口广播发送ADC参数设置命令帧
	if (bRout == true)
	{
		// 目标IP地址
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = pADCSetThread->m_pThread->m_pConstVar->m_uiIPBroadcastAddr;
		// 广播端口
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiDstIP;
		// 查询命令字内容
		// 广播端口
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdBroadCastPortSeted;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		str.Format(_T("向广播端口 = 0x%x 的仪器广播发送ADC参数设置帧，命令序号 = %d"), 
			uiDstIP, uiADCSetOperationNb);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
	}
	// 按照IP地址发送ADC参数设置命令帧
	else
	{
		// 目标IP地址
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = uiDstIP;
		str.Format(_T("向IP地址 = 0x%x 的仪器发送ADC参数设置帧，命令序号 = %d"), 
			uiDstIP, uiADCSetOperationNb);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
	}
	// 1~11为ADC参数设置命令，12~18为ADC开始采样命令，19~22为ADC停止采样命令
	switch (uiADCSetOperationNb)
	{
	case 1:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 2:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置正弦波命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCSetSineSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 3:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 4:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源低位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 5:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源高位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 6:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关低位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 7:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关高位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 8:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 9:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置写寄存器
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 10:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置读寄存器
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterReadSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 11:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置数据返回端口
		// 自动AD返回地址
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCDataReturnAddr;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 自动数据返回端口和命令
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCDataReturnPort;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 端口递增下限和上限
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCDataReturnPortLimit;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 12:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 13:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 14:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 15:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置ADC数据采样率等参数
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		// 采用高通滤波
		if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_bHPFOpen == true)
		{
			if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 250)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 67;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = 86;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 19;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 500)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 75;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = -89;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 9;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 1000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 83;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = -45;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 4;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 2000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 91;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = 105;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 2;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 4000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 99;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = 52;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 1;
			}
			else
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 83;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[6] = -45;
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[7] = 4;
			}
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[8] = 0;
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[9] = 0;
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[10] = 0;
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[11] = 0;
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[12] = 0;
			pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[13] = 64;
		}
		else
		{
			// 不经过高通滤波
			if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 250)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 66;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 500)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 74;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 1000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 2000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 90;
			}
			else if (pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSampleRate == 4000)
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 98;
			}
			// 如果不在所选采样率则按照1000采样率采样
			else
			{
				pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample[4] = 82;
			}
		}
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 16:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置连续采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCReadContinuousSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 17:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendQueryCmd;
		// 查询仪器的当前系统时间
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 查询仪器的当前网络时间
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdNetTime;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 18:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB时间高位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiTBHigh = uiTBHigh;
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdTBHigh;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 设置TB时间低位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = pADCSetThread->m_pThread->m_pConstVar->m_usTBSleepTimeLow;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = pADCSetThread->m_pThread->m_pConstVar->m_usCmdTBCtrlStartSample
			+ pADCSetThread->m_pThread->m_pConstVar->m_usCmdCtrlCloseLed;
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		str.Format(_T("ADC开始采样：TB时间高位 = 0x%x，TB时间低位 = 0x%x"), 
			pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiTBHigh, 
			pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
		break;
	case 19:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 20:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 21:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_pServerSetupData->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 22:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	default:
		break;
	}
	MakeInstrFrame(pADCSetThread->m_pADCSetFrame->m_pCommandStructSet, 
		pADCSetThread->m_pThread->m_pConstVar, 
		pADCSetThread->m_pADCSetFrame->m_cpSndFrameData, 
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord, 
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum);
	SendFrame(pADCSetThread->m_pADCSetFrame->m_oADCSetFrameSocket, pADCSetThread->m_pADCSetFrame->m_cpSndFrameData, 
		pADCSetThread->m_pThread->m_pConstVar->m_iSndFrameSize, pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usAimPort, 
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiAimIP, pADCSetThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
}
// 手动发送ADC参数设置帧
bool OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout,  char* cpADCSet, 
	int iADCSetNum, m_oEnvironmentStruct* pEnv)
{
	unsigned int uiDstIP = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	EnterCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	if (FALSE == IfLocationExistInMap(iLineIndex, iPointIndex, &pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return false;
	}
	pInstrument = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, &pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
	if (bRout == true)
	{
		pInstrumentNext = GetNextInstrAlongRout(pInstrument, iDirection);
		if (pInstrumentNext == NULL)
		{
			LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
			return false;
		}
		uiDstIP = pInstrumentNext->m_uiBroadCastPort;
	}
	else
	{
		uiDstIP = pInstrument->m_uiIP;
	}
	LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	EnterCriticalSection(&pEnv->m_pADCSetFrame->m_oSecADCSetFrame);
	// 命令字个数
	pEnv->m_pADCSetFrame->m_usCommandWordNum = 0;
	// 按照路由的广播端口广播发送ADC参数设置命令帧
	if (bRout == true)
	{
		// 目标IP地址
		pEnv->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = pEnv->m_pConstVar->m_uiIPBroadcastAddr;
		// 广播端口
		pEnv->m_pADCSetFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiDstIP;
		// 查询命令字内容
		// 广播端口
		pEnv->m_pADCSetFrame->m_cpCommandWord[pEnv->m_pADCSetFrame->m_usCommandWordNum] = pEnv->m_pConstVar->m_cCmdBroadCastPortSeted;
		pEnv->m_pADCSetFrame->m_usCommandWordNum++;
	}
	// 按照IP地址发送ADC参数设置命令帧
	else
	{
		// 目标IP地址
		pEnv->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = uiDstIP;
	}

	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pEnv->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pEnv->m_pConstVar->m_usSendSetCmd;
	// ADC设置命令
	pEnv->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = cpADCSet;
	pEnv->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = iADCSetNum;
	pEnv->m_pADCSetFrame->m_cpCommandWord[pEnv->m_pADCSetFrame->m_usCommandWordNum] = pEnv->m_pConstVar->m_cCmdADCSet;
	pEnv->m_pADCSetFrame->m_usCommandWordNum++;

	MakeInstrFrame(pEnv->m_pADCSetFrame->m_pCommandStructSet, pEnv->m_pConstVar, pEnv->m_pADCSetFrame->m_cpSndFrameData, 
		pEnv->m_pADCSetFrame->m_cpCommandWord, pEnv->m_pADCSetFrame->m_usCommandWordNum);
	SendFrame(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket, pEnv->m_pADCSetFrame->m_cpSndFrameData, 
		pEnv->m_pConstVar->m_iSndFrameSize, pEnv->m_pADCSetFrame->m_pCommandStructSet->m_usAimPort, 
		pEnv->m_pADCSetFrame->m_pCommandStructSet->m_uiAimIP, pEnv->m_pLogOutPutOpt);
	LeaveCriticalSection(&pEnv->m_pADCSetFrame->m_oSecADCSetFrame);
	return true;
}
// 发送ADC命令设置帧
void OnSendADCSetCmd(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	unsigned int uiTBHigh = 0;
	unsigned int uiTBHighOld = 0;
	unsigned int uiSysTime = 0;
	unsigned int uiTemp = 0;
	bool bTBSet = false;
	unsigned int uiTBSleepTimeHigh = 0;
	uiTBSleepTimeHigh = pADCSetThread->m_pThread->m_pConstVar->m_uiTBSleepTimeHigh;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	if (pADCSetThread->m_iADCSetOperationNb == 18)
	{
		bTBSet = true;
	}
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	if (bTBSet == true)
	{
		uiTBHighOld = pADCSetThread->m_pLineList->m_uiTBHigh;
		uiSysTime = pADCSetThread->m_pLineList->m_uiLocalSysTime;
		if (uiTBHighOld > (uiSysTime + uiTBSleepTimeHigh))
		{
			uiTemp = (0xffffffff - uiTBHighOld + uiSysTime + uiTBSleepTimeHigh) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		else
		{
			uiTemp = (uiTBSleepTimeHigh + uiSysTime - uiTBHighOld) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		uiTBHigh = uiSysTime + uiTBSleepTimeHigh - uiTemp;
		pADCSetThread->m_pLineList->m_uiTBHigh = uiTBHigh;
	}
	for (iter = pADCSetThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap.begin();
		iter != pADCSetThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		iter->second->m_bADCSetReturn = false;
		// 如果需要按照路由设置
		if (iter->second->m_bADCSetRout == true)
		{
			// 重置ADC参数设置应答标志位
			pInstrument = iter->second->m_pHead;
			do 
			{
				pInstrument = GetNextInstrAlongRout(pInstrument, iter->second->m_iRoutDirection);
				if (pInstrument == NULL)
				{
					break;
				}
				pInstrument->m_bADCSetReturn = false;
				if (bTBSet == true)
				{
					pInstrument->m_uiTBHigh = uiTBHigh;
				}
			} while (pInstrument != iter->second->m_pTail);
			if (iter->second->m_pTail == NULL)
			{
				break;
			}
			// 选择ADC参数设置命令
			OnSelectADCSetCmd(pADCSetThread, true, iter->second->m_pTail->m_uiBroadCastPort, uiTBHigh);
		}
	}
	for (iter2 = pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap.begin();
		iter2 != pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		// 重置ADC参数设置应答标志位
		iter2->second->m_bADCSetReturn = false;
		if (bTBSet == true)
		{
			iter2->second->m_uiTBHigh = uiTBHigh;
		}
		// 选择ADC参数设置命令
		OnSelectADCSetCmd(pADCSetThread, false, iter2->second->m_uiIP, uiTBHigh);
	}
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
}
// 处理ADC参数设置应答帧
void ProcADCSetReturnFrameOne(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	unsigned int uiIPInstrument = 0;
	unsigned int uiSysTime = 0;
	unsigned int uiNetTime = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiADCSetOperationNb = 0;
	// 得到仪器IP
	EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	uiIPInstrument = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSrcIP;
	uiSysTime = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSysTime;
	uiNetTime = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiNetTime;
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	uiADCSetOperationNb = pADCSetThread->m_iADCSetOperationNb;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);

	// 仪器在索引表中
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	if (FALSE == IfIndexExistInMap(uiIPInstrument, &pADCSetThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap))
	{
		LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
		EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
		GetFrameInfo(pADCSetThread->m_pADCSetFrame->m_cpRcvFrameData,
			pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
		return;
	}
	pInstrument = GetInstrumentFromMap(uiIPInstrument, &pADCSetThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap);
	pInstrument->m_bADCSetReturn = true;
	// 更新仪器的存活时间
	UpdateInstrActiveTime(pInstrument);
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	if (uiADCSetOperationNb == 17)
	{
		UpdateLocalSysTime(uiSysTime, pADCSetThread->m_pLineList);
		str.Format(_T("IP地址 = 0x%x的仪器本地时间为 = 0x%x网络时间为 = 0x%x"), uiIPInstrument, uiSysTime, uiNetTime);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne", strConv);
	}
}
// 输出接收和发送帧的统计结果
void OnOutPutADCDataRecResult(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	CString str = _T("");
	string strConv = "";
	m_oInstrumentStruct* pInstrument = NULL;

	// 应当接收到的ADC数据帧数
	int iADCDataShouldRecFrameNum = 0;
	// 实际接收到的ADC数据帧数
	int iADCDataActualRecFrameNum = 0;
	// 重发查询帧得到的应答帧数
	int iADCDataRetransmissionFrameNum = 0;
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	for (iter = pADCSetThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap.begin();
		iter != pADCSetThread->m_pLineList->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		pInstrument = iter->second;
		// 仪器类型为采集站
		if (pInstrument->m_iInstrumentType == InstrumentTypeFDU)
		{
			str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，应收ADC数据帧数 %d，实际接收帧数 %d（含重发帧 %d），丢失帧数%d"),
				pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_uiADCDataShouldRecFrameNum, 
				pInstrument->m_uiADCDataActualRecFrameNum, 
				pInstrument->m_uiADCDataRetransmissionFrameNum,
				pInstrument->m_uiADCDataShouldRecFrameNum - pInstrument->m_uiADCDataActualRecFrameNum);
			strConv = (CStringA)str;
			AddMsgToLogOutPutList(pADCSetThread->m_pLogOutPutADCFrameTime, "", strConv);
			iADCDataActualRecFrameNum += pInstrument->m_uiADCDataActualRecFrameNum;
			iADCDataShouldRecFrameNum += pInstrument->m_uiADCDataShouldRecFrameNum;
			iADCDataRetransmissionFrameNum += pInstrument->m_uiADCDataRetransmissionFrameNum;
		}
	}
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	// ADC数据接收帧结果统计
	str.Format(_T("应收ADC数据帧数%d， 实际接收帧数%d（含重发帧 %d），丢失帧数%d"), iADCDataShouldRecFrameNum, 
		iADCDataActualRecFrameNum, iADCDataRetransmissionFrameNum, 
		iADCDataShouldRecFrameNum - iADCDataActualRecFrameNum);
	strConv = (CStringA)str;
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnOutPutADCDataRecResult", strConv);
	EnterCriticalSection(&pADCSetThread->m_pLogOutPutADCFrameTime->m_oSecLogFile);
	pADCSetThread->m_pLogOutPutADCFrameTime->m_uiLogInfoCount = OutPutLogFileInfoNumLimit;
	LeaveCriticalSection(&pADCSetThread->m_pLogOutPutADCFrameTime->m_oSecLogFile);
}
// 命令应答接收完全后的操作
void OnADCSetNextOpt(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	int iADCSetCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCSetCmdNum;
	int iADCStartSampleCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleCmdNum;
	int iADCStopSampleCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleCmdNum;
	int iADCSetCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCSetCmdBeginNb;
	int iADCStartSampleCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleBeginNb;
	int iADCStopSampleCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleBeginNb;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	// 执行下一步ADC命令发送并开启应答监视定时器
	pADCSetThread->m_iADCSetOperationNb++;
	pADCSetThread->m_uiCounter = 0;
	// 完成ADC参数设置
	// 完成ADC开始数据采集
	// 完成ADC停止数据采集
	if ((pADCSetThread->m_iADCSetOperationNb == (iADCSetCmdBeginNb + iADCSetCmdNum))
		|| (pADCSetThread->m_iADCSetOperationNb == (iADCStartSampleCmdBeginNb + iADCStartSampleCmdNum))
		|| (pADCSetThread->m_iADCSetOperationNb == (iADCStopSampleCmdBeginNb + iADCStopSampleCmdNum)))
	{
		// 完成ADC停止数据采集
		if (pADCSetThread->m_iADCSetOperationNb == (iADCStopSampleCmdBeginNb + iADCStopSampleCmdNum))
		{
			OnOutPutADCDataRecResult(pADCSetThread);
		}
		pADCSetThread->m_iADCSetOperationNb = 0;
		// ADC参数设置线程停止工作
		pADCSetThread->m_pThread->m_bWork = false;
	}
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
}
// 判断ADC参数设置应答是否接收完全
bool CheckADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	bool bADCSetRoutReturn = true;
	bool bReturn = true;
	int iADCSetOperationNb = 0;
	int iADCSetCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCSetCmdNum;
	int iADCStartSampleCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleCmdNum;
	int iADCStopSampleCmdNum = pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleCmdNum;
	int iADCSetCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCSetCmdBeginNb;
	int iADCStartSampleCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleBeginNb;
	int iADCStopSampleCmdBeginNb = pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleBeginNb;
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	iADCSetOperationNb = pADCSetThread->m_iADCSetOperationNb;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	EnterCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	for (iter = pADCSetThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap.begin();
		iter != pADCSetThread->m_pLineList->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		if (false == iter->second->m_bADCSetReturn)
		{
			bADCSetRoutReturn = true;
			pInstrument = iter->second->m_pHead;
			do 
			{
				pInstrument = GetNextInstrAlongRout(pInstrument, iter->second->m_iRoutDirection);
				if (pInstrument == NULL)
				{
					break;
				}
				if (pInstrument->m_bIPSetOK == false)
				{
					break;
				}
				if (pInstrument->m_iInstrumentType == InstrumentTypeFDU)
				{
					if (false == pInstrument->m_bADCSetReturn)
					{
						// 找不到则插入索引表
						// 仪器索引表中已经有的路由不再广播发送ADC参数设置
						AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, 
							&pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
						bReturn = false;
						bADCSetRoutReturn = false;
					}
					else
					{
						if (iADCSetOperationNb == (iADCSetCmdBeginNb + iADCSetCmdNum - 1))
						{
							pInstrument->m_bADCSet = true;
						}
						else if (iADCSetOperationNb == (iADCStartSampleCmdBeginNb + iADCStartSampleCmdNum - 1))
						{
							pInstrument->m_bADCStartSample = true;
						}
						else if (iADCSetOperationNb == (iADCStopSampleCmdBeginNb + iADCStopSampleCmdNum - 1))
						{
							pInstrument->m_bADCStopSample = true;
						}
						// 如果在索引表中找到该仪器则删除
						DeleteInstrumentFromMap(pInstrument->m_uiIP, 
							&pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap);
					}
				}
			} while (pInstrument != iter->second->m_pTail);
			iter->second->m_bADCSetReturn = bADCSetRoutReturn;
			iter->second->m_bADCSetRout = bADCSetRoutReturn;
		}
	}
	for (iter2 = pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap.begin();
		iter2 != pADCSetThread->m_pLineList->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		if (false == iter2->second->m_bADCSetReturn)
		{
			bReturn = false;
			break;
		}
		else
		{
			if (iADCSetOperationNb == (iADCSetCmdBeginNb + iADCSetCmdNum - 1))
			{
				iter2->second->m_bADCSet = true;
			}
			else if (iADCSetOperationNb == (iADCStartSampleCmdBeginNb + iADCStartSampleCmdNum - 1))
			{
				iter2->second->m_bADCStartSample = true;
			}
			else if (iADCSetOperationNb == (iADCStopSampleCmdBeginNb + iADCStopSampleCmdNum - 1))
			{
				iter2->second->m_bADCStopSample = true;
			}
		}
	}
	LeaveCriticalSection(&pADCSetThread->m_pLineList->m_oSecLineList);
	return bReturn;
}
// 处理ADC参数设置应答帧
void ProcADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	CString str = _T("");
	string strConv = "";
	unsigned int uiADCSetOperationNb = 0;
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	iFrameCount = GetFrameCount(pADCSetThread->m_pADCSetFrame->m_oADCSetFrameSocket,
		pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pADCSetThread->m_pThread->m_pLogOutPut);
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
			// 得到帧数据
			if (false == GetFrameData(pADCSetThread->m_pADCSetFrame->m_oADCSetFrameSocket,
				pADCSetThread->m_pADCSetFrame->m_cpRcvFrameData, 
				pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCSetThread->m_pThread->m_pLogOutPut))
			{
				LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
				continue;
			}
			LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
			if (false == ParseInstrADCSetReturnFrame(pADCSetThread->m_pADCSetFrame, 
				pADCSetThread->m_pThread->m_pConstVar, pADCSetThread->m_pThread->m_pLogOutPut))
			{
				AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCSetReturnFrame",
					"", ErrorType, IDS_ERR_PARSE_ADCSETRETURNFRAME);
				continue;
			}
			// 处理单个ADC参数设置应答帧
			ProcADCSetReturnFrameOne(pADCSetThread);	
		}		
	}
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	uiADCSetOperationNb = pADCSetThread->m_iADCSetOperationNb;
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	// 判断ADC参数设置应答是否接收完全
	if (true == CheckADCSetReturnFrame(pADCSetThread))
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收完全"), uiADCSetOperationNb);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", 
			strConv);
		// 命令应答接收完全后的操作
		OnADCSetNextOpt(pADCSetThread);
	}
	else
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收不完全"), uiADCSetOperationNb);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", 
			strConv, WarningType);
	}
}
// 线程等待函数
void WaitADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCSetThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		bClose = pADCSetThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pADCSetThread->m_pThread->m_pConstVar->m_iADCSetSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	bool bClose = false;
	bool bWork = false;
	unsigned int uiADCSetOperationNb = 0;
	unsigned int uiCounter = 0;
	while(true)
	{
		EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		bClose = pADCSetThread->m_pThread->m_bClose;
		bWork = pADCSetThread->m_pThread->m_bWork;
		uiADCSetOperationNb = pADCSetThread->m_iADCSetOperationNb;
		uiCounter = pADCSetThread->m_uiCounter;
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			if (uiADCSetOperationNb != 0)
			{
				// 根据ADC参数设置任务队列发送参数设置帧
				uiCounter++;
				EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
				pADCSetThread->m_uiCounter = uiCounter;
				LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
				if (uiCounter == 1)
				{
					OnSendADCSetCmd(pADCSetThread);
				}
				else if (uiCounter == 2)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (uiCounter == 3)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (uiCounter == 4)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (uiCounter == 5)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else
				{
					uiCounter = 0;
					EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
					pADCSetThread->m_uiCounter = uiCounter;
					LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
				}
			}
		}
		EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		bClose = pADCSetThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		if (bClose == true)
		{
			break;
		}
		WaitADCSetThread(pADCSetThread);
	}
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pADCSetThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	return 1;
}
// 初始化ADC参数设置线程
bool OnInitADCSetThread(m_oADCSetThreadStruct* pADCSetThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	ASSERT(pADCSetThread != NULL);
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	pADCSetThread->m_iADCSetOperationNb = 0;
	pADCSetThread->m_bADCStartSample = false;
	pADCSetThread->m_bADCStopSample = false;
	if (false == OnInitThread(pADCSetThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pADCSetThread->m_pThread->m_hThreadClose);
	// 创建线程
	pADCSetThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCSetThread,
		pADCSetThread, 
		0, 
		&pADCSetThread->m_pThread->m_dwThreadID);
	if (pADCSetThread->m_pThread->m_hThread == NULL)
	{
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", 
			"pADCSetThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", 
		"ADC参数设置线程创建成功");
	return true;
}
// 初始化ADC参数设置线程
bool OnInit_ADCSetThread(m_oEnvironmentStruct* pEnv)
{
	ASSERT(pEnv != NULL);
	pEnv->m_pADCSetThread->m_pADCSetFrame = pEnv->m_pADCSetFrame;
	pEnv->m_pADCSetThread->m_pLineList = pEnv->m_pLineList;
	pEnv->m_pADCSetThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	pEnv->m_pADCSetThread->m_pCommInfo = pEnv->m_pInstrumentCommInfo;
	return OnInitADCSetThread(pEnv->m_pADCSetThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC参数设置线程
bool OnCloseADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	if (false == OnCloseThread(pADCSetThread->m_pThread))
	{
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnCloseADCSetThread", 
			"ADC参数设置线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnCloseADCSetThread", 
		"ADC参数设置线程成功关闭");
	return true;
}
// 释放ADC参数设置线程
void OnFreeADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	ASSERT(pADCSetThread != NULL);
	if (pADCSetThread->m_pThread != NULL)
	{
		delete pADCSetThread->m_pThread;
		pADCSetThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	delete pADCSetThread;
	pADCSetThread = NULL;
}