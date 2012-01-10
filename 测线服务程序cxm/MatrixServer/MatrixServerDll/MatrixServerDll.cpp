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
MatrixServerDll_API m_oEnvironmentStruct* OnCreateInstance(void)
{
	m_oEnvironmentStruct* pEnv = new m_oEnvironmentStruct;
	pEnv->m_pConstVar = NULL;
	pEnv->m_pInstrumentCommInfo = NULL;
	pEnv->m_pHeartBeatFrame = NULL;
	pEnv->m_pHeadFrame = NULL;
	pEnv->m_pIPSetFrame = NULL;
	pEnv->m_pTailFrame = NULL;
	pEnv->m_pTailTimeFrame = NULL;
	pEnv->m_pLogOutPut = NULL;
	pEnv->m_pInstrumentList = NULL;
	pEnv->m_pRoutList = NULL;
	pEnv->m_pLogOutPutThread = NULL;
	pEnv->m_pHeartBeatThread = NULL;
	pEnv->m_pHeadFrameThread = NULL;
	pEnv->m_pIPSetFrameThread = NULL;
	pEnv->m_pTailFrameThread = NULL;
	pEnv->m_pMonitorRoutThread = NULL;
	return pEnv;
}
// 创建日志输出结构体
MatrixServerDll_API m_oLogOutPutStruct* OnCreateLogOutPut()
{
	m_oLogOutPutStruct* pLogOutPut = NULL;
	pLogOutPut = new m_oLogOutPutStruct;
	InitializeCriticalSection(&pLogOutPut->m_oSecLogFile);
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
												string pFuncName = "", 
												string pVarName = "", 
												byte byErrorType = LogType, 
												unsigned int uiErrorCode = 0)
{
	CString cstrTime = _T("");
	CString cstrErrorCode = _T("");
	CString strOutPut = _T("");
	string str = "";
	string strTemp = "";
	SYSTEMTIME  sysTime;
	if (pLogOutPut == NULL)
	{
		return;
	}
	if (OutPutSelect == 1)
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
	cstrTime.Format(_T("%04d年%02d月%02d日 %02d:%02d:%02d:%03d\t"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
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
		cstrErrorCode.Format(_T("ErrorCode = %d\t"), uiErrorCode);
		strOutPut += cstrErrorCode;
	}
	else if (byErrorType == ExpandType)
	{
		strOutPut += _T("ExpandType\t");
	}
	strOutPut += cstrTime;
	str = ConvertCStrToStr(strOutPut);
	if (pFuncName.empty() == false)
	{
		strTemp = "程序运行到函数：";
		str += strTemp;
		str += pFuncName;
		str += '\t';
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
	fwrite(str.c_str(), sizeof(char), strSize, pFile);
	fflush(pFile);
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
MatrixServerDll_API void OnInitLogOutPut(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	// 日志文件指针为空
	pLogOutPut->m_pFile = NULL;
	// 日志错误计数清零
	pLogOutPut->m_uiErrorCount = 0;
	// 清空日志输出队列
	pLogOutPut->m_oLogOutputList.clear();
	// 打开日志输出文件
	OpenLogOutPutFile(pLogOutPut);
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 关闭日志输出结构体
MatrixServerDll_API void OnCloseLogOutPut(m_oLogOutPutStruct* pLogOutPut)
{
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	// 关闭日志输出文件
	CloseLogOutPutFile(pLogOutPut);
	// 清空日志输出队列
	pLogOutPut->m_oLogOutputList.clear();
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 释放日志输出结构体
MatrixServerDll_API void OnFreeLogOutPut(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pLogOutPut->m_oSecLogFile);
	delete pLogOutPut;
}
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrumentFrameHead(char* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "CheckInstrumentFrameHead", "pFrameData", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	// @@@@@@@@同步头目前只有前4位有效，其余12位可能用于它用
	for (int i=0; i<4; i++)
	{
		if (pFrameData[i] != pConstVar->m_pFrameHeadCheck[i])
		{
			AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "CheckInstrumentFrameHead", "",
								ErrorType, IDS_ERR_CHECKFRAMEHEAD);
			return false;
		}
	}
	return true;
}
// 生成帧的同步码
MatrixServerDll_API bool MakeInstrumentFrameHead(char* pFrameData, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentFrameHead", "pFrameData", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	memcpy(pFrameData, pConstVar->m_pFrameHeadCheck, pConstVar->m_iFrameHeadSize);
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
	pCommand->m_uiSetBroadCastPort = 0;
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
	pCommand->m_uiBroadCastPortSet = 0;
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
	// 仪器类型
	pCommand->m_uiInstrumentType = 0;
	
	return true;
}
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommandStruct* pCommand, char* pFrameData,
												m_oConstVarStruct* pConstVar)
{
	if (pCommand == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentFrame", "pCommand", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "ParseInstrumentFrame", "pFrameData", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	unsigned char	byCommandWord = 0;
	int iPos = 0;
	// 校验帧的同步码
	if (false == CheckInstrumentFrameHead(pFrameData, pConstVar))
	{
		return false;
	}
	iPos += pConstVar->m_iFrameHeadSize;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pCommand);
	// 源IP地址
	memcpy(&pCommand->m_uiSrcIP, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
	iPos += pConstVar->m_iFramePacketSize4B;
	// 目标IP地址
	memcpy(&pCommand->m_uiDstIP, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
	iPos += pConstVar->m_iFramePacketSize4B;
	// 目标端口号
	memcpy(&pCommand->m_usReturnPort, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
	iPos += pConstVar->m_iFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pCommand->m_usCommand, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
	iPos += pConstVar->m_iFramePacketSize2B;
	// 解析帧内容
	unsigned int uiRoutAddrNum = 0;
	while(true)
	{
		memcpy(&byCommandWord, &pFrameData[iPos], pConstVar->m_iFrameCmdSize1B);
		iPos += pConstVar->m_iFrameCmdSize1B;
		if (byCommandWord == pConstVar->m_byCmdSn)
		{
			memcpy(&pCommand->m_uiSN, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdHeadFrameTime)
		{
			memcpy(&pCommand->m_uiTimeHeadFrame, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalIPAddr)
		{
			memcpy(&pCommand->m_uiInstrumentIP, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalSysTime)
		{
			memcpy(&pCommand->m_uiSysTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalTimeFixedHigh)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedHigh, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalTimeFixedLow)
		{
			memcpy(&pCommand->m_usLocalTimeFixedLow, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnAddr)
		{
			memcpy(&pCommand->m_uiADCDataReturnAddr, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnPort)
		{
			memcpy(&pCommand->m_usADCDataReturnPort, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnCmd, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnPortLimit)
		{
			memcpy(&pCommand->m_usADCDataReturnPortLimitLow, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnPortLimitHigh, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdSetBroadCastPort)
		{
			memcpy(&pCommand->m_uiSetBroadCastPort, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdFDUErrorCode)
		{
			memcpy(&pCommand->m_byFDUErrorCodeDataCount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byFDUErrorCodeCmdCount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byTimeStatus, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byCtrlStatus, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTBHigh)
		{
			memcpy(&pCommand->m_uiTBHigh, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTbLow)
		{
			memcpy(&pCommand->m_usTBLow, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usTBCtrl, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXRoutOpenQuery)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenQuery, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXRoutOpenSet)
		{
			memcpy(&pCommand->m_byLAUXRoutOpenSet, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTailRecSndTimeLow)
		{
			memcpy(&pCommand->m_usTailRecSndTimeLow, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_uiBroadCastPortSet, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCSet)
		{
			memcpy(&pCommand->m_pcADCSet[pCommand->m_usADCSetNum], &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
			pCommand->m_usADCSetNum += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXErrorCode1)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineACount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLineBCount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineACount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXErrorCode2)
		{
			memcpy(&pCommand->m_byLAUXErrorCodeCmdCount, &pFrameData[iPos], pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXSetRout)
		{
			if (uiRoutAddrNum == 0)
			{
				memcpy(&pCommand->m_uiRoutIPRight, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 1)
			{
				memcpy(&pCommand->m_uiRoutIPLeft, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 2)
			{
				memcpy(&pCommand->m_uiRoutIPTop, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 3)
			{
				memcpy(&pCommand->m_uiRoutIPDown, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			uiRoutAddrNum++;
		}
		else if (byCommandWord == pConstVar->m_byCmdReturnRout)
		{
			memcpy(&pCommand->m_uiRoutIP, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdEnd)
		{
			break;
		}
		else
		{
			iPos += pConstVar->m_iFramePacketSize4B;
		}
	}
	return true;
}
// 生成与设备通讯帧
MatrixServerDll_API bool MakeInstrumentFrame(m_oInstrumentCommandStruct* pCommand, m_oConstVarStruct* pConstVar,
												char* pFrameData, char* pCommandWord, unsigned short usCommandWordNum)
{
	if (pConstVar == NULL)
	{
		return false;
	}
	if (pCommand == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentFrame", "pCommand", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	if (pFrameData == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentFrame", "pFrameData", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	if (pCommandWord == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentFrame", "pCommandWord", 
							ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	int iPos = 0;
	unsigned int uiRoutAddrNum = 0;
	// 生成帧的同步码
	MakeInstrumentFrameHead(pFrameData, pConstVar);
	iPos += pConstVar->m_iFrameHeadSize;
	// 源IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiSrcIP, pConstVar->m_iFramePacketSize4B);
	iPos += pConstVar->m_iFramePacketSize4B;
	// 目标IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiDstIP, pConstVar->m_iFramePacketSize4B);
	iPos += pConstVar->m_iFramePacketSize4B;
	// 目标端口号
	memcpy(&pFrameData[iPos], &pCommand->m_usReturnPort, pConstVar->m_iFramePacketSize2B);
	iPos += pConstVar->m_iFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pFrameData[iPos], &pCommand->m_usCommand, pConstVar->m_iFramePacketSize2B);
	iPos += pConstVar->m_iFramePacketSize2B;
	// 包时间
	iPos += pConstVar->m_iFramePacketSize4B;

	// 生成帧内容
	for (unsigned short i=0; i<usCommandWordNum; i++)
	{
		memcpy(&pFrameData[iPos], &pCommandWord[i], pConstVar->m_iFrameCmdSize1B);
		iPos += pConstVar->m_iFrameCmdSize1B;
		if (pCommandWord[i] == pConstVar->m_byCmdSn)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSN, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdHeadFrameTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTimeHeadFrame, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalIPAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiInstrumentIP, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalSysTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSysTime, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalTimeFixedHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedHigh, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalTimeFixedLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLocalTimeFixedLow, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiADCDataReturnAddr, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPort, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnCmd, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnPortLimit)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitLow, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitHigh, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdSetBroadCastPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSetBroadCastPort, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdFDUErrorCode)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeDataCount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byFDUErrorCodeCmdCount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byTimeStatus, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byCtrlStatus, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTBHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTBHigh, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTbLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTBLow, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usTBCtrl, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXRoutOpenQuery)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenQuery, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXRoutOpenSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXRoutOpenSet, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTailRecSndTimeLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTailRecSndTimeLow, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiBroadCastPortSet, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCSet)
		{
			for(unsigned short j=0; j<pCommand->m_usADCSetNum; j+=pConstVar->m_iFramePacketSize4B)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_pcADCSet[j], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdNetTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiNetTime, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLineTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineA, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineB, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineA, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineB, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXErrorCode1)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineACount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLineBCount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineACount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeDataLAUXLineBCount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXErrorCode2)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_byLAUXErrorCodeCmdCount, pConstVar->m_iFramePacketSize1B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXSetRout)
		{
			if (uiRoutAddrNum == 0)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPRight, pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 1)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPLeft, pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 2)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPTop, pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 3)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPDown, pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			uiRoutAddrNum++;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdReturnRout)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIP, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else
		{
			memset(&pFrameData[iPos], pConstVar->m_cSndFrameBufInit, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
	}
	pFrameData[iPos] = pConstVar->m_byCmdEnd;
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
		AddMsgToLogOutPutList(pLogOutPut, "CreateInstrumentSocket", "bind", 
							ErrorType, WSAGetLastError());
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
		AddMsgToLogOutPutList(pLogOutPut, "SetInstrumentSocketBroadCast", "setsockopt", 
							ErrorType, WSAGetLastError());
	}
}
// 设置端口接收缓冲区接收帧数量
MatrixServerDll_API void SetRcvBufferSize(SOCKET oSocket, int iRcvBufferSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_RCVBUF,  
								reinterpret_cast<const char *>(&iRcvBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, "SetRcvBufferSize", "setsockopt", 
							ErrorType, WSAGetLastError());
	}
}
// 设置端口发送缓冲区发送帧数量
MatrixServerDll_API void SetSndBufferSize(SOCKET oSocket, int iSndBufferSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_SNDBUF,  
								reinterpret_cast<const char *>(&iSndBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, "SetSndBufferSize", "setsockopt", 
							ErrorType, WSAGetLastError());
	}
}
// 得到网络接收缓冲区收到的帧数量
MatrixServerDll_API DWORD GetFrameCount(SOCKET oSocket, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	DWORD dwFrameCount = 0;
	// 得到网络接收缓冲区数据字节数
	if (SOCKET_ERROR == ioctlsocket(oSocket, FIONREAD, &dwFrameCount))
	{
		AddMsgToLogOutPutList(pLogOutPut, "GetFrameCount", "ioctlsocket", 
							ErrorType, WSAGetLastError());
	}
	else
	{
		// 得到帧数量
		dwFrameCount = dwFrameCount / iRcvFrameSize;
	}
	return dwFrameCount;
}
// 得到帧数据
MatrixServerDll_API bool GetFrameData(SOCKET oSocket, char* pFrameData, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	bool bReturn = false;
	sockaddr_in AddrFrom;
	int iSize = sizeof(AddrFrom);
	int iCount = recvfrom(oSocket, pFrameData, iRcvFrameSize, 0, reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
	if(iCount == iRcvFrameSize)	//帧大小正确
	{
		bReturn = true;
	}
	else if (iCount == SOCKET_ERROR)
	{
		AddMsgToLogOutPutList(pLogOutPut, "GetFrameData", "recvfrom", 
							ErrorType, WSAGetLastError());
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, "GetFrameData", "recvfrom", 
							ErrorType, IDS_ERR_RCVFRAMESIZE);
	}
	return bReturn;
}
// 发送帧
MatrixServerDll_API bool SendFrame(SOCKET oSocket, char* pFrameData,int iSndFrameSize, 
								unsigned short usPort, unsigned int uiIP, 
								m_oLogOutPutStruct* pLogOutPut = NULL)
{
	bool bReturn = false;
	// 发送帧
	sockaddr_in addrSend;
	addrSend.sin_family = AF_INET;											// 填充套接字地址结构
	addrSend.sin_port = htons(usPort);
	addrSend.sin_addr.S_un.S_addr = uiIP;
	int iCount = sendto(oSocket, pFrameData, iSndFrameSize, 0, 
					reinterpret_cast<sockaddr*>(&addrSend), sizeof(addrSend));
	if (iCount == iSndFrameSize)
	{
		bReturn = true;
	} 
	else if (iCount == SOCKET_ERROR)
	{
		AddMsgToLogOutPutList(pLogOutPut, "SendFrame", "sendto", ErrorType, WSAGetLastError());
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, "SendFrame", "sendto", ErrorType, IDS_ERR_SNDFRAMESIZE);
	}
	return bReturn;
}
// 创建常量信息结构体
MatrixServerDll_API m_oConstVarStruct* OnCreateConstVar(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oConstVarStruct* pConstVar = NULL;
	pConstVar = new m_oConstVarStruct;
	pConstVar->m_pFrameHeadCheck = NULL;
	pConstVar->m_pLogOutPut = pLogOutPut;
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
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "LoadIniFile", "", 
							ErrorType, IDS_ERR_FILE_EXIST);
		return;
	}
//	GetFileAttributes(strFilePath);
	try
	{
		//读取ini文件中相应字段的内容
		strSection = _T("常量设置");					// 获取当前区域

		strSectionKey=_T("OneSleepTime");				// 一次休眠的时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iOneSleepTime = _ttoi(strValue);

		strSectionKey=_T("LogOutPutSleepTimes");		// 日志输出线程写日志的延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iLogOutPutSleepTimes = _ttoi(strValue);

		strSectionKey=_T("HertBeatSleepTimes");			// 心跳线程发送心跳帧延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iHeartBeatSleepTimes = _ttoi(strValue);

		strSectionKey=_T("HeadFrameSleepTimes");		// 首包线程接收首包延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iHeadFrameSleepTimes = _ttoi(strValue);

		strSectionKey=_T("IPSetFrameSleepTimes");		// IP地址设置线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iIPSetFrameSleepTimes = _ttoi(strValue);

		strSectionKey=_T("TailFrameSleepTimes");		// 尾包线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iTailFrameSleepTimes = _ttoi(strValue);

		strSectionKey=_T("MonitorRoutSleepTimes");		// 路由监视线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iMonitorRoutSleepTimes = _ttoi(strValue);

		strSectionKey=_T("CloseThreadSleepTimes ");		// 等待线程关闭的延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iCloseThreadSleepTimes = _ttoi(strValue);

		strSectionKey=_T("HeadFrameStableNum");			// 首包稳定计数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iHeadFrameStableTimes = _ttoi(strValue);

		strSectionKey=_T("IPAddrResetTimes");		// IP地址重设次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iIPAddrResetTimes = _ttoi(strValue);

		strSectionKey=_T("TailFrameStableTimes");		// 尾包稳定计数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iTailFrameStableTimes = _ttoi(strValue);

		strSectionKey=_T("MonitorRoutStableTime");		// 路由监视稳定时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iMonitorRoutStableTime = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLAUX");		// 仪器类型-交叉站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_byInstrumentTypeLAUX = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLAUL");		// 仪器类型-电源站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_byInstrumentTypeLAUL = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeFDU");		// 仪器类型-采集站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_byInstrumentTypeFDU = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLCI");		// 仪器类型-LCI
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_byInstrumentTypeLCI= _ttoi(strValue);

		strSectionKey=_T("DirectionTop");			// 方向上方
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iDirectionTop = _ttoi(strValue);

		strSectionKey=_T("DirectionDown");			// 方向下方
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iDirectionDown = _ttoi(strValue);

		strSectionKey=_T("DirectionLeft");			// 方向左方
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iDirectionLeft = _ttoi(strValue);

		strSectionKey=_T("DirectionRight");			// 方向右方
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iDirectionRight = _ttoi(strValue);

		strSectionKey=_T("DirectionCenter");		// 方向正中
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iDirectionCenter = _ttoi(strValue);

		strSectionKey=_T("IPSetAddrStart");			// IP地址设置的起始地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iIPSetAddrStart = _ttoi(strValue);

		strSectionKey=_T("IPSetAddrInterval");		// IP地址设置的间隔
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iIPSetAddrInterval = _ttoi(strValue);

		strSectionKey=_T("RoutSetAddrStart");		// 路由地址设置的起始地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iRoutSetAddrStart = _ttoi(strValue);

		strSectionKey=_T("RoutSetAddrInterval");	// 路由地址设置的间隔
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iRoutSetAddrInterval = _ttoi(strValue);

		strSectionKey=_T("BroadcastPortStart");		// 设置广播端口起始地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_iBroadcastPortStart, sizeof(int));

		strSectionKey=_T("IPBroadcastAddr");		// 设置为广播IP
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_iIPBroadcastAddr, sizeof(int));

		//读取ini文件中相应字段的内容
		strSection = _T("帧格式设置");			// 获取当前区域
		strSectionKey=_T("FrameHeadSize");		// 帧头长度
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iFrameHeadSize = _ttoi(strValue);

		strSectionKey=_T("FrameHeadCheck");		// 同步帧头
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		if (pConstVar->m_pFrameHeadCheck != NULL)
		{
			delete[] pConstVar->m_pFrameHeadCheck;
		}
		pConstVar->m_pFrameHeadCheck = new char[pConstVar->m_iFrameHeadSize];
		for (int i=0; i<pConstVar->m_iFrameHeadSize; i++)
		{
			iDirectionNew = strValue.Find(_T("0x"), iDirectionOld);
			iDirectionNew += 2;
			csTemp = strValue.Mid(iDirectionNew, 2);
			_stscanf_s(csTemp, _T("%x"), &iTemp, sizeof(int));
			pConstVar->m_pFrameHeadCheck[i] = iTemp;
			iDirectionOld = iDirectionNew;
		}
		strSectionKey=_T("FrameCmdSize1B");		// 命令字长度1字节
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iFrameCmdSize1B = _ttoi(strValue);

		strSectionKey=_T("FramePacketSize1B");	// 命令包长度1字节
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iFramePacketSize1B = _ttoi(strValue);

		strSectionKey=_T("FramePacketSize2B");	// 命令包长度2字节
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iFramePacketSize2B = _ttoi(strValue);

		strSectionKey=_T("FramePacketSize4B");	// 命令包长度4字节
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iFramePacketSize4B = _ttoi(strValue);

		strSectionKey=_T("ADCDataSize3B");		// ADC数据所占字节数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCDataSize3B = _ttoi(strValue);

		strSectionKey=_T("CommandWordMaxNum");	// 命令字个数最大值
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iCommandWordMaxNum = _ttoi(strValue);
		pConstVar->m_iADCSetCommandMaxByte = pConstVar->m_iCommandWordMaxNum * pConstVar->m_iFramePacketSize4B;
		strSectionKey=_T("SndFrameBufInit");	// 发送帧缓冲区初值设定
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cSndFrameBufInit = iTemp;

		strSectionKey=_T("RcvFrameSize");		// 接收的网络数据帧帧长度
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iRcvFrameSize = _ttoi(strValue);

		strSectionKey=_T("SndFrameSize");		// 发送的网络数据帧帧长度
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSndFrameSize = _ttoi(strValue);

		/////////////////////////////////////////////////////////////////////////
		strSection = _T("服务器与设备命令字设置");		// 获取当前区域
		strSectionKey=_T("SendSetCmd");			// 发送设置命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usSendSetCmd = iTemp;

		strSectionKey=_T("SendQueryCmd");		// 发送查询命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usSendQueryCmd = iTemp;

		strSectionKey=_T("SendADCCmd");			// 发送ADC采样数据重发命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usSendADCCmd = iTemp;

		strSectionKey=_T("CmdTBCtrl");			// TB开始采集开关控制命令(TB_L高8位)
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_uiCmdTBCtrl, sizeof(unsigned int));

		strSectionKey=_T("CmdSn");				// 串号
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdSn = iTemp;

		strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdHeadFrameTime = iTemp;

		strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_byCmdLocalIPAddr, sizeof(char));

		strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalSysTime = iTemp;

		strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalTimeFixedHigh = iTemp;

		strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalTimeFixedLow = iTemp;

		strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnAddr = iTemp;

		strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnPort = iTemp;

		strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnPortLimit = iTemp;

		strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdSetBroadCastPort = iTemp;

		strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdFDUErrorCode = iTemp;

		strSectionKey=_T("CmdTBHigh");				// TB时刻高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTBHigh = iTemp;

		strSectionKey=_T("CmdTbLow");				// TB时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTbLow = iTemp;

		strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXRoutOpenQuery = iTemp;

		strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXRoutOpenSet = iTemp;

		strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTailRecSndTimeLow = iTemp;

		strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdBroadCastPortSet = iTemp;

		strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCSet = iTemp;

		strSectionKey=_T("CmdNetTime");				// 网络时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdNetTime = iTemp;

		strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLineTailRecTimeLAUX = iTemp;

		strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUTailRecTimeLAUX = iTemp;

		strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXErrorCode1 = iTemp;

		strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXErrorCode2 = iTemp;

		strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXSetRout = iTemp;

		strSectionKey=_T("CmdReturnRout");			// 返回路由
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdReturnRout = iTemp;

		strSectionKey=_T("CmdEnd");					// 命令解析结束命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdEnd = iTemp;
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
MatrixServerDll_API void OnInitConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	LoadIniFile(pConstVar);
}
// 关闭常量信息结构体
MatrixServerDll_API void OnCloseConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar->m_pFrameHeadCheck != NULL)
	{
		delete[] pConstVar->m_pFrameHeadCheck;
	}
}
// 释放常量信息结构体
MatrixServerDll_API void OnFreeConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	delete pConstVar;
}
// 创建仪器通讯信息结构体
MatrixServerDll_API m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(m_oLogOutPutStruct* pLogOutPut = NULL)
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
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, "OpenAppIniXMLFile", "CreateDispatch",
							ErrorType, IDS_ERR_XMLINTERFACE);
		return FALSE;
	}
	oVariant = CommXMLFilePath;
	if (_taccess(CommXMLFilePath, 0) == -1)
	{
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, "OpenAppIniXMLFile", "",
							ErrorType, IDS_ERR_FILE_EXIST);
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
		pCommInfo->m_uiAimIP = inet_addr(ConvertCStrToStr(csDstIP).c_str());
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
		strKey = _T("PortForTailFrame");
		pCommInfo->m_usTailFramePort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = _T("PortForTailTimeFrame");
		pCommInfo->m_usTailTimeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
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
MatrixServerDll_API void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	LoadLineServerAppSetupData(pCommInfo);
}
// 释放仪器通讯信息结构体
MatrixServerDll_API void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	delete pCommInfo;
}
// 创建心跳帧信息结构体
MatrixServerDll_API m_oHeartBeatFrameStruct* OnCreateInstrumentHeartBeat(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeartBeatFrameStruct* pHeartBeatFrame = NULL;
	pHeartBeatFrame = new m_oHeartBeatFrameStruct;
	InitializeCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_pcSndFrameData = NULL;
	pHeartBeatFrame->m_pcCommandWord = NULL;
	pHeartBeatFrame->m_oHeartBeatSocket = INVALID_SOCKET;
	pHeartBeatFrame->m_pCommandStruct = NULL;
	pHeartBeatFrame->m_pLogOutPut = pLogOutPut;
	return pHeartBeatFrame;
}
// 初始化心跳
MatrixServerDll_API void OnWorkInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pHeartBeatFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentHeartBeat", "pHeartBeatFrame",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentHeartBeat", "pCommInfo",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	if (pHeartBeatFrame->m_pCommandStruct != NULL)
	{
		delete pHeartBeatFrame->m_pCommandStruct;
	}
	pHeartBeatFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pHeartBeatFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pHeartBeatFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pHeartBeatFrame->m_pCommandStruct->m_uiAimPort = pCommInfo->m_uiAimPort;
	// 心跳返回端口
	pHeartBeatFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_usHeartBeatReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pHeartBeatFrame->m_pCommandStruct->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pHeartBeatFrame->m_pCommandStruct);
	// 清空发送帧缓冲区
	if (pHeartBeatFrame->m_pcSndFrameData != NULL)
	{
		delete[] pHeartBeatFrame->m_pcSndFrameData;
	}
	pHeartBeatFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pHeartBeatFrame->m_pcSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空心跳命令字集合
	if (pHeartBeatFrame->m_pcCommandWord != NULL)
	{
		delete pHeartBeatFrame->m_pcCommandWord;
	}
	pHeartBeatFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pHeartBeatFrame->m_pcCommandWord, pConstVar->m_cSndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
	// 心跳命令字个数
	pHeartBeatFrame->m_usCommandWordNum = 0;
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 关闭心跳帧信息结构体
MatrixServerDll_API void OnCloseInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	if (pHeartBeatFrame->m_pcSndFrameData != NULL)
	{
		delete[] pHeartBeatFrame->m_pcSndFrameData;
	}
	if (pHeartBeatFrame->m_pcCommandWord != NULL)
	{
		delete[] pHeartBeatFrame->m_pcCommandWord;
	}
	if (pHeartBeatFrame->m_pCommandStruct != NULL)
	{
		delete pHeartBeatFrame->m_pCommandStruct;
	}
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 释放心跳帧信息结构体
MatrixServerDll_API void OnFreeInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	if (pHeartBeatFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	delete pHeartBeatFrame;
}
// 创建首包帧信息结构体
MatrixServerDll_API m_oHeadFrameStruct* OnCreateInstrumentHeadFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeadFrameStruct* pHeadFrame = NULL;
	pHeadFrame = new m_oHeadFrameStruct;
	InitializeCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	pHeadFrame->m_pcRcvFrameData = NULL;
	pHeadFrame->m_oHeadFrameSocket = INVALID_SOCKET;
	pHeadFrame->m_pCommandStruct = NULL;
	pHeadFrame->m_pLogOutPut = pLogOutPut;
	return pHeadFrame;
}
// 初始化首包
MatrixServerDll_API void OnWorkInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pHeadFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentHeadFrame", "pHeadFrame",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentHeadFrame", "pCommInfo",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_pCommandStruct != NULL)
	{
		delete pHeadFrame->m_pCommandStruct;
	}
	pHeadFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pHeadFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pHeadFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 首包接收缓冲区帧数设定为仪器个数
	pHeadFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 接收端口
	pHeadFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_usHeadFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pHeadFrame->m_pCommandStruct);
	// 清空接收帧缓冲区
	if (pHeadFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_pcRcvFrameData;
	}
	pHeadFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pHeadFrame->m_pcRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 关闭首包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_pcRcvFrameData;
	}
	if (pHeadFrame->m_pCommandStruct != NULL)
	{
		delete pHeadFrame->m_pCommandStruct;
	}
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 释放首包帧信息结构体
MatrixServerDll_API void OnFreeInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	if (pHeadFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	delete pHeadFrame;
}
// 创建IP地址设置帧信息结构体
MatrixServerDll_API m_oIPSetFrameStruct* OnCreateInstrumentIPSetFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oIPSetFrameStruct* pIPSetFrame = NULL;
	pIPSetFrame = new m_oIPSetFrameStruct;
	InitializeCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	pIPSetFrame->m_pcSndFrameData = NULL;
	pIPSetFrame->m_pcCommandWord = NULL;
	pIPSetFrame->m_pcRcvFrameData = NULL;
	pIPSetFrame->m_oIPSetFrameSocket = INVALID_SOCKET;
	pIPSetFrame->m_pCommandStructSet = NULL;
	pIPSetFrame->m_pCommandStructReturn = NULL;
	pIPSetFrame->m_pLogOutPut = pLogOutPut;
	return pIPSetFrame;
}
// 初始化IP地址设置
MatrixServerDll_API void OnWorkInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
													m_oInstrumentCommInfoStruct* pCommInfo,
													m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pIPSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentIPSetFrame", "pIPSetFrame",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentIPSetFrame", "pCommInfo",
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
	pIPSetFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pIPSetFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pIPSetFrame->m_pCommandStructSet->m_uiAimPort = pCommInfo->m_uiAimPort;
	// IP地址设置发送缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// IP地址设置应答接收缓冲区帧数设定为仪器个数
	pIPSetFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// IP地址设置返回端口
	pIPSetFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usIPSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pIPSetFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pIPSetFrame->m_pcSndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcSndFrameData;
	}
	pIPSetFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pIPSetFrame->m_pcSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空IP地址设置命令字集合
	if (pIPSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_pcCommandWord;
	}
	pIPSetFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pIPSetFrame->m_pcCommandWord, pConstVar->m_cSndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
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
	if (pIPSetFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcRcvFrameData;
	}
	pIPSetFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pIPSetFrame->m_pcRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 关闭IP地址设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	if (pIPSetFrame->m_pcSndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcSndFrameData;
	}
	if (pIPSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_pcCommandWord;
	}
	if (pIPSetFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcRcvFrameData;
	}
	if (pIPSetFrame->m_pCommandStructSet != NULL)
	{
		delete pIPSetFrame->m_pCommandStructSet;
	}
	if (pIPSetFrame->m_pCommandStructReturn != NULL)
	{
		delete pIPSetFrame->m_pCommandStructReturn;
	}
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 释放IP地址设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	if (pIPSetFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	delete pIPSetFrame;
}
// 创建尾包帧信息结构体
MatrixServerDll_API m_oTailFrameStruct* OnCreateInstrumentTailFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oTailFrameStruct* pTailFrame = NULL;
	pTailFrame = new m_oTailFrameStruct;
	InitializeCriticalSection(&pTailFrame->m_oSecTailFrame);
	pTailFrame->m_pcRcvFrameData = NULL;
	pTailFrame->m_oTailFrameSocket = INVALID_SOCKET;
	pTailFrame->m_pCommandStruct = NULL;
	pTailFrame->m_pLogOutPut = pLogOutPut;
	return pTailFrame;
}
// 初始化尾包
MatrixServerDll_API void OnWorkInstrumentTailFrame(m_oTailFrameStruct* pTailFrame, 
												m_oInstrumentCommInfoStruct* pCommInfo,
												m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTailFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTailFrame", "pTailFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTailFrame", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	if (pTailFrame->m_pCommandStruct != NULL)
	{
		delete pTailFrame->m_pCommandStruct;
	}
	pTailFrame->m_pCommandStruct = new m_oInstrumentCommandStruct;
	// 源地址
	pTailFrame->m_pCommandStruct->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pTailFrame->m_pCommandStruct->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 尾包接收缓冲区帧数设定为仪器个数
	pTailFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 接收端口
	pTailFrame->m_pCommandStruct->m_usReturnPort = pCommInfo->m_usTailFramePort;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pTailFrame->m_pCommandStruct);
	// 清空接收帧缓冲区
	if (pTailFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailFrame->m_pcRcvFrameData;
	}
	pTailFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTailFrame->m_pcRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
}
// 关闭尾包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailFrame(m_oTailFrameStruct* pTailFrame)
{
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	if (pTailFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailFrame->m_pcRcvFrameData;
	}
	if (pTailFrame->m_pCommandStruct != NULL)
	{
		delete pTailFrame->m_pCommandStruct;
	}
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
}
// 释放尾包帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTailFrame(m_oTailFrameStruct* pTailFrame)
{
	if (pTailFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTailFrame->m_oSecTailFrame);
	delete pTailFrame;
}
// 创建尾包时刻帧信息结构体
MatrixServerDll_API m_oTailTimeFrameStruct* OnCreateInstrumentTailTimeFrame(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oTailTimeFrameStruct* pTailTimeFrame = NULL;
	pTailTimeFrame = new m_oTailTimeFrameStruct;
	InitializeCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	pTailTimeFrame->m_pcRcvFrameData = NULL;
	pTailTimeFrame->m_pcSndFrameData = NULL;
	pTailTimeFrame->m_pcCommandWord = NULL;
	pTailTimeFrame->m_oTailTimeFrameSocket = INVALID_SOCKET;
	pTailTimeFrame->m_pCommandStructSet = NULL;
	pTailTimeFrame->m_pCommandStructReturn = NULL;
	pTailTimeFrame->m_pLogOutPut = pLogOutPut;
	return pTailTimeFrame;
}
// 初始化尾包时刻帧
MatrixServerDll_API void OnWorkInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTailTimeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTailTimeFrame", "pTailTimeFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTailTimeFrame", "pCommInfo",
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
	pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pTailTimeFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pTailTimeFrame->m_pCommandStructSet->m_uiAimPort = pCommInfo->m_uiAimPort;
	// 尾包时刻发送缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// 尾包时刻应答接收缓冲区帧数设定为仪器个数
	pTailTimeFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 尾包时刻查询返回端口
	pTailTimeFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usTailTimeReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pTailTimeFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pTailTimeFrame->m_pcSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcSndFrameData;
	}
	pTailTimeFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pTailTimeFrame->m_pcSndFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空尾包时刻查询命令字集合
	if (pTailTimeFrame->m_pcCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pcCommandWord;
	}
	pTailTimeFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pTailTimeFrame->m_pcCommandWord, pConstVar->m_cSndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
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
	if (pTailTimeFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcRcvFrameData;
	}
	pTailTimeFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTailTimeFrame->m_pcRcvFrameData, pConstVar->m_cSndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 关闭尾包时刻帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_pcSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcSndFrameData;
	}
	if (pTailTimeFrame->m_pcCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pcCommandWord;
	}
	if (pTailTimeFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcRcvFrameData;
	}
	if (pTailTimeFrame->m_pCommandStructSet != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructSet;
	}
	if (pTailTimeFrame->m_pCommandStructReturn != NULL)
	{
		delete pTailTimeFrame->m_pCommandStructReturn;
	}
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 释放尾包时刻帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	if (pTailTimeFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	delete pTailTimeFrame;
}
// 初始化套接字库
MatrixServerDll_API void OnInitSocketLib(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	WSADATA data;									// 定义结构体变量
	WORD w = MAKEWORD(2, 2);			// 初始化套接字版本号
	int err = WSAStartup(w, &data);							// 初始化套接字库
	if (err == 0)
	{
		AddMsgToLogOutPutList(pLogOutPut, "InitSocketLib", "初始化套接字库成功！");
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, "InitSocketLib", "初始化套接字库失败！",
							ErrorType, WSAGetLastError());
	}
}
// 释放套接字库
MatrixServerDll_API void OnCloseSocketLib(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	// 释放套接字库
	int err = WSACleanup();	
	if (err == 0)
	{
		AddMsgToLogOutPutList(pLogOutPut, "FreeSocketLib", "释放套接字库成功！");
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPut, "FreeSocketLib", "释放套接字库失败！",
							ErrorType, WSAGetLastError());
	}
}
// 关闭Socket套接字
MatrixServerDll_API void OnCloseSocket(SOCKET oSocket)
{
	if (oSocket == INVALID_SOCKET)
	{
		return;
	}
	shutdown(oSocket, SD_BOTH);
	closesocket(oSocket);
	oSocket = INVALID_SOCKET;
}
// 创建并设置心跳端口
MatrixServerDll_API void OnCreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	// 创建套接字
	pHeartBeatFrame->m_oHeartBeatSocket = CreateInstrumentSocket(pHeartBeatFrame->m_pCommandStruct->m_usReturnPort, 
																pHeartBeatFrame->m_pCommandStruct->m_uiSrcIP,
																pHeartBeatFrame->m_pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pHeartBeatFrame->m_oHeartBeatSocket, pHeartBeatFrame->m_pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	AddMsgToLogOutPutList(pHeartBeatFrame->m_pLogOutPut, "CreateAndSetHeartBeatSocket", "创建并设置心跳端口！");
}

