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
// 宽字节字符串转换为多字节字符串
string WideCharToMultiChar(wstring str)
{
	string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	LPCWSTR lpWideCharStr = str.c_str();
	int iWideCharSize = (int)str.size();
	int len = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, iWideCharSize, NULL, 0, NULL, NULL);
	char *buffer = new char[len+1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, iWideCharSize, buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}
// 返回类型为string，不是标准C支持的类型
MATRIXSERVERDLL_API string ConvertCStrToStr(CString str)
{
	CStringW strw;
	wstring wstr;
	strw = str;
	wstr = strw;
	string mstring = WideCharToMultiChar(wstr);
	return mstring;
}
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
	pEnv->m_pLogOutPut = NULL;
	return pEnv;
}
// 创建日志输出结构体
MatrixServerDll_API m_oLogOutPutStruct* CreateLogOutPut()
{
	m_oLogOutPutStruct* pLogOutPut = NULL;
	pLogOutPut = new m_oLogOutPutStruct;
	InitializeCriticalSection(&pLogOutPut->m_oSecLogFile);
	pLogOutPut->m_pFile = NULL;
	pLogOutPut->m_uiErrorCount = 0;
	return pLogOutPut;
}
// 打开日志输出文件
MatrixServerDll_API void OpenLogOutPutFile(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	CString str = _T("");
	CString csSaveLogFilePath = _T("");
	SYSTEMTIME  sysTime;
	errno_t err;
	// 创建程序运行日志文件夹
	CreateDirectory(LogFolderPath, NULL);
	csSaveLogFilePath += LogFolderPath;
	GetLocalTime(&sysTime);
	str.Format(_T("\\%04d年%02d月%02d日%02d时%02d分%02d秒.log"),sysTime.wYear, sysTime.wMonth, 
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	csSaveLogFilePath += str;
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	err = fopen_s(&pLogOutPut->m_pFile, ConvertCStrToStr(csSaveLogFilePath).c_str(), "wt+, ccs=UNICODE");
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 加入到日志输出队列
MatrixServerDll_API void AddMsgToLogOutPutList(m_oLogOutPutStruct* pLogOutPut = NULL, 
												byte byErrorType = LogType, 
												unsigned int uiErrorCode = 0, 
												string pFuncName = "", 
												string pVarName = "")
{
	CString cstr = _T("");
	CString strOutPut = _T("");
	string str = "";
	string strTemp = "";
	SYSTEMTIME  sysTime;
	if (pLogOutPut == NULL)
	{
		return;
	}
	if (OutPutSelect == OutPutRelease)
	{
		if (byErrorType != ErrorType)
		{
			return;
		}
	}
	if (pLogOutPut->m_uiErrorCount > OutPutLogErrorLimit)
	{
		return;
	}
	GetLocalTime(&sysTime);
	cstr.Format(_T("%04d年%02d月%02d日 %02d:%02d:%02d:%03d\t"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	if (byErrorType == LogType)
	{
		strOutPut += _T("Log\t");
	}
	else if (byErrorType == WarningType)
	{
		strOutPut += _T("Warning\t");
	}
	else if (byErrorType == ErrorType)
	{
		strOutPut += _T("Error\t");
		cstr.Format(_T("ErrorCode = %d\t"), uiErrorCode);
	}
	else if (byErrorType == ExpandType)
	{
		strOutPut += _T("ExpandType\t");
	}
	strOutPut += cstr;
	str = ConvertCStrToStr(strOutPut);
	if (pFuncName.empty() == false)
	{
		strTemp = "程序运行到函数：";
		str += strTemp;
		str += pFuncName;
		str += '\t';
	}
	if (pVarName.empty() == false)
	{
		strTemp = "日志信息：";
		str += strTemp;
		str += pVarName;
	}
	str += '\r';
	str += '\n';
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	pLogOutPut->m_oLogOutputList.push_back(str);
	if (byErrorType == ErrorType)
	{
		pLogOutPut->m_uiErrorCount++;
	}
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 日志输出写入文件
MatrixServerDll_API void LogOutPutWriteInFile(FILE* pFile, string str)
{
	if (pFile == NULL)
	{
		return;
	}
	size_t strSize = str.length();
	fwrite(str.c_str(), sizeof(byte), strSize, pFile);
}
// 写入日志输出文件
MatrixServerDll_API void WriteLogOutPutListToFile(m_oLogOutPutStruct* pLogOutPut)
{
	list<string>::iterator iter;
	string str = "";
	if (pLogOutPut == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	while(pLogOutPut->m_oLogOutputList.empty() == false)
	{
		iter = pLogOutPut->m_oLogOutputList.begin();
		str += *iter;
		pLogOutPut->m_oLogOutputList.pop_front();
	}
	LogOutPutWriteInFile(pLogOutPut->m_pFile, str);
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 关闭日志输出文件
MatrixServerDll_API void CloseLogOutPutFile(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	errno_t err;
	WriteLogOutPutListToFile(pLogOutPut);
	err = fclose( pLogOutPut->m_pFile );
}

// 初始化日志输出结构体
MatrixServerDll_API void InitLogOutPut(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	// 清空日志输出队列
	pLogOutPut->m_oLogOutputList.clear();
	// 打开日志输出文件
	OpenLogOutPutFile(pLogOutPut);
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 释放日志输出结构体
MatrixServerDll_API void FreeLogOutPut(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	// 关闭日志输出文件
	CloseLogOutPutFile(pLogOutPut);
	// 清空日志输出队列
	pLogOutPut->m_oLogOutputList.clear();
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
	DeleteCriticalSection(&pLogOutPut->m_oSecLogFile);
	delete pLogOutPut;
}
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrumentFrameHead(byte* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"CheckInstrumentFrameHead", "pFrameData");
		return false;
	}
	if (pConstVar == NULL)
	{
		return false;
	}
	for (int i=0; i<pConstVar->m_oFrameHeadSize; i++)
	{
		if (pFrameData[i] != pConstVar->m_pFrameHeadCheck[i])
		{
			AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_CHECKFRAMEHEAD, 
								"CheckInstrumentFrameHead");
			return false;
		}
	}
	return true;
}
// 生成帧的同步码
MatrixServerDll_API bool MakeInstrumentFrameHead(byte* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"MakeInstrumentFrameHead", "pFrameData");
		return false;
	}
	if (pConstVar == NULL)
	{
		return false;
	}
	memcpy(pFrameData, pConstVar->m_pFrameHeadCheck, pConstVar->m_oFrameHeadSize);
	return true;
}
// 重置帧内容解析变量
MatrixServerDll_API bool ResetInstrumentFramePacket(m_oInstrumentCommandStruct* pCommand)
{
	if (pCommand == NULL)
	{
		return false;
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
	// 0x18命令数据个数
	pCommand->m_usADCSetNum = 0;
	
	return true;
}
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommandStruct* pCommand, byte* pFrameData,
												m_oConstVarStruct* pConstVar)
{
	if (pCommand == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"ParseInstrumentFrame", "pCommand");
		return false;
	}
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"ParseInstrumentFrame", "pFrameData");
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
	ResetInstrumentFramePacket(pCommand);
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
MatrixServerDll_API bool MakeInstrumentFrame(m_oInstrumentCommandStruct* pCommand, m_oConstVarStruct* pConstVar,
												byte* pFrameData, byte* pCommandWord, unsigned short usCommandWordNum)
{
	if (pCommand == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"MakeInstrumentFrame", "pCommand");
		return false;
	}
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"MakeInstrumentFrame", "pFrameData");
		return false;
	}
	if (pCommandWord == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, ErrorType, IDS_ERR_PTRISNULL, 
							"MakeInstrumentFrame", "pCommandWord");
		return false;
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
	return true;
}
// 创建CSocket接收端口并绑定端口和IP地址
MatrixServerDll_API SOCKET CreateInstrumentSocket(unsigned short usPort, unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	SOCKET oSocket = INVALID_SOCKET;
	sockaddr_in oAddr;
	oSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	oAddr.sin_family = AF_INET;											// 填充套接字地址结构
	oAddr.sin_port = htons(usPort);
	oAddr.sin_addr.S_un.S_addr = uiIP;
	// 绑定本地地址
	if (SOCKET_ERROR == bind(oSocket, reinterpret_cast<sockaddr*>(&oAddr), sizeof(oAddr)))
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"CreateInstrumentSocket", "bind");
	}
	else
	{
		listen(oSocket, 2);
	}
	return oSocket;
}
// 设置广播模式
MatrixServerDll_API void SetInstrumentSocketBroadCast(SOCKET oSocket, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_BROADCAST, 
									reinterpret_cast<const char *>(&iOptval), iOptlen))
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"SetInstrumentSocketBroadCast", "setsockopt");
	}
}
// 设置端口接收缓冲区接收帧数量
MatrixServerDll_API void SetRcvBufferSize(SOCKET oSocket, int iRcvBufferSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_RCVBUF,  
								reinterpret_cast<const char *>(&iRcvBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"SetRcvBufferSize", "setsockopt");
	}
}
// 设置端口发送缓冲区发送帧数量
MatrixServerDll_API void SetSndBufferSize(SOCKET oSocket, int iSndBufferSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_SNDBUF,  
								reinterpret_cast<const char *>(&iSndBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"SetSndBufferSize", "setsockopt");
	}
}
// 得到网络接收缓冲区收到的帧数量
MatrixServerDll_API DWORD GetFrameCount(SOCKET oSocket, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	DWORD dwFrameCount = 0;
	// 得到网络接收缓冲区数据字节数
	if (SOCKET_ERROR == ioctlsocket(oSocket, FIONREAD, &dwFrameCount))
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"GetFrameCount", "ioctlsocket");
	}
	else
	{
		// 得到帧数量
		dwFrameCount = dwFrameCount / iRcvFrameSize;
	}
	return dwFrameCount;
}
// 得到帧数据
MatrixServerDll_API bool GetFrameData(SOCKET oSocket, byte* pFrameData, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	bool bReturn = false;
	sockaddr_in AddrFrom;
	int iSize = sizeof(AddrFrom);
	int iCount = recvfrom(oSocket, reinterpret_cast<char *>(pFrameData), iRcvFrameSize, 0, reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
	if(iCount == iRcvFrameSize)	//帧大小正确
	{
		bReturn = true;
	}
	else if (iCount == SOCKET_ERROR)
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"GetFrameData", "recvfrom");
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, IDS_ERR_RCVFRAMESIZE, 
							"GetFrameData", "recvfrom");
	}
	return bReturn;
}
// 发送帧
MatrixServerDll_API bool SendFrame(SOCKET oSocket, byte* pFrameData,int iSndFrameSize, 
								unsigned short usPort, unsigned int uiIP, 
								m_oLogOutPutStruct* pLogOutPut = NULL)
{
	bool bReturn = false;
	// 发送帧
	sockaddr_in addrSend;
	addrSend.sin_family = AF_INET;											// 填充套接字地址结构
	addrSend.sin_port = htons(usPort);
	addrSend.sin_addr.S_un.S_addr = uiIP;
	int iCount = sendto(oSocket, reinterpret_cast<const char*>(&pFrameData), iSndFrameSize, 0, 
					reinterpret_cast<sockaddr*>(&addrSend), sizeof(addrSend));
	if (iCount == iSndFrameSize)
	{
		bReturn = true;
	} 
	else if (iCount == SOCKET_ERROR)
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, WSAGetLastError(), 
							"SendFrame", "sendto");
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, ErrorType, IDS_ERR_SNDFRAMESIZE, 
							"SendFrame", "sendto");
	}
	return bReturn;
}
// 创建常量信息结构体
MatrixServerDll_API m_oConstVarStruct* CreateConstVar(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oConstVarStruct* pConstVar = NULL;
	pConstVar = new m_oConstVarStruct;
	pConstVar->m_pFrameHeadCheck = NULL;
	pConstVar->m_pLogOutPut = pLogOutPut;
//	AddMsgToLogOutPutList(pLogOutPut, LogType, 0, "CreateConstVar", "成功创建常量信息结构体");
	return pConstVar;
}
// 载入INI文件
MatrixServerDll_API void LoadIniFile(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	CString strSection	= _T("");
	CString strSectionKey = _T("");
	CString strValue = _T("");
	CString strFilePath	= _T("");
	wchar_t strBuff[256];
	CString csTemp = _T("");
	int iDirectionNew = 0;
	int iDirectionOld = 0;
	int iTemp = 0;
	strFilePath = INIFilePath;
	if (_taccess(strFilePath, 0) == -1)
	{
		return;
	}
//	GetFileAttributes(strFilePath);
	try
	{
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
	catch (CMemoryException* e)
	{
		// 加入错误消息
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
}
// 初始化常量信息结构体
MatrixServerDll_API void InitConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	LoadIniFile(pConstVar);
}
// 释放常量信息结构体
MatrixServerDll_API void FreeConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pConstVar->m_pFrameHeadCheck != NULL)
	{
		delete[] pConstVar->m_pFrameHeadCheck;
	}
	delete pConstVar;
}
// 创建仪器通讯信息结构体
MatrixServerDll_API m_oInstrumentCommInfoStruct* CreateInstrumentCommInfo(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	pCommInfo->m_pLogOutPut = pLogOutPut;
	return pCommInfo;
}
// 打开程序配置文件
MatrixServerDll_API BOOL OpenAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo)
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = _T("msxml2.domdocument");
	if (FALSE == pCommInfo->m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError))
	{
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, ErrorType, IDS_ERR_XMLINTERFACE, 
							"OpenAppIniXMLFile", "CreateDispatch");
		return FALSE;
	}
	oVariant = CommXMLFilePath;
	if (_taccess(CommXMLFilePath, 0) == -1)
	{
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, ErrorType, IDS_ERR_FILE_EXIST, 
							"OpenAppIniXMLFile");
		return FALSE;
	}
	return pCommInfo->m_oXMLDOMDocument.load(oVariant);
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
	try
	{
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
		pCommInfo->m_uiSrcIP = inet_addr(ConvertCStrToStr(csSrcIP).c_str());

		strKey = _T("IPLCI");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pCommInfo->m_uiDstIP = inet_addr(ConvertCStrToStr(csDstIP).c_str());
	}
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
}
//加载端口设置数据
MatrixServerDll_API void LoadPortSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	try
	{
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
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
}
//加载数据缓冲区大小数据
MatrixServerDll_API void LoadBufferSizeSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	try
	{
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
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
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
	if (TRUE == OpenAppIniXMLFile(pCommInfo))
	{
		//加载IP地址设置数据
		LoadIPSetupData(pCommInfo);
		//加载端口设置数据
		LoadPortSetupData(pCommInfo);
		//加载数据缓冲区大小数据
		LoadBufferSizeSetupData(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppIniXMLFile(pCommInfo);
}
// 初始化仪器通讯信息结构体
MatrixServerDll_API void InitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	LoadLineServerAppSetupData(pCommInfo);
}
// 释放仪器通讯信息结构体
MatrixServerDll_API void FreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	delete pCommInfo;
}
// 创建心跳帧信息结构体
MatrixServerDll_API m_oHeartBeatFrameStruct* CreateInstrumentHeartBeat(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeartBeatFrameStruct* pHeartBeatFrame = NULL;
	pHeartBeatFrame = new m_oHeartBeatFrameStruct;
	InitializeCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_pbySndFrameData = NULL;
	pHeartBeatFrame->m_pbyCommandWord = NULL;
	pHeartBeatFrame->m_pLogOutPut = pLogOutPut;
	return pHeartBeatFrame;
}
// 初始化心跳
MatrixServerDll_API void InitInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pHeartBeatFrame == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		return;
	}
	if (pConstVar == NULL)
	{
		return;
	}
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	// 源地址
	pHeartBeatFrame->m_oCommandStruct.m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pHeartBeatFrame->m_oCommandStruct.m_uiDstIP = pCommInfo->m_uiDstIP;
	// 目标IP地址端口号
	pHeartBeatFrame->m_oCommandStruct.m_uiAimPort = pCommInfo->m_uiAimPort;
	// 心跳返回端口
	pHeartBeatFrame->m_oCommandStruct.m_usReturnPort = pCommInfo->m_usHeartBeatReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pHeartBeatFrame->m_oCommandStruct.m_usCommand = pConstVar->m_oSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pHeartBeatFrame->m_oCommandStruct);
	// 清空发送帧缓冲区
	pHeartBeatFrame->m_pbySndFrameData = new byte[pConstVar->m_oSndFrameSize];
	memset(pHeartBeatFrame->m_pbySndFrameData, pConstVar->m_oSndFrameBufInit, pConstVar->m_oSndFrameSize);
	// 清空心跳命令字集合
	pHeartBeatFrame->m_pbyCommandWord = new byte[pConstVar->m_oCommandWordMaxNum];
	memset(pHeartBeatFrame->m_pbyCommandWord, pConstVar->m_oSndFrameBufInit, pConstVar->m_oCommandWordMaxNum);
	// 心跳命令字个数
	pHeartBeatFrame->m_usCommandWordNum = 0;
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 释放心跳帧信息结构体
MatrixServerDll_API void FreeInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	if (pHeartBeatFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	if (pHeartBeatFrame->m_pbySndFrameData != NULL)
	{
		delete[] pHeartBeatFrame->m_pbySndFrameData;
	}
	if (pHeartBeatFrame->m_pbyCommandWord != NULL)
	{
		delete[] pHeartBeatFrame->m_pbyCommandWord;
	}
	delete pHeartBeatFrame;
}
// 创建首包帧信息结构体
MatrixServerDll_API m_oHeadFrameStruct* CreateInstrumentHeadFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeadFrameStruct* pHeadFrame = NULL;
	pHeadFrame = new m_oHeadFrameStruct;
	InitializeCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	pHeadFrame->m_pbyRcvFrameData = NULL;
	pHeadFrame->m_pLogOutPut = pLogOutPut;
	return pHeadFrame;
}
// 初始化首包
MatrixServerDll_API void InitInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		return;
	}
	if (pConstVar == NULL)
	{
		return;
	}
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	// 源地址
	pHeadFrame->m_oCommandStruct.m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pHeadFrame->m_oCommandStruct.m_uiDstIP = pCommInfo->m_uiDstIP;
	// 首包接收缓冲区帧数设定为仪器个数
	pHeadFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_oRcvFrameSize;
	// 接收端口
	pHeadFrame->m_oCommandStruct.m_usReturnPort = pCommInfo->m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pHeadFrame->m_oCommandStruct);
	// 清空接收帧缓冲区
	pHeadFrame->m_pbyRcvFrameData = new byte[pConstVar->m_oRcvFrameSize];
	memset(pHeadFrame->m_pbyRcvFrameData, pConstVar->m_oSndFrameBufInit, pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 释放首包帧信息结构体
MatrixServerDll_API void FreeInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_pbyRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_pbyRcvFrameData;
	}
	delete pHeadFrame;
}
// 创建IP地址设置帧信息结构体
MatrixServerDll_API m_oIPSetFrameStruct* CreateInstrumentIPSetFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oIPSetFrameStruct* pIPSetFrame = NULL;
	pIPSetFrame = new m_oIPSetFrameStruct;
	InitializeCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	pIPSetFrame->m_pbySndFrameData = NULL;
	pIPSetFrame->m_pbyCommandWord = NULL;
	pIPSetFrame->m_pbyRcvFrameData = NULL;
	pIPSetFrame->m_pLogOutPut = pLogOutPut;
	return pIPSetFrame;
}
// 初始化IP地址设置
MatrixServerDll_API void InitInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		return;
	}
	if (pConstVar == NULL)
	{
		return;
	}
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 源地址
	pIPSetFrame->m_oCommandStructSet.m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pIPSetFrame->m_oCommandStructSet.m_uiDstIP = pCommInfo->m_uiDstIP;
	// 目标IP地址端口号
	pIPSetFrame->m_oCommandStructSet.m_uiAimPort = pCommInfo->m_uiAimPort;
	// IP地址设置发送缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_oSndFrameSize;
	// IP地址设置应答接收缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_oRcvFrameSize;
	// IP地址设置返回端口
	pIPSetFrame->m_oCommandStructSet.m_usReturnPort = pCommInfo->m_usIPSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pIPSetFrame->m_oCommandStructSet.m_usCommand = pConstVar->m_oSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pIPSetFrame->m_oCommandStructSet);
	// 清空发送帧缓冲区
	pIPSetFrame->m_pbySndFrameData = new byte[pConstVar->m_oSndFrameSize];
	memset(pIPSetFrame->m_pbySndFrameData, pConstVar->m_oSndFrameBufInit, pConstVar->m_oSndFrameSize);
	// 清空IP地址设置命令字集合
	pIPSetFrame->m_pbyCommandWord = new byte[pConstVar->m_oCommandWordMaxNum];
	memset(pIPSetFrame->m_pbyCommandWord, pConstVar->m_oSndFrameBufInit, pConstVar->m_oCommandWordMaxNum);
	// IP地址设置命令字个数
	pIPSetFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	ResetInstrumentFramePacket(&pIPSetFrame->m_oCommandStructReturn);
	// 清空接收帧缓冲区
	pIPSetFrame->m_pbyRcvFrameData = new byte[pConstVar->m_oRcvFrameSize];
	memset(pIPSetFrame->m_pbyRcvFrameData, pConstVar->m_oSndFrameBufInit, pConstVar->m_oRcvFrameSize);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 释放IP地址设置帧信息结构体
