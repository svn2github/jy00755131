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
// 创建实例，并返回实例指针
MatrixServerDll_API m_oEnvironmentStruct* CreateInstance(void)
{
	m_oEnvironmentStruct* ptr = new m_oEnvironmentStruct;
	return ptr;
}
// 释放实例资源
MatrixServerDll_API void FreeInstance(m_oEnvironmentStruct* ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrumentFrameHead(byte* pFrameData)
{
	byte byFrameHeadCheck[] = FrameHeadCheck;
	for (int i=0; i<FrameHeadSize; i++)
	{
		if (pFrameData[i] != byFrameHeadCheck[i])
		{
			return false;
		}
	}
	return true;
}
// 生成帧的同步码
MatrixServerDll_API void MakeInstrumentFrameHead(byte* pFrameData)
{
	byte byFrameHeadCheck[] = FrameHeadCheck;
	for (int i=0; i<FrameHeadSize; i++)
	{
		pFrameData[i] = byFrameHeadCheck[i];
	}
}
// 重置帧内容解析变量
MatrixServerDll_API void ResetInstrumentFramePacket(m_oInstrumentCommandStruct* pCommand)
{
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
	memset(pCommand->m_byADCSet, SndFrameBufInit, ADCSetCommandMaxByte);
	// 0x18命令数据个数
	pCommand->m_usADCSetNum = 0;
}
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommandStruct* pCommand, byte* pFrameData)
{
	unsigned char	byCommandWord = 0;
	int iPos = 0;
	// 校验帧的同步码
	if (false == CheckInstrumentFrameHead(pFrameData))
	{
		return false;
	}
	iPos += FrameHeadSize;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pCommand);
	// 源IP地址
	memcpy(&pCommand->m_uiSrcIP, &pFrameData[iPos], FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&pCommand->m_uiDstIP, &pFrameData[iPos], FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&pCommand->m_usReturnPort, &pFrameData[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pCommand->m_usCommand, &pFrameData[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 解析帧内容

	while(true)
	{
		memcpy(&byCommandWord, &pFrameData[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (byCommandWord == CmdSn)
		{
			memcpy(&pCommand->m_uiSN, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdHeadFrameTime)
		{
			memcpy(&pCommand->m_uiTimeHeadFrame, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalIPAddr)
		{
			memcpy(&pCommand->m_uiInstrumentIP, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalSysTime)
		{
			memcpy(&pCommand->m_uiSysTime, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalTimeFixedHigh)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedHigh, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalTimeFixedLow)
		{
			memcpy(&pCommand->m_usLocalTimeFixedLow, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdADCDataReturnAddr)
		{
			memcpy(&pCommand->m_uiADCDataReturnAddr, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdADCDataReturnPort)
		{
			memcpy(&pCommand->m_usADCDataReturnPort, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnCmd, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdADCDataReturnPortLimit)
		{
			memcpy(&pCommand->m_usADCDataReturnPortLimitLow, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnPortLimitHigh, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdSetBroadCastPort)
		{
			memcpy(&pCommand->m_usSetBroadCastPort, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdFDUErrorCode)
		{
			memcpy(&pCommand->m_byFDUErrorCodeDataCount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byFDUErrorCodeCmdCount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byTimeStatus, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byCtrlStatus, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (byCommandWord == CmdTBHigh)
		{
			memcpy(&pCommand->m_uiTBHigh, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdTbLow)
		{
			memcpy(&pCommand->m_usTBLow, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usTBCtrl, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUXRoutOpenQuery)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenQuery, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLAUXRoutOpenSet)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenSet, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdTailRecSndTimeLow)
		{
			memcpy(&pCommand->m_usTailRecSndTimeLow, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_usBroadCastPortSet, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdADCSet)
		{
			memcpy(&pCommand->m_byADCSet[pCommand->m_usADCSetNum], &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
			pCommand->m_usADCSetNum += FramePacketSize4B;
		}
		else if (byCommandWord == CmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUXErrorCode1)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineACount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineBCount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineACount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (byCommandWord == CmdLAUXErrorCode2)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeCmdCount, &pFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdReturnRout)
		{
			memcpy(&pCommand->m_uiRoutIP, &pFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdEnd)
		{
			break;
		}
		else
		{
			iPos += FramePacketSize4B;
		}
	}
	return true;
}
// 生成与设备通讯帧
MatrixServerDll_API void MakeInstrumentFrame(m_oInstrumentCommandStruct* pCommand, byte* pFrameData, byte* pCommandWord, unsigned short usCommandWordNum)
{
	int iPos = 0;
	// 生成帧的同步码
	MakeInstrumentFrameHead(pFrameData);
	iPos += FrameHeadSize;
	// 源IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiSrcIP, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiDstIP, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&pFrameData[iPos], &pCommand->m_usReturnPort, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pFrameData[iPos], &pCommand->m_usCommand, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 包时间
	iPos += FramePacketSize4B;

	// 生成帧内容
	for (unsigned short i=0; i<usCommandWordNum; i++)
	{
		memcpy(&pFrameData[iPos], &pCommandWord[i], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (pCommandWord[i] == CmdSn)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSN, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdHeadFrameTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTimeHeadFrame, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLocalIPAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiInstrumentIP, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLocalSysTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSysTime, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLocalTimeFixedHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedHigh, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLocalTimeFixedLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLocalTimeFixedLow, FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdADCDataReturnAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiADCDataReturnAddr, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdADCDataReturnPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPort, FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnCmd, FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (pCommandWord[i] == CmdADCDataReturnPortLimit)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitLow, FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitHigh, FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (pCommandWord[i] == CmdSetBroadCastPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usSetBroadCastPort, FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdFDUErrorCode)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeDataCount, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeCmdCount, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byTimeStatus, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byCtrlStatus, FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (pCommandWord[i] == CmdTBHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTBHigh, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdTbLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTBLow, FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usTBCtrl, FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (pCommandWord[i] == CmdLAUXRoutOpenQuery)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenQuery, FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLAUXRoutOpenSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenSet, FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdTailRecSndTimeLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTailRecSndTimeLow, FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdBroadCastPortSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usBroadCastPortSet, FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdADCSet)
		{
			for(unsigned short j=0; j<pCommand->m_usADCSetNum; j+=FramePacketSize4B)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_byADCSet[j], FramePacketSize4B);
				iPos += FramePacketSize4B;
			}
		}
		else if (pCommandWord[i] == CmdNetTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiNetTime, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdLineTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineA, FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineB, FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (pCommandWord[i] == CmdLAUTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineA, FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineB, FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (pCommandWord[i] == CmdLAUXErrorCode1)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineACount, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineBCount, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineACount, FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (pCommandWord[i] == CmdLAUXErrorCode2)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeCmdCount, FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdReturnRout)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIP, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (pCommandWord[i] == CmdEnd)
		{
			break;
		}
		else
		{
			memset(&pFrameData[iPos], SndFrameBufInit, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
	}
}
// 重置帧内通讯信息
MatrixServerDll_API void ResetInstrumentFrameComm(m_oInstrumentCommandStruct* pCommand, m_oEnvironmentStruct* pEnv)
{
	// 源地址
	pCommand->m_csSrcIP = pEnv->m_csSrcIP;
	pCommand->m_uiSrcIP = pEnv->m_uiSrcIP;
	// 目的地址
	pCommand->m_csDstIP = pEnv->m_csDstIP;
	pCommand->m_uiDstIP = pEnv->m_uiDstIP;
	// 目标IP地址端口号
	pCommand->m_uiAimPort = pEnv->m_uiAimPort;
}
// 创建CSocket接收端口并绑定端口和IP地址
MatrixServerDll_API BOOL CreateInstrumentSocket(m_oInstrumentCommandStruct* pCommand, CSocket* pCSocket)
{
	BOOL bReturn = FALSE;
	// 生成网络端口
	bReturn = pCSocket->Create(pCommand->m_usReturnPort, SOCK_DGRAM, pCommand->m_csSrcIP);
	pCSocket->Bind(pCommand->m_usReturnPort,pCommand->m_csSrcIP);
	return bReturn;
}
// 设置广播模式
MatrixServerDll_API BOOL SetInstrumentSocketBroadCast(CSocket* pCSocket)
{
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	return pCSocket->SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
}
// 设置端口接收缓冲区接收帧数量
MatrixServerDll_API BOOL SetRcvBufferSize(CSocket* pCSocket, int iFrameCount)
{
	int iOptionValue = iFrameCount * RcvFrameSize;
	int iOptionLen = sizeof(int);
	return pCSocket->SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}
// 设置端口发送缓冲区发送帧数量
MatrixServerDll_API BOOL SetSndBufferSize(CSocket* pCSocket, int iFrameCount)
{
	int iOptionValue = iFrameCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	return pCSocket->SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}
// 得到网络接收缓冲区收到的帧数量
MatrixServerDll_API DWORD GetFrameCount(CSocket* pCSocket)
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	pCSocket->IOCtl(FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / RcvFrameSize;

	return dwFrameCount;
}
// 得到帧数据
MatrixServerDll_API BOOL GetFrameData(CSocket* pCSocket, byte* pFrameData)
{
	BOOL bReturn = FALSE;
	// 得到帧数据
	int iCount = pCSocket->Receive(pFrameData, RcvFrameSize);
	if(iCount == RcvFrameSize)	//帧大小正确
	{
		bReturn = TRUE;
	}
	return bReturn;
}
// 发送仪器IP地址设置帧
MatrixServerDll_API BOOL SendFrame(CSocket* pCSocket, byte* pFrameData, unsigned int uiPort, CString strIP)
{
	BOOL bReturn = FALSE;
	// 发送帧
	int iCount = pCSocket->SendTo(pFrameData, SndFrameSize, uiPort, strIP);
	if(iCount == SndFrameSize)	//帧大小正确
	{
		bReturn = TRUE;
	}
	return bReturn;
}
// 初始化心跳
MatrixServerDll_API void InitInstrumentHeartBeat(m_oEnvironmentStruct* pEnv)
{
	// 重置帧内通讯信息
	ResetInstrumentFrameComm(&pEnv->m_oHeartBeatFrame.m_oCommandStruct, pEnv);
	// 应答端口
	pEnv->m_oHeartBeatFrame.m_oCommandStruct.m_usReturnPort = pEnv->m_usHeartBeatReturnPort;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pEnv->m_oHeartBeatFrame.m_oCommandStruct.m_usCommand = SendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_oHeartBeatFrame.m_oCommandStruct);
	// 清空发送帧缓冲区
	memset(&pEnv->m_oHeartBeatFrame.m_bySndFrameData, SndFrameBufInit, SndFrameSize);
	// 清空心跳命令字集合
	memset(&pEnv->m_oHeartBeatFrame.m_byCommandWord, SndFrameBufInit, CommandWordMaxNum);
	// 心跳命令字个数
	pEnv->m_oHeartBeatFrame.m_usCommandWordNum = 0;
}
// 初始化首包
MatrixServerDll_API void InitInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	// 重置帧内通讯信息
	ResetInstrumentFrameComm(&pEnv->m_oHeadFrame.m_oCommandStruct, pEnv);
	// 应答端口
	pEnv->m_oHeadFrame.m_oCommandStruct.m_usReturnPort = pEnv->m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_oHeadFrame.m_oCommandStruct);
	// 清空发送帧缓冲区
	memset(&pEnv->m_oHeadFrame.m_byRcvFrameData, SndFrameBufInit, RcvFrameSize);
}
// 初始化IP地址设置
MatrixServerDll_API void InitInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	// 重置帧内通讯信息
	ResetInstrumentFrameComm(&pEnv->m_oIPSetFrame.m_oCommandStruct, pEnv);
	// 应答端口
	pEnv->m_oIPSetFrame.m_oCommandStruct.m_usReturnPort = pEnv->m_usIPSetReturnPort;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pEnv->m_oIPSetFrame.m_oCommandStruct.m_usCommand = SendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_oIPSetFrame.m_oCommandStruct);
	// 清空发送帧缓冲区
	memset(&pEnv->m_oIPSetFrame.m_bySndFrameData, SndFrameBufInit, SndFrameSize);
	// 清空心跳命令字集合
	memset(&pEnv->m_oIPSetFrame.m_byCommandWord, SndFrameBufInit, CommandWordMaxNum);
	// 心跳命令字个数
	pEnv->m_oIPSetFrame.m_usCommandWordNum = 0;
}
// 初始化IP地址设置应答
MatrixServerDll_API void InitInstrumentIPSetReturnFrame(m_oEnvironmentStruct* pEnv)
{
	// 重置帧内通讯信息
	ResetInstrumentFrameComm(&pEnv->m_oIPSetReturnFrame.m_oCommandStruct, pEnv);
	// 应答端口
	pEnv->m_oIPSetReturnFrame.m_oCommandStruct.m_usReturnPort = pEnv->m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pEnv->m_oIPSetReturnFrame.m_oCommandStruct);
	// 清空发送帧缓冲区
	memset(&pEnv->m_oIPSetReturnFrame.m_byRcvFrameData, SndFrameBufInit, RcvFrameSize);
}
// 解析首包帧
MatrixServerDll_API bool ParseInstrumentHeadFrame(m_oEnvironmentStruct* pEnv)
{
	return ParseInstrumentFrame(&pEnv->m_oHeadFrame.m_oCommandStruct, pEnv->m_oHeadFrame.m_byRcvFrameData);
}
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrumentIPSetReturnFrame(m_oEnvironmentStruct* pEnv)
{
	return ParseInstrumentFrame(&pEnv->m_oIPSetReturnFrame.m_oCommandStruct, pEnv->m_oHeadFrame.m_byRcvFrameData);
}
// 生成心跳帧
MatrixServerDll_API void MakeInstrumentHeartBeatFrame(m_oEnvironmentStruct* pEnv)
{
	MakeInstrumentFrame(&pEnv->m_oHeartBeatFrame.m_oCommandStruct, pEnv->m_oHeartBeatFrame.m_bySndFrameData, pEnv->m_oHeartBeatFrame.m_byCommandWord, pEnv->m_oHeartBeatFrame.m_usCommandWordNum);
}
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oEnvironmentStruct* pEnv)
{
	MakeInstrumentFrame(&pEnv->m_oIPSetFrame.m_oCommandStruct, pEnv->m_oIPSetFrame.m_bySndFrameData, pEnv->m_oIPSetFrame.m_byCommandWord, pEnv->m_oIPSetFrame.m_usCommandWordNum);
}