// 创建并设置首包端口
MatrixServerDll_API void OnCreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame)
{
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	// 创建套接字
	pHeadFrame->m_oHeadFrameSocket = CreateInstrumentSocket(pHeadFrame->m_pCommandStruct->m_usReturnPort, 
															pHeadFrame->m_pCommandStruct->m_uiSrcIP,
															pHeadFrame->m_pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pHeadFrame->m_oHeadFrameSocket, pHeadFrame->m_uiRcvBufferSize, pHeadFrame->m_pLogOutPut);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	AddMsgToLogOutPutList(pHeadFrame->m_pLogOutPut, "CreateAndSetHeadFrameSocket", "创建并设置首包端口！");
}
// 创建并设置IP地址设置端口
MatrixServerDll_API void OnCreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 创建套接字
	pIPSetFrame->m_oIPSetFrameSocket = CreateInstrumentSocket(pIPSetFrame->m_pCommandStructSet->m_usReturnPort, 
																pIPSetFrame->m_pCommandStructSet->m_uiSrcIP,
																pIPSetFrame->m_pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiSndBufferSize, pIPSetFrame->m_pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_uiRcvBufferSize, pIPSetFrame->m_pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	AddMsgToLogOutPutList(pIPSetFrame->m_pLogOutPut, "CreateAndSetIPSetFrameSocket", "创建并设置IP地址设置端口！");
}
// 创建并设置尾包端口
MatrixServerDll_API void OnCreateAndSetTailFrameSocket(m_oTailFrameStruct* pTailFrame)
{
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	// 创建套接字
	pTailFrame->m_oTailFrameSocket = CreateInstrumentSocket(pTailFrame->m_pCommandStruct->m_usReturnPort, 
															pTailFrame->m_pCommandStruct->m_uiSrcIP,
															pTailFrame->m_pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pTailFrame->m_oTailFrameSocket, pTailFrame->m_uiRcvBufferSize, pTailFrame->m_pLogOutPut);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
	AddMsgToLogOutPutList(pTailFrame->m_pLogOutPut, "OnCreateAndSetTailFrameSocket", "创建并设置尾包端口！");
}
// 创建并设置尾包时刻端口
MatrixServerDll_API void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 创建套接字
	pTailTimeFrame->m_oTailTimeFrameSocket = CreateInstrumentSocket(pTailTimeFrame->m_pCommandStructSet->m_usReturnPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP,
		pTailTimeFrame->m_pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_uiSndBufferSize, pTailTimeFrame->m_pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_uiRcvBufferSize, pTailTimeFrame->m_pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	AddMsgToLogOutPutList(pTailTimeFrame->m_pLogOutPut, "OnCreateAndSetTailTimeFrameSocket", "创建并设置尾包时刻查询帧端口！");
}
// 仪器信息重置
MatrixServerDll_API void OnInstrumentReset(m_oInstrumentStruct* pInstrument)
{
	// 仪器是否使用中
	pInstrument->m_bInUsed = false;
	// 仪器设备号
	pInstrument->m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	pInstrument->m_uiInstrumentType = 0;
// 	// 仪器IP地址
// 	pInstrument->m_uiIP = 0;
	// 仪器路由IP地址
	pInstrument->m_uiRoutIP = 0;
	// 路由方向 1-上；2-下；3-左；4右
	pInstrument->m_uiRoutDirection = 0;
	// 路由地址 交叉线方向 上方
	pInstrument->m_uiRoutIPTop = 0;
	// 路由地址 交叉线方向 下方
	pInstrument->m_uiRoutIPDown = 0;
	// 路由地址 测线线方向 左方
	pInstrument->m_uiRoutIPLeft = 0;
	// 路由地址 测线线方向 右方
	pInstrument->m_uiRoutIPRight = 0;
	// 路由开关
	pInstrument->m_byLAUXRoutOpenSet = 0;
	// 链接的仪器 上方
	pInstrument->m_pInstrumentTop = NULL;
	// 链接的仪器 下方
	pInstrument->m_pInstrumentDown = NULL;
	// 链接的仪器 左方
	pInstrument->m_pInstrumentLeft = NULL;
	// 链接的仪器 右方
	pInstrument->m_pInstrumentRight = NULL;

	/** 仪器本地系统时间*/
	pInstrument->m_uiTimeSystem = 0;
	/** 仪器网络时间*/
	pInstrument->m_uiNetTime = 0;
	/** 仪器网络状态*/
	pInstrument->m_uiNetState = 0;
	/** 仪器参数备用1*/
	pInstrument->m_uiNetOrder = 0;

	// 首包时刻
	pInstrument->m_uiTimeHeadFrame = 0;
	// 尾包计数
	pInstrument->m_iTailFrameCount = 0;
	// 仪器时延
	pInstrument->m_uiTimeDelay = 0;

	pInstrument->m_usReceiveTime = 0;	// 16bits时间，接收时刻低位
	pInstrument->m_usSendTime = 0;	// 16bits时间，发送时刻低位

	pInstrument->m_usLineLeftReceiveTime = 0;	// 16bits 测线方向左面尾包接收时刻
	pInstrument->m_usLineRightReceiveTime = 0;	// 16bits 测线方向右面尾包接收时刻

	pInstrument->m_usCrossTopReceiveTime = 0;	// 16bits 交叉线方向上面尾包接收时刻
	pInstrument->m_usCrossDownReceiveTime = 0;	// 16bits 交叉线方下面尾包接收时刻

	pInstrument->m_uiBroadCastPort = 0;			// 采集站或电源站的广播端口
	// 测线
	pInstrument->m_iLineNb = 0;
	// 标记点号
	pInstrument->m_uiPointNb = 0;
	/** 测点号*/
	pInstrument->m_iPointIndex = 0;
	// 	// 测道
	// 	m_uiChannelNb = 0;

	// 是否跳过道
	pInstrument->m_bJumpedChannel = false;
	// 是否连接检波器
	pInstrument->m_bSensor = false;
	// 是否辅助道
	pInstrument->m_bAux = false;
	// 是否连接爆炸机
	pInstrument->m_bBlastMachine = false;
	// 是否迂回道
	pInstrument->m_bDetour = false;
	// 是否迂回道低端标记点
	pInstrument->m_bDetourMarkerLow = false;
	// 是否迂回道高端标记点
	pInstrument->m_bDetourMarkerHigh = false;
	// 是否迂回道停止标记
	pInstrument->m_uiStopMarking = false;
	// 是否标记点
	pInstrument->m_bMarker = false;
	// 首包位置稳定次数
	pInstrument->m_iHeadFrameStableNum = 0;
	// 第几次设置IP地址
	pInstrument->m_iIPSetCount = 0;
	// IP地址设置是否成功
	pInstrument->m_bIPSetOK = false;

	// 第几次尾包时刻查询
	pInstrument->m_iTailTimeQueryCount = 0;
	// 尾包时刻查询是否成功
	pInstrument->m_bTailTimeQueryOK = false;

	// 本地时间修正高位
	pInstrument->m_uiTimeHigh = 0;
	// 本地时间修正低位
	pInstrument->m_uiTimeLow = 0;
	// 第几次设置仪器时间
	pInstrument->m_iTimeSetCount = 0;
	// 仪器时间设置是否成功
	pInstrument->m_bTimeSetOK = false;
}
// 创建仪器队列结构体
MatrixServerDll_API m_oInstrumentListStruct* OnCreateInstrumentList(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oInstrumentListStruct* pInstrumentList = NULL;
	pInstrumentList = new m_oInstrumentListStruct;
	InitializeCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	pInstrumentList->m_pArrayInstrument = NULL;
	pInstrumentList->m_pLogOutPut = pLogOutPut;
	return pInstrumentList;
}
// 初始化仪器队列结构体
MatrixServerDll_API void OnWorkInstrumentList(m_oInstrumentListStruct* pInstrumentList,
											m_oInstrumentCommInfoStruct* pCommInfo,
											m_oConstVarStruct* pConstVar)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "InitInstrumentList", "pCommInfo",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pConstVar == NULL)
	{
		AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "InitInstrumentList", "pConstVar",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	// 清空SN仪器索引表
	pInstrumentList->m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	pInstrumentList->m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	pInstrumentList->m_olsInstrumentFree.clear();
	// 清空IP地址设置仪器索引表
	pInstrumentList->m_oIPSetMap.clear();
	// 仪器队列中仪器个数
 	pInstrumentList->m_uiCountAll = pCommInfo->m_uiInstrumentNum;
 	// 生成仪器数组
	if (pInstrumentList->m_pArrayInstrument != NULL)
	{
		delete[] pInstrumentList->m_pArrayInstrument;
	}
 	pInstrumentList->m_pArrayInstrument = new m_oInstrumentStruct[pInstrumentList->m_uiCountAll];
	// 空闲仪器数量
	pInstrumentList->m_uiCountFree = pInstrumentList->m_uiCountAll;
	for(unsigned int i = 0; i < pInstrumentList->m_uiCountAll; i++)
	{
		// 仪器在仪器数组中的位置
		pInstrumentList->m_pArrayInstrument[i].m_uiIndex = i;
		pInstrumentList->m_pArrayInstrument[i].m_uiIP = pConstVar->m_iIPSetAddrStart + i * pConstVar->m_iIPSetAddrInterval;
		// 重置仪器
		OnInstrumentReset(&pInstrumentList->m_pArrayInstrument[i]);
		// 仪器加在空闲仪器队列尾部
		pInstrumentList->m_olsInstrumentFree.push_back(&pInstrumentList->m_pArrayInstrument[i]);
	}
	LeaveCriticalSection(&pInstrumentList->m_oSecInstrumentList);
}
// 关闭仪器队列结构体
MatrixServerDll_API void OnCloseInstrumentList(m_oInstrumentListStruct* pInstrumentList)
{
	EnterCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	// 清空SN仪器索引表
	pInstrumentList->m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	pInstrumentList->m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	pInstrumentList->m_olsInstrumentFree.clear();
	// 清空IP地址设置仪器索引表
	pInstrumentList->m_oIPSetMap.clear();
	// 删除仪器数组
	if (pInstrumentList->m_pArrayInstrument != NULL)
	{
		delete[] pInstrumentList->m_pArrayInstrument;
	}
	LeaveCriticalSection(&pInstrumentList->m_oSecInstrumentList);
}
// 释放仪器队列结构体
MatrixServerDll_API void OnFreeInstrumentList(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	delete pInstrumentList;
}

