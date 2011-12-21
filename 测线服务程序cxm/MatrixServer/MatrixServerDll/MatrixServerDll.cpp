// MatrixServerDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "MatrixServerDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
// // 宽字节字符串转换为多字节字符串
// string WideCharToMultiChar(wstring str)
// {
// 	string return_value;
// 	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
// 	LPCWSTR lpWideCharStr = str.c_str();
// 	int cchWideChar = (int)str.size();
// 	int len = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
// 	char *buffer = new char[len+1];
// 	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, cchWideChar, buffer, len, NULL, NULL);
// 	buffer[len] = '\0';
// 	//删除缓冲区并返回值
// 	return_value.append(buffer);
// 	delete []buffer;
// 	return return_value;
// }
// const char* ConvertCStringToConstCharPointer(CString str)
// {
// 	const char* pChar = NULL;
// 	char pach[2000];
// 	CStringW strw;
// 	wstring wstr;
// 	strw = str;
// 	wstr = strw;
// 	string mstring = WideCharToMultiChar(wstr);
// 	strcpy_s( pach, sizeof(pach), mstring.c_str());
// 	pChar = pach;
// 	return pChar;
// }
// 创建实例，并返回实例指针
MatrixServerDll_API m_oEnvironmentStruct* CreateInstance(void)
{
	m_oEnvironmentStruct* pEnv = new m_oEnvironmentStruct;
	pEnv->m_pConstVar = NULL;
	pEnv->m_pInstrumentCommInfo = NULL;
	pEnv->m_pHeartBeatFrame = NULL;
	pEnv->m_pHeadFrame = NULL;
	pEnv->m_pIPSetFrame = NULL;
	pEnv->m_pThreadProcFlag = NULL;
	return pEnv;
}
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrumentFrameHead(byte* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pFrameData == NULL)
	{
		return false;
	}
	for (int i=0; i<pConstVar->m_oFrameHeadSize; i++)
	{
		if (pFrameData[i] != pConstVar->m_pFrameHeadCheck[i])
		{
			return false;
		}
	}
	return true;
}
// 生成帧的同步码
MatrixServerDll_API void MakeInstrumentFrameHead(byte* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pFrameData == NULL)
	{
		return;
	}
	for (int i=0; i<pConstVar->m_oFrameHeadSize; i++)
	{
		pFrameData[i] = pConstVar->m_pFrameHeadCheck[i];
	}
}
// 重置帧内容解析变量
MatrixServerDll_API void ResetInstrumentFramePacket(m_oInstrumentCommandStruct* pCommand, m_oConstVarStruct* pConstVar)
{
	if (pCommand == NULL)
	{
		return;
	}
	// SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站
	pCommand->m_uiSN = 0;
	// 首包时刻
	pCommand->m_uiTimeHeadFrame = 0;
	// 仪器IP地址
	pCommand->m_uiInstrumentIP = 0;
	// 本地时间
	pCommand->m_uiSysTime = 0;
	// 本地时间修正高位
	pCommand->m_uiLocalTimeFixedHigh = 0;
	// 本地时间修正低位
	pCommand->m_usLocalTimeFixedLow = 0;
	// 自动数据返回地址
	pCommand->m_uiADCDataReturnAddr = 0;
	// 自动数据返回端口
	pCommand->m_usADCDataReturnPort = 0;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	pCommand->m_usADCDataReturnCmd = 0;
	// 当返回命令高位为1时，返回端口自动加1，到高限时返回低限端口
	// 端口递增下限
	pCommand->m_usADCDataReturnPortLimitLow = 0;
	// 端口递增上限
	pCommand->m_usADCDataReturnPortLimitHigh = 0;
	// 设置网络等待端口，指设置接收上位机广播命令的端口
	pCommand->m_usSetBroadCastPort = 0;
	// 网络数据错误计数
	pCommand->m_byFDUErrorCodeDataCount = 0;
	// 命令错误计数
	pCommand->m_byFDUErrorCodeCmdCount = 0;
	// 时间报文状态
	pCommand->m_byTimeStatus = 0;
	// 控制状态
	pCommand->m_byCtrlStatus = 0;
	// TB时刻高位
	pCommand->m_uiTBHigh = 0;
	// TB时刻低位
	pCommand->m_usTBLow = 0;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	pCommand->m_usTBCtrl = 0;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	pCommand->m_byLAUXRoutOpenQuery = 0;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	pCommand->m_byLAUXRoutOpenSet = 0;
	// 尾包接收\发送时刻低位//交叉站尾包发送时刻，低14位有效
	pCommand->m_usTailRecSndTimeLow = 0;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	pCommand->m_usBroadCastPortSet = 0;
	// 网络时刻
	pCommand->m_uiNetTime = 0;
	// 交叉站大线A尾包接收时刻
	pCommand->m_usLAUXTailRecTimeLineA = 0;
	// 交叉站大线B尾包接收时刻
	pCommand->m_usLAUXTailRecTimeLineB = 0;
	// 交叉站交叉线A尾包接收时刻
	pCommand->m_usLAUXTailRecTimeLAUXLineA = 0;
	// 交叉站交叉线B尾包接收时刻
	pCommand->m_usLAUXTailRecTimeLAUXLineB = 0;
	// 交叉站大线A数据故障
	pCommand->m_byLAUXErrorCodeDataLineACount = 0;
	// 交叉站大线B数据故障
	pCommand->m_byLAUXErrorCodeDataLineBCount = 0;
	// 交叉站交叉线A数据故障
	pCommand->m_byLAUXErrorCodeDataLAUXLineACount = 0;
	// 交叉站交叉线B数据故障
	pCommand->m_byLAUXErrorCodeDataLAUXLineBCount = 0;
	// 交叉站命令口故障
	pCommand->m_byLAUXErrorCodeCmdCount = 0;
	// 交叉站上方路由IP
	pCommand->m_uiRoutIPTop = 0;
	// 交叉站下方路由IP
	pCommand->m_uiRoutIPDown = 0;
	// 交叉站左方路由IP
	pCommand->m_uiRoutIPLeft = 0;
	// 交叉站右方路由IP
	pCommand->m_uiRoutIPRight = 0;
	// 路由IP地址
	pCommand->m_uiRoutIP = 0;
	// 0x18命令数组
// 	if (pCommand->m_pbyADCSet != NULL)
// 	{
// 		memset(pCommand->m_pbyADCSet, pConstVar->m_oSndFrameBufInit, pConstVar->m_oADCSetCommandMaxByte);
// 	}
	// 0x18命令数据个数
	pCommand->m_usADCSetNum = 0;
}
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommandStruct* pCommand, byte* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pCommand == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		return false;
	}
	unsigned char	byCommandWord = 0;
	int iPos = 0;
	// 校验帧的同步码
	if (false == CheckInstrumentFrameHead(pFrameData, pConstVar))
	{
		return false;
	}
	iPos += pConstVar->m_oFrameHeadSize;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pCommand, pConstVar);
	// 源IP地址
	memcpy(&pCommand->m_uiSrcIP, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
	iPos += pConstVar->m_oFramePacketSize4B;
	// 目标IP地址
	memcpy(&pCommand->m_uiDstIP, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
	iPos += pConstVar->m_oFramePacketSize4B;
	// 目标端口号
	memcpy(&pCommand->m_usReturnPort, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
	iPos += pConstVar->m_oFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pCommand->m_usCommand, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
	iPos += pConstVar->m_oFramePacketSize2B;
	// 解析帧内容

	while(true)
	{
		memcpy(&byCommandWord, &pFrameData[iPos], pConstVar->m_oFrameCmdSize1B);
		iPos += pConstVar->m_oFrameCmdSize1B;
		if (byCommandWord == pConstVar->m_oCmdSn)
		{
			memcpy(&pCommand->m_uiSN, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdHeadFrameTime)
		{
			memcpy(&pCommand->m_uiTimeHeadFrame, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLocalIPAddr)
		{
			memcpy(&pCommand->m_uiInstrumentIP, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLocalSysTime)
		{
			memcpy(&pCommand->m_uiSysTime, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLocalTimeFixedHigh)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedHigh, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLocalTimeFixedLow)
		{
			memcpy(&pCommand->m_usLocalTimeFixedLow, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdADCDataReturnAddr)
		{
			memcpy(&pCommand->m_uiADCDataReturnAddr, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdADCDataReturnPort)
		{
			memcpy(&pCommand->m_usADCDataReturnPort, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnCmd, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_oCmdADCDataReturnPortLimit)
		{
			memcpy(&pCommand->m_usADCDataReturnPortLimitLow, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnPortLimitHigh, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_oCmdSetBroadCastPort)
		{
			memcpy(&pCommand->m_usSetBroadCastPort, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdFDUErrorCode)
		{
			memcpy(&pCommand->m_byFDUErrorCodeDataCount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byFDUErrorCodeCmdCount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byTimeStatus, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byCtrlStatus, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_oCmdTBHigh)
		{
			memcpy(&pCommand->m_uiTBHigh, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdTbLow)
		{
			memcpy(&pCommand->m_usTBLow, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pCommand->m_usTBCtrl, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLAUXRoutOpenQuery)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenQuery, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLAUXRoutOpenSet)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenSet, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdTailRecSndTimeLow)
		{
			memcpy(&pCommand->m_usTailRecSndTimeLow, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_usBroadCastPortSet, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdADCSet)
		{
			memcpy(&pCommand->m_pbyADCSet[pCommand->m_usADCSetNum], &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
			pCommand->m_usADCSetNum += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pFrameData[iPos], pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLAUXErrorCode1)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineACount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineBCount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineACount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_oCmdLAUXErrorCode2)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeCmdCount, &pFrameData[iPos], pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdReturnRout)
		{
			memcpy(&pCommand->m_uiRoutIP, &pFrameData[iPos], pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_oCmdEnd)
		{
			break;
		}
		else
		{
			iPos += pConstVar->m_oFramePacketSize4B;
		}
	}
	return true;
}
// 生成与设备通讯帧
MatrixServerDll_API void MakeInstrumentFrame(m_oInstrumentCommandStruct* pCommand, byte* pFrameData, byte* pCommandWord, unsigned short usCommandWordNum, m_oConstVarStruct* pConstVar)
{
	if (pCommand == NULL)
	{
		return;
	}
	if (pFrameData == NULL)
	{
		return;
	}
	int iPos = 0;
	// 生成帧的同步码
	MakeInstrumentFrameHead(pFrameData, pConstVar);
	iPos += pConstVar->m_oFrameHeadSize;
	// 源IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiSrcIP, pConstVar->m_oFramePacketSize4B);
	iPos += pConstVar->m_oFramePacketSize4B;
	// 目标IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiDstIP, pConstVar->m_oFramePacketSize4B);
	iPos += pConstVar->m_oFramePacketSize4B;
	// 目标端口号
	memcpy(&pFrameData[iPos], &pCommand->m_usReturnPort, pConstVar->m_oFramePacketSize2B);
	iPos += pConstVar->m_oFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pFrameData[iPos], &pCommand->m_usCommand, pConstVar->m_oFramePacketSize2B);
	iPos += pConstVar->m_oFramePacketSize2B;
	// 包时间
	iPos += pConstVar->m_oFramePacketSize4B;

	// 生成帧内容
	for (unsigned short i=0; i<usCommandWordNum; i++)
	{
		memcpy(&pFrameData[iPos], &pCommandWord[i], pConstVar->m_oFrameCmdSize1B);
		iPos += pConstVar->m_oFrameCmdSize1B;
		if (pCommandWord[i] == pConstVar->m_oCmdSn)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSN, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdHeadFrameTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTimeHeadFrame, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLocalIPAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiInstrumentIP, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLocalSysTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSysTime, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLocalTimeFixedHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedHigh, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLocalTimeFixedLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLocalTimeFixedLow, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdADCDataReturnAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiADCDataReturnAddr, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdADCDataReturnPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPort, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnCmd, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdADCDataReturnPortLimit)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitLow, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitHigh, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdSetBroadCastPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usSetBroadCastPort, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdFDUErrorCode)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeDataCount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeCmdCount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byTimeStatus, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byCtrlStatus, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdTBHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTBHigh, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdTbLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTBLow, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usTBCtrl, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLAUXRoutOpenQuery)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenQuery, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLAUXRoutOpenSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenSet, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdTailRecSndTimeLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTailRecSndTimeLow, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdBroadCastPortSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usBroadCastPortSet, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdADCSet)
		{
			for(unsigned short j=0; j<pCommand->m_usADCSetNum; j+=pConstVar->m_oFramePacketSize4B)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_pbyADCSet[j], pConstVar->m_oFramePacketSize4B);
				iPos += pConstVar->m_oFramePacketSize4B;
			}
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdNetTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiNetTime, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLineTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineA, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineB, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineA, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineB, pConstVar->m_oFramePacketSize2B);
			iPos += pConstVar->m_oFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLAUXErrorCode1)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineACount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineBCount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineACount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdLAUXErrorCode2)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeCmdCount, pConstVar->m_oFramePacketSize1B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_oCmdReturnRout)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIP, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
		else
		{
			memset(&pFrameData[iPos], pConstVar->m_oSndFrameBufInit, pConstVar->m_oFramePacketSize4B);
			iPos += pConstVar->m_oFramePacketSize4B;
		}
	}
	pFrameData[iPos] = pConstVar->m_oCmdEnd;
}
// 创建CSocket接收端口并绑定端口和IP地址
MatrixServerDll_API SOCKET CreateInstrumentSocket(unsigned short usPort, unsigned int uiIP)
{
	SOCKET oSocket = INVALID_SOCKET;
	sockaddr_in oAddr;
	oSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	oAddr.sin_family = AF_INET;											// 填充套接字地址结构
	oAddr.sin_port = htons(usPort);
	oAddr.sin_addr.S_un.S_addr = uiIP;
	int iReturn = bind(oSocket, reinterpret_cast<sockaddr*>(&oAddr), sizeof(oAddr));	// 绑定本地地址
	listen(oSocket, 2);
	if (iReturn == SOCKET_ERROR)
	{

	}
	return oSocket;
}
// 设置广播模式
MatrixServerDll_API void SetInstrumentSocketBroadCast(SOCKET oSocket)
{
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	int iReturn = setsockopt(oSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&iOptval), iOptlen);
	if (iReturn == SOCKET_ERROR)
	{

	}
}
// 设置端口接收缓冲区接收帧数量
MatrixServerDll_API void SetRcvBufferSize(SOCKET oSocket, int iRcvBufferSize)
{
	int iOptionLen = sizeof(int);
	int iReturn = setsockopt(oSocket, SOL_SOCKET, SO_RCVBUF,  reinterpret_cast<const char *>(&iRcvBufferSize), iOptionLen);
	if (iReturn == SOCKET_ERROR)
	{
	}
}
// 设置端口发送缓冲区发送帧数量
MatrixServerDll_API void SetSndBufferSize(SOCKET oSocket, int iSndBufferSize)
{
	int iOptionLen = sizeof(int);
	int iReturn = setsockopt(oSocket, SOL_SOCKET, SO_SNDBUF,  reinterpret_cast<const char *>(&iSndBufferSize), iOptionLen);
	if (iReturn == SOCKET_ERROR)
	{
	}
}
// 得到网络接收缓冲区收到的帧数量
MatrixServerDll_API DWORD GetFrameCount(SOCKET oSocket, int iRcvFrameSize)
{
	DWORD dwFrameCount = 0;
	// 得到网络接收缓冲区数据字节数
	ioctlsocket(oSocket, FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / iRcvFrameSize;

	return dwFrameCount;
}
// 得到帧数据
MatrixServerDll_API bool GetFrameData(SOCKET oSocket, byte* pFrameData, int iRcvFrameSize)
{
	bool bReturn = false;
	sockaddr_in AddrFrom;
	int iSize = sizeof(AddrFrom);
	int iCount = recvfrom(oSocket, reinterpret_cast<char *>(pFrameData), iRcvFrameSize, 0, reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
	if(iCount == iRcvFrameSize)	//帧大小正确
	{
		bReturn = true;
	}
	else
	{

	}
	return bReturn;
}
// 发送帧
MatrixServerDll_API void SendFrame(SOCKET oSocket, byte* pFrameData,int iSndFrameSize, unsigned short usPort, unsigned int uiIP)
{
	// 发送帧
	sockaddr_in addrSend;
	addrSend.sin_family = AF_INET;											// 填充套接字地址结构
	addrSend.sin_port = htons(usPort);
	addrSend.sin_addr.S_un.S_addr = uiIP;
	sendto(oSocket, reinterpret_cast<const char*>(&pFrameData), iSndFrameSize, 0, reinterpret_cast<sockaddr*>(&addrSend), sizeof(addrSend));
}
// 创建常量信息结构体
MatrixServerDll_API void CreateConstVar(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	pEnv->m_pConstVar = new m_oConstVarStruct;
	pEnv->m_pConstVar->m_pFrameHeadCheck = NULL;
}
// 载入INI文件
MatrixServerDll_API void LoadIniFile(m_oConstVarStruct* pConstVar)
{
	CString strSection	= _T("");
	CString strSectionKey = _T("");
	CString strValue = _T("");
	CString strFilePath	= _T("");
	wchar_t strBuff[256];
	CString csTemp = _T("");
	int iDirectionNew = 0;
	int iDirectionOld = 0;
	int iTemp = 0;
	strFilePath = _T("..\\parameter\\MatrixServerDLL.ini");

	//读取ini文件中相应字段的内容
	strSection = _T("帧格式设置");			// 获取当前区域
	strSectionKey=_T("FrameHeadSize");		// 帧头长度
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oFrameHeadSize = _ttoi(strValue);

	strSectionKey=_T("FrameHeadCheck");		// 同步帧头
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_pFrameHeadCheck = new byte[pConstVar->m_oFrameHeadSize];
	for (int i=0; i<pConstVar->m_oFrameHeadSize; i++)
	{
		iDirectionNew = strValue.Find(_T("0x"), iDirectionOld);
		iDirectionNew += 2;
		csTemp = strValue.Mid(iDirectionNew, 2);
		_stscanf_s(csTemp, _T("%x"), &iTemp, sizeof(int));
		pConstVar->m_pFrameHeadCheck[i] = iTemp;
		iDirectionOld = iDirectionNew;
	}
	strSectionKey=_T("FrameCmdSize1B");		// 命令字长度1字节
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oFrameCmdSize1B = _ttoi(strValue);

	strSectionKey=_T("FramePacketSize1B");	// 命令包长度1字节
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oFramePacketSize1B = _ttoi(strValue);

	strSectionKey=_T("FramePacketSize2B");	// 命令包长度2字节
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oFramePacketSize2B = _ttoi(strValue);

	strSectionKey=_T("FramePacketSize4B");	// 命令包长度4字节
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oFramePacketSize4B = _ttoi(strValue);

	strSectionKey=_T("ADCDataSize3B");		// ADC数据所占字节数
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oADCDataSize3B = _ttoi(strValue);

	strSectionKey=_T("CommandWordMaxNum");	// 命令字个数最大值
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oCommandWordMaxNum = _ttoi(strValue);
	pConstVar->m_oADCSetCommandMaxByte = pConstVar->m_oCommandWordMaxNum * pConstVar->m_oFramePacketSize4B;
	strSectionKey=_T("SndFrameBufInit");	// 发送帧缓冲区初值设定
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oSndFrameBufInit = iTemp;

	strSectionKey=_T("RcvFrameSize");		// 接收的网络数据帧帧长度
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oRcvFrameSize = _ttoi(strValue);

	strSectionKey=_T("SndFrameSize");		// 发送的网络数据帧帧长度
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	pConstVar->m_oSndFrameSize = _ttoi(strValue);

	/////////////////////////////////////////////////////////////////////////
	strSection = _T("服务器与设备命令字设置");		// 获取当前区域
	strSectionKey=_T("SendSetCmd");			// 发送设置命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oSendSetCmd = iTemp;

	strSectionKey=_T("SendQueryCmd");		// 发送查询命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oSendQueryCmd = iTemp;

	strSectionKey=_T("SendADCCmd");			// 发送ADC采样数据重发命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oSendADCCmd = iTemp;

	strSectionKey=_T("CmdTBCtrl");			// TB开始采集开关控制命令(TB_L高8位)
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_oCmdTBCtrl, sizeof(unsigned int));

	strSectionKey=_T("CmdSn");				// 串号
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdSn = iTemp;

	strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdHeadFrameTime = iTemp;

	strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_oCmdLocalIPAddr, sizeof(byte));

	strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLocalSysTime = iTemp;

	strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLocalTimeFixedHigh = iTemp;

	strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLocalTimeFixedLow = iTemp;

	strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdADCDataReturnAddr = iTemp;

	strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdADCDataReturnPort = iTemp;

	strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdADCDataReturnPortLimit = iTemp;

	strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdSetBroadCastPort = iTemp;

	strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdFDUErrorCode = iTemp;

	strSectionKey=_T("CmdTBHigh");				// TB时刻高位
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdTBHigh = iTemp;

	strSectionKey=_T("CmdTbLow");				// TB时刻低位
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdTbLow = iTemp;

	strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUXRoutOpenQuery = iTemp;

	strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUXRoutOpenSet = iTemp;

	strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdTailRecSndTimeLow = iTemp;

	strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdBroadCastPortSet = iTemp;

	strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdADCSet = iTemp;

	strSectionKey=_T("CmdNetTime");				// 网络时刻
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdNetTime = iTemp;

	strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLineTailRecTimeLAUX = iTemp;

	strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUTailRecTimeLAUX = iTemp;

	strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUXErrorCode1 = iTemp;

	strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUXErrorCode2 = iTemp;

	strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdLAUXSetRout = iTemp;

	strSectionKey=_T("CmdReturnRout");			// 返回路由
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdReturnRout = iTemp;

	strSectionKey=_T("CmdEnd");					// 命令解析结束命令
	GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff),strFilePath);
	strValue = strBuff;
	_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
	pConstVar->m_oCmdEnd = iTemp;
}
// 初始化常量信息结构体
MatrixServerDll_API void InitConstVar(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	LoadIniFile(pEnv->m_pConstVar);
}
// 释放常量信息结构体
MatrixServerDll_API void FreeConstVar(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pConstVar != NULL)
	{
		if (pEnv->m_pConstVar->m_pFrameHeadCheck != NULL)
		{
			delete[] pEnv->m_pConstVar->m_pFrameHeadCheck;
		}
		delete pEnv->m_pConstVar;
	}
}
// 创建仪器通讯信息结构体
MatrixServerDll_API void CreateInstrumentCommInfo(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	pEnv->m_pInstrumentCommInfo = new m_oInstrumentCommInfoStruct;
}
// 打开程序配置文件
MatrixServerDll_API void OpenAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo)
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = _T("msxml2.domdocument");
	BOOL bData = pCommInfo->m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = _T("..\\parameter\\MatrixLineApp.XML");
	bData = pCommInfo->m_oXMLDOMDocument.load(oVariant);
}
//加载IP地址设置数据
MatrixServerDll_API void LoadIPSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	CString csSrcIP = _T("");
	CString csDstIP = _T("");
	// 找到IP地址设置区
	strKey = _T("LineServerAppIPSetup");
	lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	// 和现场仪器通讯的本机IP地址
	strKey = _T("IPForInstrument");
	csSrcIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
