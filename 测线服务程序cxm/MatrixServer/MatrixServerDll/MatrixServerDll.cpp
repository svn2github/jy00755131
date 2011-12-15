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
// 重置帧内容解析变量
MatrixServerDll_API void ResetInstrumentFrame(m_oInstrumentCommand* pCommand)
{
	// 源地址
	pCommand->m_uiSrcIP = 0;
	// 目的地址
	pCommand->m_uiDstIP = 0;
	// 端口
	pCommand->m_usAimPort = 0;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pCommand->m_usCommand = 0;
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
}
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommand* pCommand)
{
	unsigned char	byCommandWord = 0;
	int iPos = 0;
	// 校验帧的同步码
	if (false == CheckInstrumentFrameHead(pCommand->m_byRcvFrameData))
	{
		return false;
	}
	iPos += FrameHeadSize;
	// 重置帧内容解析变量
	ResetInstrumentFrame(pCommand);
	// 源IP地址
	memcpy(&pCommand->m_uiSrcIP, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&pCommand->m_uiDstIP, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&pCommand->m_usAimPort, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pCommand->m_usCommand, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 解析帧内容

	while(true)
	{
		memcpy(&byCommandWord, &pCommand->m_byRcvFrameData[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (byCommandWord == CmdSn)
		{
			memcpy(&pCommand->m_uiSN, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdHeadFrameTime)
		{
			memcpy(&pCommand->m_uiTimeHeadFrame, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalIPAddr)
		{
			memcpy(&pCommand->m_uiInstrumentIP, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalSysTime)
		{
			memcpy(&pCommand->m_uiSysTime, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalTimeFixedHigh)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedHigh, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLocalTimeFixedLow)
		{
			memcpy(&pCommand->m_usLocalTimeFixedLow, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdADCDataReturnAddr)
		{
			memcpy(&pCommand->m_uiADCDataReturnAddr, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdADCDataReturnPort)
		{
			memcpy(&pCommand->m_usADCDataReturnPort, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnCmd, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdADCDataReturnPortLimit)
		{
			memcpy(&pCommand->m_usADCDataReturnPortLimitLow, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnPortLimitHigh, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdSetBroadCastPort)
		{
			memcpy(&pCommand->m_usSetBroadCastPort, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdFDUErrorCode)
		{
			memcpy(&pCommand->m_byFDUErrorCodeDataCount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byFDUErrorCodeCmdCount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byTimeStatus, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byCtrlStatus, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (byCommandWord == CmdTBHigh)
		{
			memcpy(&pCommand->m_uiTBHigh, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdTbLow)
		{
			memcpy(&pCommand->m_usTBLow, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usTBCtrl, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUXRoutOpenQuery)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenQuery, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLAUXRoutOpenSet)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenSet, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdTailRecSndTimeLow)
		{
			memcpy(&pCommand->m_usTailRecSndTimeLow, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_usBroadCastPortSet, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pCommand->m_byRcvFrameData[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
		}
		else if (byCommandWord == CmdLAUXErrorCode1)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineACount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineBCount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineACount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize1B;
		}
		else if (byCommandWord == CmdLAUXErrorCode2)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeCmdCount, &pCommand->m_byRcvFrameData[iPos], FramePacketSize1B);
			iPos += FramePacketSize4B;
		}
		else if (byCommandWord == CmdReturnRout)
		{
			memcpy(&pCommand->m_uiRoutIP, &pCommand->m_byRcvFrameData[iPos], FramePacketSize4B);
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