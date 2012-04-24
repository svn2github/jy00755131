#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建ADC参数设置线程
m_oADCSetThreadStruct* OnCreateADCSetThread(void)
{
	m_oADCSetThreadStruct* pADCSetThread = NULL;
	pADCSetThread = new m_oADCSetThreadStruct;
	pADCSetThread->m_pThread = new m_oThreadStruct;
	pADCSetThread->m_pADCSetFrame = NULL;
	pADCSetThread->m_pInstrumentList = NULL;
	pADCSetThread->m_pRoutList = NULL;
	pADCSetThread->m_pCommInfo = NULL;
	pADCSetThread->m_pLogOutPutADCFrameTime = NULL;
	InitializeCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	return pADCSetThread;
}
// 发送ADC命令设置帧
void OnSelectADCSetCmd(m_oADCSetThreadStruct* pADCSetThread, bool bRout, 
	unsigned int uiDstIP)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	CString str = _T("");
	string strConv = "";
	unsigned int uiTemp = 0;
	unsigned int uiADCSetOperationNb = 0;
	unsigned int uiLocalSysTime = 0;
	unsigned int uiTBTimeOld = 0;
	str.Format(_T("Now操作序号为 %d"), 1234);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
	uiADCSetOperationNb = pADCSetThread->m_uiADCSetOperationNb;
	uiLocalSysTime = pADCSetThread->m_uiLocalSysTime;
	uiTBTimeOld = pADCSetThread->m_uiTBTimeOld;
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
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
	}
	// 按照IP地址发送ADC参数设置命令帧
	else
	{
		// 目标IP地址
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = uiDstIP;
		str.Format(_T("向IP地址 = 0x%x 的仪器发送ADC参数设置帧，命令序号 = %d"), 
			uiDstIP, uiADCSetOperationNb);
		ConvertCStrToStr(str, &strConv);
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
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCSetSine;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCSetSineSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 3:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 4:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源低位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 5:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源高位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 6:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关低位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 7:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关高位
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 8:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 9:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置写寄存器
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCRegisterWrite;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 10:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置读寄存器
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCRegisterRead;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterReadSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
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
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 13:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
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
		// @@@需要界面设置采样率等参数，暂选为1K采样率
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 16:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置连续采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCReadContinuous;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCReadContinuousSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 17:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendQueryCmd;
		// 查询仪器的当前系统时间
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 18:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB时间高位
		if (uiLocalSysTime < uiTBTimeOld)
		{
			uiTemp = (0xffffffff - uiTBTimeOld + uiLocalSysTime) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		else
		{
			uiTemp = (uiLocalSysTime - uiTBTimeOld) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		if (uiLocalSysTime < uiTemp)
		{
			uiLocalSysTime = 0xffffffff - uiTemp + uiLocalSysTime;
		}
		else
		{
			uiLocalSysTime -= uiTemp;
		}
		pADCSetThread->m_uiTBTimeOld = uiLocalSysTime;
		pADCSetThread->m_uiLocalSysTime = uiLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiTBHigh = 
			pADCSetThread->m_pThread->m_pConstVar->m_uiTBSleepTimeHigh + uiLocalSysTime;
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
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", strConv);
		break;
	case 19:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 20:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_cCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 21:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		EnterCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_cpADCSet = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize;
		LeaveCriticalSection(&pADCSetThread->m_pCommInfo->m_oSecCommInfo);
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
	MakeInstrumentFrame(pADCSetThread->m_pADCSetFrame->m_pCommandStructSet, 
		pADCSetThread->m_pThread->m_pConstVar, 
		pADCSetThread->m_pADCSetFrame->m_cpSndFrameData, 
		pADCSetThread->m_pADCSetFrame->m_cpCommandWord, 
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	// 发送ADC参数设置帧
	SendInstrumentADCSetFrame(pADCSetThread->m_pADCSetFrame, pADCSetThread->m_pThread->m_pConstVar);
}
// 手动发送ADC参数设置帧
bool OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout,  char* cpADCSet, 
	int iADCSetNum, m_oEnvironmentStruct* pEnv)
{
	unsigned int uiDstIP = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	if (FALSE == IfLocationExistInMap(iLineIndex, iPointIndex, &pEnv->m_pInstrumentList->m_oInstrumentLocationMap))
	{
		return false;
	}
	pInstrument = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, &pEnv->m_pInstrumentList->m_oInstrumentLocationMap);
	if (bRout == true)
	{
		pInstrumentNext = GetNextInstrument(iDirection, pInstrument, pEnv->m_pConstVar);
		if (pInstrumentNext == NULL)
		{
			return false;
		}
		uiDstIP = pInstrumentNext->m_uiBroadCastPort;
	}
	else
	{
		uiDstIP = pInstrument->m_uiIP;
	}

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

	MakeInstrumentFrame(pEnv->m_pADCSetFrame->m_pCommandStructSet, pEnv->m_pConstVar, pEnv->m_pADCSetFrame->m_cpSndFrameData, 
		pEnv->m_pADCSetFrame->m_cpCommandWord, pEnv->m_pADCSetFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pEnv->m_pADCSetFrame->m_oSecADCSetFrame);
	// 发送ADC参数设置帧
	SendInstrumentADCSetFrame(pEnv->m_pADCSetFrame, pEnv->m_pConstVar);
	return true;
}
// 发送ADC命令设置帧
void OnSendADCSetCmd(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	EnterCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
	for (iter = pADCSetThread->m_pRoutList->m_oADCSetRoutMap.begin();
		iter != pADCSetThread->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		iter->second->m_bADCSetReturn = false;
		// 如果需要按照路由设置
		if (iter->second->m_bADCSetRout == true)
		{
			// 重置ADC参数设置应答标志位
			pInstrument = iter->second->m_pHead;
			do 
			{
				pInstrument = GetNextInstrument(iter->second->m_iRoutDirection, 
					pInstrument, pADCSetThread->m_pThread->m_pConstVar);
				if (pInstrument == NULL)
				{
					break;
				}
				pInstrument->m_bADCSetReturn = false;
			} while (pInstrument != iter->second->m_pTail);
			if (iter->second->m_pTail == NULL)
			{
				break;
			}
			// 选择ADC参数设置命令
			OnSelectADCSetCmd(pADCSetThread, true, iter->second->m_pTail->m_uiBroadCastPort);
		}
	}
	LeaveCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
	EnterCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
	for (iter2 = pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.begin();
		iter2 != pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		// 重置ADC参数设置应答标志位
		iter2->second->m_bADCSetReturn = false;
		// 选择ADC参数设置命令
		OnSelectADCSetCmd(pADCSetThread, false, iter2->second->m_uiIP);
	}
	LeaveCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
}
// 处理ADC参数设置应答帧
void ProcADCSetReturnFrameOne(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	string strFrameData = "";
	string strConv = "";
	unsigned int uiADCSetOperationNb = 0;
	// 得到仪器IP
	EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	uiIPInstrument = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSrcIP;
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	uiADCSetOperationNb = pADCSetThread->m_uiADCSetOperationNb;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, &pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap);
		pInstrument->m_bADCSetReturn = true;
		if (uiADCSetOperationNb == 17)
		{
			EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
			pADCSetThread->m_uiLocalSysTime = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSysTime;
			LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
			str.Format(_T("IP地址 = 0x%x的仪器本地时间为 = 0x%x"), uiIPInstrument, pADCSetThread->m_uiLocalSysTime);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne", strConv);
		}
	}
	else
	{
		GetFrameInfo(pADCSetThread->m_pADCSetFrame->m_cpRcvFrameData,
			pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne",
			strFrameData, ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 输出接收和发送帧的统计结果
void OnOutPutADCDataRecResult(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
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
	for (iter = pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap.begin();
		iter != pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		pInstrument = iter->second;
		// 仪器类型为采集站
		if (pInstrument->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
		{
			str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，应收ADC数据帧数 %d，实际接收帧数 %d（含重发帧 %d），丢失帧数%d"),
				pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_uiADCDataShouldRecFrameNum, 
				pInstrument->m_uiADCDataActualRecFrameNum, 
				pInstrument->m_uiADCDataRetransmissionFrameNum,
				pInstrument->m_uiADCDataShouldRecFrameNum - pInstrument->m_uiADCDataActualRecFrameNum);
			ConvertCStrToStr(str, &strConv);
			AddMsgToLogOutPutList(pADCSetThread->m_pLogOutPutADCFrameTime, "", strConv);
			iADCDataActualRecFrameNum += pInstrument->m_uiADCDataActualRecFrameNum;
			iADCDataShouldRecFrameNum += pInstrument->m_uiADCDataShouldRecFrameNum;
			iADCDataRetransmissionFrameNum += pInstrument->m_uiADCDataRetransmissionFrameNum;
		}
	}
	// ADC数据接收帧结果统计
	str.Format(_T("应收ADC数据帧数%d， 实际接收帧数%d（含重发帧 %d），丢失帧数%d"), iADCDataShouldRecFrameNum, 
		iADCDataActualRecFrameNum, iADCDataRetransmissionFrameNum, 
		iADCDataShouldRecFrameNum - iADCDataActualRecFrameNum);
	ConvertCStrToStr(str, &strConv);
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnOutPutADCDataRecResult", strConv);
	pADCSetThread->m_pLogOutPutADCFrameTime->m_uiLogInfoCount = OutPutLogFileInfoNumLimit;
}
// 命令应答接收完全后的操作
void OnADCSetNextOpt(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 执行下一步ADC命令发送并开启应答监视定时器
	pADCSetThread->m_uiADCSetOperationNb++;
	pADCSetThread->m_uiCounter = 0;
	// 完成ADC参数设置
	// 完成ADC开始数据采集
	// 完成ADC停止数据采集
	if ((pADCSetThread->m_uiADCSetOperationNb == 12)
		|| (pADCSetThread->m_uiADCSetOperationNb == 19)
		|| (pADCSetThread->m_uiADCSetOperationNb == 23))
	{
		pADCSetThread->m_uiADCSetOperationNb = 0;
		// ADC参数设置线程停止工作
		pADCSetThread->m_pThread->m_bWork = false;
		// 完成ADC停止数据采集
		if (pADCSetThread->m_uiADCSetOperationNb == 23)
		{
			OnOutPutADCDataRecResult(pADCSetThread);
		}
	}
}
// 判断ADC参数设置应答是否接收完全
bool CheckADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return false;
	}
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	bool bADCSetRoutReturn = true;
	bool bReturn = true;
	unsigned int uiADCSetOperationNb = 0;
	uiADCSetOperationNb = pADCSetThread->m_uiADCSetOperationNb;
	for (iter = pADCSetThread->m_pRoutList->m_oADCSetRoutMap.begin();
		iter != pADCSetThread->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		if (false == iter->second->m_bADCSetReturn)
		{
			bADCSetRoutReturn = true;
			pInstrument = iter->second->m_pHead;
			do 
			{
				pInstrument = GetNextInstrument(iter->second->m_iRoutDirection, 
					pInstrument, pADCSetThread->m_pThread->m_pConstVar);
				if (pInstrument == NULL)
				{
					break;
				}
				if (pInstrument->m_bIPSetOK == false)
				{
					break;
				}
				if (pInstrument->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
				{
					if (false == pInstrument->m_bADCSetReturn)
					{
						// 找不到则插入索引表
						// 仪器索引表中已经有的路由不再广播发送ADC参数设置
						AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, 
							&pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
						bReturn = false;
						bADCSetRoutReturn = false;
					}
					else
					{
						if (uiADCSetOperationNb == 11)
						{
							pInstrument->m_bADCSet = true;
						}
						else if (uiADCSetOperationNb == 18)
						{
							pInstrument->m_bADCStartSample = true;
						}
						else if (uiADCSetOperationNb == 22)
						{
							pInstrument->m_bADCStopSample = true;
						}
						// 如果在索引表中找到该仪器则删除
						DeleteInstrumentFromMap(pInstrument->m_uiIP, 
							&pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
					}
				}
			} while (pInstrument != iter->second->m_pTail);
			iter->second->m_bADCSetReturn = bADCSetRoutReturn;
			iter->second->m_bADCSetRout = bADCSetRoutReturn;
		}
	}
	for (iter2 = pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.begin();
		iter2 != pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		if (false == iter2->second->m_bADCSetReturn)
		{
			bReturn = false;
			break;
		}
		else
		{
			if (uiADCSetOperationNb == 11)
			{
				iter2->second->m_bADCSet = true;
			}
			else if (uiADCSetOperationNb == 18)
			{
				iter2->second->m_bADCStartSample = true;
			}
			else if (uiADCSetOperationNb == 22)
			{
				iter2->second->m_bADCStopSample = true;
			}
		}
	}
	return bReturn;
}
// 处理ADC参数设置应答帧
void ProcADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	CString str = _T("");
	string strConv = "";
	unsigned int uiADCSetOperationNb = 0;
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pADCSetThread->m_pADCSetFrame->m_oADCSetFrameSocket,
		pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pADCSetThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pADCSetThread->m_pADCSetFrame->m_oADCSetFrameSocket,
				pADCSetThread->m_pADCSetFrame->m_cpRcvFrameData, 
				pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCSetThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentADCSetReturnFrame(pADCSetThread->m_pADCSetFrame, 
					pADCSetThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCSetReturnFrame",
						"", ErrorType, IDS_ERR_PARSE_ADCSETRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
					// 处理单个ADC参数设置应答帧
					ProcADCSetReturnFrameOne(pADCSetThread);
					LeaveCriticalSection(&pADCSetThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
	uiADCSetOperationNb = pADCSetThread->m_uiADCSetOperationNb;
	// 判断ADC参数设置应答是否接收完全
	if (true == CheckADCSetReturnFrame(pADCSetThread))
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收完全"), uiADCSetOperationNb);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", 
			strConv);
		// 命令应答接收完全后的操作
		OnADCSetNextOpt(pADCSetThread);
	}
	else
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收不完全"), uiADCSetOperationNb);
		ConvertCStrToStr(str, &strConv);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", 
			strConv, WarningType);
	}
}
// 线程等待函数
void WaitADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
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
	if (pADCSetThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		if (pADCSetThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
			break;
		}
		if (pADCSetThread->m_pThread->m_bWork == true)
		{
			if (pADCSetThread->m_uiADCSetOperationNb != 0)
			{
				// 根据ADC参数设置任务队列发送参数设置帧
				pADCSetThread->m_uiCounter++;
				if (pADCSetThread->m_uiCounter == 1)
				{
					OnSendADCSetCmd(pADCSetThread);
				}
				else if (pADCSetThread->m_uiCounter == 2)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (pADCSetThread->m_uiCounter == 3)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (pADCSetThread->m_uiCounter == 4)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else if (pADCSetThread->m_uiCounter == 5)
				{
					// 接收ADC参数设置应答帧
					ProcADCSetReturnFrame(pADCSetThread);
				}
				else
				{
					pADCSetThread->m_uiCounter = 0;
				}
			}
		}
		if (pADCSetThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
			break;
		}
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		WaitADCSetThread(pADCSetThread);
	}
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pADCSetThread->m_pThread->m_hThreadClose);
	return 1;
}
// 初始化ADC参数设置线程
bool OnInitADCSetThread(m_oADCSetThreadStruct* pADCSetThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pADCSetThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	pADCSetThread->m_uiADCSetOperationNb = 0;
	pADCSetThread->m_bADCStartSample = false;
	pADCSetThread->m_bADCStopSample = false;
	pADCSetThread->m_uiLocalSysTime = 0;
	pADCSetThread->m_uiTBTimeOld = 0;
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
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", 
			"pADCSetThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
		return false;
	}
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", 
		"ADC参数设置线程创建成功");
	LeaveCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	return true;
}
// 初始化ADC参数设置线程
bool OnInit_ADCSetThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pADCSetThread->m_pADCSetFrame = pEnv->m_pADCSetFrame;
	pEnv->m_pADCSetThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pADCSetThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pADCSetThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	pEnv->m_pADCSetThread->m_pCommInfo = pEnv->m_pInstrumentCommInfo;
	return OnInitADCSetThread(pEnv->m_pADCSetThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC参数设置线程
bool OnCloseADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pADCSetThread->m_pThread))
	{
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnCloseADCSetThread", 
			"ADC参数设置线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnCloseADCSetThread", 
		"ADC参数设置线程成功关闭");
	return true;
}
// 释放ADC参数设置线程
void OnFreeADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	if (pADCSetThread->m_pThread != NULL)
	{
		delete pADCSetThread->m_pThread;
	}
	DeleteCriticalSection(&pADCSetThread->m_oSecADCSetThread);
	delete pADCSetThread;
}