//	pCommInfo->m_uiSrcIP = inet_addr(ConvertCStringToConstCharPointer(csSrcIP));
	pCommInfo->m_uiSrcIP = inet_addr("192.168.0.19");
	strKey = _T("IPLCI");
	csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
//	pCommInfo->m_uiDstIP = inet_addr(ConvertCStringToConstCharPointer(csDstIP));
	pCommInfo->m_uiDstIP = inet_addr("255.255.255.255");
}
//加载端口设置数据
MatrixServerDll_API void LoadPortSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到IP地址设置区
	strKey = _T("LineServerAppPortSetup");
	lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = _T("PortLCI");
	pCommInfo->m_uiAimPort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForHeartBeat");
	pCommInfo->m_usHeartBeatReturnPort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForHeadFrame");
	pCommInfo->m_usHeadFramePort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForIPSet");
	pCommInfo->m_usIPSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}
//加载数据缓冲区大小数据
MatrixServerDll_API void LoadBufferSizeSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到数据缓冲区大小设置区
	strKey = _T("LineServerAppBufferSizeSetup");
	lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = _T("InstrumentCountAll");
	pCommInfo->m_uiInstrumentNum = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}
// 关闭程序配置文件
MatrixServerDll_API void CloseAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo)
{
	pCommInfo->m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}