MatrixServerDll_API void FreeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	if (pIPSetFrame->m_pbySndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pbySndFrameData;
	}
	if (pIPSetFrame->m_pbyCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_pbyCommandWord;
	}
	if (pIPSetFrame->m_pbyRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pbyRcvFrameData;
	}
	delete pIPSetFrame;
}
// 创建线程处理标志位信息结构体
MatrixServerDll_API m_oThreadProcFlagStruct* CreateThreadProcFlag()
{
	m_oThreadProcFlagStruct* pThreadProcFlag = NULL;
	pThreadProcFlag = new m_oThreadProcFlagStruct;
	InitializeCriticalSection(&pThreadProcFlag->m_oSecFlag);
	return pThreadProcFlag;
}
// 初始化线程处理标志位
MatrixServerDll_API void InitThreadProcFlag(m_oThreadProcFlagStruct* pThreadProcFlag)
{
	if (pThreadProcFlag == NULL)
	{
		return;
	}
	EnterCriticalSection(&pThreadProcFlag->m_oSecFlag);
	// 心跳处理线程停止标志位
	pThreadProcFlag->m_bProcHeartBeatStop = false;
	// 首包处理线程停止标志位
	pThreadProcFlag->m_bProcHeadFrameStop = false;
	// 仪器IP地址设置线程停止标志位
	pThreadProcFlag->m_bProcIPSetStop = false;
	LeaveCriticalSection(&pThreadProcFlag->m_oSecFlag);
}
// 释放线程处理标志位信息结构体
MatrixServerDll_API void FreeThreadProcFlag(m_oThreadProcFlagStruct* pThreadProcFlag)
{
	if (pThreadProcFlag == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pThreadProcFlag->m_oSecFlag);
	delete pThreadProcFlag;
}
// 初始化套接字库
MatrixServerDll_API void InitSocketLib()
{
	WSADATA data;									// 定义结构体变量
	WORD w = MAKEWORD(2, 2);			// 初始化套接字版本号
	int err = WSAStartup(w, &data);							// 初始化套接字库
	if (err == 0)
	{
		/*m_oLogFile.OnWriteLogFile(_T("CMatrixTestSoftDlg::OnInitDialog"), _T("初始化套接字库成功！"), SuccessStatus);*/
	}
	else
	{
		WSAGetLastError();
// 		str.Format(_T("初始化套接字库失败，错误码为%d！"), err);
// 		m_oLogFile.OnWriteLogFile(_T("CMatrixTestSoftDlg::OnInitDialog"), str, ErrorStatus);
	}
}
// 释放套接字库
MatrixServerDll_API void FreeSocketLib()
{
	// 释放套接字库
	int err = WSACleanup();	
	if (err == 0)
	{
//		m_oLogFile.OnWriteLogFile(_T("CMatrixTestSoftDlg::OnClose"), _T("释放套接字库成功！"), SuccessStatus);
	}
	else
	{
		WSAGetLastError();
// 		CString str = _T("");
// 		str.Format(_T("释放套接字库失败，错误码为%d！"), err);
// 		m_oLogFile.OnWriteLogFile(_T("CMatrixTestSoftDlg::OnClose"), str, ErrorStatus);
	}
}
// 创建并设置心跳端口
MatrixServerDll_API void CreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	// 创建套接字
	pHeartBeatFrame->m_oHeartBeatSocket = CreateInstrumentSocket(pHeartBeatFrame->m_oCommandStruct.m_usReturnPort, 
																pHeartBeatFrame->m_oCommandStruct.m_uiSrcIP,
																pHeartBeatFrame->m_pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pHeartBeatFrame->m_oHeartBeatSocket, pHeartBeatFrame->m_pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 创建并设置首包端口
MatrixServerDll_API void CreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame)
{
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	// 创建套接字
	pHeadFrame->m_oHeadFrameSocket = CreateInstrumentSocket(pHeadFrame->m_oCommandStruct.m_usReturnPort, 
															pHeadFrame->m_oCommandStruct.m_uiSrcIP,
															pHeadFrame->m_pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pHeadFrame->m_oHeadFrameSocket, pHeadFrame->m_uiRcvBufferSize, pHeadFrame->m_pLogOutPut);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 创建并设置IP地址设置端口
MatrixServerDll_API void CreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 创建套接字
	pIPSetFrame->m_oIPSetFrameSocket = CreateInstrumentSocket(pIPSetFrame->m_oCommandStructSet.m_usReturnPort, 
																pIPSetFrame->m_oCommandStructSet.m_uiSrcIP,
																pIPSetFrame->m_pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiSndBufferSize, pIPSetFrame->m_pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiRcvBufferSize, pIPSetFrame->m_pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 初始化实例
MatrixServerDll_API void InitInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 创建日志输出结构体
	pEnv->m_pLogOutPut = CreateLogOutPut();
	// 创建常量信息结构体
	pEnv->m_pConstVar = CreateConstVar(pEnv->m_pLogOutPut);
	// 创建仪器通讯信息结构体
	pEnv->m_pInstrumentCommInfo = CreateInstrumentCommInfo(pEnv->m_pLogOutPut);
	// 创建心跳帧信息结构体
	pEnv->m_pHeartBeatFrame = CreateInstrumentHeartBeat(pEnv->m_pLogOutPut);
	// 创建首包帧信息结构体
	pEnv->m_pHeadFrame = CreateInstrumentHeadFrame(pEnv->m_pLogOutPut);
	// 创建IP地址设置帧信息结构体
	pEnv->m_pIPSetFrame = CreateInstrumentIPSetFrame(pEnv->m_pLogOutPut);
	// 创建线程处理标志位信息结构体
	pEnv->m_pThreadProcFlag = CreateThreadProcFlag();

	// 初始化日志输出结构体
	InitLogOutPut(pEnv->m_pLogOutPut);
	// 初始化套接字库
	InitSocketLib();
	// 初始化常量信息结构体
	InitConstVar(pEnv->m_pConstVar);
	// 初始化仪器通讯信息结构体
	InitInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	// 初始化心跳
	InitInstrumentHeartBeat(pEnv->m_pHeartBeatFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化首包
	InitInstrumentHeadFrame(pEnv->m_pHeadFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化IP地址设置
	InitInstrumentIPSetFrame(pEnv->m_pIPSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化线程处理标志位
	InitThreadProcFlag(pEnv->m_pThreadProcFlag);
	// 创建并设置心跳端口
	CreateAndSetHeartBeatSocket(pEnv->m_pHeartBeatFrame);
	// 创建并设置首包端口
	CreateAndSetHeadFrameSocket(pEnv->m_pHeadFrame);
	// 创建并设置IP地址设置端口
	CreateAndSetIPSetFrameSocket(pEnv->m_pIPSetFrame);
}
// 释放实例资源
MatrixServerDll_API void FreeInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 释放日志输出结构体
	FreeLogOutPut(pEnv->m_pLogOutPut);
	FreeConstVar(pEnv->m_pConstVar);
	FreeInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	FreeInstrumentHeartBeat(pEnv->m_pHeartBeatFrame);
	FreeInstrumentHeadFrame(pEnv->m_pHeadFrame);
	FreeInstrumentIPSetFrame(pEnv->m_pIPSetFrame);
	FreeThreadProcFlag(pEnv->m_pThreadProcFlag);
	delete pEnv;
	pEnv = NULL;
	// 释放套接字库
	FreeSocketLib();
}
// 解析首包帧
MatrixServerDll_API bool ParseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	bReturn = ParseInstrumentFrame(&pHeadFrame->m_oCommandStruct, pHeadFrame->m_pbyRcvFrameData, pConstVar);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	return bReturn;
}
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrumentIPSetReturnFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	bReturn = ParseInstrumentFrame(&pIPSetFrame->m_oCommandStructReturn, pIPSetFrame->m_pbyRcvFrameData, pConstVar);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	return bReturn;
}
// 生成心跳帧
MatrixServerDll_API void MakeInstrumentHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	MakeInstrumentFrame(&pHeartBeatFrame->m_oCommandStruct,  pConstVar, pHeartBeatFrame->m_pbySndFrameData,
						pHeartBeatFrame->m_pbyCommandWord, pHeartBeatFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	MakeInstrumentFrame(&pIPSetFrame->m_oCommandStructSet, pConstVar, pIPSetFrame->m_pbySndFrameData, 
						pIPSetFrame->m_pbyCommandWord, pIPSetFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}