// 得到一个空闲仪器
MatrixServerDll_API m_oInstrumentStruct* GetFreeInstrument(m_oInstrumentListStruct* pInstrumentList)
{
	m_oInstrumentStruct* pInstrument = NULL;
	list <m_oInstrumentStruct*>::iterator iter;
	if(pInstrumentList->m_uiCountFree > 0)	//有空闲仪器
	{
		// 从空闲队列中得到一个仪器
		iter = pInstrumentList->m_olsInstrumentFree.begin();
		pInstrument = *iter;
		pInstrumentList->m_olsInstrumentFree.pop_front();	
		pInstrument->m_bInUsed = true;	// 设置仪器为使用中
		pInstrumentList->m_uiCountFree--;	// 空闲仪器总数减1
	}
	return pInstrument;
}


// 增加一个空闲仪器
MatrixServerDll_API void AddFreeInstrument(m_oInstrumentStruct* pInstrument, m_oInstrumentListStruct* pInstrumentList)
{
	//初始化仪器
	OnInstrumentReset(pInstrument);
	//加入空闲队列
	pInstrumentList->m_olsInstrumentFree.push_back(pInstrument);
	pInstrumentList->m_uiCountFree++;	// 空闲仪器总数加1
}
// 增加一个IP地址设置仪器
MatrixServerDll_API void AddInstrumentToMap(unsigned int uiIndex, m_oInstrumentStruct* pInstrument, 
											hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	pMap->insert(hash_map<unsigned int, m_oInstrumentStruct*>::value_type (uiIndex, pInstrument));
}
// 判断仪器索引号是否已加入索引表
MatrixServerDll_API BOOL IfIndexExistInMap(unsigned int uiIndex, 
										hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter != pMap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 根据输入索引号，由索引表得到仪器指针
MatrixServerDll_API m_oInstrumentStruct* GetInstrumentFromMap(unsigned int uiIndex, 
											hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	return iter->second;
}
// 从IP地址设置索引表删除索引号指向的仪器指针
MatrixServerDll_API BOOL DeleteInstrumentFromMap(unsigned int uiIndex, 
												hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter != pMap->end())
	{
		pMap->erase(iter);
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 重置路由信息
MatrixServerDll_API void OnRoutReset(m_oRoutStruct* pRout)
{
	// 仪器是否使用中
	pRout->m_bInUsed = false;
	// 路由方向 1-上；2-下；3-左；4右
	pRout->m_uiRoutDirection = 0;
	// 路由头仪器
	pRout->m_pHead = NULL;
	// 路由尾仪器
	pRout->m_pTail = NULL;
	// 路由时刻
	pRout->m_uiRoutTime = 0;
	// 上次时统处理时刻
	pRout->m_uiDelayProcTime = 0;
	// 路由过期标志位
	pRout->m_bExpired = false;
}
// 更新路由对象的路由时间
MatrixServerDll_API void UpdateRoutTime(m_oRoutStruct* pRout)
{
	// 路由时刻
	pRout->m_uiRoutTime = GetTickCount();
}

// 更新上次时统处理时刻
MatrixServerDll_API void UpdateRoutDelayProcTime(m_oRoutStruct* pRout)
{
	// 上次时统处理时刻
	pRout->m_uiDelayProcTime = GetTickCount();
}
// 创建路由队列结构体
MatrixServerDll_API m_oRoutListStruct* OnCreateRoutList(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oRoutListStruct* pRoutList = NULL;
	pRoutList = new m_oRoutListStruct;
	InitializeCriticalSection(&pRoutList->m_oSecRoutList);
	pRoutList->m_pArrayRout = NULL;
	pRoutList->m_pLogOutPut = pLogOutPut;
	return pRoutList;
}
// 初始化路由队列结构体
MatrixServerDll_API void OnWorkRoutList(m_oRoutListStruct* pRoutList,
										m_oInstrumentCommInfoStruct* pCommInfo,
										m_oConstVarStruct* pConstVar)
{
	if (pRoutList == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pRoutList->m_pLogOutPut, "InitRoutList", "pCommInfo",
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 路由队列中路由个数
	pRoutList->m_uiCountAll = pCommInfo->m_uiInstrumentNum;
	// 空闲路由总数
	pRoutList->m_uiCountFree = pRoutList->m_uiCountAll;
	// 生成路由数组
	if (pRoutList->m_pArrayRout != NULL)
	{
		delete[] pRoutList->m_pArrayRout;
	}
	pRoutList->m_pArrayRout = new m_oRoutStruct[pRoutList->m_uiCountAll];

	// 加入空闲路由队列
	for(unsigned int i = 0; i < pRoutList->m_uiCountAll; i++)
	{
		// 设置路由IP地址
		pRoutList->m_pArrayRout[i].m_uiRoutIP = pConstVar->m_iRoutSetAddrStart + i * pConstVar->m_iRoutSetAddrInterval;
		// 路由在路由数组中的位置
		pRoutList->m_pArrayRout[i].m_uiIndex = i;
		// 重置路由
		OnRoutReset(&pRoutList->m_pArrayRout[i]);
		// 路由加在空闲路由队列尾部
		pRoutList->m_olsRoutFree.push_back(&pRoutList->m_pArrayRout[i]);
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// 关闭仪器队列结构体
MatrixServerDll_API void OnCloseRoutList(m_oRoutListStruct* pRoutList)
{
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 删除路由数组
	if (pRoutList->m_pArrayRout != NULL)
	{
		delete[] pRoutList->m_pArrayRout;
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// 释放仪器队列结构体
MatrixServerDll_API void OnFreeRoutList(m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pRoutList->m_oSecRoutList);
	delete pRoutList;
}
// 得到一个空闲路由
MatrixServerDll_API m_oRoutStruct* GetFreeRout(m_oRoutListStruct* pRoutList)
{
	m_oRoutStruct* pRout = NULL;
	list <m_oRoutStruct*>::iterator iter;
	if(pRoutList->m_uiCountFree > 0)	//有空闲路由
	{
		// 从空闲路由队列头部得到一个空闲路由
		iter = pRoutList->m_olsRoutFree.begin();
		pRout = *iter;
		pRoutList->m_olsRoutFree.pop_front();	
		// 路由是否使用中
		pRout->m_bInUsed = true;	
		// 空闲路由计数减1
		pRoutList->m_uiCountFree--;
	}
	return pRout;
}
// 增加一个空闲路由
MatrixServerDll_API void AddFreeRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList)
{
	//初始化路由
	OnRoutReset(pRout);
	//加入空闲队列
	pRoutList->m_olsRoutFree.push_back(pRout);
	// 空闲路由计数加1
	pRoutList->m_uiCountFree++;
}
// 增加一个路由
MatrixServerDll_API void AddRout(unsigned int uiIndex, 
								m_oRoutStruct* pRout, 
								hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	pRoutMap->insert(hash_map<unsigned int, m_oRoutStruct*>::value_type (uiIndex, pRout));
}
// 根据输入索引号，由索引表得到一个路由指针
MatrixServerDll_API m_oRoutStruct* GetRout(unsigned int uiIndex, hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiIndex);
	return iter->second;
}
// 路由地址是否已加入索引表
MatrixServerDll_API BOOL IfIndexExistInRoutMap(unsigned int uiRoutIP, hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiRoutIP);
	if (iter != pRoutMap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 根据输入索引号，从索引表中删除一个路由
MatrixServerDll_API void DeleteRout(unsigned int uiIndex, hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiIndex);
	if (iter != pRoutMap->end())
	{
		pRoutMap->erase(iter);
	}
}
// 解析首包帧
MatrixServerDll_API bool ParseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	bReturn = ParseInstrumentFrame(pHeadFrame->m_pCommandStruct, pHeadFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	return bReturn;
}
// 解析尾包帧
MatrixServerDll_API bool ParseInstrumentTailFrame(m_oTailFrameStruct* pTailFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	bReturn = ParseInstrumentFrame(pTailFrame->m_pCommandStruct, pTailFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
	return bReturn;
}
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrumentIPSetReturnFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	bReturn = ParseInstrumentFrame(pIPSetFrame->m_pCommandStructReturn, pIPSetFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	return bReturn;
}
// 生成心跳帧
MatrixServerDll_API void MakeInstrumentHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_pCommandStruct->m_uiDstIP = pConstVar->m_iIPBroadcastAddr;
	MakeInstrumentFrame(pHeartBeatFrame->m_pCommandStruct,  pConstVar, pHeartBeatFrame->m_pcSndFrameData,
		pHeartBeatFrame->m_pcCommandWord, pHeartBeatFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 发送心跳帧
MatrixServerDll_API void SendInstrumentHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	SendFrame(pHeartBeatFrame->m_oHeartBeatSocket, pHeartBeatFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pHeartBeatFrame->m_pCommandStruct->m_uiAimPort, pHeartBeatFrame->m_pCommandStruct->m_uiAimIP, 
		pHeartBeatFrame->m_pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 生成IP地址查询帧
MatrixServerDll_API void MakeInstrumentIPQueryFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	CString str = _T("");
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// IP地址查询命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	pIPSetFrame->m_pcCommandWord[0] = pConstVar->m_byCmdLocalIPAddr;
	// 查询命令字个数
	pIPSetFrame->m_usCommandWordNum = 1;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_pcSndFrameData, 
		pIPSetFrame->m_pcCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送IP地址查询帧"), pIPSetFrame->m_pCommandStructSet->m_uiDstIP);
	AddMsgToLogOutPutList(pIPSetFrame->m_pLogOutPut, "MakeInstrumentIPQueryFrame", ConvertCStrToStr(str));
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	CString str = _T("");
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 广播IP地址
	pIPSetFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_iIPBroadcastAddr;
	// 仪器SN命令字
	pIPSetFrame->m_pcCommandWord[0] = pConstVar->m_byCmdSn;
	// 仪器IP命令字
	pIPSetFrame->m_pcCommandWord[1] = pConstVar->m_byCmdLocalIPAddr;
	// 生成IP地址设置帧
	if((pIPSetFrame->m_pCommandStructSet->m_uiInstrumentType == pConstVar->m_byInstrumentTypeFDU)
		|| (pIPSetFrame->m_pCommandStructSet->m_uiInstrumentType == pConstVar->m_byInstrumentTypeLAUL))
	{
		// 仪器广播端口命令字
		pIPSetFrame->m_pcCommandWord[2] = pConstVar->m_byCmdBroadCastPortSet;
		// 命令字个数
		pIPSetFrame->m_usCommandWordNum = 3;
	}
	// LCI和交叉站需要设置路由IP地址
	else
	{
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[2] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[3] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[4] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[5] = pConstVar->m_byCmdLAUXSetRout;
		// 打开仪器路由命令字
		pIPSetFrame->m_pcCommandWord[6] = pConstVar->m_byCmdLAUXRoutOpenSet;
		// 命令字个数
		pIPSetFrame->m_usCommandWordNum = 7;
	}
	// IP地址设置命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_pcSndFrameData, 
		pIPSetFrame->m_pcCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器SN = 0x%x，IP地址 = 0x%x 的仪器发送IP地址设置帧"), 
		pIPSetFrame->m_pCommandStructSet->m_uiSN, pIPSetFrame->m_pCommandStructSet->m_uiInstrumentIP);
	AddMsgToLogOutPutList(pIPSetFrame->m_pLogOutPut, "MakeInstrumentIPSetFrame", ConvertCStrToStr(str));
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 发送IP地址设置帧
MatrixServerDll_API void SendInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	SendFrame(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pIPSetFrame->m_pCommandStructSet->m_uiAimPort, pIPSetFrame->m_pCommandStructSet->m_uiAimIP, 
		pIPSetFrame->m_pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 创建日志输出线程
MatrixServerDll_API m_oLogOutPutThreadStruct* OnCreateLogOutPutThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oLogOutPutThreadStruct* pLogOutPutThread = NULL;
	pLogOutPutThread = new m_oLogOutPutThreadStruct;
	pLogOutPutThread->m_pThread = new m_oThreadStruct;
	pLogOutPutThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pLogOutPutThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pLogOutPutThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pLogOutPutThread->m_pThread->m_dwThreadID = 0;
	return pLogOutPutThread;
}
// 线程等待函数
MatrixServerDll_API void WaitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pLogOutPutThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pLogOutPutThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pLogOutPutThread->m_pThread->m_pConstVar->m_iLogOutPutSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	while(true)
	{
		if (pLogOutPutThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pLogOutPutThread->m_pThread->m_bWork == true)
		{
			WriteLogOutPutListToFile(pLogOutPutThread->m_pThread->m_pLogOutPut);
		}
		if (pLogOutPutThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitLogOutPutThread(pLogOutPutThread);
	}
	::SetEvent(pLogOutPutThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化日志输出线程
MatrixServerDll_API bool OnInitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
							"pLogOutPutThread", ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	pLogOutPutThread->m_pThread->m_bClose = false;
	pLogOutPutThread->m_pThread->m_bWork = false;
	pLogOutPutThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pLogOutPutThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
							"pLogOutPutThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
							"pLogOutPutThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pLogOutPutThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pLogOutPutThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
															0,
															(LPTHREAD_START_ROUTINE)RunLogOutPutThread,
															pLogOutPutThread, 
															0, 
															&pLogOutPutThread->m_pThread->m_dwThreadID);
	if (pLogOutPutThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
							"pLogOutPutThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", "日志输出线程创建成功");
	return true;
}
// 关闭日志输出线程
MatrixServerDll_API bool OnCloseLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pLogOutPutThread->m_pThread->m_hThreadClose, 
									pLogOutPutThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
									* pLogOutPutThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pLogOutPutThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", 
							"日志输出线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", "日志输出线程成功关闭");
	return true;
}
// 释放日志输出线程
MatrixServerDll_API void OnFreeLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return;
	}
	if (pLogOutPutThread->m_pThread != NULL)
	{
		delete pLogOutPutThread->m_pThread;
	}
	delete pLogOutPutThread;
}
// 创建心跳线程
MatrixServerDll_API m_oHeartBeatThreadStruct* OnCreateHeartBeatThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeartBeatThreadStruct* pHeartBeatThread = NULL;
	pHeartBeatThread = new m_oHeartBeatThreadStruct;
	pHeartBeatThread->m_pThread = new m_oThreadStruct;
	pHeartBeatThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pHeartBeatThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pHeartBeatThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pHeartBeatThread->m_pThread->m_dwThreadID = 0;
	return pHeartBeatThread;
}
// 线程等待函数
MatrixServerDll_API void WaitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pHeartBeatThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pHeartBeatThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pHeartBeatThread->m_pThread->m_pConstVar->m_iHeartBeatSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pHeartBeatThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pHeartBeatThread->m_pThread->m_bWork == true)
		{
			MakeInstrumentHeartBeatFrame(pHeartBeatThread->m_pHeartBeatFrame, pHeartBeatThread->m_pThread->m_pConstVar);
			SendInstrumentHeartBeatFrame(pHeartBeatThread->m_pHeartBeatFrame, pHeartBeatThread->m_pThread->m_pConstVar);
		}
		if (pHeartBeatThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitHeartBeatThread(pHeartBeatThread);
	}
	::SetEvent(pHeartBeatThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化心跳线程
MatrixServerDll_API bool OnInitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	pHeartBeatThread->m_pThread->m_bClose = false;
	pHeartBeatThread->m_pThread->m_bWork = false;
	pHeartBeatThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pHeartBeatThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", 
							"pHeartBeatThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", 
							"pHeartBeatThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pHeartBeatThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pHeartBeatThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
															0,
															(LPTHREAD_START_ROUTINE)RunHeartBeatThread,
															pHeartBeatThread, 
															0, 
															&pHeartBeatThread->m_pThread->m_dwThreadID);
	if (pHeartBeatThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", 
							"pHeartBeatThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", "心跳线程创建成功");
	return true;
}
// 关闭心跳线程
MatrixServerDll_API bool OnCloseHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pHeartBeatThread->m_pThread->m_hThreadClose, 
									pHeartBeatThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
									* pHeartBeatThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pHeartBeatThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", 
							"心跳线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", "心跳线程成功关闭");
	return true;
}
// 释放心跳线程
MatrixServerDll_API void OnFreeHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return;
	}
	if (pHeartBeatThread->m_pThread != NULL)
	{
		delete pHeartBeatThread->m_pThread;
	}
	delete pHeartBeatThread;
}
// 创建首包接收线程
MatrixServerDll_API m_oHeadFrameThreadStruct* OnCreateHeadFrameThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oHeadFrameThreadStruct* pHeadFrameThread = NULL;
	pHeadFrameThread = new m_oHeadFrameThreadStruct;
	pHeadFrameThread->m_pThread = new m_oThreadStruct;
	pHeadFrameThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pHeadFrameThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pHeadFrameThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pHeadFrameThread->m_pThread->m_dwThreadID = 0;
	return pHeadFrameThread;
}
/**
* 根据链接方向，得到连接的下一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetNextInstrument(unsigned int uiRoutDirection, 
														m_oInstrumentStruct* pInstrument,
														m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 判断方向
	if (uiRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrumentNext = pInstrument->m_pInstrumentTop;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrumentNext = pInstrument->m_pInstrumentDown;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrumentNext = pInstrument->m_pInstrumentLeft;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrumentNext = pInstrument->m_pInstrumentRight;
	}

	return pInstrumentNext;
}
/**
* 根据链接方向，得到连接的前一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的前一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetPreviousInstrument(unsigned int uiRoutDirection, 
															m_oInstrumentStruct* pInstrument,
															m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrumentPrevious = NULL;
	// 判断方向
	if (uiRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentDown;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentTop;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentRight;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentLeft;
	}

	return pInstrumentPrevious;
}
// 仪器位置按照首包时刻排序
MatrixServerDll_API void InstrumentLocationSortLeft(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	bool bLocation = false;
	bool bStable = true;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrumentLeft = NULL;
	m_oInstrumentStruct* pInstrumentRight = NULL;
	if (pConstVar == NULL)
	{
		return;
	}
	if (pInstrument == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InstrumentLocationSortLeft", "pInstrument", 
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InstrumentLocationSortLeft", "pRout", 
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 该路由方向尾仪器为空
	if (pRout->m_pTail == NULL)
	{
		pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex - 1;
		pRout->m_pTail = pInstrument;
		pRout->m_pHead->m_pInstrumentLeft = pInstrument;
		pInstrument->m_pInstrumentRight = pRout->m_pHead;
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		// 该路由方向尾仪器首包时刻小于该仪器的首包时刻
		if (pRout->m_pTail->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
		{
			pInstrument->m_iPointIndex = pRout->m_pTail->m_iPointIndex - 1;
			pRout->m_pTail->m_pInstrumentLeft = pInstrument;
			pInstrument->m_pInstrumentRight = pRout->m_pTail;
			pRout->m_pTail = pInstrument;
			pInstrument->m_iHeadFrameStableNum++;
		} 
		// 仪器位于路由链中
		else
		{
			pInstrumentNext = pRout->m_pHead;
			while(pInstrumentNext != NULL)
			{
				if (pInstrumentNext->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
				{
					pInstrumentRight = pInstrumentNext;
				}
				else
				{
					// 找到路由表第一个首包时刻大于等于仪器首包时刻的仪器
					if (bLocation == false)
					{
						bLocation = true;
						pInstrumentLeft = pInstrumentNext;
						if (pInstrumentLeft == pInstrument)
						{
							pInstrument->m_iHeadFrameStableNum++;
							pInstrument->m_iPointIndex = pInstrumentRight->m_iPointIndex - 1;
						}
						// 在路由表中两个仪器中间插入仪器
						else
						{
							pInstrumentRight->m_pInstrumentLeft = pInstrument;
							pInstrument->m_pInstrumentRight = pInstrumentRight;
							pInstrumentLeft->m_pInstrumentRight = pInstrument;
							pInstrument->m_pInstrumentLeft = pInstrumentLeft;
							pInstrumentLeft->m_iHeadFrameStableNum = 0;
							pInstrument->m_iHeadFrameStableNum = 0;
							pInstrument->m_iPointIndex = pInstrumentRight->m_iPointIndex - 1;
							pInstrumentLeft->m_iPointIndex--;
							bStable = false;	
						}
					}
					else
					{
						if (bStable == false)
						{
							pInstrumentNext->m_iHeadFrameStableNum = 0;
							pInstrumentNext->m_iPointIndex--;
						}
					}
				}
				pInstrumentNext = GetNextInstrument(pConstVar->m_iDirectionLeft, pInstrumentNext, pConstVar);
			}
		}
	}
}
// 仪器位置按照首包时刻排序
MatrixServerDll_API void InstrumentLocationSortRight(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	bool bLocation = false;
	bool bStable = true;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrumentLeft = NULL;
	m_oInstrumentStruct* pInstrumentRight = NULL;
	if (pConstVar == NULL)
	{
		return;
	}
	if (pInstrument == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InstrumentLocationSortRight", "pInstrument", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InstrumentLocationSortRight", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 该路由方向尾仪器为空
	if (pRout->m_pTail == NULL)
	{
		pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex + 1;
		pRout->m_pTail = pInstrument;
		pRout->m_pHead->m_pInstrumentRight = pInstrument;
		pInstrument->m_pInstrumentLeft = pRout->m_pHead;
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		// 该路由方向尾仪器首包时刻小于该仪器的首包时刻
		if (pRout->m_pTail->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
		{
			pInstrument->m_iPointIndex = pRout->m_pTail->m_iPointIndex + 1;
			pRout->m_pTail->m_pInstrumentRight = pInstrument;
			pInstrument->m_pInstrumentLeft = pRout->m_pTail;
			pRout->m_pTail = pInstrument;
			pInstrument->m_iHeadFrameStableNum++;
		} 
		// 仪器位于路由链中
		else
		{
			pInstrumentNext = pRout->m_pHead;
			while(pInstrumentNext != NULL)
			{
				if (pInstrumentNext->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
				{
					pInstrumentLeft = pInstrumentNext;
				}
				else
				{
					// 找到路由表第一个首包时刻大于等于仪器首包时刻的仪器
					if (bLocation == false)
					{
						bLocation = true;
						pInstrumentRight = pInstrumentNext;
						if (pInstrumentRight == pInstrument)
						{
							pInstrument->m_iHeadFrameStableNum++;
							pInstrument->m_iPointIndex = pInstrumentLeft->m_iPointIndex + 1;
						}
						// 在路由表中两个仪器中间插入仪器
						else
						{
							pInstrumentRight->m_pInstrumentLeft = pInstrument;
							pInstrument->m_pInstrumentRight = pInstrumentRight;
							pInstrumentLeft->m_pInstrumentRight = pInstrument;
							pInstrument->m_pInstrumentLeft = pInstrumentLeft;
							pInstrumentRight->m_iHeadFrameStableNum = 0;
							pInstrument->m_iHeadFrameStableNum = 0;
							pInstrument->m_iPointIndex = pInstrumentLeft->m_iPointIndex + 1;
							pInstrumentRight->m_iPointIndex++;
							bStable = false;	
						}
					}
					else
					{
						if (bStable == false)
						{
							pInstrumentNext->m_iHeadFrameStableNum = 0;
							pInstrumentNext->m_iPointIndex++;
						}
					}
				}
				pInstrumentNext = GetNextInstrument(pConstVar->m_iDirectionRight, pInstrumentNext, pConstVar);
			}
		}
	}
}
// 设置仪器的位置
MatrixServerDll_API void SetInstrumentLocation(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pInstrument == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SetInstrumentLocation", "pInstrument", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SetInstrumentLocation", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 在路由索引表中找到路由头仪器
	// 如果是LCI及交叉线方向的交叉站
	if ((pInstrument->m_uiRoutDirection == pConstVar->m_iDirectionCenter)
		|| (pInstrument->m_uiRoutDirection == pConstVar->m_iDirectionTop)
		|| (pInstrument->m_uiRoutDirection == pConstVar->m_iDirectionDown))
	{
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		if (pRout != NULL)
		{
			// 大线左方
			if (pInstrument->m_uiRoutDirection == pConstVar->m_iDirectionLeft)
			{
				InstrumentLocationSortLeft(pInstrument, pRout, pConstVar);
			}
			// 大线右方
			else
			{
				InstrumentLocationSortRight(pInstrument, pRout, pConstVar);
			}
		}
	}
}
/**
* 设置交叉站某个方向的路由
* @param CInstrument* &pInstrument 仪器指针
* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
MatrixServerDll_API void SetCrossRout(m_oInstrumentStruct* pInstrument,  
									unsigned int uiRoutDirection, 
									m_oConstVarStruct* pConstVar,
									m_oRoutListStruct* pRoutList)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pInstrument == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SetCrossRout", "pInstrument", 
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "SetCrossRout", "pRoutList", 
							ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oRoutStruct* pRout = NULL;
	pRout = GetFreeRout(pRoutList);
	// 判断方向，设置仪器路由地址
	if (uiRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrument->m_uiRoutIPTop = pRout->m_uiRoutIP;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrument->m_uiRoutIPDown = pRout->m_uiRoutIP;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrument->m_uiRoutIPLeft = pRout->m_uiRoutIP;
	}
	else if (uiRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrument->m_uiRoutIPRight = pRout->m_uiRoutIP;
	}
	// 设置交叉站路由方向
	pRout->m_uiRoutDirection = uiRoutDirection;
	// 设置路由头
	pRout->m_pHead = pInstrument;
	if (uiRoutDirection == pConstVar->m_iDirectionCenter)
	{
		// 设置LCI路由
		pRout->m_uiRoutIP = 0;
		// 设置路由尾
		pRout->m_pTail = pInstrument;
	}
	else
	{
		// 设置路由尾
		pRout->m_pTail = NULL;
	}
	// 更新路由对象的路由时间
	UpdateRoutTime(pRout);
	// 路由对象加入路由索引表
	AddRout(pRout->m_uiRoutIP, pRout,&pRoutList->m_oRoutMap);
}
// 处理单个首包帧
MatrixServerDll_API void ProcHeadFrameOne(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	// 判断仪器SN是否在SN索引表中
	if(TRUE == IfIndexExistInMap(pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiSN, 
		&pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap))
	{
		// 在索引表中则找到该仪器,得到该仪器指针
		pInstrument = GetInstrumentFromMap(pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiSN, 
										&pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
		if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP,
										&pHeadFrameThread->m_pRoutList->m_oRoutMap))
		{
			pRout = GetRout(pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP,
				&pHeadFrameThread->m_pRoutList->m_oRoutMap);
			// 更新路由对象的路由时间
			UpdateRoutTime(pRout);
		}
		// 设置新仪器的首包时刻
		pInstrument->m_uiTimeHeadFrame = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiTimeHeadFrame;
		// 更新仪器的位置信息
		SetInstrumentLocation(pInstrument, pRout, pHeadFrameThread->m_pThread->m_pConstVar);
		// 如果仪器在其路由方向上位置稳定次数超过设定次数
		// 则将该仪器加入IP地址设置队列
		if (pInstrument->m_iHeadFrameStableNum >= pHeadFrameThread->m_pThread->m_pConstVar->m_iHeadFrameStableTimes)
		{
			if (FALSE == IfIndexExistInMap(pInstrument->m_uiIP, &pHeadFrameThread->m_pInstrumentList->m_oIPSetMap))
			{
				AddInstrumentToMap(pInstrument->m_uiIP, pInstrument, &pHeadFrameThread->m_pInstrumentList->m_oIPSetMap);
			}
		}
	}
	else
	{
		int iDirection = 0;
		// 得到新仪器
		pInstrument = GetFreeInstrument(pHeadFrameThread->m_pInstrumentList);
		//设置新仪器的SN
		pInstrument->m_uiSN = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiSN;
		// 仪器类型
		pInstrument->m_uiInstrumentType = pInstrument->m_uiSN & 0xff;
		//设置新仪器的路由IP地址
		pInstrument->m_uiRoutIP = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP;
		//设置新仪器的首包时刻
		pInstrument->m_uiTimeHeadFrame = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiTimeHeadFrame;
		// 设置新仪器的仪器类型
		// 路由地址为0为LCI
		if (pInstrument->m_uiRoutIP == 0)
		{
			pInstrument->m_uiInstrumentType = pHeadFrameThread->m_pThread->m_pConstVar->m_byInstrumentTypeLCI;
			pInstrument->m_uiRoutDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			// LCI测点序号为0
			pInstrument->m_iPointIndex = 0;
			// LCI测线序号为0
			pInstrument->m_iLineNb = 0;
			// 得到空闲路由对象
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
		}
		else
		{
			if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP,
											&pHeadFrameThread->m_pRoutList->m_oRoutMap))
			{
				pRout = GetRout(pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP,
					&pHeadFrameThread->m_pRoutList->m_oRoutMap);
				pInstrument->m_uiRoutDirection = pRout->m_uiRoutDirection;
				if (pRout->m_pHead->m_uiRoutIPTop == pInstrument->m_uiRoutIP)
				{
					pRout->m_pHead->m_pInstrumentTop = pInstrument;
					pRout->m_pTail = pInstrument;
					pInstrument->m_pInstrumentDown = pRout->m_pHead;
					pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb - 1;
					pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex;
				}
				else if (pRout->m_pHead->m_uiRoutIPDown == pInstrument->m_uiRoutIP)
				{
					pRout->m_pHead->m_pInstrumentDown = pInstrument;
					pRout->m_pTail = pInstrument;
					pInstrument->m_pInstrumentTop = pRout->m_pHead;
					pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb + 1;
					pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex;
				}
				else
				{
					pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb;
				}
			}
			else
			{
				str.Format(_T("仪器SN = 0x%x，路由IP = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
				// 不是LCI却又找不到路由则记录日志
				AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
									ConvertCStrToStr(str), ErrorType, IDS_ERR_ROUT_NOTEXIT);
			}
		}

		if ((pInstrument->m_uiInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_byInstrumentTypeFDU)
			|| (pInstrument->m_uiInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_byInstrumentTypeLAUL))
		{
			pInstrument->m_uiBroadCastPort = pHeadFrameThread->m_pThread->m_pConstVar->m_iBroadcastPortStart + pInstrument->m_uiRoutIP;
		}
		else
		{
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionTop;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionDown;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionLeft;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
			// 设置交叉站路由
			iDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionRight;
			SetCrossRout(pInstrument, iDirection, pHeadFrameThread->m_pThread->m_pConstVar, pHeadFrameThread->m_pRoutList);
		}
		// 新仪器加入SN索引表
		AddInstrumentToMap(pInstrument->m_uiSN, pInstrument, &pHeadFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
	}
	str.Format(_T("接收到SN = 0x%x的仪器首包帧，首包时刻 = 0x%x，路由IP = 0x%x, 测线号 = %d，测点序号 = %d"), 
		pInstrument->m_uiSN, pInstrument->m_uiTimeHeadFrame, pInstrument->m_uiRoutIP, pInstrument->m_iLineNb, pInstrument->m_iPointIndex);
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", ConvertCStrToStr(str));
}
// 处理首包帧
MatrixServerDll_API void ProcHeadFrame(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
								pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
								pHeadFrameThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket,
									pHeadFrameThread->m_pHeadFrame->m_pcRcvFrameData, 
									pHeadFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
									pHeadFrameThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentHeadFrame(pHeadFrameThread->m_pHeadFrame, 
														pHeadFrameThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ParseInstrumentHeadFrame", 
										"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				}
				else
				{
					EnterCriticalSection(&pHeadFrameThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pHeadFrameThread->m_pRoutList->m_oSecRoutList);
					// 处理单个首包帧
					ProcHeadFrameOne(pHeadFrameThread);
					LeaveCriticalSection(&pHeadFrameThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pHeadFrameThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 线程等待函数
MatrixServerDll_API void WaitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pHeadFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pHeadFrameThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pHeadFrameThread->m_pThread->m_pConstVar->m_iHeadFrameSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pHeadFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pHeadFrameThread->m_pThread->m_bWork == true)
		{
			// 处理首包帧
			ProcHeadFrame(pHeadFrameThread);
		}
		if (pHeadFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitHeadFrameThread(pHeadFrameThread);
	}
	::SetEvent(pHeadFrameThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化首包接收线程
MatrixServerDll_API bool OnInitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	pHeadFrameThread->m_pThread->m_bClose = false;
	pHeadFrameThread->m_pThread->m_bWork = false;
	pHeadFrameThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pHeadFrameThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
							"pHeadFrameThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
							"pHeadFrameThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pHeadFrameThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pHeadFrameThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
															0,
															(LPTHREAD_START_ROUTINE)RunHeadFrameThread,
															pHeadFrameThread, 
															0, 
															&pHeadFrameThread->m_pThread->m_dwThreadID);
	if (pHeadFrameThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", 
							"pHeadFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnInitHeadFrameThread", "首包接收线程创建成功");
	return true;
}
// 关闭首包接收线程
MatrixServerDll_API bool OnCloseHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pHeadFrameThread->m_pThread->m_hThreadClose, 
									pHeadFrameThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
									* pHeadFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pHeadFrameThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnCloseHeadFrameThread", 
							"首包线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnCloseHeadFrameThread", "首包线程成功关闭");
	return true;
}
// 释放首包接收线程
MatrixServerDll_API void OnFreeHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return;
	}
	if (pHeadFrameThread->m_pThread != NULL)
	{
		delete pHeadFrameThread->m_pThread;
	}
	delete pHeadFrameThread;
}
// 创建IP地址设置线程
MatrixServerDll_API m_oIPSetFrameThreadStruct* OnCreateIPSetFrameThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oIPSetFrameThreadStruct* pIPSetFrameThread = NULL;
	pIPSetFrameThread = new m_oIPSetFrameThreadStruct;
	pIPSetFrameThread->m_pThread = new m_oThreadStruct;
	pIPSetFrameThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pIPSetFrameThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pIPSetFrameThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pIPSetFrameThread->m_pThread->m_dwThreadID = 0;
	return pIPSetFrameThread;
}
// 处理单个IP地址设置应答帧
MatrixServerDll_API void  ProcIPSetReturnFrameOne(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	unsigned short usCommand = 0;
	CString str = _T("");
	if (pIPSetFrameThread == NULL)
	{
		return;
	}
	// 得到仪器IP
	uiIPInstrument = pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructReturn->m_uiInstrumentIP;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, &pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap);
		// 从IP地址设置索引表中删除仪器
		DeleteInstrumentFromMap(uiIPInstrument, &pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap);
		// 将仪器加入IP地址索引表
		pInstrument->m_bIPSetOK = true;
		AddInstrumentToMap(uiIPInstrument, pInstrument, &pIPSetFrameThread->m_pInstrumentList->m_oIPInstrumentMap);
		usCommand = pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructReturn->m_usCommand;
		if (usCommand == pIPSetFrameThread->m_pThread->m_pConstVar->m_usSendSetCmd)
		{
			str.Format(_T("接收到SN = 0x%x，IP地址 = 0x%x仪器的IP地址设置应答"), 
				uiIPInstrument, pInstrument->m_uiSN);
		}
		else if (usCommand == pIPSetFrameThread->m_pThread->m_pConstVar->m_usSendQueryCmd)
		{
			str.Format(_T("接收到SN = 0x%x，IP地址 = 0x%x仪器的IP地址查询应答"), 
				uiIPInstrument, pInstrument->m_uiSN);
		}
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "ProcIPSetReturnFrameOne", ConvertCStrToStr(str));
	}
	else
	{
		str.Format(_T("接收到IP地址 = 0x%x的IP地址应答"), uiIPInstrument);
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "ProcIPSetReturnFrameOne", ConvertCStrToStr(str),
							ErrorType, IDS_ERR_IPSETMAP_NOTEXIT);
	}
}
// 处理IP地址设置应答帧
MatrixServerDll_API void ProcIPSetReturnFrame(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pIPSetFrameThread->m_pIPSetFrame->m_oIPSetFrameSocket,
								pIPSetFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
								pIPSetFrameThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pIPSetFrameThread->m_pIPSetFrame->m_oIPSetFrameSocket,
									pIPSetFrameThread->m_pIPSetFrame->m_pcRcvFrameData, 
									pIPSetFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
									pIPSetFrameThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentIPSetReturnFrame(pIPSetFrameThread->m_pIPSetFrame, 
															pIPSetFrameThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "ParseInstrumentIPSetReturnFrame",
										"", ErrorType, IDS_ERR_PARSE_IPSETRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pIPSetFrameThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个IP地址设置应答帧
					ProcIPSetReturnFrameOne(pIPSetFrameThread);
					LeaveCriticalSection(&pIPSetFrameThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 按照IP地址设置索引发送IP地址设置帧
MatrixServerDll_API void ProcIPSetFrame(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return;
	}
	CString str = _T("");
	EnterCriticalSection(&pIPSetFrameThread->m_pInstrumentList->m_oSecInstrumentList);
	// hash_map迭代器
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator  iter;
	// IP地址设置索引不为空
	if (false == pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap.empty())
	{
		// 发送索引表内设备的IP地址设置帧
		for(iter = pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap.begin(); iter != pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap.end();)
		{
			// 仪器SN号
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiSN = iter->second->m_uiSN;
			// 仪器本地IP
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiInstrumentIP = iter->second->m_uiIP;
			// 设置广播端口
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiBroadCastPortSet = iter->second->m_uiBroadCastPort;
			// 路由IP地址，路由方向 1-上
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiRoutIPTop = iter->second->m_uiRoutIPTop;
			// 路由IP地址，路由方向 2-下
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiRoutIPDown = iter->second->m_uiRoutIPDown;
			// 路由IP地址，路由方向 3-左
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiRoutIPLeft = iter->second->m_uiRoutIPLeft;
			// 路由IP地址，路由方向 4-右
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiRoutIPRight = iter->second->m_uiRoutIPRight;
			// 路由开关打开
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_byLAUXRoutOpenSet = iter->second->m_byLAUXRoutOpenSet;
			// 仪器类型
			pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiInstrumentType = iter->second->m_uiInstrumentType;
			// IP地址设置次数为0
			if (iter->second->m_iIPSetCount == 0)
			{
				// 生成IP地址设置帧
				MakeInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 发送IP地址设置帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
			}
			// IP地址设置次数不超过指定次数则重新设置IP地址和查询IP地址
			else if (iter->second->m_iIPSetCount <= pIPSetFrameThread->m_pThread->m_pConstVar->m_iIPAddrResetTimes)
			{
				// 仪器IP地址
				pIPSetFrameThread->m_pIPSetFrame->m_pCommandStructSet->m_uiDstIP = iter->second->m_uiIP;
				// 生成IP地址查询帧
				MakeInstrumentIPQueryFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 发送IP地址查询帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);

				// 生成IP地址设置帧
				MakeInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 发送IP地址设置帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
			}
			// IP地址设置次数超过指定次数则从索引表中删除该仪器指针
			else
			{
				str.Format(_T("仪器SN = 0x%x，IP地址 = 0x%x 的仪器发送IP地址设置帧超过指定次数"), 
					iter->second->m_uiSN, iter->second->m_uiIP);
				// 加入错误日志
				AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "ProcIPSetFrame",
									ConvertCStrToStr(str), WarningType);
				iter->second->m_iIPSetCount = 0;
				pIPSetFrameThread->m_pInstrumentList->m_oIPSetMap.erase(iter++);
			}
		}
	}
	LeaveCriticalSection(&pIPSetFrameThread->m_pInstrumentList->m_oSecInstrumentList);
}
// 线程等待函数
MatrixServerDll_API void WaitIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pIPSetFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pIPSetFrameThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pIPSetFrameThread->m_pThread->m_pConstVar->m_iIPSetFrameSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pIPSetFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pIPSetFrameThread->m_pThread->m_bWork == true)
		{
			// 处理IP地址设置应答帧
			ProcIPSetReturnFrame(pIPSetFrameThread);
			// 按照IP地址设置索引发送IP地址设置帧
			ProcIPSetFrame(pIPSetFrameThread);
		}
		if (pIPSetFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitIPSetFrameThread(pIPSetFrameThread);
	}
	::SetEvent(pIPSetFrameThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化IP地址设置线程
MatrixServerDll_API bool OnInitIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return false;
	}
	pIPSetFrameThread->m_pThread->m_bClose = false;
	pIPSetFrameThread->m_pThread->m_bWork = false;
	pIPSetFrameThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pIPSetFrameThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnInitIPSetFrameThread",
							"pIPSetFrameThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnInitIPSetFrameThread", 
							"pIPSetFrameThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pIPSetFrameThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pIPSetFrameThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
															0,
															(LPTHREAD_START_ROUTINE)RunIPSetFrameThread,
															pIPSetFrameThread, 
															0, 
															&pIPSetFrameThread->m_pThread->m_dwThreadID);
	if (pIPSetFrameThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnInitIPSetFrameThread", 
							"pIPSetFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnInitIPSetFrameThread", "IP地址设置线程创建成功");
	return true;
}
// 关闭IP地址设置线程
MatrixServerDll_API bool OnCloseIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pIPSetFrameThread->m_pThread->m_hThreadClose, 
									pIPSetFrameThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
									* pIPSetFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pIPSetFrameThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnCloseIPSetFrameThread", 
							"IP地址设置线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnCloseIPSetFrameThread", "IP地址设置线程成功关闭");
	return true;
}
// 释放IP地址设置线程
MatrixServerDll_API void OnFreeIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return;
	}
	if (pIPSetFrameThread->m_pThread != NULL)
	{
		delete pIPSetFrameThread->m_pThread;
	}
	delete pIPSetFrameThread;
}
// 创建尾包接收线程
MatrixServerDll_API m_oTailFrameThreadStruct* OnCreateTailFrameThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oTailFrameThreadStruct* pTailFrameThread = NULL;
	pTailFrameThread = new m_oTailFrameThreadStruct;
	pTailFrameThread->m_pThread = new m_oThreadStruct;
	pTailFrameThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pTailFrameThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pTailFrameThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pTailFrameThread->m_pThread->m_dwThreadID = 0;
	return pTailFrameThread;
}
// 线程等待函数
MatrixServerDll_API void WaitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pTailFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pTailFrameThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pTailFrameThread->m_pThread->m_pConstVar->m_iTailFrameSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 清除与该仪器路由方向相同的之前仪器的尾包计数
MatrixServerDll_API void OnClearSameRoutTailCount(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pInstrument == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		return;
	}
	// 下一个仪器指针为空
	m_oInstrumentStruct* pInstrumentNext = NULL;
	pInstrumentNext = GetNextInstrument(pInstrument->m_uiRoutDirection, pRout->m_pHead, pConstVar);
	while(pInstrument != pInstrumentNext)
	{
		if (pInstrumentNext == NULL)
		{
			break;
		}
		pInstrumentNext->m_iTailFrameCount = 0;
		pInstrumentNext = GetNextInstrument(pInstrumentNext->m_uiRoutDirection, pInstrumentNext, pConstVar);
	}
}
// 回收一个仪器
MatrixServerDll_API void FreeInstrumentFromMap(m_oInstrumentStruct* pInstrument, 
												m_oInstrumentListStruct* pInstrumentList, 
												m_oRoutListStruct* pRoutList, 
												m_oConstVarStruct* pConstVar)
{
	if (pInstrument == NULL)
	{
		return;
	}
	if (pInstrumentList == NULL)
	{
		return;
	}
	CString str = _T("");
	m_oRoutStruct* pRout = NULL;
	// 从SN索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiSN, &pInstrumentList->m_oSNInstrumentMap);
	// 从IP地址设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPSetMap);
	// 从IP地址索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPInstrumentMap);
	str.Format(_T("删除仪器的SN = 0x%x，路由 = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
	AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "DeleteInstrumentAlongRout", ConvertCStrToStr(str));
	// 删除仪器的类型为LCI或交叉站
	if ((pInstrument->m_uiInstrumentType == pConstVar->m_byInstrumentTypeLCI)
		|| (pInstrument->m_uiInstrumentType == pConstVar->m_byInstrumentTypeLAUX))
	{
		// 如果删除LCI则路由地址为0的过期路由标志位为true
		if (pInstrument->m_uiInstrumentType == pConstVar->m_byInstrumentTypeLCI)
		{
			pRout = GetRout(pInstrument->m_uiRoutIP, &pRoutList->m_oRoutMap);
			if (pRout != NULL)
			{
				pRout->m_bExpired = true;
			}
		}
		// 仪器上方的过期路由标志位为true
		pRout = GetRout(pInstrument->m_uiRoutIPTop, &pRoutList->m_oRoutMap);
		if (pRout != NULL)
		{
			pRout->m_bExpired = true;
		}
		// 仪器下方的过期路由标志位为true
		pRout = GetRout(pInstrument->m_uiRoutIPDown, &pRoutList->m_oRoutMap);
		if (pRout != NULL)
		{
			pRout->m_bExpired = true;
		}
		// 仪器左方的过期路由标志位为true
		pRout = GetRout(pInstrument->m_uiRoutIPLeft, &pRoutList->m_oRoutMap);
		if (pRout != NULL)
		{
			pRout->m_bExpired = true;
		}
		// 仪器右方的过期路由标志位为true
		pRout = GetRout(pInstrument->m_uiRoutIPRight, &pRoutList->m_oRoutMap);
		if (pRout != NULL)
		{
			pRout->m_bExpired = true;
		}
	}
	// 将仪器加入到空闲仪器队列
	AddFreeInstrument(pInstrument, pInstrumentList);
}
// 删除该路由方向指定仪器之后的仪器
MatrixServerDll_API void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, 
												m_oRoutStruct* pRout, 
												m_oInstrumentListStruct* pInstrumentList, 
												m_oRoutListStruct* pRoutList, 
												m_oConstVarStruct* pConstVar)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	if (pInstrument == NULL)
	{
		AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "DeleteInstrumentAlongRout", 
							"pInstrument", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "DeleteInstrumentAlongRout", 
							"pRout", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pConstVar == NULL)
	{
		AddMsgToLogOutPutList(pInstrumentList->m_pLogOutPut, "DeleteInstrumentAlongRout", 
							"pConstVar", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 前一个仪器指针为路由尾仪器
	m_oInstrumentStruct* pInstrumentPrevious = pRout->m_pTail;
	m_oInstrumentStruct* pInstrumentDelete = NULL;
	CString str = _T("");
	while (pInstrument != pInstrumentPrevious)
	{
		if (pInstrumentPrevious == NULL)
		{
			break;
		}
		pInstrumentDelete = pInstrumentPrevious;
		// 得到要删除仪器沿着路由方向的前一个仪器的指针
		pInstrumentPrevious = GetPreviousInstrument(pInstrumentDelete->m_uiRoutDirection, pInstrumentDelete, pConstVar);
		pRout->m_pTail = pInstrumentPrevious;
		// 回收一个仪器
		FreeInstrumentFromMap(pInstrumentDelete, pInstrumentList, pRoutList, pConstVar);
	}
}
// 处理单个尾包帧
MatrixServerDll_API void ProcTailFrameOne(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	str.Format(_T("接收到SN = 0x%x，路由 = 0x%x 的仪器的尾包"),
		pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSN,pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP);
	// 判断仪器SN是否在SN索引表中
	if(TRUE == IfIndexExistInMap(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSN, 
								&pTailFrameThread->m_pInstrumentList->m_oSNInstrumentMap))
	{
		// 在索引表中则找到该仪器,得到该仪器指针
		pInstrument = GetInstrumentFromMap(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSN, 
										&pTailFrameThread->m_pInstrumentList->m_oSNInstrumentMap);
		// 尾包计数器加一
		pInstrument->m_iTailFrameCount++;
		// 在路由索引表中找到该尾包所在的路由
		if (TRUE == IfIndexExistInRoutMap(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP,
										&pTailFrameThread->m_pRoutList->m_oRoutMap))
		{
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", ConvertCStrToStr(str));
			pRout = GetRout(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP,
							&pTailFrameThread->m_pRoutList->m_oRoutMap);
			// 清除与该仪器路由方向相同的之前仪器的尾包计数
			OnClearSameRoutTailCount(pInstrument, pRout, pTailFrameThread->m_pThread->m_pConstVar);
			// 判断尾包计数器达到设定值则删除相同路由之后的仪器
			if (pInstrument->m_iTailFrameCount > pTailFrameThread->m_pThread->m_pConstVar->m_iTailFrameStableTimes)
			{
				if (pRout->m_pTail != NULL)
				{
					DeleteInstrumentAlongRout(pInstrument, pRout, pTailFrameThread->m_pInstrumentList, 
						pTailFrameThread->m_pRoutList, pTailFrameThread->m_pThread->m_pConstVar);
				}
				pInstrument->m_iTailFrameCount = 0;
			}
			// 更新路由对象的路由时间
			UpdateRoutTime(pRout);
		}
		else
		{
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", ConvertCStrToStr(str), 
								ErrorType, IDS_ERR_ROUT_NOTEXIT);
		}
	}
	else
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", ConvertCStrToStr(str));
	}
}
// 处理尾包帧
MatrixServerDll_API void ProcTailFrame(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
								pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
								pTailFrameThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTailFrameThread->m_pTailFrame->m_oTailFrameSocket,
									pTailFrameThread->m_pTailFrame->m_pcRcvFrameData, 
									pTailFrameThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
									pTailFrameThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTailFrame(pTailFrameThread->m_pTailFrame, 
													pTailFrameThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ParseInstrumentTailFrame", 
										"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				}
				else
				{
					EnterCriticalSection(&pTailFrameThread->m_pInstrumentList->m_oSecInstrumentList);
					EnterCriticalSection(&pTailFrameThread->m_pRoutList->m_oSecRoutList);
					// 处理单个尾包帧
					ProcTailFrameOne(pTailFrameThread);
					LeaveCriticalSection(&pTailFrameThread->m_pRoutList->m_oSecRoutList);
					LeaveCriticalSection(&pTailFrameThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 线程函数
DWORD WINAPI RunTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pTailFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pTailFrameThread->m_pThread->m_bWork == true)
		{
			// 处理尾包接收
			ProcTailFrame(pTailFrameThread);
		}
		if (pTailFrameThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitTailFrameThread(pTailFrameThread);
	}
	::SetEvent(pTailFrameThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化尾包接收线程
MatrixServerDll_API bool OnInitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	pTailFrameThread->m_pThread->m_bClose = false;
	pTailFrameThread->m_pThread->m_bWork = false;
	pTailFrameThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pTailFrameThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread",
							"pTailFrameThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
							"pTailFrameThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pTailFrameThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pTailFrameThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
															0,
															(LPTHREAD_START_ROUTINE)RunTailFrameThread,
															pTailFrameThread, 
															0, 
															&pTailFrameThread->m_pThread->m_dwThreadID);
	if (pTailFrameThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", 
							"pTailFrameThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnInitTailFrameThread", "尾包接收线程创建成功");
	return true;
}
// 关闭尾包接收线程
MatrixServerDll_API bool OnCloseTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pTailFrameThread->m_pThread->m_hThreadClose, 
									pTailFrameThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
									* pTailFrameThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pTailFrameThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
							"尾包接收线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", "尾包接收线程成功关闭");
	return true;
}
// 释放尾包接收线程
MatrixServerDll_API void OnFreeTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return;
	}
	if (pTailFrameThread->m_pThread != NULL)
	{
		delete pTailFrameThread->m_pThread;
	}
	delete pTailFrameThread;
}
// 创建路由监视线程
MatrixServerDll_API m_oMonitorRoutThreadStruct* OnCreateMonitorRoutThread(m_oLogOutPutStruct* pLogOutPut = NULL)
{
	m_oMonitorRoutThreadStruct* pMonitorRoutThread = NULL;
	pMonitorRoutThread = new m_oMonitorRoutThreadStruct;
	pMonitorRoutThread->m_pThread = new m_oThreadStruct;
	pMonitorRoutThread->m_pThread->m_pLogOutPut = pLogOutPut;
	pMonitorRoutThread->m_pThread->m_hThread = INVALID_HANDLE_VALUE;
	pMonitorRoutThread->m_pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pMonitorRoutThread->m_pThread->m_dwThreadID = 0;
	return pMonitorRoutThread;
}
// 线程等待函数
MatrixServerDll_API void WaitMonitorRoutThread(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pMonitorRoutThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pMonitorRoutThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pMonitorRoutThread->m_pThread->m_pConstVar->m_iMonitorRoutSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 处理路由监视
MatrixServerDll_API void ProcMonitorRout(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	unsigned int uiTimeNow = GetTickCount();
	// hash_map迭代器
	hash_map<unsigned int, m_oRoutStruct*>::iterator  iter;
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	for(iter = pMonitorRoutThread->m_pRoutList->m_oRoutMap.begin(); iter != pMonitorRoutThread->m_pRoutList->m_oRoutMap.end();iter++)
	{
		if (uiTimeNow > (iter->second->m_uiRoutTime + pMonitorRoutThread->m_pThread->m_pConstVar->m_iMonitorRoutStableTime))
		{
			pRout = iter->second;
			if (pRout->m_pTail != NULL)
			{
				str.Format(_T("路由IP = 0x%x的路由时间过期，删除该路由方向上的仪器"), pRout->m_uiRoutIP);
				AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "ProcMonitorRout", ConvertCStrToStr(str));
				// 如果路由尾仪器为LCI则表明要删除的是LCI本身
				if (pRout->m_pTail->m_uiInstrumentType == pMonitorRoutThread->m_pThread->m_pConstVar->m_byInstrumentTypeLCI)
				{
					// 回收一个仪器
					FreeInstrumentFromMap(pRout->m_pTail, pMonitorRoutThread->m_pInstrumentList, 
						pMonitorRoutThread->m_pRoutList, pMonitorRoutThread->m_pThread->m_pConstVar);
				}
				else
				{
					DeleteInstrumentAlongRout(pRout->m_pHead, pRout, pMonitorRoutThread->m_pInstrumentList, 
						pMonitorRoutThread->m_pRoutList, pMonitorRoutThread->m_pThread->m_pConstVar);
				}
				pRout->m_pTail = NULL;
			}
		}
	}
	// 回收过期路由
	for(iter = pMonitorRoutThread->m_pRoutList->m_oRoutMap.begin(); iter != pMonitorRoutThread->m_pRoutList->m_oRoutMap.end();)
	{
		pRout = iter->second;
		if (pRout->m_bExpired == true)
		{
			pMonitorRoutThread->m_pRoutList->m_oRoutMap.erase(iter++);
			str.Format(_T("回收路由IP = 0x%x的时间过期路由"), pRout->m_uiRoutIP);
			AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "ProcMonitorRout", ConvertCStrToStr(str));
			// 将过期路由回收到空闲路由队列
			AddFreeRout(pRout, pMonitorRoutThread->m_pRoutList);
		}
		else
		{
			iter++;
		}
	}
}
// 线程函数
DWORD WINAPI RunMonitorRoutThread(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pMonitorRoutThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pMonitorRoutThread->m_pThread->m_bWork == true)
		{
			EnterCriticalSection(&pMonitorRoutThread->m_pInstrumentList->m_oSecInstrumentList);
			EnterCriticalSection(&pMonitorRoutThread->m_pRoutList->m_oSecRoutList);
			// 处理路由监视
			ProcMonitorRout(pMonitorRoutThread);
			LeaveCriticalSection(&pMonitorRoutThread->m_pRoutList->m_oSecRoutList);
			LeaveCriticalSection(&pMonitorRoutThread->m_pInstrumentList->m_oSecInstrumentList);
		}
		if (pMonitorRoutThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitMonitorRoutThread(pMonitorRoutThread);
	}
	::SetEvent(pMonitorRoutThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化路由监视线程
MatrixServerDll_API bool OnInitMonitorRoutThread(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return false;
	}
	pMonitorRoutThread->m_pThread->m_bClose = false;
	pMonitorRoutThread->m_pThread->m_bWork = false;
	pMonitorRoutThread->m_pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pMonitorRoutThread->m_pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnInitMonitorRoutThread",
			"pMonitorRoutThread->m_pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnInitMonitorRoutThread", 
			"pMonitorRoutThread->m_pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
	}
	::ResetEvent(pMonitorRoutThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pMonitorRoutThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
																0,
																(LPTHREAD_START_ROUTINE)RunMonitorRoutThread,
																pMonitorRoutThread, 
																0, 
																&pMonitorRoutThread->m_pThread->m_dwThreadID);
	if (pMonitorRoutThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnInitMonitorRoutThread", 
			"pMonitorRoutThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnInitMonitorRoutThread", "路由监视线程创建成功");
	return true;
}
// 关闭路由监视线程
MatrixServerDll_API bool OnCloseMonitorRoutThread(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pMonitorRoutThread->m_pThread->m_hThreadClose, 
		pMonitorRoutThread->m_pThread->m_pConstVar->m_iCloseThreadSleepTimes
		* pMonitorRoutThread->m_pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pMonitorRoutThread->m_pThread->m_hThread, 0);
		AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnCloseMonitorRoutThread", 
			"路由监视线程强制关闭", WarningType);
		return false;
	}
	AddMsgToLogOutPutList(pMonitorRoutThread->m_pThread->m_pLogOutPut, "OnCloseMonitorRoutThread", "路由监视线程成功关闭");
	return true;
}
// 释放路由监视线程
MatrixServerDll_API void OnFreeMonitorRoutThread(m_oMonitorRoutThreadStruct* pMonitorRoutThread)
{
	if (pMonitorRoutThread == NULL)
	{
		return;
	}
	if (pMonitorRoutThread->m_pThread != NULL)
	{
		delete pMonitorRoutThread->m_pThread;
	}
	delete pMonitorRoutThread;
}
// 初始化实例
MatrixServerDll_API void OnInitInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 创建日志输出结构体
	pEnv->m_pLogOutPut = OnCreateLogOutPut();
	// 创建常量信息结构体
	pEnv->m_pConstVar = OnCreateConstVar(pEnv->m_pLogOutPut);
	// 创建仪器通讯信息结构体
	pEnv->m_pInstrumentCommInfo = OnCreateInstrumentCommInfo(pEnv->m_pLogOutPut);
	// 创建心跳帧信息结构体
	pEnv->m_pHeartBeatFrame = OnCreateInstrumentHeartBeat(pEnv->m_pLogOutPut);
	// 创建首包帧信息结构体
	pEnv->m_pHeadFrame = OnCreateInstrumentHeadFrame(pEnv->m_pLogOutPut);
	// 创建IP地址设置帧信息结构体
	pEnv->m_pIPSetFrame = OnCreateInstrumentIPSetFrame(pEnv->m_pLogOutPut);
	// 创建尾包帧信息结构体
	pEnv->m_pTailFrame = OnCreateInstrumentTailFrame(pEnv->m_pLogOutPut);
	// 创建尾包时刻帧信息结构体
	pEnv->m_pTailTimeFrame = OnCreateInstrumentTailTimeFrame(pEnv->m_pLogOutPut);
	// 创建仪器队列结构体
	pEnv->m_pInstrumentList = OnCreateInstrumentList(pEnv->m_pLogOutPut);
	// 创建路由队列结构体
	pEnv->m_pRoutList = OnCreateRoutList(pEnv->m_pLogOutPut);
	// 创建日志输出线程
	pEnv->m_pLogOutPutThread = OnCreateLogOutPutThread(pEnv->m_pLogOutPut);
 	// 创建心跳线程
 	pEnv->m_pHeartBeatThread = OnCreateHeartBeatThread(pEnv->m_pLogOutPut);
	// 创建首包接收线程
	pEnv->m_pHeadFrameThread = OnCreateHeadFrameThread(pEnv->m_pLogOutPut);
	// 创建IP地址设置线程
	pEnv->m_pIPSetFrameThread = OnCreateIPSetFrameThread(pEnv->m_pLogOutPut);
	// 创建尾包接收线程
	pEnv->m_pTailFrameThread = OnCreateTailFrameThread(pEnv->m_pLogOutPut);
	// 创建路由监视线程
	pEnv->m_pMonitorRoutThread = OnCreateMonitorRoutThread(pEnv->m_pLogOutPut);
	// 初始化日志输出结构体
	OnInitLogOutPut(pEnv->m_pLogOutPut);
	// 初始化常量信息结构体
	OnInitConstVar(pEnv->m_pConstVar);
	// 初始化仪器通讯信息结构体
	OnInitInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	// 初始化套接字库
	OnInitSocketLib(pEnv->m_pLogOutPut);
	// 初始化日志输出线程
	pEnv->m_pLogOutPutThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
	OnInitLogOutPutThread(pEnv->m_pLogOutPutThread);
	// 初始化心跳线程
 	pEnv->m_pHeartBeatThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
 	pEnv->m_pHeartBeatThread->m_pHeartBeatFrame = pEnv->m_pHeartBeatFrame;
 	OnInitHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 初始化首包接收线程
	pEnv->m_pHeadFrameThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
	pEnv->m_pHeadFrameThread->m_pHeadFrame = pEnv->m_pHeadFrame;
	pEnv->m_pHeadFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pHeadFrameThread->m_pRoutList = pEnv->m_pRoutList;
	OnInitHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 初始化IP地址设置线程
	pEnv->m_pIPSetFrameThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
	pEnv->m_pIPSetFrameThread->m_pIPSetFrame = pEnv->m_pIPSetFrame;
	pEnv->m_pIPSetFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	OnInitIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 初始化尾包接收线程
	pEnv->m_pTailFrameThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
	pEnv->m_pTailFrameThread->m_pTailFrame = pEnv->m_pTailFrame;
	pEnv->m_pTailFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pTailFrameThread->m_pRoutList = pEnv->m_pRoutList;
	OnInitTailFrameThread(pEnv->m_pTailFrameThread);
	// 初始化路由监视线程
	pEnv->m_pMonitorRoutThread->m_pThread->m_pConstVar = pEnv->m_pConstVar;
	pEnv->m_pMonitorRoutThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pMonitorRoutThread->m_pRoutList = pEnv->m_pRoutList;
	OnInitMonitorRoutThread(pEnv->m_pMonitorRoutThread);
}
// 初始化
MatrixServerDll_API void OnInit(m_oEnvironmentStruct* pEnv)
{
	OnInitInstance(pEnv);
}
// 关闭
MatrixServerDll_API void OnClose(m_oEnvironmentStruct* pEnv)
{
	// 线程关闭标志位为true
	pEnv->m_pLogOutPutThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pHeartBeatThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pHeadFrameThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pIPSetFrameThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pTailFrameThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pMonitorRoutThread->m_pThread->m_bClose = true;
	// 关闭心跳线程
	OnCloseHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 关闭首包接收线程
	OnCloseHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 关闭IP地址设置线程
	OnCloseIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 关闭尾包接收线程
	OnCloseTailFrameThread(pEnv->m_pTailFrameThread);
	// 关闭路由监视线程
	OnCloseMonitorRoutThread(pEnv->m_pMonitorRoutThread);
	// 关闭日志输出线程
	OnCloseLogOutPutThread(pEnv->m_pLogOutPutThread);

	// 关闭心跳Socket
	OnCloseSocket(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	// 关闭首包Socket
	OnCloseSocket(pEnv->m_pHeadFrame->m_oHeadFrameSocket);
	// 关闭IP地址设置Socket
	OnCloseSocket(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 关闭尾包Socket
	OnCloseSocket(pEnv->m_pTailFrame->m_oTailFrameSocket);
	// 关闭尾包时刻Socket
	OnCloseSocket(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket);
	// 释放套接字库
	OnCloseSocketLib(pEnv->m_pLogOutPut);
	// 释放常量资源
	OnCloseConstVar(pEnv->m_pConstVar);
	// 释放心跳资源
	OnCloseInstrumentHeartBeat(pEnv->m_pHeartBeatFrame);
	// 释放首包资源
	OnCloseInstrumentHeadFrame(pEnv->m_pHeadFrame);
	// 释放IP地址设置资源
	OnCloseInstrumentIPSetFrame(pEnv->m_pIPSetFrame);
	// 释放尾包资源
	OnCloseInstrumentTailFrame(pEnv->m_pTailFrame);
	// 释放尾包时刻资源
	OnCloseInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame);
	// 释放仪器队列资源
	OnCloseInstrumentList(pEnv->m_pInstrumentList);
	// 释放路由队列资源
	OnCloseRoutList(pEnv->m_pRoutList);
	// 关闭日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPut);
}
// 工作
MatrixServerDll_API void OnWork(m_oEnvironmentStruct* pEnv)
{
	// 初始化心跳
	OnWorkInstrumentHeartBeat(pEnv->m_pHeartBeatFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化首包
	OnWorkInstrumentHeadFrame(pEnv->m_pHeadFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化IP地址设置
	OnWorkInstrumentIPSetFrame(pEnv->m_pIPSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包
	OnWorkInstrumentTailFrame(pEnv->m_pTailFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包时刻
	OnWorkInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化仪器队列结构体
	OnWorkInstrumentList(pEnv->m_pInstrumentList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化路由队列结构体
	OnWorkRoutList(pEnv->m_pRoutList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 关闭心跳Socket
	OnCloseSocket(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	// 创建并设置心跳端口
	OnCreateAndSetHeartBeatSocket(pEnv->m_pHeartBeatFrame);
	// 关闭首包Socket
	OnCloseSocket(pEnv->m_pHeadFrame->m_oHeadFrameSocket);
	// 创建并设置首包端口
	OnCreateAndSetHeadFrameSocket(pEnv->m_pHeadFrame);
	// 关闭IP地址设置Socket
	OnCloseSocket(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 创建并设置IP地址设置端口
	OnCreateAndSetIPSetFrameSocket(pEnv->m_pIPSetFrame);
	// 关闭尾包Socket
	OnCloseSocket(pEnv->m_pTailFrame->m_oTailFrameSocket);
	// 创建并设置尾包端口
	OnCreateAndSetTailFrameSocket(pEnv->m_pTailFrame);
	// 关闭尾包时刻Socket
	OnCloseSocket(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket);
	// 创建并设置尾包端口
	OnCreateAndSetTailTimeFrameSocket(pEnv->m_pTailTimeFrame);
	// 日志输出线程开始工作
	pEnv->m_pLogOutPutThread->m_pThread->m_bWork = true;
 	// 心跳线程开始工作
 	pEnv->m_pHeartBeatThread->m_pThread->m_bWork = true;
	// 首包接收线程开始工作
	pEnv->m_pHeadFrameThread->m_pThread->m_bWork = true;
	// IP地址设置线程开始工作
	pEnv->m_pIPSetFrameThread->m_pThread->m_bWork = true;
	// 尾包接收线程开始工作
	pEnv->m_pTailFrameThread->m_pThread->m_bWork = true;
	// 路由监视线程开始工作
	pEnv->m_pMonitorRoutThread->m_pThread->m_bWork = true;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPut, "OnWork", "开始工作");
}
// 停止
MatrixServerDll_API void OnStop(m_oEnvironmentStruct* pEnv)
{
	// 日志输出线程停止工作
	pEnv->m_pLogOutPutThread->m_pThread->m_bWork = false;
	// 心跳线程停止工作
	pEnv->m_pHeartBeatThread->m_pThread->m_bWork = false;
	// 首包接收线程停止工作
	pEnv->m_pHeadFrameThread->m_pThread->m_bWork = false;
	// IP地址设置线程停止工作
	pEnv->m_pIPSetFrameThread->m_pThread->m_bWork = false;
	// 尾包接收线程停止工作
	pEnv->m_pTailFrameThread->m_pThread->m_bWork = false;
	// 路由监视线程停止工作
	pEnv->m_pMonitorRoutThread->m_pThread->m_bWork = false;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPut, "OnStop", "停止工作");
}
// 释放实例资源
MatrixServerDll_API void OnFreeInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 释放日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPut);
	// 释放INI文件导入常量结构体资源
	OnFreeConstVar(pEnv->m_pConstVar);
	// 释放XML文件导入的通讯信息设置结构体资源
	OnFreeInstrumentCommInfo(pEnv->m_pInstrumentCommInfo);
	// 释放心跳帧结构体资源
	OnFreeInstrumentHeartBeat(pEnv->m_pHeartBeatFrame);
	// 释放首包帧结构体资源
	OnFreeInstrumentHeadFrame(pEnv->m_pHeadFrame);
	// 释放IP地址设置帧结构体资源
	OnFreeInstrumentIPSetFrame(pEnv->m_pIPSetFrame);
	// 释放尾包帧结构体资源
	OnFreeInstrumentTailFrame(pEnv->m_pTailFrame);
	// 释放尾包时刻帧结构体资源
	OnFreeInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame);
	// 释放仪器队列结构体资源
	OnFreeInstrumentList(pEnv->m_pInstrumentList);
	// 释放路由队列结构体资源
	OnFreeRoutList(pEnv->m_pRoutList);
	// 释放日志输出线程
	OnFreeLogOutPutThread(pEnv->m_pLogOutPutThread);
	// 释放心跳线程
	OnFreeHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 释放首包接收线程
	OnFreeHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 释放IP地址设置线程
	OnFreeIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 释放尾包接收线程
	OnFreeTailFrameThread(pEnv->m_pTailFrameThread);
	// 释放路由监视线程
	OnFreeMonitorRoutThread(pEnv->m_pMonitorRoutThread);
	delete pEnv;
	pEnv = NULL;
}