//加载测线服务器程序设置数据
MatrixServerDll_API void LoadLineServerAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	// 打开程序配置文件
	OpenAppIniXMLFile(pCommInfo);

	//加载IP地址设置数据
	LoadIPSetupData(pCommInfo);
	//加载端口设置数据
	LoadPortSetupData(pCommInfo);
	//加载数据缓冲区大小数据
	LoadBufferSizeSetupData(pCommInfo);

	// 关闭程序配置文件
	CloseAppIniXMLFile(pCommInfo);
}
// 初始化仪器通讯信息结构体
MatrixServerDll_API void InitInstrumentCommInfo(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	LoadLineServerAppSetupData(pEnv->m_pInstrumentCommInfo);
// 	pEnv->m_pInstrumentCommInfo->m_uiDstIP = pCommInfo->m_uiDstIP;
// 	pEnv->m_pInstrumentCommInfo->m_uiSrcIP = pCommInfo->m_uiSrcIP;
// 	pEnv->m_pInstrumentCommInfo->m_uiAimPort = pCommInfo->m_uiAimPort;
// 	pEnv->m_pInstrumentCommInfo->m_usHeartBeatReturnPort = pCommInfo->m_usHeartBeatReturnPort;
// 	pEnv->m_pInstrumentCommInfo->m_usHeadFramePort = pCommInfo->m_usHeadFramePort;
// 	pEnv->m_pInstrumentCommInfo->m_usIPSetReturnPort = pCommInfo->m_usIPSetReturnPort;
}
// 释放仪器通讯信息结构体
MatrixServerDll_API void FreeInstrumentCommInfo(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pInstrumentCommInfo != NULL)
	{
		delete pEnv->m_pInstrumentCommInfo;
	}
}
// 创建心跳帧信息结构体
MatrixServerDll_API void CreateInstrumentHeartBeat(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pHeartBeatFrame = new m_oHeartBeatFrameStruct;
	InitializeCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
	pEnv->m_pHeartBeatFrame->m_pbySndFrameData = NULL;
	pEnv->m_pHeartBeatFrame->m_pbyCommandWord = NULL;
}
// 初始化心跳
MatrixServerDll_API void InitInstrumentHeartBeat(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
	// 源地址
	pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_uiSrcIP = pEnv->m_pInstrumentCommInfo->m_uiSrcIP;
	// 目的地址
	pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_uiDstIP = pEnv->m_pInstrumentCommInfo->m_uiDstIP;
	// 目标IP地址端口号
	pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_uiAimPort = pEnv->m_pInstrumentCommInfo->m_uiAimPort;
	// 心跳返回端口
	pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_usReturnPort = pEnv->m_pInstrumentCommInfo->m_usHeartBeatReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_usCommand = pEnv->m_pConstVar->m_oSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_pHeartBeatFrame->m_oCommandStruct, pEnv->m_pConstVar);
	// 清空发送帧缓冲区
	pEnv->m_pHeartBeatFrame->m_pbySndFrameData = new byte[pEnv->m_pConstVar->m_oSndFrameSize];
	memset(pEnv->m_pHeartBeatFrame->m_pbySndFrameData, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oSndFrameSize);
	// 清空心跳命令字集合
	pEnv->m_pHeartBeatFrame->m_pbyCommandWord = new byte[pEnv->m_pConstVar->m_oCommandWordMaxNum];
	memset(pEnv->m_pHeartBeatFrame->m_pbyCommandWord, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oCommandWordMaxNum);
	// 心跳命令字个数
	pEnv->m_pHeartBeatFrame->m_usCommandWordNum = 0;
	LeaveCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
}
// 释放心跳帧信息结构体
MatrixServerDll_API void FreeInstrumentHeartBeat(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pHeartBeatFrame != NULL)
	{
		DeleteCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
		if (pEnv->m_pHeartBeatFrame->m_pbySndFrameData != NULL)
		{
			delete[] pEnv->m_pHeartBeatFrame->m_pbySndFrameData;
		}
		if (pEnv->m_pHeartBeatFrame->m_pbyCommandWord != NULL)
		{
			delete[] pEnv->m_pHeartBeatFrame->m_pbyCommandWord;
		}
		delete pEnv->m_pHeartBeatFrame;
	}
}
// 创建首包帧信息结构体
MatrixServerDll_API void CreateInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pHeadFrame = new m_oHeadFrameStruct;
	InitializeCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	pEnv->m_pHeadFrame->m_pbyRcvFrameData = NULL;
}
// 初始化首包
MatrixServerDll_API void InitInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	// 首包接收缓冲区帧数设定为仪器个数
	pEnv->m_pHeadFrame->m_uiRcvFrameNum = pEnv->m_pInstrumentCommInfo->m_uiInstrumentNum;
	// 接收端口
	pEnv->m_pHeadFrame->m_oCommandStruct.m_usReturnPort = pEnv->m_pInstrumentCommInfo->m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_pHeadFrame->m_oCommandStruct, pEnv->m_pConstVar);
	// 清空接收帧缓冲区
	pEnv->m_pHeadFrame->m_pbyRcvFrameData = new byte[pEnv->m_pConstVar->m_oRcvFrameSize];
	memset(pEnv->m_pHeadFrame->m_pbyRcvFrameData, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
}
// 释放首包帧信息结构体
MatrixServerDll_API void FreeInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pHeadFrame != NULL)
	{
		DeleteCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
		if (pEnv->m_pHeadFrame->m_pbyRcvFrameData != NULL)
		{
			delete[] pEnv->m_pHeadFrame->m_pbyRcvFrameData;
		}
		delete pEnv->m_pHeadFrame;
	}
}
// 创建IP地址设置帧信息结构体
MatrixServerDll_API void CreateInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pIPSetFrame = new m_oIPSetFrameStruct;
	InitializeCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	pEnv->m_pIPSetFrame->m_pbySndFrameData = NULL;
	pEnv->m_pIPSetFrame->m_pbyCommandWord = NULL;
	pEnv->m_pIPSetFrame->m_pbyRcvFrameData = NULL;
}
// 初始化IP地址设置
MatrixServerDll_API void InitInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	// 源地址
	pEnv->m_pIPSetFrame->m_oCommandStructSet.m_uiSrcIP = pEnv->m_pInstrumentCommInfo->m_uiSrcIP;
	// 目的地址
	pEnv->m_pIPSetFrame->m_oCommandStructSet.m_uiDstIP = pEnv->m_pInstrumentCommInfo->m_uiDstIP;
	// 目标IP地址端口号
	pEnv->m_pIPSetFrame->m_oCommandStructSet.m_uiAimPort = pEnv->m_pInstrumentCommInfo->m_uiAimPort;
	// IP地址设置发送缓冲区帧数设定为仪器个数
	pEnv->m_pIPSetFrame->m_uiSndFrameNum = pEnv->m_pInstrumentCommInfo->m_uiInstrumentNum;
	// IP地址设置应答接收缓冲区帧数设定为仪器个数
	pEnv->m_pIPSetFrame->m_uiRcvFrameNum = pEnv->m_pInstrumentCommInfo->m_uiInstrumentNum;
	// IP地址设置返回端口
	pEnv->m_pIPSetFrame->m_oCommandStructSet.m_usReturnPort = pEnv->m_pInstrumentCommInfo->m_usIPSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pEnv->m_pIPSetFrame->m_oCommandStructSet.m_usCommand = pEnv->m_pConstVar->m_oSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_pIPSetFrame->m_oCommandStructSet, pEnv->m_pConstVar);
	// 清空发送帧缓冲区
	pEnv->m_pIPSetFrame->m_pbySndFrameData = new byte[pEnv->m_pConstVar->m_oSndFrameSize];
	memset(pEnv->m_pIPSetFrame->m_pbySndFrameData, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oSndFrameSize);
	// 清空IP地址设置命令字集合
	pEnv->m_pIPSetFrame->m_pbyCommandWord = new byte[pEnv->m_pConstVar->m_oCommandWordMaxNum];
	memset(pEnv->m_pIPSetFrame->m_pbyCommandWord, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oCommandWordMaxNum);
	// IP地址设置命令字个数
	pEnv->m_pIPSetFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_pIPSetFrame->m_oCommandStructReturn, pEnv->m_pConstVar);
	// 清空接收帧缓冲区
	pEnv->m_pIPSetFrame->m_pbyRcvFrameData = new byte[pEnv->m_pConstVar->m_oRcvFrameSize];
	memset(pEnv->m_pIPSetFrame->m_pbyRcvFrameData, pEnv->m_pConstVar->m_oSndFrameBufInit, pEnv->m_pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
}
// 释放IP地址设置帧信息结构体
MatrixServerDll_API void FreeInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pIPSetFrame != NULL)
	{
		DeleteCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
		if (pEnv->m_pIPSetFrame->m_pbySndFrameData != NULL)
		{
			delete[] pEnv->m_pIPSetFrame->m_pbySndFrameData;
		}
		if (pEnv->m_pIPSetFrame->m_pbyCommandWord != NULL)
		{
			delete[] pEnv->m_pIPSetFrame->m_pbyCommandWord;
		}
		if (pEnv->m_pIPSetFrame->m_pbyRcvFrameData != NULL)
		{
			delete[] pEnv->m_pIPSetFrame->m_pbyRcvFrameData;
		}
		delete pEnv->m_pIPSetFrame;
	}
}
// 创建线程处理标志位信息结构体
MatrixServerDll_API void CreateThreadProcFlag(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pThreadProcFlag = new m_oThreadProcFlagStruct;
	InitializeCriticalSection(&pEnv->m_pThreadProcFlag->m_oSecFlag);
}
// 初始化线程处理标志位
MatrixServerDll_API void InitThreadProcFlag(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pThreadProcFlag->m_oSecFlag);
	// 心跳处理线程停止标志位
	pEnv->m_pThreadProcFlag->m_bProcHeartBeatStop = false;
	// 首包处理线程停止标志位
	pEnv->m_pThreadProcFlag->m_bProcHeadFrameStop = false;
	// 仪器IP地址设置线程停止标志位
	pEnv->m_pThreadProcFlag->m_bProcIPSetStop = false;
	LeaveCriticalSection(&pEnv->m_pThreadProcFlag->m_oSecFlag);
}
// 释放线程处理标志位信息结构体
MatrixServerDll_API void FreeThreadProcFlag(m_oEnvironmentStruct* pEnv)
{
	if (pEnv->m_pThreadProcFlag != NULL)
	{
		DeleteCriticalSection(&pEnv->m_pThreadProcFlag->m_oSecFlag);
		delete pEnv->m_pThreadProcFlag;
	}
}
// 创建并设置心跳端口
MatrixServerDll_API void CreateAndSetHeartBeatSocket(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
	// 创建套接字
	pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket = CreateInstrumentSocket(pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_usReturnPort, pEnv->m_pHeartBeatFrame->m_oCommandStruct.m_uiSrcIP);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	LeaveCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
}
// 创建并设置首包端口
MatrixServerDll_API void CreateAndSetHeadFrameSocket(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	// 创建套接字
	pEnv->m_pHeadFrame->m_oHeadFrameSocket = CreateInstrumentSocket(pEnv->m_pHeadFrame->m_oCommandStruct.m_usReturnPort, pEnv->m_pHeadFrame->m_oCommandStruct.m_uiSrcIP);
	// 设置接收缓冲区
	SetRcvBufferSize(pEnv->m_pHeadFrame->m_oHeadFrameSocket, pEnv->m_pHeadFrame->m_uiRcvFrameNum * pEnv->m_pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
}
// 创建并设置IP地址设置端口
MatrixServerDll_API void CreateAndSetIPSetFrameSocket(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	// 创建套接字
	pEnv->m_pIPSetFrame->m_oIPSetFrameSocket = CreateInstrumentSocket(pEnv->m_pIPSetFrame->m_oCommandStructSet.m_usReturnPort, pEnv->m_pIPSetFrame->m_oCommandStructSet.m_uiSrcIP);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 设置发送缓冲区
	SetSndBufferSize(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket, pEnv->m_pIPSetFrame->m_uiSndFrameNum * pEnv->m_pConstVar->m_oSndFrameSize);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket, pEnv->m_pIPSetFrame->m_uiRcvFrameNum * pEnv->m_pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
}
// 初始化实例
MatrixServerDll_API void InitInstance(m_oEnvironmentStruct* pEnv)
{
	// 创建常量信息结构体
	CreateConstVar(pEnv);
	// 初始化常量信息结构体
	InitConstVar(pEnv);
	// 创建仪器通讯信息结构体
	CreateInstrumentCommInfo(pEnv);
	// 初始化仪器通讯信息结构体
	InitInstrumentCommInfo(pEnv);
	// 创建心跳帧信息结构体
	CreateInstrumentHeartBeat(pEnv);
	// 初始化心跳
	InitInstrumentHeartBeat(pEnv);
	// 创建首包帧信息结构体
	CreateInstrumentHeadFrame(pEnv);
	// 初始化首包
	InitInstrumentHeadFrame(pEnv);
	// 创建IP地址设置帧信息结构体
	CreateInstrumentIPSetFrame(pEnv);
	// 初始化IP地址设置
	InitInstrumentIPSetFrame(pEnv);
	// 创建线程处理标志位信息结构体
	CreateThreadProcFlag(pEnv);
	// 初始化线程处理标志位
	InitThreadProcFlag(pEnv);
	// 创建并设置心跳端口
	CreateAndSetHeartBeatSocket(pEnv);
	// 创建并设置首包端口
	CreateAndSetHeadFrameSocket(pEnv);
	// 创建并设置IP地址设置端口
	CreateAndSetIPSetFrameSocket(pEnv);
}
// 释放实例资源
MatrixServerDll_API void FreeInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv != NULL)
	{
		FreeConstVar(pEnv);
		FreeInstrumentCommInfo(pEnv);
		FreeInstrumentHeartBeat(pEnv);
		FreeInstrumentHeadFrame(pEnv);
		FreeInstrumentIPSetFrame(pEnv);
		FreeThreadProcFlag(pEnv);
		delete pEnv;
		pEnv = NULL;
	}
}
// 解析首包帧
MatrixServerDll_API bool ParseInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	bool bReturn = false;
	EnterCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	bReturn = ParseInstrumentFrame(&pEnv->m_pHeadFrame->m_oCommandStruct, pEnv->m_pHeadFrame->m_pbyRcvFrameData,
		pEnv->m_pConstVar);
	LeaveCriticalSection(&pEnv->m_pHeadFrame->m_oSecHeadFrame);
	return bReturn;
}
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrumentIPSetReturnFrame(m_oEnvironmentStruct* pEnv)
{
	bool bReturn = false;
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	bReturn = ParseInstrumentFrame(&pEnv->m_pIPSetFrame->m_oCommandStructReturn, pEnv->m_pIPSetFrame->m_pbyRcvFrameData, pEnv->m_pConstVar);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	return bReturn;
}
// 生成心跳帧
MatrixServerDll_API void MakeInstrumentHeartBeatFrame(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
	MakeInstrumentFrame(&pEnv->m_pHeartBeatFrame->m_oCommandStruct, pEnv->m_pHeartBeatFrame->m_pbySndFrameData,
		pEnv->m_pHeartBeatFrame->m_pbyCommandWord, pEnv->m_pHeartBeatFrame->m_usCommandWordNum, pEnv->m_pConstVar);
	LeaveCriticalSection(&pEnv->m_pHeartBeatFrame->m_oSecHeartBeat);
}
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	EnterCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
	MakeInstrumentFrame(&pEnv->m_pIPSetFrame->m_oCommandStructSet, pEnv->m_pIPSetFrame->m_pbySndFrameData, 
		pEnv->m_pIPSetFrame->m_pbyCommandWord, pEnv->m_pIPSetFrame->m_usCommandWordNum, pEnv->m_pConstVar);
	LeaveCriticalSection(&pEnv->m_pIPSetFrame->m_oSecIPSetFrame);
}