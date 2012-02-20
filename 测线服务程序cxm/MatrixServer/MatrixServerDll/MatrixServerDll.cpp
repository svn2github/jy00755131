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
// 解析INI文件数组常量
MATRIXSERVERDLL_API void ParseCStringToArray(char** pData, int iSize, CString str)
{
	if ((*pData) != NULL)
	{
		delete[] (*pData);
	}
	*pData = new char[iSize];
	int iDirectionOld = 0;
	int iDirectionNew = 0;
	CString csTemp = _T("");
	int iTemp = 0;
	for (int i=0; i<iSize; i++)
	{
		iDirectionNew = str.Find(_T("0x"), iDirectionOld);
		iDirectionNew += 2;
		csTemp = str.Mid(iDirectionNew, 2);
		_stscanf_s(csTemp, _T("%x"), &iTemp, sizeof(int));
		(*pData)[i] = static_cast<char>(iTemp);
		iDirectionOld = iDirectionNew;
	}
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
	errno_t err;
	// 创建程序运行日志文件夹
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	if (OutPutSelect == 1)
	{
		if (pLogOutPut->m_byLogFileType != OptLogType)
		{
			LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
			return;
		}
	}
	CreateDirectory(pLogOutPut->m_csSaveLogFilePath, NULL);
	pLogOutPut->m_uiLogFileNb++;
	str.Format(_T("\\%d.log"),pLogOutPut->m_uiLogFileNb);
	pLogOutPut->m_uiLogInfoCount = 0;
	err = fopen_s(&pLogOutPut->m_pFile, ConvertCStrToStr(pLogOutPut->m_csSaveLogFilePath + str).c_str(), "wt+, ccs=UNICODE");
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
	if (OutPutSelect == 1)
	{
		if (pLogOutPut->m_byLogFileType != OptLogType)
		{
			LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
			return;
		}
	}
	if (pLogOutPut->m_uiErrorCount > OutPutLogErrorLimit)
	{
		LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
		return;
	}
	pLogOutPut->m_oLogOutputList.push_back(str);
	if (byErrorType == ErrorType)
	{
		pLogOutPut->m_uiErrorCount++;
	}
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
}
// 日志输出写入文件
MatrixServerDll_API void LogOutPutWriteInFile(FILE* pFile, string str, bool bFlush = false)
{
	if (pFile == NULL)
	{
		return;
	}
	size_t strSize = str.length();
	fwrite(str.c_str(), sizeof(char), strSize, pFile);
	if (bFlush == true)
	{
		fflush(pFile);
	}
}
// 写入日志输出文件
MatrixServerDll_API void WriteLogOutPutListToFile(m_oLogOutPutStruct* pLogOutPut)
{
	list<string>::iterator iter;
	string str = "";
	errno_t err;
	bool bOpenNewFile = false;
	if (pLogOutPut == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLogOutPut->m_oSecLogFile);
	if (OutPutSelect == 1)
	{
		if (pLogOutPut->m_byLogFileType != OptLogType)
		{
			LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
			return;
		}
	}
	while(pLogOutPut->m_oLogOutputList.empty() == false)
	{
		iter = pLogOutPut->m_oLogOutputList.begin();
		str += *iter;
		pLogOutPut->m_oLogOutputList.pop_front();
		pLogOutPut->m_uiLogInfoCount++;
	}
	if (pLogOutPut->m_byLogFileType == OptLogType)
	{
		LogOutPutWriteInFile(pLogOutPut->m_pFile, str, true);
	}
	else
	{
		LogOutPutWriteInFile(pLogOutPut->m_pFile, str, false);
	}
	if (pLogOutPut->m_uiLogInfoCount > OutPutLogFileInfoNumLimit)
	{
		err = fclose( pLogOutPut->m_pFile );
		bOpenNewFile = true;
	}
	LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
	if (bOpenNewFile == true)
	{
		OpenLogOutPutFile(pLogOutPut);
	}
}
// 关闭日志输出文件
MatrixServerDll_API void CloseLogOutPutFile(m_oLogOutPutStruct* pLogOutPut)
{
	if (pLogOutPut == NULL)
	{
		return;
	}
	if (OutPutSelect == 1)
	{
		if (pLogOutPut->m_byLogFileType != OptLogType)
		{
			return;
		}
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
	if (OutPutSelect == 1)
	{
		if (pLogOutPut->m_byLogFileType != OptLogType)
		{
			LeaveCriticalSection(&pLogOutPut->m_oSecLogFile);
			return;
		}
	}
	// 日志文件指针为空
	pLogOutPut->m_pFile = NULL;
	// 日志错误计数清零
	pLogOutPut->m_uiErrorCount = 0;
	// 清空日志输出队列
	pLogOutPut->m_oLogOutputList.clear();
	// 日志文件序号设定
	pLogOutPut->m_uiLogFileNb = 0;
	// 日志文件存储信息条数记数
	pLogOutPut->m_uiLogInfoCount = 0;
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
	pCommand->m_uiLocalTimeFixedLow = 0;
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
	pCommand->m_uiBroadCastPortSet = 0;
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
	// 尾包接收时刻，低14位有效
	pCommand->m_usTailRecTime = 0;
	// 尾包发送时刻//交叉站尾包发送时刻，低14位有效
	pCommand->m_usTailSndTime = 0;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	pCommand->m_uiBroadCastPortSeted = 0;
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
	pCommand->m_iADCSetNum = 0;
	// ADC数据指针偏移量
	pCommand->m_usADCDataPoint = 0;
	// ADC数据采集时仪器本地时间
	pCommand->m_uiADCSampleSysTime = 0;
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
	unsigned int uiRoutAddrNum = 0;
	int iPos = 0;
	int iTemp = 0;
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
	// 如果为ADC采样数据帧
	if (pCommand->m_usCommand == pConstVar->m_usSendADCCmd)
	{
		// ADC数据指针偏移量
		memcpy(&pCommand->m_usADCDataPoint, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
		iPos += pConstVar->m_iFramePacketSize2B;
		// ADC数据采集时仪器本地时间
		memcpy(&pCommand->m_uiADCSampleSysTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
		iPos += pConstVar->m_iFramePacketSize4B;
		iPos += pConstVar->m_iFramePacketSize2B;
		for (int i=0; i<pConstVar->m_iADCDataInOneFrameNum; i++)
		{
			memcpy(&iTemp, &pFrameData[iPos], pConstVar->m_iADCDataSize3B);
			iPos += pConstVar->m_iADCDataSize3B;
			if (iTemp > 0x800000)
			{
				iTemp = -(0xffffff - iTemp);
			}
			pCommand->m_pADCData[i] = iTemp;
		}
		return true;
	}
	// 解析帧内容
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
			memcpy(&pCommand->m_uiLocalTimeFixedLow, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
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
		else if (byCommandWord == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_uiBroadCastPortSet, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
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
		else if (byCommandWord == pConstVar->m_byCmdTailRecSndTime)
		{
			memcpy(&pCommand->m_usTailRecTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usTailRecTime &= 0x3fff;
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usTailSndTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usTailSndTime &= 0x3fff;
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdBroadCastPortSeted)
		{
			memcpy(&pCommand->m_uiBroadCastPortSeted, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCSet)
		{
			memcpy(&pCommand->m_pcADCSet[pCommand->m_iADCSetNum], &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
			pCommand->m_iADCSetNum += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pFrameData[iPos], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLineA &= 0x3fff;
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLineB &= 0x3fff;
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLAUXLineA &= 0x3fff;
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pFrameData[iPos], pConstVar->m_iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLAUXLineB &= 0x3fff;
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
	// 如果为ADC数据重发帧
	if (pCommand->m_usCommand == pConstVar->m_usSendADCCmd)
	{
		// 指针偏移量
		memcpy(&pFrameData[iPos], &pCommand->m_usADCDataPoint, pConstVar->m_iFramePacketSize2B);
		iPos += pConstVar->m_iFramePacketSize2B;
		// 截止
		pFrameData[iPos] = pConstVar->m_byCmdEnd;
		return true;
	}
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
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedLow, pConstVar->m_iFramePacketSize4B);
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
		else if (pCommandWord[i] == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiBroadCastPortSet, pConstVar->m_iFramePacketSize4B);
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
		else if (pCommandWord[i] == pConstVar->m_byCmdTailRecSndTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTailRecTime, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usTailSndTime, pConstVar->m_iFramePacketSize2B);
			iPos += pConstVar->m_iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdBroadCastPortSeted)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiBroadCastPortSeted, pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCSet)
		{
			if (pCommand->m_iADCSetNum == pConstVar->m_iFramePacketSize4B)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_pcADCSet[0], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else
			{
				memcpy(&pFrameData[iPos], &pCommand->m_pcADCSet[0], pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
				for (int j = pConstVar->m_iFramePacketSize4B; j < pCommand->m_iADCSetNum; j += pConstVar->m_iFramePacketSize4B)
				{
					memcpy(&pFrameData[iPos], &pConstVar->m_byCmdADCSet, pConstVar->m_iFrameCmdSize1B);
					iPos += pConstVar->m_iFrameCmdSize1B;
					memcpy(&pFrameData[iPos], &pCommand->m_pcADCSet[j], pConstVar->m_iFramePacketSize4B);
					iPos += pConstVar->m_iFramePacketSize4B;
				}
			}
			memcpy(&pFrameData[iPos], &pCommand->m_pcADCSet[pCommand->m_iADCSetNum], pConstVar->m_iFramePacketSize4B);
			iPos += pConstVar->m_iFramePacketSize4B;
			pCommand->m_iADCSetNum += pConstVar->m_iFramePacketSize4B;
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
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPLeft, pConstVar->m_iFramePacketSize4B);
				iPos += pConstVar->m_iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 1)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPRight, pConstVar->m_iFramePacketSize4B);
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
			memset(&pFrameData[iPos], pConstVar->m_bySndFrameBufInit, pConstVar->m_iFramePacketSize4B);
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
// 清空接收帧缓冲区
MatrixServerDll_API void ClearRcvFrameBuf(SOCKET oSocket, char* pFrameData, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	sockaddr_in AddrFrom;
	int iSize = sizeof(AddrFrom);
	DWORD dwFrameCount = 0;
	int iCount = 0;
	while (true)
	{
		// 得到网络接收缓冲区数据字节数
		if (SOCKET_ERROR == ioctlsocket(oSocket, FIONREAD, &dwFrameCount))
		{
			AddMsgToLogOutPutList(pLogOutPut, "ClearRcvFrameBuf", "ioctlsocket", 
				ErrorType, WSAGetLastError());
			break;
		}
		else
		{
			if(dwFrameCount > 0) 
			{
				iCount = recvfrom(oSocket, pFrameData, iRcvFrameSize, 0, reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
				if (iCount == SOCKET_ERROR)
				{
					AddMsgToLogOutPutList(pLogOutPut, "ClearRcvFrameBuf", "recvfrom", 
						ErrorType, WSAGetLastError());
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
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
MatrixServerDll_API m_oConstVarStruct* OnCreateConstVar(void)
{
	m_oConstVarStruct* pConstVar = NULL;
	pConstVar = new m_oConstVarStruct;
	pConstVar->m_pFrameHeadCheck = NULL;
	pConstVar->m_pSetADCSetSine = NULL;
	pConstVar->m_pSetADCStopSample = NULL;
	pConstVar->m_pSetADCOpenTBPowerLow = NULL;
	pConstVar->m_pSetADCOpenTBPowerHigh = NULL;
	pConstVar->m_pSetADCOpenSwitchTBLow = NULL;
	pConstVar->m_pSetADCOpenSwitchTBHigh = NULL;
	pConstVar->m_pSetADCRegisterRead = NULL;
	pConstVar->m_pSetADCRegisterWrite = NULL;
	pConstVar->m_pSetADCTBSwitchOpen = NULL;
	pConstVar->m_pSetADCSample = NULL;
	pConstVar->m_pSetADCReadContinuous = NULL;
	pConstVar->m_pLogOutPut = NULL;
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

		strSectionKey=_T("MonitorSleepTimes");		// 路由监视线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iMonitorSleepTimes = _ttoi(strValue);

		strSectionKey=_T("TimeDelaySleepTimes");		// 时统设置线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iTimeDelaySleepTimes = _ttoi(strValue);

		strSectionKey=_T("ADCSetSleepTimes");			// ADC参数设置线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCSetSleepTimes = _ttoi(strValue);

		strSectionKey=_T("ErrorCodeSleepTimes");		// 误码查询线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iErrorCodeSleepTimes = _ttoi(strValue);

		strSectionKey=_T("ADCDataRecSleepTimes");		// ADC数据接收线程延时次数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCDataRecSleepTimes = _ttoi(strValue);

		strSectionKey=_T("CloseThreadSleepTimes");		// 等待线程关闭的延时次数
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

		strSectionKey=_T("MonitorStableTime");		// 路由监视稳定时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iMonitorStableTime = _ttoi(strValue);

		strSectionKey=_T("LineSysStableTime");		// 测网系统达到稳定状态时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iLineSysStableTime = _ttoi(strValue);

		strSectionKey=_T("ADCSetOptNb");			// ADC参数设置操作序号
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCSetOptNb = _ttoi(strValue);

		strSectionKey=_T("ADCStartSampleOptNb");	// ADC开始采集操作序号
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCStartSampleOptNb = _ttoi(strValue);

		strSectionKey=_T("ADCStopSampleOptNb");		// ADC停止采集操作序号
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCStopSampleOptNb = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLAUX");		// 仪器类型-交叉站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iInstrumentTypeLAUX = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLAUL");		// 仪器类型-电源站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iInstrumentTypeLAUL = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeFDU");		// 仪器类型-采集站
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iInstrumentTypeFDU = _ttoi(strValue);

		strSectionKey=_T("InstrumentTypeLCI");		// 仪器类型-LCI
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iInstrumentTypeLCI= _ttoi(strValue);

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
		ParseCStringToArray(&pConstVar->m_pFrameHeadCheck, pConstVar->m_iFrameHeadSize, strValue);

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

		strSectionKey=_T("ADCDataInOneFrameNum");	// 一帧内ADC数据个数
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCDataInOneFrameNum = _ttoi(strValue);

		strSectionKey=_T("ADCFramePointLimit");	// ADC数据帧指针偏移量上限
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iADCFramePointLimit = _ttoi(strValue);

		strSectionKey=_T("CommandWordMaxNum");	// 命令字个数最大值
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iCommandWordMaxNum = _ttoi(strValue);
		pConstVar->m_iADCSetCommandMaxByte = pConstVar->m_iCommandWordMaxNum * pConstVar->m_iFramePacketSize4B;
		strSectionKey=_T("SndFrameBufInit");	// 发送帧缓冲区初值设定
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_bySndFrameBufInit = static_cast<char>(iTemp);

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
		pConstVar->m_usSendSetCmd = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("SendQueryCmd");		// 发送查询命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usSendQueryCmd = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("SendADCCmd");			// 发送ADC采样数据重发命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usSendADCCmd = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("CmdSn");				// 串号
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdSn = static_cast<char>(iTemp);

		strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdHeadFrameTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_byCmdLocalIPAddr, sizeof(char));

		strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalSysTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalTimeFixedHigh = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLocalTimeFixedLow = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnAddr = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnPort = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCDataReturnPortLimit = static_cast<char>(iTemp);

		strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdBroadCastPortSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdFDUErrorCode = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTBHigh");				// TB时刻高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTBHigh = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTbLow");				// TB时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTbLow = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXRoutOpenQuery = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXRoutOpenSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdTailRecSndTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdBroadCastPortSeted = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdADCSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdNetTime");				// 网络时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdNetTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLineTailRecTimeLAUX = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUTailRecTimeLAUX = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXErrorCode1 = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXErrorCode2 = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdLAUXSetRout = static_cast<char>(iTemp);

		strSectionKey=_T("CmdReturnRout");			// 返回路由
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdReturnRout = static_cast<char>(iTemp);

		strSectionKey=_T("CmdEnd");					// 命令解析结束命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_byCmdEnd = static_cast<char>(iTemp);

		//读取ini文件中相应字段的内容
		strSection = _T("ADC参数设置");			// 获取当前区域
		strSectionKey=_T("SetADCSetSineSize");	// ADC设置正弦波命令大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCSetSineSize = _ttoi(strValue);

		strSectionKey=_T("SetADCSetSine");		// ADC设置正弦波命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCSetSine, pConstVar->m_iSetADCSetSineSize, strValue);

		strSectionKey=_T("SetADCStopSampleSize");	// ADC设置停止采样命令大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCStopSampleSize = _ttoi(strValue);

		strSectionKey=_T("SetADCStopSample");		// ADC设置停止采样命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCStopSample, pConstVar->m_iSetADCStopSampleSize, strValue);

		strSectionKey=_T("SetADCOpenTBPowerLowSize");	// ADC设置打开TB电源低位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenTBPowerLowSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenTBPowerLow");		// ADC设置打开TB电源低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCOpenTBPowerLow, pConstVar->m_iSetADCOpenTBPowerLowSize, strValue);

		strSectionKey=_T("m_iSetADCOpenTBPowerHighSize");	// ADC设置打开TB电源高位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenTBPowerHighSize = _ttoi(strValue);

		strSectionKey=_T("m_iSetADCOpenTBPowerHigh");		// ADC设置打开TB电源高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCOpenTBPowerHigh, pConstVar->m_iSetADCOpenTBPowerHighSize, strValue);

		strSectionKey=_T("SetADCOpenSwitchTBLowSize");	// ADC设置打开TB开关低位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenSwitchTBLowSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenSwitchTBLow");		// ADC设置打开TB开关低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCOpenSwitchTBLow, pConstVar->m_iSetADCOpenSwitchTBLowSize, strValue);

		strSectionKey=_T("SetADCOpenSwitchTBHighSize");	// ADC设置打开TB开关高位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenSwitchTBHighSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenSwitchTBHigh");		// ADC设置打开TB开关高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCOpenSwitchTBHigh, pConstVar->m_iSetADCOpenSwitchTBHighSize, strValue);

		strSectionKey=_T("SetADCRegisterReadSize");	// ADC设置读寄存器大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCRegisterReadSize = _ttoi(strValue);

		strSectionKey=_T("SetADCRegisterRead");		// ADC设置读寄存器
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCRegisterRead, pConstVar->m_iSetADCRegisterReadSize, strValue);

		strSectionKey=_T("SetADCRegisterWriteSize");	// ADC设置写寄存器大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCRegisterWriteSize = _ttoi(strValue);

		strSectionKey=_T("SetADCRegisterWrite");		// ADC设置写寄存器
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCRegisterWrite, pConstVar->m_iSetADCRegisterWriteSize, strValue);

		strSectionKey=_T("SetADCTBSwitchOpenSize");	// ADC设置打开TB开关大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCTBSwitchOpenSize = _ttoi(strValue);

		strSectionKey=_T("SetADCTBSwitchOpen");		// ADC设置打开TB开关
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCTBSwitchOpen, pConstVar->m_iSetADCTBSwitchOpenSize, strValue);

		strSectionKey=_T("SetADCSampleSize");	// ADC采样设置大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCSampleSize = _ttoi(strValue);

		strSectionKey=_T("SetADCSample");		// ADC采样设置
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCSample, pConstVar->m_iSetADCSampleSize, strValue);

		strSectionKey=_T("SetADCReadContinuousSize");	// ADC设置连续采样大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCReadContinuousSize = _ttoi(strValue);

		strSectionKey=_T("SetADCReadContinuous");		// ADC设置连续采样
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_pSetADCReadContinuous, pConstVar->m_iSetADCReadContinuousSize, strValue);

		strSectionKey=_T("TBSleepTimeHigh");			// TB设置延时高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_uiTBSleepTimeHigh, sizeof(int));

		strSectionKey=_T("TBSleepTimeLow");				// TB设置延时低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usTBSleepTimeLow = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("CmdTBCtrlStartSample");		// TB开关控制ADC数据采集命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usCmdTBCtrlStartSample = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("CmdTBLoseCtrlStartSample");	// 无需TB开关控制ADC数据采集命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usCmdTBLoseCtrlStartSample = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("CmdTBCtrlStopSample");		// TB开关控制ADC数据停止采集命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usCmdTBCtrlStopSample = static_cast<unsigned short>(iTemp);

		strSectionKey=_T("CmdCtrlCloseLed");			// LED灯灭
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_usCmdCtrlCloseLed = static_cast<unsigned short>(iTemp);
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
MatrixServerDll_API void OnInitConstVar(m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (pConstVar == NULL)
	{
		return;
	}
	pConstVar->m_pLogOutPut = pLogOutPut;
	LoadIniFile(pConstVar);
}
// 关闭常量信息结构体
MatrixServerDll_API void OnCloseConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar->m_pFrameHeadCheck != NULL)
	{
		delete[] pConstVar->m_pFrameHeadCheck;
		pConstVar->m_pFrameHeadCheck = NULL;
	}
	if (pConstVar->m_pSetADCSetSine != NULL)
	{
		delete[] pConstVar->m_pSetADCSetSine;
		pConstVar->m_pSetADCSetSine = NULL;
	}
	if (pConstVar->m_pSetADCStopSample != NULL)
	{
		delete[] pConstVar->m_pSetADCStopSample;
		pConstVar->m_pSetADCStopSample = NULL;
	}
	if (pConstVar->m_pSetADCOpenTBPowerLow != NULL)
	{
		delete[] pConstVar->m_pSetADCOpenTBPowerLow;
		pConstVar->m_pSetADCOpenTBPowerLow = NULL;
	}
	if (pConstVar->m_pSetADCOpenTBPowerHigh != NULL)
	{
		delete[] pConstVar->m_pSetADCOpenTBPowerHigh;
		pConstVar->m_pSetADCOpenTBPowerHigh = NULL;
	}
	if (pConstVar->m_pSetADCOpenSwitchTBLow != NULL)
	{
		delete[] pConstVar->m_pSetADCOpenSwitchTBLow;
		pConstVar->m_pSetADCOpenSwitchTBLow = NULL;
	}
	if (pConstVar->m_pSetADCOpenSwitchTBHigh != NULL)
	{
		delete[] pConstVar->m_pSetADCOpenSwitchTBHigh;
		pConstVar->m_pSetADCOpenSwitchTBHigh = NULL;
	}
	if (pConstVar->m_pSetADCRegisterRead != NULL)
	{
		delete[] pConstVar->m_pSetADCRegisterRead;
		pConstVar->m_pSetADCRegisterRead = NULL;
	}
	if (pConstVar->m_pSetADCRegisterWrite != NULL)
	{
		delete[] pConstVar->m_pSetADCRegisterWrite;
		pConstVar->m_pSetADCRegisterWrite = NULL;
	}
	if (pConstVar->m_pSetADCTBSwitchOpen != NULL)
	{
		delete[] pConstVar->m_pSetADCTBSwitchOpen;
		pConstVar->m_pSetADCTBSwitchOpen = NULL;
	}
	if (pConstVar->m_pSetADCSample != NULL)
	{
		delete[] pConstVar->m_pSetADCSample;
		pConstVar->m_pSetADCSample = NULL;
	}
	if (pConstVar->m_pSetADCReadContinuous != NULL)
	{
		delete[] pConstVar->m_pSetADCReadContinuous;
		pConstVar->m_pSetADCReadContinuous = NULL;
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
MatrixServerDll_API m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	pCommInfo->m_pLogOutPut = NULL;
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
		// LCI的IP地址
		strKey = _T("IPLCI");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pCommInfo->m_uiAimIP = inet_addr(ConvertCStrToStr(csDstIP).c_str());
		// ADC数据返回地址
		strKey = _T("IPForADCData");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		pCommInfo->m_uiADCDataReturnAddr = inet_addr(ConvertCStrToStr(csDstIP).c_str());
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
		pCommInfo->m_usAimPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeartBeat");
		pCommInfo->m_usHeartBeatReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeadFrame");
		pCommInfo->m_usHeadFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForIPSet");
		pCommInfo->m_usIPSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailFrame");
		pCommInfo->m_usTailFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailTimeFrame");
		pCommInfo->m_usTailTimeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTimeSet");
		pCommInfo->m_usTimeDelayReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCSet");
		pCommInfo->m_usADCSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForErrorCode");
		pCommInfo->m_usErrorCodeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCData");
		pCommInfo->m_usADCDataReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
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
MatrixServerDll_API void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	pCommInfo->m_pLogOutPut = pLogOutPut;
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
MatrixServerDll_API m_oHeartBeatFrameStruct* OnCreateInstrumentHeartBeat(void)
{
	m_oHeartBeatFrameStruct* pHeartBeatFrame = NULL;
	pHeartBeatFrame = new m_oHeartBeatFrameStruct;
	InitializeCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	pHeartBeatFrame->m_pcSndFrameData = NULL;
	pHeartBeatFrame->m_pcCommandWord = NULL;
	pHeartBeatFrame->m_oHeartBeatSocket = INVALID_SOCKET;
	pHeartBeatFrame->m_pCommandStruct = NULL;
	return pHeartBeatFrame;
}
// 初始化心跳
MatrixServerDll_API void OnInitInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
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
	pHeartBeatFrame->m_pCommandStruct->m_usAimPort = pCommInfo->m_usAimPort;
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
	memset(pHeartBeatFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空心跳命令字集合
	if (pHeartBeatFrame->m_pcCommandWord != NULL)
	{
		delete pHeartBeatFrame->m_pcCommandWord;
	}
	pHeartBeatFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pHeartBeatFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
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
		pHeartBeatFrame->m_pcSndFrameData = NULL;
	}
	if (pHeartBeatFrame->m_pcCommandWord != NULL)
	{
		delete[] pHeartBeatFrame->m_pcCommandWord;
		pHeartBeatFrame->m_pcCommandWord = NULL;
	}
	if (pHeartBeatFrame->m_pCommandStruct != NULL)
	{
		delete pHeartBeatFrame->m_pCommandStruct;
		pHeartBeatFrame->m_pCommandStruct = NULL;
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
MatrixServerDll_API m_oHeadFrameStruct* OnCreateInstrumentHeadFrame(void)
{
	m_oHeadFrameStruct* pHeadFrame = NULL;
	pHeadFrame = new m_oHeadFrameStruct;
	InitializeCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	pHeadFrame->m_pcRcvFrameData = NULL;
	pHeadFrame->m_oHeadFrameSocket = INVALID_SOCKET;
	pHeadFrame->m_pCommandStruct = NULL;
	return pHeadFrame;
}
// 初始化首包
MatrixServerDll_API void OnInitInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
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
	memset(pHeadFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
}
// 关闭首包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame)
{
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	if (pHeadFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pHeadFrame->m_pcRcvFrameData;
		pHeadFrame->m_pcRcvFrameData = NULL;
	}
	if (pHeadFrame->m_pCommandStruct != NULL)
	{
		delete pHeadFrame->m_pCommandStruct;
		pHeadFrame->m_pCommandStruct = NULL;
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
MatrixServerDll_API m_oIPSetFrameStruct* OnCreateInstrumentIPSetFrame(void)
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
	return pIPSetFrame;
}
// 初始化IP地址设置
MatrixServerDll_API void OnInitInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
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
	pIPSetFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
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
	memset(pIPSetFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空IP地址设置命令字集合
	if (pIPSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_pcCommandWord;
	}
	pIPSetFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pIPSetFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
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
	memset(pIPSetFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 关闭IP地址设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	if (pIPSetFrame->m_pcSndFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcSndFrameData;
		pIPSetFrame->m_pcSndFrameData = NULL;
	}
	if (pIPSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pIPSetFrame->m_pcCommandWord;
		pIPSetFrame->m_pcCommandWord = NULL;
	}
	if (pIPSetFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pIPSetFrame->m_pcRcvFrameData;
		pIPSetFrame->m_pcRcvFrameData = NULL;
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
MatrixServerDll_API m_oTailFrameStruct* OnCreateInstrumentTailFrame(void)
{
	m_oTailFrameStruct* pTailFrame = NULL;
	pTailFrame = new m_oTailFrameStruct;
	InitializeCriticalSection(&pTailFrame->m_oSecTailFrame);
	pTailFrame->m_pcRcvFrameData = NULL;
	pTailFrame->m_oTailFrameSocket = INVALID_SOCKET;
	pTailFrame->m_pCommandStruct = NULL;
	return pTailFrame;
}
// 初始化尾包
MatrixServerDll_API void OnInitInstrumentTailFrame(m_oTailFrameStruct* pTailFrame, 
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
	memset(pTailFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
}
// 关闭尾包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailFrame(m_oTailFrameStruct* pTailFrame)
{
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	if (pTailFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailFrame->m_pcRcvFrameData;
		pTailFrame->m_pcRcvFrameData = NULL;
	}
	if (pTailFrame->m_pCommandStruct != NULL)
	{
		delete pTailFrame->m_pCommandStruct;
		pTailFrame->m_pCommandStruct = NULL;
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
MatrixServerDll_API m_oTailTimeFrameStruct* OnCreateInstrumentTailTimeFrame(void)
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
	return pTailTimeFrame;
}
// 初始化尾包时刻帧
MatrixServerDll_API void OnInitInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
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
	pTailTimeFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
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
	memset(pTailTimeFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空尾包时刻查询命令字集合
	if (pTailTimeFrame->m_pcCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pcCommandWord;
	}
	pTailTimeFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pTailTimeFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
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
	memset(pTailTimeFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 关闭尾包时刻帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	if (pTailTimeFrame->m_pcSndFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcSndFrameData;
		pTailTimeFrame->m_pcSndFrameData = NULL;
	}
	if (pTailTimeFrame->m_pcCommandWord != NULL)
	{
		delete[] pTailTimeFrame->m_pcCommandWord;
		pTailTimeFrame->m_pcCommandWord = NULL;
	}
	if (pTailTimeFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTailTimeFrame->m_pcRcvFrameData;
		pTailTimeFrame->m_pcRcvFrameData = NULL;
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
MatrixServerDll_API void OnFreeInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame)
{
	if (pTailTimeFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	delete pTailTimeFrame;
}
// 创建时统设置帧信息结构体
MatrixServerDll_API m_oTimeDelayFrameStruct* OnCreateInstrumentTimeDelayFrame(void)
{
	m_oTimeDelayFrameStruct* pTimeDelayFrame = NULL;
	pTimeDelayFrame = new m_oTimeDelayFrameStruct;
	InitializeCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	pTimeDelayFrame->m_pcRcvFrameData = NULL;
	pTimeDelayFrame->m_pcSndFrameData = NULL;
	pTimeDelayFrame->m_pcCommandWord = NULL;
	pTimeDelayFrame->m_oTimeDelayFrameSocket = INVALID_SOCKET;
	pTimeDelayFrame->m_pCommandStructSet = NULL;
	pTimeDelayFrame->m_pCommandStructReturn = NULL;
	return pTimeDelayFrame;
}
// 初始化时统设置帧
MatrixServerDll_API void OnInitInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pTimeDelayFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTimeDelayFrame", "pTimeDelayFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentTimeDelayFrame", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	if (pTimeDelayFrame->m_pCommandStructSet != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructSet;
	}
	pTimeDelayFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pTimeDelayFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
	// 时统设置发送缓冲区帧数设定为仪器个数
	pTimeDelayFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// 时统设置应答接收缓冲区帧数设定为仪器个数
	pTimeDelayFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 时统设置返回端口
	pTimeDelayFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usTimeDelayReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pTimeDelayFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pTimeDelayFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pTimeDelayFrame->m_pcSndFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_pcSndFrameData;
	}
	pTimeDelayFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pTimeDelayFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空时统设置命令字集合
	if (pTimeDelayFrame->m_pcCommandWord != NULL)
	{
		delete[] pTimeDelayFrame->m_pcCommandWord;
	}
	pTimeDelayFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pTimeDelayFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
	// 时统设置命令字个数
	pTimeDelayFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pTimeDelayFrame->m_pCommandStructReturn != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructReturn;
	}
	pTimeDelayFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pTimeDelayFrame->m_pCommandStructReturn);
	// 清空接收帧缓冲区
	if (pTimeDelayFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_pcRcvFrameData;
	}
	pTimeDelayFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pTimeDelayFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 关闭时统设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame)
{
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	if (pTimeDelayFrame->m_pcSndFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_pcSndFrameData;
		pTimeDelayFrame->m_pcSndFrameData = NULL;
	}
	if (pTimeDelayFrame->m_pcCommandWord != NULL)
	{
		delete[] pTimeDelayFrame->m_pcCommandWord;
		pTimeDelayFrame->m_pcCommandWord = NULL;
	}
	if (pTimeDelayFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pTimeDelayFrame->m_pcRcvFrameData;
		pTimeDelayFrame->m_pcRcvFrameData = NULL;
	}
	if (pTimeDelayFrame->m_pCommandStructSet != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructSet;
		pTimeDelayFrame->m_pCommandStructSet = NULL;
	}
	if (pTimeDelayFrame->m_pCommandStructReturn != NULL)
	{
		delete pTimeDelayFrame->m_pCommandStructReturn;
		pTimeDelayFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 释放时统设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame)
{
	if (pTimeDelayFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	delete pTimeDelayFrame;
}
// 创建ADC参数设置帧信息结构体
MatrixServerDll_API m_oADCSetFrameStruct* OnCreateInstrumentADCSetFrame(void)
{
	m_oADCSetFrameStruct* pADCSetFrame = NULL;
	pADCSetFrame = new m_oADCSetFrameStruct;
	InitializeCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	pADCSetFrame->m_pcRcvFrameData = NULL;
	pADCSetFrame->m_pcSndFrameData = NULL;
	pADCSetFrame->m_pcCommandWord = NULL;
	pADCSetFrame->m_oADCSetFrameSocket = INVALID_SOCKET;
	pADCSetFrame->m_pCommandStructSet = NULL;
	pADCSetFrame->m_pCommandStructReturn = NULL;
	return pADCSetFrame;
}
// 初始化ADC参数设置帧
MatrixServerDll_API void OnInitInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pADCSetFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentADCSetFrame", "pADCSetFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentADCSetFrame", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	if (pADCSetFrame->m_pCommandStructSet != NULL)
	{
		delete pADCSetFrame->m_pCommandStructSet;
	}
	pADCSetFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pADCSetFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pADCSetFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pADCSetFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
	// ADC参数设置发送缓冲区帧数设定为仪器个数
	pADCSetFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// ADC参数设置应答接收缓冲区帧数设定为仪器个数
	pADCSetFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// ADC参数设置返回端口
	pADCSetFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usADCSetReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pADCSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pADCSetFrame->m_pCommandStructSet);
	// ADC数据返回地址
	pADCSetFrame->m_pCommandStructSet->m_uiADCDataReturnAddr = pCommInfo->m_uiADCDataReturnAddr;
	// ADC数据返回端口
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPort = pCommInfo->m_usADCDataReturnPort;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnCmd = pConstVar->m_usSendADCCmd;
	// 端口递增下限
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPortLimitLow = pCommInfo->m_usADCDataReturnPort;
	// 端口递增上限
	pADCSetFrame->m_pCommandStructSet->m_usADCDataReturnPortLimitHigh = pCommInfo->m_usADCDataReturnPort;
	// 清空发送帧缓冲区
	if (pADCSetFrame->m_pcSndFrameData != NULL)
	{
		delete[] pADCSetFrame->m_pcSndFrameData;
	}
	pADCSetFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pADCSetFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空时统设置命令字集合
	if (pADCSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pADCSetFrame->m_pcCommandWord;
	}
	pADCSetFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pADCSetFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
	// ADC参数设置命令字个数
	pADCSetFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pADCSetFrame->m_pCommandStructReturn != NULL)
	{
		delete pADCSetFrame->m_pCommandStructReturn;
	}
	pADCSetFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pADCSetFrame->m_pCommandStructReturn);
	pADCSetFrame->m_pCommandStructReturn->m_pcADCSet = new char[pConstVar->m_iSndFrameSize];
	// 清空接收帧缓冲区
	if (pADCSetFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pADCSetFrame->m_pcRcvFrameData;
	}
	pADCSetFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pADCSetFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}
// 关闭ADC参数设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame)
{
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	if (pADCSetFrame->m_pcSndFrameData != NULL)
	{
		delete[] pADCSetFrame->m_pcSndFrameData;
		pADCSetFrame->m_pcSndFrameData = NULL;
	}
	if (pADCSetFrame->m_pcCommandWord != NULL)
	{
		delete[] pADCSetFrame->m_pcCommandWord;
		pADCSetFrame->m_pcCommandWord = NULL;
	}
	if (pADCSetFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pADCSetFrame->m_pcRcvFrameData;
		pADCSetFrame->m_pcRcvFrameData = NULL;
	}
	if (pADCSetFrame->m_pCommandStructSet != NULL)
	{
		delete pADCSetFrame->m_pCommandStructSet;
		pADCSetFrame->m_pCommandStructSet = NULL;
	}
	if (pADCSetFrame->m_pCommandStructReturn != NULL)
	{
		if (pADCSetFrame->m_pCommandStructReturn->m_pcADCSet != NULL)
		{
			delete[] pADCSetFrame->m_pCommandStructReturn->m_pcADCSet;
			pADCSetFrame->m_pCommandStructReturn->m_pcADCSet = NULL;
		}
		delete pADCSetFrame->m_pCommandStructReturn;
		pADCSetFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}
// 释放ADC参数设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame)
{
	if (pADCSetFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	delete pADCSetFrame;
}
// 创建误码查询帧信息结构体
MatrixServerDll_API m_oErrorCodeFrameStruct* OnCreateInstrumentErrorCodeFrame(void)
{
	m_oErrorCodeFrameStruct* pErrorCodeFrame = NULL;
	pErrorCodeFrame = new m_oErrorCodeFrameStruct;
	InitializeCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	pErrorCodeFrame->m_pcRcvFrameData = NULL;
	pErrorCodeFrame->m_pcSndFrameData = NULL;
	pErrorCodeFrame->m_oErrorCodeFrameSocket = INVALID_SOCKET;
	pErrorCodeFrame->m_pcCommandWord = NULL;
	pErrorCodeFrame->m_pCommandStructSet = NULL;
	pErrorCodeFrame->m_pCommandStructReturn = NULL;
	return pErrorCodeFrame;
}
// 初始化误码查询帧
MatrixServerDll_API void OnInitInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pErrorCodeFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentErrorCodeFrame", "pErrorCodeFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnWorkInstrumentErrorCodeFrame", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	if (pErrorCodeFrame->m_pCommandStructSet != NULL)
	{
		delete pErrorCodeFrame->m_pCommandStructSet;
	}
	pErrorCodeFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pErrorCodeFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pErrorCodeFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pErrorCodeFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
	// 误码查询发送缓冲区帧数设定为仪器个数
	pErrorCodeFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// 误码查询应答接收缓冲区帧数设定为仪器个数
	pErrorCodeFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// 误码查询返回端口
	pErrorCodeFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usErrorCodeReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pErrorCodeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pErrorCodeFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pErrorCodeFrame->m_pcSndFrameData != NULL)
	{
		delete[] pErrorCodeFrame->m_pcSndFrameData;
	}
	pErrorCodeFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pErrorCodeFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);
	// 清空误码查询命令字集合
	if (pErrorCodeFrame->m_pcCommandWord != NULL)
	{
		delete[] pErrorCodeFrame->m_pcCommandWord;
	}
	pErrorCodeFrame->m_pcCommandWord = new char[pConstVar->m_iCommandWordMaxNum];
	memset(pErrorCodeFrame->m_pcCommandWord, pConstVar->m_bySndFrameBufInit, pConstVar->m_iCommandWordMaxNum);
	// 误码查询命令字个数
	pErrorCodeFrame->m_usCommandWordNum = 0;

	// 重置帧内容解析变量
	if (pErrorCodeFrame->m_pCommandStructReturn != NULL)
	{
		delete pErrorCodeFrame->m_pCommandStructReturn;
	}
	pErrorCodeFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pErrorCodeFrame->m_pCommandStructReturn);
	pErrorCodeFrame->m_pCommandStructReturn->m_pcADCSet = new char[pConstVar->m_iSndFrameSize];
	// 清空接收帧缓冲区
	if (pErrorCodeFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pErrorCodeFrame->m_pcRcvFrameData;
	}
	pErrorCodeFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pErrorCodeFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
}
// 关闭误码查询帧信息结构体
MatrixServerDll_API void OnCloseInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame)
{
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	if (pErrorCodeFrame->m_pcSndFrameData != NULL)
	{
		delete[] pErrorCodeFrame->m_pcSndFrameData;
		pErrorCodeFrame->m_pcSndFrameData = NULL;
	}
	if (pErrorCodeFrame->m_pcCommandWord != NULL)
	{
		delete[] pErrorCodeFrame->m_pcCommandWord;
		pErrorCodeFrame->m_pcCommandWord = NULL;
	}
	if (pErrorCodeFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pErrorCodeFrame->m_pcRcvFrameData;
		pErrorCodeFrame->m_pcRcvFrameData = NULL;
	}
	if (pErrorCodeFrame->m_pCommandStructSet != NULL)
	{
		delete pErrorCodeFrame->m_pCommandStructSet;
		pErrorCodeFrame->m_pCommandStructSet = NULL;
	}
	if (pErrorCodeFrame->m_pCommandStructReturn != NULL)
	{
		delete pErrorCodeFrame->m_pCommandStructReturn;
		pErrorCodeFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
}
// 释放误码查询帧信息结构体
MatrixServerDll_API void OnFreeInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame)
{
	if (pErrorCodeFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	delete pErrorCodeFrame;
}
// 创建ADC数据帧信息结构体
MatrixServerDll_API m_oADCDataFrameStruct* OnCreateInstrumentADCDataFrame(void)
{
	m_oADCDataFrameStruct* pADCDataFrame = NULL;
	pADCDataFrame = new m_oADCDataFrameStruct;
	InitializeCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	pADCDataFrame->m_pcRcvFrameData = NULL;
	pADCDataFrame->m_pcSndFrameData = NULL;
	pADCDataFrame->m_oADCDataFrameSocket = INVALID_SOCKET;
	pADCDataFrame->m_pCommandStructSet = NULL;
	pADCDataFrame->m_pCommandStructReturn = NULL;
	return pADCDataFrame;
}
// 初始化ADC数据帧
MatrixServerDll_API void OnInitInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pADCDataFrame == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentADCDataFrame", "pADCDataFrame",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitInstrumentADCDataFrame", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	if (pADCDataFrame->m_pCommandStructSet != NULL)
	{
		delete pADCDataFrame->m_pCommandStructSet;
	}
	pADCDataFrame->m_pCommandStructSet = new m_oInstrumentCommandStruct;
	// 源地址
	pADCDataFrame->m_pCommandStructSet->m_uiSrcIP = pCommInfo->m_uiSrcIP;
	// 目的地址
	pADCDataFrame->m_pCommandStructSet->m_uiAimIP = pCommInfo->m_uiAimIP;
	// 目标IP地址端口号
	pADCDataFrame->m_pCommandStructSet->m_usAimPort = pCommInfo->m_usAimPort;
	// ADC数据帧发送缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiSndBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iSndFrameSize;
	// ADC数据帧接收缓冲区帧数设定为仪器个数
	pADCDataFrame->m_uiRcvBufferSize = pCommInfo->m_uiInstrumentNum * pConstVar->m_iRcvFrameSize;
	// ADC数据帧返回端口
	pADCDataFrame->m_pCommandStructSet->m_usReturnPort = pCommInfo->m_usADCDataReturnPort;
	// 重置帧内通讯信息
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	pADCDataFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendADCCmd;
	// 重置帧内容解析变量
	ResetInstrumentFramePacket(pADCDataFrame->m_pCommandStructSet);
	// 清空发送帧缓冲区
	if (pADCDataFrame->m_pcSndFrameData != NULL)
	{
		delete[] pADCDataFrame->m_pcSndFrameData;
	}
	pADCDataFrame->m_pcSndFrameData = new char[pConstVar->m_iSndFrameSize];
	memset(pADCDataFrame->m_pcSndFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iSndFrameSize);

	// 重置帧内容解析变量
	if (pADCDataFrame->m_pCommandStructReturn != NULL)
	{
		delete pADCDataFrame->m_pCommandStructReturn;
	}
	pADCDataFrame->m_pCommandStructReturn = new m_oInstrumentCommandStruct;
	ResetInstrumentFramePacket(pADCDataFrame->m_pCommandStructReturn);
	pADCDataFrame->m_pCommandStructReturn->m_pADCData = new int[pConstVar->m_iADCDataInOneFrameNum];
	// 清空接收帧缓冲区
	if (pADCDataFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pADCDataFrame->m_pcRcvFrameData;
	}
	pADCDataFrame->m_pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	memset(pADCDataFrame->m_pcRcvFrameData, pConstVar->m_bySndFrameBufInit, pConstVar->m_iRcvFrameSize);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 关闭ADC数据帧信息结构体
MatrixServerDll_API void OnCloseInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	if (pADCDataFrame->m_pcSndFrameData != NULL)
	{
		delete[] pADCDataFrame->m_pcSndFrameData;
		pADCDataFrame->m_pcSndFrameData = NULL;
	}
	if (pADCDataFrame->m_pcRcvFrameData != NULL)
	{
		delete[] pADCDataFrame->m_pcRcvFrameData;
		pADCDataFrame->m_pcRcvFrameData = NULL;
	}
	if (pADCDataFrame->m_pCommandStructSet != NULL)
	{
		delete pADCDataFrame->m_pCommandStructSet;
		pADCDataFrame->m_pCommandStructSet = NULL;
	}
	if (pADCDataFrame->m_pCommandStructReturn != NULL)
	{
		if (pADCDataFrame->m_pCommandStructReturn->m_pADCData != NULL)
		{
			delete[] pADCDataFrame->m_pCommandStructReturn->m_pADCData;
		}
		delete pADCDataFrame->m_pCommandStructReturn;
		pADCDataFrame->m_pCommandStructReturn = NULL;
	}
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
}
// 释放ADC数据帧信息结构体
MatrixServerDll_API void OnFreeInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame)
{
	if (pADCDataFrame == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	delete pADCDataFrame;
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
MatrixServerDll_API void OnCreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	EnterCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	// 创建套接字
	pHeartBeatFrame->m_oHeartBeatSocket = CreateInstrumentSocket(pHeartBeatFrame->m_pCommandStruct->m_usReturnPort, 
		pHeartBeatFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pHeartBeatFrame->m_oHeartBeatSocket, pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetHeartBeatSocket", "创建并设置心跳端口！");
}

// 创建并设置首包端口
MatrixServerDll_API void OnCreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	EnterCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	// 创建套接字
	pHeadFrame->m_oHeadFrameSocket = CreateInstrumentSocket(pHeadFrame->m_pCommandStruct->m_usReturnPort, 
		pHeadFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pHeadFrame->m_oHeadFrameSocket, pHeadFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pHeadFrame->m_oSecHeadFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetHeadFrameSocket", "创建并设置首包端口！");
}
// 创建并设置IP地址设置端口
MatrixServerDll_API void OnCreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
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
// 创建并设置尾包端口
MatrixServerDll_API void OnCreateAndSetTailFrameSocket(m_oTailFrameStruct* pTailFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	EnterCriticalSection(&pTailFrame->m_oSecTailFrame);
	// 创建套接字
	pTailFrame->m_oTailFrameSocket = CreateInstrumentSocket(pTailFrame->m_pCommandStruct->m_usReturnPort, 
		pTailFrame->m_pCommandStruct->m_uiSrcIP, pLogOutPut);
	// 设置接收缓冲区
	SetRcvBufferSize(pTailFrame->m_oTailFrameSocket, pTailFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTailFrame->m_oSecTailFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTailFrameSocket", "创建并设置尾包端口！");
}
// 创建并设置尾包时刻端口
MatrixServerDll_API void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 创建套接字
	pTailTimeFrame->m_oTailTimeFrameSocket = CreateInstrumentSocket(pTailTimeFrame->m_pCommandStructSet->m_usReturnPort, 
		pTailTimeFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pTailTimeFrame->m_oTailTimeFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTailTimeFrameSocket", "创建并设置尾包时刻查询帧端口！");
}
// 创建并设置时统设置端口
MatrixServerDll_API void OnCreateAndSetTimeDelayFrameSocket(m_oTimeDelayFrameStruct* pTimeDelayFrame, m_oLogOutPutStruct* pLogOutPut = NULL)
{
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 创建套接字
	pTimeDelayFrame->m_oTimeDelayFrameSocket = CreateInstrumentSocket(pTimeDelayFrame->m_pCommandStructSet->m_usReturnPort, 
		pTimeDelayFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pTimeDelayFrame->m_oTimeDelayFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pTimeDelayFrame->m_oTimeDelayFrameSocket, pTimeDelayFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pTimeDelayFrame->m_oTimeDelayFrameSocket, pTimeDelayFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetTimeDelayFrameSocket", "创建并设置时统设置帧端口！");
}
// 创建并设置ADC参数设置端口
MatrixServerDll_API void OnCreateAndSetADCSetFrameSocket(m_oADCSetFrameStruct* pADCSetFrame, m_oLogOutPutStruct* pLogOutPut)
{
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	// 创建套接字
	pADCSetFrame->m_oADCSetFrameSocket = CreateInstrumentSocket(pADCSetFrame->m_pCommandStructSet->m_usReturnPort, 
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
// 创建并设置误码查询端口
MatrixServerDll_API void OnCreateAndSetErrorCodeFrameSocket(m_oErrorCodeFrameStruct* pErrorCodeFrame, m_oLogOutPutStruct* pLogOutPut)
{
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	// 创建套接字
	pErrorCodeFrame->m_oErrorCodeFrameSocket = CreateInstrumentSocket(pErrorCodeFrame->m_pCommandStructSet->m_usReturnPort, 
		pErrorCodeFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置为广播端口
	SetInstrumentSocketBroadCast(pErrorCodeFrame->m_oErrorCodeFrameSocket, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pErrorCodeFrame->m_oErrorCodeFrameSocket, pErrorCodeFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pErrorCodeFrame->m_oErrorCodeFrameSocket, pErrorCodeFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetErrorCodeFrameSocket", "创建并设置误码查询帧端口！");
}
// 创建并设置ADC数据帧端口
MatrixServerDll_API void OnCreateAndSetADCDataFrameSocket(m_oADCDataFrameStruct* pADCDataFrame, m_oLogOutPutStruct* pLogOutPut)
{
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	// 创建套接字
	pADCDataFrame->m_oADCDataFrameSocket = CreateInstrumentSocket(pADCDataFrame->m_pCommandStructSet->m_usReturnPort, 
		pADCDataFrame->m_pCommandStructSet->m_uiSrcIP, pLogOutPut);
	// 设置发送缓冲区
	SetSndBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiSndBufferSize, pLogOutPut);
	// 设置接收缓冲区大小
	SetRcvBufferSize(pADCDataFrame->m_oADCDataFrameSocket, pADCDataFrame->m_uiRcvBufferSize, pLogOutPut);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	AddMsgToLogOutPutList(pLogOutPut, "OnCreateAndSetADCDataFrameSocket", "创建并设置ADC数据帧端口！");
}
// 清空接收缓冲区
MatrixServerDll_API void OnClearSocketRcvBuf(SOCKET oSocket, m_oConstVarStruct* pConstVar)
{
	// 帧数量设置为0
	int iFrameCount = 0;
	// 接收缓冲区
	char* pcRcvFrameData = NULL;
	pcRcvFrameData = new char[pConstVar->m_iRcvFrameSize];
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(oSocket,pConstVar->m_iRcvFrameSize);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			GetFrameData(oSocket, pcRcvFrameData, pConstVar->m_iRcvFrameSize);		
		}		
	}
	delete[] pcRcvFrameData;
}

// 仪器信息重置
MatrixServerDll_API void OnInstrumentReset(m_oInstrumentStruct* pInstrument)
{
	// 仪器是否使用中
	pInstrument->m_bInUsed = false;
	// 仪器设备号
	pInstrument->m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	pInstrument->m_iInstrumentType = 0;
	// 	// 仪器IP地址
	// 	pInstrument->m_uiIP = 0;
	// 仪器路由IP地址
	pInstrument->m_uiRoutIP = 0;
	// 路由方向 1-上；2-下；3-左；4右
	pInstrument->m_iRoutDirection = 0;
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
	pInstrument->m_uiSystemTime = 0;
	/** 仪器网络时间*/
	pInstrument->m_uiNetTime = 0;
	/** 仪器网络状态*/
	pInstrument->m_uiNetState = 0;
	/** 仪器参数备用1*/
	pInstrument->m_uiNetOrder = 0;

	// 首包时刻
	pInstrument->m_uiTimeHeadFrame = 0;
// 	// 尾包时刻
// 	pInstrument->m_uiTailSysTime = 0;
	// 尾包计数
	pInstrument->m_iTailFrameCount = 0;
	// 仪器时延
	pInstrument->m_uiDelayTime = 0;

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
	/** 尾包时刻查询应答次数*/
	pInstrument->m_iTailTimeReturnCount = 0;
	// 尾包时刻查询是否成功
	pInstrument->m_bTailTimeQueryOK = false;

	// 本地时间修正高位
	pInstrument->m_uiTimeHigh = 0;
	// 本地时间修正低位
	pInstrument->m_uiTimeLow = 0;
	// 第几次设置仪器时间
	pInstrument->m_iTimeSetCount = 0;
	/** 仪器时统成功次数*/
	pInstrument->m_iTimeSetReturnCount = 0;
	// 仪器时间设置是否成功
	pInstrument->m_bTimeSetOK = false;
	// ADC命令设置是否应答
	pInstrument->m_bADCSetReturn = false;
	// 仪器是否进行了ADC参数设置
	pInstrument->m_bADCSet = false;
	// 仪器开始ADC数据采集设置成功
	pInstrument->m_bADCStartSample = false;
	// 仪器停止ADC数据采集设置成功
	pInstrument->m_bADCStopSample = false;

	// 误码查询发送帧数
	pInstrument->m_uiErrorCodeQueryNum = 0;
	// 误码查询应答帧数
	pInstrument->m_uiErrorCodeReturnNum = 0;
	// 采集站和电源站网络数据错误计数
	pInstrument->m_iFDUErrorCodeDataCount = 0;
	// 采集站和电源站命令错误计数
	pInstrument->m_iFDUErrorCodeCmdCount = 0;
	// 采集站和电源站网络数据错误计数
	pInstrument->m_byFDUErrorCodeDataCountOld = 0;
	// 采集站和电源站命令错误计数
	pInstrument->m_byFDUErrorCodeCmdCountOld = 0;

	// 交叉站大线A数据故障
	pInstrument->m_iLAUXErrorCodeDataLineACount = 0;
	// 交叉站大线B数据故障
	pInstrument->m_iLAUXErrorCodeDataLineBCount = 0;
	// 交叉站交叉线A数据故障
	pInstrument->m_iLAUXErrorCodeDataLAUXLineACount = 0;
	// 交叉站交叉线B数据故障
	pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount = 0;
	// 交叉站命令口故障
	pInstrument->m_iLAUXErrorCodeCmdCount = 0;

	// 交叉站大线A数据故障
	pInstrument->m_byLAUXErrorCodeDataLineACountOld = 0;
	// 交叉站大线B数据故障
	pInstrument->m_byLAUXErrorCodeDataLineBCountOld = 0;
	// 交叉站交叉线A数据故障
	pInstrument->m_byLAUXErrorCodeDataLAUXLineACountOld = 0;
	// 交叉站交叉线B数据故障
	pInstrument->m_byLAUXErrorCodeDataLAUXLineBCountOld = 0;
	// 交叉站命令口故障
	pInstrument->m_byLAUXErrorCodeCmdCountOld = 0;

	// 实际接收ADC数据帧数
	pInstrument->m_uiADCDataActualRecFrameNum = 0;
	// 应该接收ADC数据帧数（含丢帧）
	pInstrument->m_uiADCDataShouldRecFrameNum = 0;
	// ADC数据帧的指针偏移量
	pInstrument->m_usADCDataFramePoint = 0;
	// ADC数据帧发送时的本地时间
	pInstrument->m_uiADCDataFrameSysTime = 0;
}
// 创建仪器队列结构体
MatrixServerDll_API m_oInstrumentListStruct* OnCreateInstrumentList(void)
{
	m_oInstrumentListStruct* pInstrumentList = NULL;
	pInstrumentList = new m_oInstrumentListStruct;
	InitializeCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	pInstrumentList->m_pArrayInstrument = NULL;
	return pInstrumentList;
}
// 初始化仪器队列结构体
MatrixServerDll_API void OnInitInstrumentList(m_oInstrumentListStruct* pInstrumentList,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InitInstrumentList", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pInstrumentList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InitInstrumentList", "pInstrumentList",
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
	// 清空未完成ADC参数设置的仪器队列
	pInstrumentList->m_oADCSetInstrumentMap.clear();
	// 测网系统发生变化的时间
	pInstrumentList->m_ullLineChangeTime = 0;
	// 测网状态为不稳定
	pInstrumentList->m_bLineStableChange = false;
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
	// 清空未完成ADC参数设置的仪器队列
	pInstrumentList->m_oADCSetInstrumentMap.clear();
	// 删除仪器数组
	if (pInstrumentList->m_pArrayInstrument != NULL)
	{
		delete[] pInstrumentList->m_pArrayInstrument;
		pInstrumentList->m_pArrayInstrument = NULL;
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
// 增加一个IP地址设置仪器
MatrixServerDll_API void AddInstrumentToMap(unsigned int uiIndex, m_oInstrumentStruct* pInstrument, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	if (false == IfIndexExistInMap(uiIndex, pMap))
	{
		pMap->insert(hash_map<unsigned int, m_oInstrumentStruct*>::value_type (uiIndex, pInstrument));
	}
}
// 根据输入索引号，由索引表得到仪器指针
MatrixServerDll_API m_oInstrumentStruct* GetInstrumentFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	return iter->second;
}
// 从索引表删除索引号指向的仪器指针
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
	pRout->m_iRoutDirection = 0;
	// 路由头仪器
	pRout->m_pHead = NULL;
	// 路由尾仪器
	pRout->m_pTail = NULL;
	// 路由时刻
	pRout->m_ullRoutTime = 0xFFFFFFFF00000000;
	// 路由是否为交叉线路由
	pRout->m_bRoutLaux = false;
	// 路由是否接收到ADC设置参数应答
	pRout->m_bADCSetReturn = false;
	// 路由是否发送ADC参数设置帧
	pRout->m_bADCSetRout = true;
	// 该路由方向上仪器的个数
	pRout->m_uiInstrumentNum = 0;
}
// 更新路由对象的路由时间
MatrixServerDll_API void UpdateRoutTime(m_oRoutStruct* pRout)
{
	// 路由时刻
	pRout->m_ullRoutTime = GetTickCount64();
}

// 更新上次测网系统变化时刻
MatrixServerDll_API void UpdateLineChangeTime(m_oInstrumentListStruct* pInstrumentList)
{
	// 上次测网系统变化时刻
	pInstrumentList->m_ullLineChangeTime = GetTickCount64();
}
// 创建路由队列结构体
MatrixServerDll_API m_oRoutListStruct* OnCreateRoutList(void)
{
	m_oRoutListStruct* pRoutList = NULL;
	pRoutList = new m_oRoutListStruct;
	InitializeCriticalSection(&pRoutList->m_oSecRoutList);
	pRoutList->m_pArrayRout = NULL;
	return pRoutList;
}
// 初始化路由队列结构体
MatrixServerDll_API void OnInitRoutList(m_oRoutListStruct* pRoutList,
	m_oInstrumentCommInfoStruct* pCommInfo,
	m_oConstVarStruct* pConstVar)
{
	if (pRoutList == NULL)
	{
		return;
	}
	if (pCommInfo == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InitRoutList", "pCommInfo",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空需要删除的路由地址索引
	pRoutList->m_oRoutDeleteMap.clear();
	// 清空ADC参数设置任务索引
	pRoutList->m_oADCSetRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 清空仪器时统的任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
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
	// 清空需要删除的路由地址索引
	pRoutList->m_oRoutDeleteMap.clear();
	// 清空ADC参数设置任务索引
	pRoutList->m_oADCSetRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 清空仪器时统的任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 删除路由数组
	if (pRoutList->m_pArrayRout != NULL)
	{
		delete[] pRoutList->m_pArrayRout;
		pRoutList->m_pArrayRout = NULL;
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
// 增加一个路由
MatrixServerDll_API void AddRout(unsigned int uiIndex, 
	m_oRoutStruct* pRout, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	if (false == IfIndexExistInRoutMap(uiIndex, pRoutMap))
	{
		pRoutMap->insert(hash_map<unsigned int, m_oRoutStruct*>::value_type (uiIndex, pRout));
	}
}
// 根据输入索引号，由索引表得到一个路由指针
MatrixServerDll_API m_oRoutStruct* GetRout(unsigned int uiIndex, hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiIndex);
	return iter->second;
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
// 解析尾包时刻查询帧
MatrixServerDll_API bool ParseInstrumentTailTimeReturnFrame(m_oTailTimeFrameStruct* pTailTimeFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	bReturn = ParseInstrumentFrame(pTailTimeFrame->m_pCommandStructReturn, pTailTimeFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	return bReturn;
}
// 解析时统设置应答帧
MatrixServerDll_API bool ParseInstrumentTimeDelayReturnFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	bReturn = ParseInstrumentFrame(pTimeDelayFrame->m_pCommandStructReturn, pTimeDelayFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	return bReturn;
}
// 解析ADC参数设置应答帧
MatrixServerDll_API bool ParseInstrumentADCSetReturnFrame(m_oADCSetFrameStruct* pADCSetFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	bReturn = ParseInstrumentFrame(pADCSetFrame->m_pCommandStructReturn, pADCSetFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	return bReturn;
}
// 解析误码查询应答帧
MatrixServerDll_API bool ParseInstrumentErrorCodeReturnFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	bReturn = ParseInstrumentFrame(pErrorCodeFrame->m_pCommandStructReturn, pErrorCodeFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	return bReturn;
}
// 解析ADC数据接收帧
MatrixServerDll_API bool ParseInstrumentADCDataRecFrame(m_oADCDataFrameStruct* pADCDataFrame, m_oConstVarStruct* pConstVar)
{
	bool bReturn = false;
	EnterCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
	bReturn = ParseInstrumentFrame(pADCDataFrame->m_pCommandStructReturn, pADCDataFrame->m_pcRcvFrameData, pConstVar);
	LeaveCriticalSection(&pADCDataFrame->m_oSecADCDataFrame);
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
		pHeartBeatFrame->m_pCommandStruct->m_usAimPort, pHeartBeatFrame->m_pCommandStruct->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pHeartBeatFrame->m_oSecHeartBeat);
}
// 生成IP地址查询帧
MatrixServerDll_API void MakeInstrumentIPQueryFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP)
{
	CString str = _T("");
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	// 仪器IP地址
	pIPSetFrame->m_pCommandStructSet->m_uiDstIP = uiInstrumentIP;
	// IP地址查询命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	pIPSetFrame->m_pcCommandWord[0] = pConstVar->m_byCmdLocalIPAddr;
	// 查询命令字个数
	pIPSetFrame->m_usCommandWordNum = 1;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_pcSndFrameData, 
		pIPSetFrame->m_pcCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送IP地址查询帧"), pIPSetFrame->m_pCommandStructSet->m_uiDstIP);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPQueryFrame", ConvertCStrToStr(str));
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument)
{
	CString str = _T("");
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
	pIPSetFrame->m_pCommandStructSet->m_byLAUXRoutOpenSet = pInstrument->m_byLAUXRoutOpenSet;
	// 广播IP地址
	pIPSetFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_iIPBroadcastAddr;
	// 时间修正高位
	pIPSetFrame->m_pCommandStructSet->m_uiLocalTimeFixedHigh = pInstrument->m_uiTimeHigh;
	// 时间修正低位
	pIPSetFrame->m_pCommandStructSet->m_uiLocalTimeFixedLow = pInstrument->m_uiTimeLow;

	// 仪器SN命令字
	pIPSetFrame->m_pcCommandWord[0] = pConstVar->m_byCmdSn;
	// 仪器IP命令字
	pIPSetFrame->m_pcCommandWord[1] = pConstVar->m_byCmdLocalIPAddr;
	// 时间修正高位命令字
	pIPSetFrame->m_pcCommandWord[2] = pConstVar->m_byCmdLocalTimeFixedHigh;
	// 时间修正低位命令字
	pIPSetFrame->m_pcCommandWord[3] = pConstVar->m_byCmdLocalTimeFixedLow;
	// 仪器广播端口命令字
	pIPSetFrame->m_pcCommandWord[4] = pConstVar->m_byCmdBroadCastPortSet;

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
		pIPSetFrame->m_pcCommandWord[5] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[6] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[7] = pConstVar->m_byCmdLAUXSetRout;
		// 仪器路由IP命令字
		pIPSetFrame->m_pcCommandWord[8] = pConstVar->m_byCmdLAUXSetRout;
		// 打开仪器路由命令字
		pIPSetFrame->m_pcCommandWord[9] = pConstVar->m_byCmdLAUXRoutOpenSet;
		// 命令字个数
		pIPSetFrame->m_usCommandWordNum = 10;
	}
	// IP地址设置命令
	pIPSetFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	MakeInstrumentFrame(pIPSetFrame->m_pCommandStructSet, pConstVar, pIPSetFrame->m_pcSndFrameData, 
		pIPSetFrame->m_pcCommandWord, pIPSetFrame->m_usCommandWordNum);
	str.Format(_T("向仪器SN = 0x%x，IP地址 = 0x%x 的仪器发送IP地址设置帧"), 
		pIPSetFrame->m_pCommandStructSet->m_uiSN, pIPSetFrame->m_pCommandStructSet->m_uiInstrumentIP);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "MakeInstrumentIPSetFrame", ConvertCStrToStr(str));
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 发送IP地址设置帧
MatrixServerDll_API void SendInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
	SendFrame(pIPSetFrame->m_oIPSetFrameSocket, pIPSetFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pIPSetFrame->m_pCommandStructSet->m_usAimPort, pIPSetFrame->m_pCommandStructSet->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pIPSetFrame->m_oSecIPSetFrame);
}
// 按IP地址查询尾包时刻帧
MatrixServerDll_API void MakeInstrumentTailTimeQueryFramebyIP(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 仪器IP地址
	pTailTimeFrame->m_pCommandStructSet->m_uiDstIP = uiInstrumentIP;
	// 查询命令
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 查询命令字内容
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_pcCommandWord[0] = pConstVar->m_byCmdLAUTailRecTimeLAUX;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_pcCommandWord[1] = pConstVar->m_byCmdLineTailRecTimeLAUX;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_pcCommandWord[2] = pConstVar->m_byCmdTailRecSndTime;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = 3;
	MakeInstrumentFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_pcSndFrameData, 
		pTailTimeFrame->m_pcCommandWord, pTailTimeFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 广播查询尾包时刻帧
MatrixServerDll_API void MakeInstrumentTailTimeQueryFramebyBroadCast(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	// 仪器IP地址
	pTailTimeFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_iIPBroadcastAddr;
	// 查询命令
	pTailTimeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 广播端口
	pTailTimeFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiBroadCastPort;
	// 查询命令字内容
	// 广播端口
	pTailTimeFrame->m_pcCommandWord[0] = pConstVar->m_byCmdBroadCastPortSeted;
	// 交叉站交叉线尾包接收时刻
	pTailTimeFrame->m_pcCommandWord[1] = pConstVar->m_byCmdLAUTailRecTimeLAUX;
	// 交叉站大线尾包接收时刻
	pTailTimeFrame->m_pcCommandWord[2] = pConstVar->m_byCmdLineTailRecTimeLAUX;
	// 尾包接收/发送时刻
	pTailTimeFrame->m_pcCommandWord[3] = pConstVar->m_byCmdTailRecSndTime;
	// 查询命令字个数
	pTailTimeFrame->m_usCommandWordNum = 4;
	MakeInstrumentFrame(pTailTimeFrame->m_pCommandStructSet, pConstVar, pTailTimeFrame->m_pcSndFrameData, 
		pTailTimeFrame->m_pcCommandWord, pTailTimeFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 发送尾包时刻查询帧
MatrixServerDll_API void SendInstrumentTailTimeQueryFrame(m_oTailTimeFrameStruct* pTailTimeFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
	SendFrame(pTailTimeFrame->m_oTailTimeFrameSocket, pTailTimeFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pTailTimeFrame->m_pCommandStructSet->m_usAimPort, pTailTimeFrame->m_pCommandStructSet->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pTailTimeFrame->m_oSecTailTimeFrame);
}
// 生成时统设置帧
MatrixServerDll_API void MakeInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument)
{
	CString str = _T("");
	// 接收到时统设置应答标志位
	pInstrument->m_bTimeSetOK = false;
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	// 仪器IP地址
	pTimeDelayFrame->m_pCommandStructSet->m_uiDstIP = pInstrument->m_uiIP;
	// 设置命令
	pTimeDelayFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendSetCmd;
	// 时统设置高位
	pTimeDelayFrame->m_pCommandStructSet->m_uiLocalTimeFixedHigh = pInstrument->m_uiTimeHigh;
	// 时统设置低位
	pTimeDelayFrame->m_pCommandStructSet->m_uiLocalTimeFixedLow = pInstrument->m_uiTimeLow;
	// 设置命令字内容
	// 时统设置高位
	pTimeDelayFrame->m_pcCommandWord[0] = pConstVar->m_byCmdLocalTimeFixedHigh;
	// 时统设置低位
	pTimeDelayFrame->m_pcCommandWord[1] = pConstVar->m_byCmdLocalTimeFixedLow;
	// 设置命令字个数
	pTimeDelayFrame->m_usCommandWordNum = 2;
	MakeInstrumentFrame(pTimeDelayFrame->m_pCommandStructSet, pConstVar, pTimeDelayFrame->m_pcSndFrameData, 
		pTimeDelayFrame->m_pcCommandWord, pTimeDelayFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 发送时统设置帧
MatrixServerDll_API void SendInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
	SendFrame(pTimeDelayFrame->m_oTimeDelayFrameSocket, pTimeDelayFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pTimeDelayFrame->m_pCommandStructSet->m_usAimPort, pTimeDelayFrame->m_pCommandStructSet->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pTimeDelayFrame->m_oSecTimeDelayFrame);
}
// 发送ADC参数设置帧
MatrixServerDll_API void SendInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
	SendFrame(pADCSetFrame->m_oADCSetFrameSocket, pADCSetFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pADCSetFrame->m_pCommandStructSet->m_usAimPort, pADCSetFrame->m_pCommandStructSet->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pADCSetFrame->m_oSecADCSetFrame);
}
// 广播查询误码
MatrixServerDll_API void MakeInstrumentErrorCodeQueryFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort)
{
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	// 仪器IP地址
	pErrorCodeFrame->m_pCommandStructSet->m_uiDstIP = pConstVar->m_iIPBroadcastAddr;
	// 查询命令
	pErrorCodeFrame->m_pCommandStructSet->m_usCommand = pConstVar->m_usSendQueryCmd;
	// 广播端口
	pErrorCodeFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiBroadCastPort;
	// 查询命令字内容
	// 广播端口
	pErrorCodeFrame->m_pcCommandWord[0] = pConstVar->m_byCmdBroadCastPortSeted;
	// 交叉站交叉线尾包接收时刻
	pErrorCodeFrame->m_pcCommandWord[1] = pConstVar->m_byCmdFDUErrorCode;
	// 交叉站大线尾包接收时刻
	pErrorCodeFrame->m_pcCommandWord[2] = pConstVar->m_byCmdLAUXErrorCode1;
	// 尾包接收/发送时刻
	pErrorCodeFrame->m_pcCommandWord[3] = pConstVar->m_byCmdLAUXErrorCode2;
	// 查询命令字个数
	pErrorCodeFrame->m_usCommandWordNum = 4;
	MakeInstrumentFrame(pErrorCodeFrame->m_pCommandStructSet, pConstVar, pErrorCodeFrame->m_pcSndFrameData, 
		pErrorCodeFrame->m_pcCommandWord, pErrorCodeFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
}
// 发送误码查询帧
MatrixServerDll_API void SendInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, m_oConstVarStruct* pConstVar)
{
	EnterCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
	SendFrame(pErrorCodeFrame->m_oErrorCodeFrameSocket, pErrorCodeFrame->m_pcSndFrameData, pConstVar->m_iSndFrameSize,
		pErrorCodeFrame->m_pCommandStructSet->m_usAimPort, pErrorCodeFrame->m_pCommandStructSet->m_uiAimIP, 
		pConstVar->m_pLogOutPut);
	LeaveCriticalSection(&pErrorCodeFrame->m_oSecErrorCodeFrame);
}
// 初始化线程函数
MatrixServerDll_API bool OnInitThread(m_oThreadStruct* pThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	pThread->m_bClose = false;
	pThread->m_bWork = false;
	pThread->m_dwThreadID = 0;
	pThread->m_pLogOutPut = pLogOutPut;
	pThread->m_pConstVar = pConstVar;
	pThread->m_hThread = INVALID_HANDLE_VALUE;
	pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	pThread->m_hThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if (pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pLogOutPut, "OnInitThread", 
			"pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnInitThread", 
			"pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
		return false;
	}
	return true;
}
// 关闭线程函数的句柄
MatrixServerDll_API void OnCloseThreadHandle(m_oThreadStruct* pThread)
{
	if (FALSE == CloseHandle(pThread->m_hThread))
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnCloseThread", 
			"pThread->m_hThread", ErrorType);
	}
	if (FALSE == CloseHandle(pThread->m_hThreadClose))
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnCloseThread", 
			"pThread->m_hThreadClose", ErrorType);
	}
}
// 关闭线程函数
MatrixServerDll_API bool OnCloseThread(m_oThreadStruct* pThread)
{
	if (pThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pThread->m_hThreadClose, 
		pThread->m_pConstVar->m_iCloseThreadSleepTimes
		* pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		::TerminateThread(pThread->m_hThread, 0);
		OnCloseThreadHandle(pThread);
		return false;
	}
	else
	{
		OnCloseThreadHandle(pThread);
		return true;
	}
}

// 创建日志输出线程
MatrixServerDll_API m_oLogOutPutThreadStruct* OnCreateLogOutPutThread(void)
{
	m_oLogOutPutThreadStruct* pLogOutPutThread = NULL;
	pLogOutPutThread = new m_oLogOutPutThreadStruct;
	pLogOutPutThread->m_pThread = new m_oThreadStruct;
	pLogOutPutThread->m_pLogOutPutTimeDelay = NULL;
	pLogOutPutThread->m_pLogOutPutErrorCode = NULL;
	pLogOutPutThread->m_pLogOutPutADCFrameTime = NULL;
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
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutTimeDelay);
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutErrorCode);
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutADCFrameTime);
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
MatrixServerDll_API bool OnInitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pLogOutPutThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pLogOutPutThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
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
// 初始化日志输出线程
MatrixServerDll_API bool OnInit_LogOutPutThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pLogOutPutThread->m_pLogOutPutTimeDelay = pEnv->m_pLogOutPutTimeDelay;
	pEnv->m_pLogOutPutThread->m_pLogOutPutErrorCode = pEnv->m_pLogOutPutErrorCode;
	pEnv->m_pLogOutPutThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	return OnInitLogOutPutThread(pEnv->m_pLogOutPutThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭日志输出线程
MatrixServerDll_API bool OnCloseLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pLogOutPutThread->m_pThread))
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", 
			"日志输出线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", "日志输出线程成功关闭");
		return true;
	}
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
MatrixServerDll_API m_oHeartBeatThreadStruct* OnCreateHeartBeatThread(void)
{
	m_oHeartBeatThreadStruct* pHeartBeatThread = NULL;
	pHeartBeatThread = new m_oHeartBeatThreadStruct;
	pHeartBeatThread->m_pThread = new m_oThreadStruct;
	pHeartBeatThread->m_pHeartBeatFrame = NULL;
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
MatrixServerDll_API bool OnInitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pHeartBeatThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
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
// 初始化心跳线程
MatrixServerDll_API bool OnInit_HeartBeatThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pHeartBeatThread->m_pHeartBeatFrame = pEnv->m_pHeartBeatFrame;
	return OnInitHeartBeatThread(pEnv->m_pHeartBeatThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭心跳线程
MatrixServerDll_API bool OnCloseHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	if(false == OnCloseThread(pHeartBeatThread->m_pThread))
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", 
			"心跳线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", "心跳线程成功关闭");
		return true;
	}
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
MatrixServerDll_API m_oHeadFrameThreadStruct* OnCreateHeadFrameThread(void)
{
	m_oHeadFrameThreadStruct* pHeadFrameThread = NULL;
	pHeadFrameThread = new m_oHeadFrameThreadStruct;
	pHeadFrameThread->m_pThread = new m_oThreadStruct;
	pHeadFrameThread->m_pHeadFrame = NULL;
	pHeadFrameThread->m_pInstrumentList = NULL;
	pHeadFrameThread->m_pRoutList = NULL;
	return pHeadFrameThread;
}
/**
* 根据链接方向，得到连接的下一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetNextInstrument(int iRoutDirection, 
	m_oInstrumentStruct* pInstrument,
	m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 判断方向
	if (iRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrumentNext = pInstrument->m_pInstrumentTop;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrumentNext = pInstrument->m_pInstrumentDown;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrumentNext = pInstrument->m_pInstrumentLeft;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrumentNext = pInstrument->m_pInstrumentRight;
	}
	else
	{
		pInstrumentNext = pInstrument;
	}
	if (pInstrumentNext != NULL)
	{
		if (pInstrumentNext->m_bInUsed == false)
		{
			return NULL;
		}
	}
	return pInstrumentNext;
}
/**
* 根据链接方向，得到连接的前一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的前一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetPreviousInstrument(int iRoutDirection, 
	m_oInstrumentStruct* pInstrument,
	m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrumentPrevious = NULL;
	// 判断方向
	if (iRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentDown;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentTop;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentRight;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentLeft;
	}
	else
	{
		pInstrumentPrevious = pInstrument;
	}
	if (pInstrumentPrevious != NULL)
	{
		if (pInstrumentPrevious->m_bInUsed == false)
		{
			return NULL;
		}
	}
	return pInstrumentPrevious;
}
// 仪器位置按照首包时刻排序
MatrixServerDll_API void InstrumentLocationSortTop(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	bool bLocation = false;
	bool bStable = true;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrumentTop = NULL;
	m_oInstrumentStruct* pInstrumentDown = NULL;
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
		pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb - 1;
		pRout->m_pTail = pInstrument;
		pRout->m_pHead->m_pInstrumentTop = pInstrument;
		pInstrument->m_pInstrumentDown = pRout->m_pHead;
		pInstrument->m_iHeadFrameStableNum++;
		
	}
	else
	{
		// 该路由方向尾仪器首包时刻小于该仪器的首包时刻
		if (pRout->m_pTail->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
		{
			pInstrument->m_iLineNb = pRout->m_pTail->m_iLineNb - 1;
			pRout->m_pTail->m_pInstrumentTop = pInstrument;
			pInstrument->m_pInstrumentDown = pRout->m_pTail;
			pRout->m_pTail = pInstrument;
			pInstrument->m_iHeadFrameStableNum++;
			
		} 
		// 仪器位于路由链中
		else
		{
			pInstrumentNext = pRout->m_pHead;
			pInstrumentDown = pRout->m_pHead;
			while(pInstrumentNext != NULL)
			{
				if (pInstrumentNext->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
				{
					pInstrumentDown = pInstrumentNext;
				}
				else
				{
					// 找到路由表第一个首包时刻大于等于仪器首包时刻的仪器
					if (bLocation == false)
					{
						bLocation = true;
						pInstrumentTop = pInstrumentNext;
						if (pInstrumentTop == pInstrument)
						{
							pInstrument->m_iHeadFrameStableNum++;
							pInstrument->m_iLineNb = pInstrumentDown->m_iLineNb - 1;
						}
						// 在路由表中两个仪器中间插入仪器
						else
						{
							pInstrumentDown->m_pInstrumentTop = pInstrument;
							pInstrument->m_pInstrumentDown = pInstrumentDown;
							pInstrumentTop->m_pInstrumentDown = pInstrument;
							pInstrument->m_pInstrumentTop = pInstrumentTop;
							pInstrumentTop->m_iHeadFrameStableNum = 0;
							pInstrument->m_iHeadFrameStableNum = 0;
							pInstrument->m_iLineNb = pInstrumentDown->m_iLineNb - 1;
							pInstrumentTop->m_iLineNb--;
							bStable = false;	
						}
					}
					else
					{
						if (bStable == false)
						{
							pInstrumentNext->m_iHeadFrameStableNum = 0;
							pInstrumentNext->m_iLineNb--;
						}
					}
				}
				pInstrumentNext = GetNextInstrument(pConstVar->m_iDirectionTop, pInstrumentNext, pConstVar);
			}
		}
	}
}
// 仪器位置按照首包时刻排序
MatrixServerDll_API void InstrumentLocationSortDown(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	bool bLocation = false;
	bool bStable = true;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrumentTop = NULL;
	m_oInstrumentStruct* pInstrumentDown = NULL;
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
		pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb + 1;
		pRout->m_pTail = pInstrument;
		pRout->m_pHead->m_pInstrumentDown = pInstrument;
		pInstrument->m_pInstrumentTop = pRout->m_pHead;
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		// 该路由方向尾仪器首包时刻小于该仪器的首包时刻
		if (pRout->m_pTail->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
		{
			pInstrument->m_iLineNb = pRout->m_pTail->m_iLineNb + 1;
			pRout->m_pTail->m_pInstrumentDown = pInstrument;
			pInstrument->m_pInstrumentTop = pRout->m_pTail;
			pRout->m_pTail = pInstrument;
			pInstrument->m_iHeadFrameStableNum++;
		} 
		// 仪器位于路由链中
		else
		{
			pInstrumentNext = pRout->m_pHead;
			pInstrumentTop = pRout->m_pHead;
			while(pInstrumentNext != NULL)
			{
				if (pInstrumentNext->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
				{
					pInstrumentTop = pInstrumentNext;
				}
				else
				{
					// 找到路由表第一个首包时刻大于等于仪器首包时刻的仪器
					if (bLocation == false)
					{
						bLocation = true;
						pInstrumentDown = pInstrumentNext;
						if (pInstrumentDown == pInstrument)
						{
							pInstrument->m_iHeadFrameStableNum++;
							pInstrument->m_iLineNb = pInstrumentTop->m_iLineNb + 1;
						}
						// 在路由表中两个仪器中间插入仪器
						else
						{
							pInstrumentDown->m_pInstrumentTop = pInstrument;
							pInstrument->m_pInstrumentDown = pInstrumentDown;
							pInstrumentTop->m_pInstrumentDown = pInstrument;
							pInstrument->m_pInstrumentTop = pInstrumentTop;
							pInstrumentDown->m_iHeadFrameStableNum = 0;
							pInstrument->m_iHeadFrameStableNum = 0;
							pInstrument->m_iLineNb = pInstrumentTop->m_iLineNb + 1;
							pInstrumentDown->m_iLineNb++;
							bStable = false;	
						}
					}
					else
					{
						if (bStable == false)
						{
							pInstrumentNext->m_iHeadFrameStableNum = 0;
							pInstrumentNext->m_iLineNb++;
						}
					}
				}
				pInstrumentNext = GetNextInstrument(pConstVar->m_iDirectionDown, pInstrumentNext, pConstVar);
			}
		}
	}
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
			pInstrumentRight = pRout->m_pHead;
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
			pInstrumentLeft = pRout->m_pHead;
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

	// 上方
	if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex;
		InstrumentLocationSortTop(pInstrument, pRout, pConstVar);
		pRout->m_uiInstrumentNum = pRout->m_pHead->m_iLineNb - pRout->m_pTail->m_iLineNb;
	}
	// 下方
	else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrument->m_iPointIndex = pRout->m_pHead->m_iPointIndex;
		InstrumentLocationSortDown(pInstrument, pRout, pConstVar);
		pRout->m_uiInstrumentNum = pRout->m_pTail->m_iLineNb - pRout->m_pHead->m_iLineNb;
	}
	// 大线左方
	else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb;
		InstrumentLocationSortLeft(pInstrument, pRout, pConstVar);
		pRout->m_uiInstrumentNum = pRout->m_pHead->m_iPointIndex - pRout->m_pTail->m_iPointIndex;
	}
	// 大线右方
	else if (pInstrument->m_iRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrument->m_iLineNb = pRout->m_pHead->m_iLineNb;
		InstrumentLocationSortRight(pInstrument, pRout, pConstVar);
		pRout->m_uiInstrumentNum = pRout->m_pTail->m_iPointIndex - pRout->m_pHead->m_iPointIndex;
	}
	// LCI
	else
	{
		pInstrument->m_iHeadFrameStableNum++;
		pRout->m_uiInstrumentNum = 1;
	}
}
/**
* 设置交叉站某个方向的路由
* @param CInstrument* &pInstrument 仪器指针
* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
MatrixServerDll_API void SetCrossRout(m_oInstrumentStruct* pInstrument,  
	int iRoutDirection, 
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
	if (iRoutDirection == pConstVar->m_iDirectionTop)
	{
		pInstrument->m_uiRoutIPTop = pRout->m_uiRoutIP;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionDown)
	{
		pInstrument->m_uiRoutIPDown = pRout->m_uiRoutIP;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionLeft)
	{
		pInstrument->m_uiRoutIPLeft = pRout->m_uiRoutIP;
	}
	else if (iRoutDirection == pConstVar->m_iDirectionRight)
	{
		pInstrument->m_uiRoutIPRight = pRout->m_uiRoutIP;
	}
	// 设置交叉站路由方向
	pRout->m_iRoutDirection = iRoutDirection;
	// 设置路由头
	pRout->m_pHead = pInstrument;
	if (iRoutDirection == pConstVar->m_iDirectionCenter)
	{
		// 设置LCI路由
		pRout->m_uiRoutIP = 0;
		// 设置路由尾
		pRout->m_pTail = pInstrument;
		// 路由为交叉线方向路由
		pRout->m_bRoutLaux = true;
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
		if (TRUE == IfIndexExistInRoutMap(pInstrument->m_uiRoutIP, &pHeadFrameThread->m_pRoutList->m_oRoutMap))
		{
			pRout = GetRout(pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP,
				&pHeadFrameThread->m_pRoutList->m_oRoutMap);
			// 更新路由对象的路由时间
			UpdateRoutTime(pRout);
			// 设置新仪器的首包时刻
			pInstrument->m_uiTimeHeadFrame = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiTimeHeadFrame;
			// 更新仪器的位置信息
			SetInstrumentLocation(pInstrument, pRout, pHeadFrameThread->m_pThread->m_pConstVar);
		}
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
		pInstrument->m_iInstrumentType = pInstrument->m_uiSN & 0xff;
		//设置新仪器的路由IP地址
		pInstrument->m_uiRoutIP = pHeadFrameThread->m_pHeadFrame->m_pCommandStruct->m_uiRoutIP;
		// 设置新仪器的仪器类型
		// 路由地址为0为LCI
		if (pInstrument->m_uiRoutIP == 0)
		{
			pInstrument->m_iInstrumentType = pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI;
			pInstrument->m_iRoutDirection = pHeadFrameThread->m_pThread->m_pConstVar->m_iDirectionCenter;
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
				// 更新路由对象的路由时间
				UpdateRoutTime(pRout);
				pInstrument->m_iRoutDirection = pRout->m_iRoutDirection;
			}
			else
			{
				str.Format(_T("仪器SN = 0x%x，路由IP = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
				// 不是LCI却又找不到路由则记录日志
				AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "ProcHeadFrameOne", 
					ConvertCStrToStr(str), ErrorType, IDS_ERR_ROUT_NOTEXIT);
			}
		}

		if ((pInstrument->m_iInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pHeadFrameThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
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
		pInstrument->m_uiBroadCastPort = pHeadFrameThread->m_pThread->m_pConstVar->m_iBroadcastPortStart + pInstrument->m_uiRoutIP;
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
					// 系统发生变化的时间
					UpdateLineChangeTime(pHeadFrameThread->m_pInstrumentList);
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
MatrixServerDll_API bool OnInitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pHeadFrameThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
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
// 初始化首包接收线程
MatrixServerDll_API bool OnInit_HeadFrameThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pHeadFrameThread->m_pHeadFrame = pEnv->m_pHeadFrame;
	pEnv->m_pHeadFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pHeadFrameThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitHeadFrameThread(pEnv->m_pHeadFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭首包接收线程
MatrixServerDll_API bool OnCloseHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread)
{
	if (pHeadFrameThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pHeadFrameThread->m_pThread))
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, "OnCloseHeadFrameThread", 
			"首包线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pHeadFrameThread->m_pThread->m_pLogOutPut, 
			"OnCloseHeadFrameThread", "首包线程成功关闭");
		return true;
	}
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
MatrixServerDll_API m_oIPSetFrameThreadStruct* OnCreateIPSetFrameThread(void)
{
	m_oIPSetFrameThreadStruct* pIPSetFrameThread = NULL;
	pIPSetFrameThread = new m_oIPSetFrameThreadStruct;
	pIPSetFrameThread->m_pThread = new m_oThreadStruct;
	pIPSetFrameThread->m_pInstrumentList = NULL;
	pIPSetFrameThread->m_pIPSetFrame = NULL;
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
				pInstrument->m_uiSN, uiIPInstrument);
		}
		else if (usCommand == pIPSetFrameThread->m_pThread->m_pConstVar->m_usSendQueryCmd)
		{
			str.Format(_T("接收到SN = 0x%x，IP地址 = 0x%x仪器的IP地址查询应答"), 
				pInstrument->m_uiSN, uiIPInstrument);
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
			// IP地址设置次数为0
			if (iter->second->m_iIPSetCount == 0)
			{
				// 生成IP地址设置帧
				MakeInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar, iter->second);
				// 发送IP地址设置帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
			}
			// IP地址设置次数不超过指定次数则重新设置IP地址和查询IP地址
			else if (iter->second->m_iIPSetCount <= pIPSetFrameThread->m_pThread->m_pConstVar->m_iIPAddrResetTimes)
			{
				// 生成IP地址查询帧
				MakeInstrumentIPQueryFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar, iter->second->m_uiIP);
				// 发送IP地址查询帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);

				// 生成IP地址设置帧
				MakeInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar, iter->second);
				// 发送IP地址设置帧
				SendInstrumentIPSetFrame(pIPSetFrameThread->m_pIPSetFrame, pIPSetFrameThread->m_pThread->m_pConstVar);
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
				AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "ProcIPSetFrame",
					"重发IP地址设置帧和IP地址查询帧", WarningType);
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
MatrixServerDll_API bool OnInitIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pIPSetFrameThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pIPSetFrameThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
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
// 初始化IP地址设置线程
MatrixServerDll_API bool OnInit_IPSetFrameThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pIPSetFrameThread->m_pIPSetFrame = pEnv->m_pIPSetFrame;
	pEnv->m_pIPSetFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	return OnInitIPSetFrameThread(pEnv->m_pIPSetFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭IP地址设置线程
MatrixServerDll_API bool OnCloseIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread)
{
	if (pIPSetFrameThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pIPSetFrameThread->m_pThread))
	{
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnCloseIPSetFrameThread", 
			"IP地址设置线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pIPSetFrameThread->m_pThread->m_pLogOutPut, "OnCloseIPSetFrameThread", "IP地址设置线程成功关闭");
		return true;
	}
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
MatrixServerDll_API m_oTailFrameThreadStruct* OnCreateTailFrameThread(void)
{
	m_oTailFrameThreadStruct* pTailFrameThread = NULL;
	pTailFrameThread = new m_oTailFrameThreadStruct;
	pTailFrameThread->m_pThread = new m_oThreadStruct;
	pTailFrameThread->m_pInstrumentList = NULL;
	pTailFrameThread->m_pRoutList = NULL;
	pTailFrameThread->m_pTailFrame = NULL;
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
	pInstrumentNext = GetNextInstrument(pInstrument->m_iRoutDirection, pRout->m_pHead, pConstVar);
	while(pInstrument != pInstrumentNext)
	{
		if (pInstrumentNext == NULL)
		{
			break;
		}
		pInstrumentNext->m_iTailFrameCount = 0;
		pInstrumentNext = GetNextInstrument(pInstrumentNext->m_iRoutDirection, pInstrumentNext, pConstVar);
	}
}
// 回收一个路由
MatrixServerDll_API void FreeRoutFromMap(unsigned int uiRoutIP, m_oRoutListStruct* pRoutList)
{
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	pRout = GetRout(uiRoutIP, &pRoutList->m_oRoutMap);
	AddRout(uiRoutIP, pRout, &pRoutList->m_oRoutDeleteMap);
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
	// 从SN索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiSN, &pInstrumentList->m_oSNInstrumentMap);
	// 从IP地址设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPSetMap);
	// 从IP地址索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oIPInstrumentMap);
	// 从ADC参数设置索引表中删除该仪器指针
	DeleteInstrumentFromMap(pInstrument->m_uiIP, &pInstrumentList->m_oADCSetInstrumentMap);
	str.Format(_T("删除仪器的SN = 0x%x，路由 = 0x%x"), pInstrument->m_uiSN, pInstrument->m_uiRoutIP);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "FreeInstrumentFromMap", ConvertCStrToStr(str));

	// 删除仪器的类型为LCI或交叉站
	if ((pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		|| (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
	{
		if (pInstrument->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
		{
			FreeRoutFromMap(pInstrument->m_uiRoutIP, pRoutList);
		}
		// 仪器上方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPTop, pRoutList);
		// 仪器下方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPDown, pRoutList);
		// 仪器左方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPLeft, pRoutList);
		// 仪器右方路由
		FreeRoutFromMap(pInstrument->m_uiRoutIPRight, pRoutList);
	}
	// 将仪器加入到空闲仪器队列
	AddFreeInstrument(pInstrument, pInstrumentList);
	// 系统发生变化的时间
	UpdateLineChangeTime(pInstrumentList);
}
// 删除该路由方向指定仪器之后的仪器
MatrixServerDll_API void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, 
	m_oInstrumentListStruct* pInstrumentList, 
	m_oRoutListStruct* pRoutList, 
	m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "DeleteInstrumentAlongRout", 
			"pRout", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	if (pInstrumentList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "DeleteInstrumentAlongRout", 
			"pInstrumentList", ErrorType, IDS_ERR_PTRISNULL);
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
		pInstrumentPrevious = GetPreviousInstrument(pInstrumentDelete->m_iRoutDirection, pInstrumentDelete, pConstVar);
		pRout->m_pTail = pInstrumentPrevious;
		if (pRout->m_pTail == pRout->m_pHead)
		{
			pRout->m_pTail = NULL;
		}
		// 回收一个仪器
		FreeInstrumentFromMap(pInstrumentDelete, pInstrumentList, pRoutList, pConstVar);
		pRout->m_uiInstrumentNum--;
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
	hash_map <unsigned int, m_oRoutStruct*> :: iterator iter;
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
// 		// 更新尾包仪器的尾包时刻
// 		pInstrument->m_uiTailSysTime = pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiSysTime;
		// 在路由索引表中找到该尾包所在的路由
		if (TRUE == IfIndexExistInRoutMap(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP,
			&pTailFrameThread->m_pRoutList->m_oRoutMap))
		{
			AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", ConvertCStrToStr(str));
			// 由路由IP得到路由对象
			pRout = GetRout(pTailFrameThread->m_pTailFrame->m_pCommandStruct->m_uiRoutIP,
				&pTailFrameThread->m_pRoutList->m_oRoutMap);
			// 更新路由对象的路由时间
			UpdateRoutTime(pRout);
			// 清除与该仪器路由方向相同的之前仪器的尾包计数
			OnClearSameRoutTailCount(pInstrument, pRout, pTailFrameThread->m_pThread->m_pConstVar);
			// 判断尾包计数器达到设定值则删除相同路由之后的仪器
			if (pInstrument->m_iTailFrameCount > pTailFrameThread->m_pThread->m_pConstVar->m_iTailFrameStableTimes)
			{
				if (pRout->m_pTail != NULL)
				{
					DeleteInstrumentAlongRout(pInstrument, pRout, pTailFrameThread->m_pInstrumentList, 
						pTailFrameThread->m_pRoutList, pTailFrameThread->m_pThread->m_pConstVar);
					while(true)
					{
						if (pTailFrameThread->m_pRoutList->m_oRoutDeleteMap.empty() == true)
						{
							break;
						}
						iter = pTailFrameThread->m_pRoutList->m_oRoutDeleteMap.begin();
						DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, pTailFrameThread->m_pInstrumentList, 
							pTailFrameThread->m_pRoutList, pTailFrameThread->m_pThread->m_pConstVar);
						// 路由索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oRoutMap);
						str.Format(_T("回收路由IP = 0x%x的过期路由"), iter->first);
						AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "ProcTailFrameOne", ConvertCStrToStr(str));
						// 将过期路由回收到空闲路由队列
						AddFreeRout(iter->second, pTailFrameThread->m_pRoutList);
						// 路由删除索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oRoutDeleteMap);
						// ADC参数设置索引表回收路由
						DeleteRout(iter->first, &pTailFrameThread->m_pRoutList->m_oADCSetRoutMap);
					}
				}
				pInstrument->m_iTailFrameCount = 0;
			}
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
MatrixServerDll_API bool OnInitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pTailFrameThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
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
// 初始化尾包接收线程
MatrixServerDll_API bool OnInit_TailFrameThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pTailFrameThread->m_pTailFrame = pEnv->m_pTailFrame;
	pEnv->m_pTailFrameThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pTailFrameThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitTailFrameThread(pEnv->m_pTailFrameThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭尾包接收线程
MatrixServerDll_API bool OnCloseTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread)
{
	if (pTailFrameThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pTailFrameThread->m_pThread))
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", 
			"尾包接收线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pTailFrameThread->m_pThread->m_pLogOutPut, "OnCloseTailFrameThread", "尾包接收线程成功关闭");
		return true;
	}
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
MatrixServerDll_API m_oMonitorThreadStruct* OnCreateMonitorThread(void)
{
	m_oMonitorThreadStruct* pMonitorThread = NULL;
	pMonitorThread = new m_oMonitorThreadStruct;
	pMonitorThread->m_pThread = new m_oThreadStruct;
	pMonitorThread->m_pTimeDelayThread = NULL;
	pMonitorThread->m_pADCSetThread = NULL;
	pMonitorThread->m_pErrorCodeThread = NULL;
	pMonitorThread->m_pInstrumentList = NULL;
	pMonitorThread->m_pRoutList = NULL;
	return pMonitorThread;
}
// 线程等待函数
MatrixServerDll_API void WaitMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pMonitorThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pMonitorThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pMonitorThread->m_pThread->m_pConstVar->m_iMonitorSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 沿着路由方向得到时统任务
MatrixServerDll_API void GetTimeDelayTaskAlongRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		pInstrument = pInstrumentNext;
		// 如果仪器为交叉站
		if ((pInstrumentNext->m_iInstrumentType == pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrumentNext->m_iInstrumentType == pConstVar->m_iInstrumentTypeLAUX))
		{
			// 将仪器四个方向的路由加入临时队列
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPTop);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPDown);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPLeft);
			pRoutList->m_olsTimeDelayTemp.push_back(pInstrumentNext->m_uiRoutIPRight);
		}
	} while (pInstrumentNext != pRout->m_pTail);
}
// 得到时统任务
MatrixServerDll_API void GetTimeDelayTask(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTask", "pRoutList", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oRoutStruct* pRout = NULL;
	unsigned int uiRoutIP = 0;
	CString str = _T("");
	while(false == pRoutList->m_olsTimeDelayTemp.empty())
	{
		uiRoutIP = *pRoutList->m_olsTimeDelayTemp.begin();
		pRoutList->m_olsTimeDelayTemp.pop_front();
		// 判断该路由是否在路由索引表中
		if (TRUE == IfIndexExistInRoutMap(uiRoutIP, &pRoutList->m_oRoutMap))
		{
			// 得到路由指针
			pRout = GetRout(uiRoutIP, &pRoutList->m_oRoutMap);
			if (pRout->m_pTail != NULL)
			{
				// 不是LCI
				if (uiRoutIP != 0)
				{
					// 将该路由IP加入任务队列
					pRoutList->m_olsTimeDelayTaskQueue.push_back(uiRoutIP);
					str.Format(_T("将路由IP = 0x%x 加入时统任务队列"), uiRoutIP);
					AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GetTimeDelayTask", ConvertCStrToStr(str));
				}
				// 沿着路由方向得到时统任务
				GetTimeDelayTaskAlongRout(pRout, pRoutList, pConstVar);
			}
		}
	}
}
// 生成时统任务队列
MatrixServerDll_API void GenTimeDelayTaskQueue(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GenTimeDelayTaskQueue", "pRoutList", ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	// 清空任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 将LCI路由加入临时队列
	pRoutList->m_olsTimeDelayTemp.push_back(0);
	AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "GenTimeDelayTaskQueue", "当前系统稳定，生成时统任务队列");
	// 得到时统任务
	GetTimeDelayTask(pRoutList, pConstVar);
}
// 判断路由方向上是否有采集站
MatrixServerDll_API void GetADCTaskQueueByRout(m_oADCSetThreadStruct* pADCSetThread, 
	m_oRoutStruct* pRout, int iOpt)
{
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	bool bRoutADCSet = true;
	bool bADCSet = true;
	if (pRout->m_pTail == NULL)
	{
		return;
	}
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
			pADCSetThread->m_pThread->m_pConstVar);
		if (pInstrumentNext->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
		{
			if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb)
			{
				if (false == pInstrumentNext->m_bADCSet)
				{
					bADCSet = false;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb)
			{
				if ((true == pADCSetThread->m_bADCStartSample)
					&& (false == pInstrumentNext->m_bADCStartSample))
				{
					bADCSet = false;
					// 实际接收ADC数据帧数
					pInstrumentNext->m_uiADCDataActualRecFrameNum = 0;
					// 应该接收ADC数据帧数（含丢帧）
					pInstrumentNext->m_uiADCDataShouldRecFrameNum = 0;
					// ADC数据帧的指针偏移量
					pInstrumentNext->m_usADCDataFramePoint = 0;
					// ADC数据帧发送时的本地时间
					pInstrumentNext->m_uiADCDataFrameSysTime = 0;
				}
				else
				{
					bADCSet = true;
				}
			}
			else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb)
			{
				if ((true == pADCSetThread->m_bADCStopSample)
					&& (false == pInstrumentNext->m_bADCStopSample))
				{
					bADCSet = false;
				}
				else
				{
					bADCSet = true;
				}
			}
			if (bADCSet == false)
			{
				pInstrumentNext->m_bADCSetReturn = false;
				AddInstrumentToMap(pInstrumentNext->m_uiIP, pInstrumentNext, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
			else
			{
				DeleteInstrumentFromMap(pInstrumentNext->m_uiIP, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
				bRoutADCSet = false;
			}
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);
	if (bRoutADCSet == true)
	{
		// 将路由加入索引
		pRout->m_bADCSetReturn = false;
		pRout->m_bADCSetRout = true;
		AddRout(pRout->m_uiRoutIP, pRout, &pADCSetThread->m_pRoutList->m_oADCSetRoutMap);
		// 从仪器索引表删除该路由的仪器
		pInstrument = pRout->m_pHead;
		do 
		{
			pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pADCSetThread->m_pThread->m_pConstVar);
			if (pInstrumentNext->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
			{
				DeleteInstrumentFromMap(pInstrumentNext->m_uiIP, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
			}
			pInstrument = pInstrumentNext;
		} while (pInstrumentNext != pRout->m_pTail);
	}
	else
	{
		DeleteRout(pRout->m_uiRoutIP, &pADCSetThread->m_pRoutList->m_oADCSetRoutMap);
	}
}
// 生成ADC参数设置任务队列
MatrixServerDll_API void GetADCTaskQueue(m_oADCSetThreadStruct* pADCSetThread, int iOpt)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	for (iter = pADCSetThread->m_pRoutList->m_oRoutMap.begin(); iter != pADCSetThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		// 将路由索引表中的大线方向路由加入到ADC参数设置任务索引
		if (iter->second->m_bRoutLaux == false)
		{
			GetADCTaskQueueByRout(pADCSetThread, iter->second, iOpt);
		}
	}
}
MatrixServerDll_API void OnADCCmdAuto(m_oADCSetThreadStruct* pADCSetThread, int iOpt)
{
	// 生成ADC参数设置任务队列
	GetADCTaskQueue(pADCSetThread, iOpt);
	if ((false == pADCSetThread->m_pRoutList->m_oADCSetRoutMap.empty())
		|| (false == pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.empty()))
	{
		// 进行ADC参数设置
		pADCSetThread->m_uiCounter = 0;
		if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 1;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 12;
		}
		else if (iOpt == pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb)
		{
			pADCSetThread->m_uiADCSetOperationNb = 19;
		}
		// ADC参数设置线程开始工作
		pADCSetThread->m_pThread->m_bWork = true;
	}
}
// ADC参数设置
MatrixServerDll_API void OnADCSetAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCSetOptNb);
}
// ADC开始采集命令
MatrixServerDll_API void OnADCStartSampleAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCStartSampleOptNb);
}
// ADC停止采集命令
MatrixServerDll_API void OnADCStopSampleAuto(m_oADCSetThreadStruct* pADCSetThread)
{
	OnADCCmdAuto(pADCSetThread, pADCSetThread->m_pThread->m_pConstVar->m_iADCStopSampleOptNb);
}
// 按路由重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLableByRout(m_oRoutStruct* pRout, int iOpt, m_oConstVarStruct* pConstVar)
{
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	if (pRout->m_pTail == NULL)
	{
		return;
	}
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		if (pInstrumentNext->m_iInstrumentType == pConstVar->m_iInstrumentTypeFDU)
		{
			if (iOpt == pConstVar->m_iADCSetOptNb)
			{
				pInstrumentNext->m_bADCSet = false;
			}
			else if (iOpt == pConstVar->m_iADCStartSampleOptNb)
			{
				pInstrumentNext->m_bADCStartSample = false;
			}
			else if (iOpt == pConstVar->m_iADCStopSampleOptNb)
			{
				pInstrumentNext->m_bADCStopSample = false;
			}
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);

}
// 重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLable(m_oRoutListStruct* pRoutList, int iOpt, m_oConstVarStruct* pConstVar)
{
	if (pRoutList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	for (iter = pRoutList->m_oRoutMap.begin(); iter != pRoutList->m_oRoutMap.end(); iter++)
	{
		// 将路由索引表中的大线方向路由加入到ADC参数设置任务索引
		if (iter->second->m_bRoutLaux == false)
		{
			OnResetADCSetLableByRout(iter->second, iOpt, pConstVar);
		}
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// ADC参数设置
MatrixServerDll_API void OnADCSet(m_oEnvironmentStruct* pEnv)
{
	// 重置ADC参数设置成功的标志位
	OnResetADCSetLable(pEnv->m_pRoutList, pEnv->m_pConstVar->m_iADCSetOptNb, pEnv->m_pConstVar);
}

// ADC开始采集命令
MatrixServerDll_API void OnADCStartSample(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pADCSetThread->m_bADCStartSample = true;
	pEnv->m_pADCSetThread->m_bADCStopSample = false;
	pEnv->m_pTimeDelayThread->m_bADCStartSample = true;
	// 重置ADC开始采集命令成功的标志位
	OnResetADCSetLable(pEnv->m_pRoutList, pEnv->m_pConstVar->m_iADCStartSampleOptNb, pEnv->m_pConstVar);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnADCStartSample", "开始ADC数据采集");
}
// ADC停止采集命令
MatrixServerDll_API void OnADCStopSample(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pADCSetThread->m_bADCStartSample = false;
	pEnv->m_pADCSetThread->m_bADCStopSample = true;
	pEnv->m_pTimeDelayThread->m_bADCStartSample = false;
	// 重置ADC停止采集命令成功的标志位
	OnResetADCSetLable(pEnv->m_pRoutList, pEnv->m_pConstVar->m_iADCStopSampleOptNb, pEnv->m_pConstVar);
	pEnv->m_pTimeDelayThread->m_uiCounter = 0;
	// 清空尾包时刻查询帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket, pEnv->m_pConstVar);
	// 清空时统设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket, pEnv->m_pConstVar);
	// 时统设置线程开始工作
	pEnv->m_pTimeDelayThread->m_pThread->m_bWork = true;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnADCStopSample", "停止ADC数据采集");
}

// 监视过期路由及仪器
MatrixServerDll_API void MonitorAndInstrument(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	ULONGLONG ullTimeNow = 0;
	ullTimeNow = GetTickCount64();
	// hash_map迭代器
	hash_map<unsigned int, m_oRoutStruct*>::iterator  iter;
	CString str = _T("");
	// 删除过期仪器，将过期路由加入路由删除索引表
	for(iter = pMonitorThread->m_pRoutList->m_oRoutMap.begin(); iter != pMonitorThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		if ((ullTimeNow > (iter->second->m_ullRoutTime + pMonitorThread->m_pThread->m_pConstVar->m_iMonitorStableTime))
			&& (iter->second->m_pTail != NULL))
		{
			str.Format(_T("路由IP = 0x%x的路由时间过期，删除该路由方向上的仪器"), iter->second->m_uiRoutIP);
			AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "ProcMonitor", ConvertCStrToStr(str));
			// 如果路由尾仪器为LCI则表明要删除的是LCI本身
			if (iter->second->m_pTail->m_iInstrumentType == pMonitorThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			{
				// 回收一个仪器
				FreeInstrumentFromMap(iter->second->m_pHead, pMonitorThread->m_pInstrumentList, 
					pMonitorThread->m_pRoutList, pMonitorThread->m_pThread->m_pConstVar);
				iter->second->m_uiInstrumentNum = 0;
			}
			else
			{
				DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, pMonitorThread->m_pInstrumentList, 
					pMonitorThread->m_pRoutList, pMonitorThread->m_pThread->m_pConstVar);
			}
		}
	}
	// 删除路由删除索引表中的仪器
	while(1)
	{
		if (pMonitorThread->m_pRoutList->m_oRoutDeleteMap.empty() == true)
		{
			break;
		}
		iter = pMonitorThread->m_pRoutList->m_oRoutDeleteMap.begin();
		DeleteInstrumentAlongRout(iter->second->m_pHead, iter->second, pMonitorThread->m_pInstrumentList, 
			pMonitorThread->m_pRoutList, pMonitorThread->m_pThread->m_pConstVar);
		// 路由索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oRoutMap);
		str.Format(_T("回收路由IP = 0x%x的过期路由"), iter->first);
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "ProcMonitor", ConvertCStrToStr(str));
		// 将过期路由回收到空闲路由队列
		AddFreeRout(iter->second, pMonitorThread->m_pRoutList);
		// 路由删除索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oRoutDeleteMap);
		// ADC参数设置索引表回收路由
		DeleteRout(iter->first, &pMonitorThread->m_pRoutList->m_oADCSetRoutMap);
	}
}
// 检查时统设置应答是否接收完全
MatrixServerDll_API bool CheckTimeDelayReturnByRout(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread, bool bSample)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTimeDelayReturnByRout", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	if (pRout->m_pTail == NULL)
	{
		return true;
	}
	// LCI
	if (pRout->m_pTail == pRout->m_pHead)
	{
		return true;
	}
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	pInstrument = pRout->m_pHead;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pTimeDelayThread->m_pThread->m_pConstVar);
		pInstrument = pInstrumentNext;
		if (bSample == true)
		{
			if (pInstrumentNext->m_iTimeSetReturnCount == 0)
			{
				str.Format(_T("数据采样过程中，路由IP = 0x%x的仪器的没有全部实现时统"), pRout->m_uiRoutIP);
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTimeDelayReturnByRout", ConvertCStrToStr(str), WarningType);
				return false;
			}
		}
		else
		{
			/** 时统设置是否成功*/
			if (pInstrumentNext->m_bTimeSetOK == false)
			{
				str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收不完全"), pRout->m_uiRoutIP);
				AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTimeDelayReturnByRout", ConvertCStrToStr(str));
				return false;
			}
		}
	} while (pInstrumentNext != pRout->m_pTail);
	if (bSample == false)
	{
		str.Format(_T("路由IP = 0x%x的仪器的时统设置应答接收完全"), pRout->m_uiRoutIP);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "CheckTimeDelayReturnByRout", ConvertCStrToStr(str));
	}
	return true;
}
// 检查仪器是否都完成了时统
MatrixServerDll_API bool CheckTimeDelayReturn(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	// hash_map迭代器
	hash_map<unsigned int, m_oRoutStruct*>::iterator  iter;
	for(iter = pTimeDelayThread->m_pRoutList->m_oRoutMap.begin(); iter != pTimeDelayThread->m_pRoutList->m_oRoutMap.end(); iter++)
	{
		if (false == CheckTimeDelayReturnByRout(iter->second, pTimeDelayThread, true))
		{
			return false;
		}
	}
	return true;
}
// 监视时统
MatrixServerDll_API void MonitorTimeDelay(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	ULONGLONG ullTimeNow = 0;
	ullTimeNow = GetTickCount64();
	// 判断系统稳定
	if (ullTimeNow > (pTimeDelayThread->m_pInstrumentList->m_ullLineChangeTime 
		+ pTimeDelayThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 测网状态由不稳定变为稳定
		if (pTimeDelayThread->m_pInstrumentList->m_bLineStableChange == false)
		{
			pTimeDelayThread->m_pInstrumentList->m_bLineStableChange = true;
			// 生成时统任务队列
			GenTimeDelayTaskQueue(pTimeDelayThread->m_pRoutList, pTimeDelayThread->m_pThread->m_pConstVar);
			pTimeDelayThread->m_uiCounter = 0;
			// 清空尾包时刻查询帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar);
			// 清空时统设置应答帧接收缓冲区
			OnClearSocketRcvBuf(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket, 
				pTimeDelayThread->m_pThread->m_pConstVar);
			// 时统设置线程开始工作
			pTimeDelayThread->m_pThread->m_bWork = true;
		}
		else
		{
			// 采集过程中，如果仪器都完成了时统则停止时统线程
			if (pTimeDelayThread->m_bADCStartSample == true)
			{
				if (true == CheckTimeDelayReturn(pTimeDelayThread))
				{
					// 时统设置线程停止工作
					pTimeDelayThread->m_pThread->m_bWork = false;
				}
			}
		}
	} 
	else
	{
		// 时统设置线程停止工作
		pTimeDelayThread->m_pThread->m_bWork = false;
		// 测网状态为不稳定
		pTimeDelayThread->m_pInstrumentList->m_bLineStableChange = false;
	}
}
// 监视仪器的ADC参数设置
MatrixServerDll_API void MonitorADCSet(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	ULONGLONG ullTimeNow = 0;
	ullTimeNow = GetTickCount64();
	// 判断系统稳定
	if (ullTimeNow > (pADCSetThread->m_pInstrumentList->m_ullLineChangeTime 
		+ pADCSetThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 自动进行未完成的ADC参数设置
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCSetAuto(pADCSetThread);
		}
		// 自动进行未完成的开始ADC数据采集
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCStartSampleAuto(pADCSetThread);
		}
		// 自动进行未完成的停止ADC数据采集
		if (pADCSetThread->m_uiADCSetOperationNb == 0)
		{
			OnADCStopSampleAuto(pADCSetThread);
		}
	}
}
// 监视仪器的误码
MatrixServerDll_API void MonitorErrorCode(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 得到当前系统时间
	ULONGLONG ullTimeNow = 0;
	ullTimeNow = GetTickCount64();
	// 判断系统稳定
	if (ullTimeNow > (pErrorCodeThread->m_pInstrumentList->m_ullLineChangeTime 
		+ pErrorCodeThread->m_pThread->m_pConstVar->m_iLineSysStableTime))
	{
		// 误码查询线程开始工作
		pErrorCodeThread->m_pThread->m_bWork = true;
	}
	else
	{
		// 误码查询线程停止工作
		pErrorCodeThread->m_pThread->m_bWork = false;
	}
}
// 处理路由监视
MatrixServerDll_API void ProcMonitor(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	// 监视过期路由及仪器
	MonitorAndInstrument(pMonitorThread);
	// 系统稳定且不进行ADC数据采集时进行时统
	MonitorTimeDelay(pMonitorThread->m_pTimeDelayThread);
	// 系统稳定则对仪器进行ADC参数设置
	MonitorADCSet(pMonitorThread->m_pADCSetThread);
	// 系统稳定则进行误码查询
	MonitorErrorCode(pMonitorThread->m_pErrorCodeThread);
}
// 线程函数
DWORD WINAPI RunMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pMonitorThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pMonitorThread->m_pThread->m_bWork == true)
		{
			EnterCriticalSection(&pMonitorThread->m_pInstrumentList->m_oSecInstrumentList);
			EnterCriticalSection(&pMonitorThread->m_pRoutList->m_oSecRoutList);
			// 处理路由监视
			ProcMonitor(pMonitorThread);
			LeaveCriticalSection(&pMonitorThread->m_pRoutList->m_oSecRoutList);
			LeaveCriticalSection(&pMonitorThread->m_pInstrumentList->m_oSecInstrumentList);
		}
		if (pMonitorThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitMonitorThread(pMonitorThread);
	}
	::SetEvent(pMonitorThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化路由监视线程
MatrixServerDll_API bool OnInitMonitorThread(m_oMonitorThreadStruct* pMonitorThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pMonitorThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pMonitorThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
	}
	::ResetEvent(pMonitorThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pMonitorThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunMonitorThread,
		pMonitorThread, 
		0, 
		&pMonitorThread->m_pThread->m_dwThreadID);
	if (pMonitorThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", 
			"pMonitorThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnInitMonitorThread", "路由监视线程创建成功");
	return true;
}
// 初始化路由监视线程
MatrixServerDll_API bool OnInit_MonitorThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pMonitorThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pMonitorThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pMonitorThread->m_pTimeDelayThread = pEnv->m_pTimeDelayThread;
	pEnv->m_pMonitorThread->m_pADCSetThread = pEnv->m_pADCSetThread;
	pEnv->m_pMonitorThread->m_pErrorCodeThread = pEnv->m_pErrorCodeThread;
	return OnInitMonitorThread(pEnv->m_pMonitorThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭路由监视线程
MatrixServerDll_API bool OnCloseMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pMonitorThread->m_pThread))
	{
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", 
			"路由监视线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pMonitorThread->m_pThread->m_pLogOutPut, "OnCloseMonitorThread", "路由监视线程成功关闭");
		return true;
	}
}
// 释放路由监视线程
MatrixServerDll_API void OnFreeMonitorThread(m_oMonitorThreadStruct* pMonitorThread)
{
	if (pMonitorThread == NULL)
	{
		return;
	}
	if (pMonitorThread->m_pThread != NULL)
	{
		delete pMonitorThread->m_pThread;
	}
	delete pMonitorThread;
}
// 创建时统线程
MatrixServerDll_API m_oTimeDelayThreadStruct* OnCreateTimeDelayThread(void)
{
	m_oTimeDelayThreadStruct* pTimeDelayThread = NULL;
	pTimeDelayThread = new m_oTimeDelayThreadStruct;
	pTimeDelayThread->m_pThread = new m_oThreadStruct;
	pTimeDelayThread->m_pLogOutPutTimeDelay = NULL;
	pTimeDelayThread->m_pInstrumentList = NULL;
	pTimeDelayThread->m_pRoutList = NULL;
	pTimeDelayThread->m_pTailTimeFrame = NULL;
	pTimeDelayThread->m_pTimeDelayFrame = NULL;
	return pTimeDelayThread;
}
// 线程等待函数
MatrixServerDll_API void WaitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pTimeDelayThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pTimeDelayThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pTimeDelayThread->m_pThread->m_pConstVar->m_iTimeDelaySleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 尾包时刻查询准备工作
MatrixServerDll_API void PrepareTailTimeFrame(m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar)
{
	if (pRout == NULL)
	{
		return;
	}
	if (pConstVar == NULL)
	{
		return;
	}

	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = pRout->m_pHead;
	pInstrument->m_bTailTimeQueryOK = false;
	pInstrument->m_iTailTimeQueryCount++;
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pConstVar);
		pInstrument = pInstrumentNext;
		/** 尾包时刻查询计数*/
		pInstrumentNext->m_iTailTimeQueryCount++;
		/** 尾包时刻查询是否成功*/
		pInstrumentNext->m_bTailTimeQueryOK = false;
	} while (pInstrumentNext != pRout->m_pTail);
}
// 处理尾包时刻查询
MatrixServerDll_API void ProcTailTimeFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTailTimeFrame", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	CString str = _T("");
	// 准备工作
	PrepareTailTimeFrame(pRout, pTimeDelayThread->m_pThread->m_pConstVar);
	// 按IP查询路由头仪器的尾包时刻
	MakeInstrumentTailTimeQueryFramebyIP(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pHead->m_uiIP);
	SendInstrumentTailTimeQueryFrame(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar);
	str.Format(_T("向仪器IP地址 = 0x%x 的仪器发送尾包时刻查询帧"), pRout->m_pHead->m_uiIP);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(str));
	// 广播查询路由仪器的尾包时刻
	MakeInstrumentTailTimeQueryFramebyBroadCast(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort);
	SendInstrumentTailTimeQueryFrame(pTimeDelayThread->m_pTailTimeFrame,
		pTimeDelayThread->m_pThread->m_pConstVar);
	str.Format(_T("向广播端口 = 0x%x 的仪器广播发送尾包时刻查询帧"), pRout->m_pTail->m_uiBroadCastPort);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(str));
}
// 处理单个尾包时刻查询应答帧
MatrixServerDll_API void ProcTailTimeReturnFrameOne(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOutPut = _T("");
	// 判断仪器IP是否在SN索引表中
	if (TRUE == IfIndexExistInMap(pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSrcIP,
		&pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSrcIP,
			&pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap);
		// @@@@@@@暂不判断尾包时刻过期的情况
		// 接收到尾包时刻查询应答标志位设为true
		pInstrument->m_bTailTimeQueryOK = true;
		pInstrument->m_iTailTimeReturnCount++;
		str.Format(_T("接收到IP地址 = 0x%x 仪器的尾包时刻查询应答帧,	"), pInstrument->m_uiIP);
		strOutPut += str;
		// 如果仪器类型为交叉站或者LCI
		if ((pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 如果为交叉线方向路由
			if (pRout->m_bRoutLaux == true)
			{
				pInstrument->m_usCrossTopReceiveTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineA;
				pInstrument->m_usCrossDownReceiveTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLAUXLineB;
				str.Format(_T("交叉站交叉线A尾包接收时刻 = 0x%x，交叉站交叉线B尾包接收时刻 = 0x%x,	"), 
					pInstrument->m_usCrossTopReceiveTime, pInstrument->m_usCrossDownReceiveTime);
				strOutPut += str;
			}
			// 大线方向
			else
			{
				pInstrument->m_usLineLeftReceiveTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineA;
				pInstrument->m_usLineRightReceiveTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usLAUXTailRecTimeLineB;
				str.Format(_T("交叉站大线A尾包接收时刻 = 0x%x，交叉站大线B尾包接收时刻 = 0x%x,	"), 
					pInstrument->m_usLineLeftReceiveTime, pInstrument->m_usLineRightReceiveTime);
				strOutPut += str;
			}
		}
		else
		{
			// 采集站或电源站尾包接收时刻
			pInstrument->m_usReceiveTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailRecTime;
			str.Format(_T("采集站尾包接收时刻 = 0x%x,	"), pInstrument->m_usReceiveTime);
			strOutPut += str;
		}
		// 尾包发送时刻
		pInstrument->m_usSendTime = pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_usTailSndTime;
		str.Format(_T("尾包发送时刻 = 0x%x"), pInstrument->m_usSendTime);
		strOutPut += str;
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(strOutPut));
	}
	else
	{
		str.Format(_T("接收到IP地址 = 0x%x 仪器的尾包时刻查询应答帧"), pTimeDelayThread->m_pTailTimeFrame->m_pCommandStructReturn->m_uiSrcIP);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTailTimeReturnFrameOne", ConvertCStrToStr(str),
			ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 处理尾包时刻查询应答
MatrixServerDll_API void ProcTailTimeReturnFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTimeDelayThread->m_pTailTimeFrame->m_oTailTimeFrameSocket,
				pTimeDelayThread->m_pTailTimeFrame->m_pcRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTailTimeReturnFrame(pTimeDelayThread->m_pTailTimeFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ParseInstrumentTailTimeReturnFrame", 
						"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				}
				else
				{
					// 处理单个尾包时刻查询应答帧
					ProcTailTimeReturnFrameOne(pRout, pTimeDelayThread);
				}	
			}		
		}		
	}
}
// 检查尾包时刻查询结果是否接收完全
MatrixServerDll_API bool CheckTailTimeReturn(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return false;
	}
	m_oInstrumentStruct* pInstrumentNext = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	pInstrument = pRout->m_pHead;
	if (pInstrument->m_bTailTimeQueryOK == false)
	{
		str.Format(_T("没有收到路由IP = 0x%x的路由头仪器IP = 0x%x的尾包时刻查询帧"), 
			pRout->m_uiRoutIP, pInstrument->m_uiIP);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", ConvertCStrToStr(str), WarningType);
		return false;
	}
	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pTimeDelayThread->m_pThread->m_pConstVar);
		pInstrument = pInstrumentNext;
		/** 尾包时刻查询是否成功*/
		if (pInstrumentNext->m_bTailTimeQueryOK == false)
		{
			str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收不完全"), pRout->m_uiRoutIP);
			AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "CheckTailTimeReturn", ConvertCStrToStr(str), WarningType);
			return false;
		}
	} while (pInstrumentNext != pRout->m_pTail);
	str.Format(_T("路由IP = 0x%x的仪器的尾包时刻查询接收完全"), pRout->m_uiRoutIP);
	AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "CheckTailTimeReturn", ConvertCStrToStr(str));
	return true;
}
// 处理时统设置
MatrixServerDll_API void ProcTimeDelayFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	if (pRout == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTimeDelayFrame", "pRout", 
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 临时变量
	int itmp1 = 0;
	int itmp2 = 0;
	CString str = _T("");
	CString strOutPut = _T("");
	pInstrument = pRout->m_pHead;

	do 
	{
		pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, pTimeDelayThread->m_pThread->m_pConstVar);
		// 仪器类型为LCI或交叉站
		if ((pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pTimeDelayThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 如果路由方向为上方
			if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionTop)
			{
				itmp1 = pInstrument->m_usCrossTopReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为下方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionDown)
			{
				itmp1 = pInstrument->m_usCrossDownReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为左方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionLeft)
			{
				itmp1 = pInstrument->m_usLineLeftReceiveTime - pInstrumentNext->m_usSendTime;
			}
			// 如果路由方向为右方
			else if (pRout->m_iRoutDirection == pTimeDelayThread->m_pThread->m_pConstVar->m_iDirectionRight)
			{
				itmp1 = pInstrument->m_usLineRightReceiveTime - pInstrumentNext->m_usSendTime;
			}
		}
		else
		{
			itmp1 = pInstrument->m_usReceiveTime - pInstrumentNext->m_usSendTime;
		}
		if (itmp1 < 0)
		{
			itmp1 += 0x3fff;
		}
		str.Format(_T("IP地址 = 0x%x 的仪器的尾包时刻差值为 %d,	"), pInstrumentNext->m_uiIP, itmp1);
		strOutPut = str;
		itmp1 = itmp1 / 2;
		itmp2 += itmp1;
		// 时间修正低位
		pInstrumentNext->m_uiTimeLow = itmp2 & 0x3fff;
		// 时间修正高位
		pInstrumentNext->m_uiTimeHigh = (pInstrumentNext->m_uiNetTime - pInstrumentNext->m_uiSystemTime) & 0xffffffff;
		// 在数据采集期间只针对未时统的仪器进行时统设置
		if (pTimeDelayThread->m_bADCStartSample == true)
		{
			if (pInstrumentNext->m_iTimeSetReturnCount == 0)
			{
				// 时统设置次数加一
				pInstrumentNext->m_iTimeSetCount++;
				// 生成并发送时统设置帧
				MakeInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, pTimeDelayThread->m_pThread->m_pConstVar, pInstrumentNext);
				SendInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, pTimeDelayThread->m_pThread->m_pConstVar);
				str.Format(_T("发送时统设置帧时统修正高位为 0x%x，时统修正低位为 0x%x"), 
					pInstrumentNext->m_uiTimeHigh, pInstrumentNext->m_uiTimeLow);
				strOutPut += str;
				AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(strOutPut));
			}
		}
		else
		{
			// 时统设置次数加一
			pInstrumentNext->m_iTimeSetCount++;
			// 生成并发送时统设置帧
			MakeInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, pTimeDelayThread->m_pThread->m_pConstVar, pInstrumentNext);
			SendInstrumentDelayTimeFrame(pTimeDelayThread->m_pTimeDelayFrame, pTimeDelayThread->m_pThread->m_pConstVar);
			str.Format(_T("发送时统设置帧时统修正高位为 0x%x，时统修正低位为 0x%x"), 
				pInstrumentNext->m_uiTimeHigh, pInstrumentNext->m_uiTimeLow);
			strOutPut += str;
			AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(strOutPut));
		}
		pInstrument = pInstrumentNext;
	} while (pInstrumentNext != pRout->m_pTail);
}
// 处理单个时统设置应答
MatrixServerDll_API void ProcTimeDelayReturnFrameOne(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 新仪器指针为空
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	// 判断仪器IP是否在SN索引表中
	if (TRUE == IfIndexExistInMap(pTimeDelayThread->m_pTimeDelayFrame->m_pCommandStructReturn->m_uiSrcIP,
		&pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(pTimeDelayThread->m_pTimeDelayFrame->m_pCommandStructReturn->m_uiSrcIP,
			&pTimeDelayThread->m_pInstrumentList->m_oIPInstrumentMap);
		// 接收到时统设置应答标志位
		pInstrument->m_bTimeSetOK = true;
		pInstrument->m_iTimeSetReturnCount++;
		str.Format(_T("接收到IP地址 = 0x%x 仪器的时统设置应答帧"), pInstrument->m_uiIP);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(str));
	}
	else
	{
		str.Format(_T("接收到IP地址 = 0x%x 仪器的时统设置应答帧"), pTimeDelayThread->m_pTimeDelayFrame->m_pCommandStructReturn->m_uiSrcIP);
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ProcTimeDelayReturnFrameOne", ConvertCStrToStr(str),
			ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 处理时统设置应答
MatrixServerDll_API void ProcTimeDelayReturnFrame(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
		pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pTimeDelayThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pTimeDelayThread->m_pTimeDelayFrame->m_oTimeDelayFrameSocket,
				pTimeDelayThread->m_pTimeDelayFrame->m_pcRcvFrameData, 
				pTimeDelayThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pTimeDelayThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentTimeDelayReturnFrame(pTimeDelayThread->m_pTimeDelayFrame, 
					pTimeDelayThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "ParseInstrumentTimeDelayReturnFrame", 
						"", ErrorType, IDS_ERR_PARSE_HEADFRAME);
				}
				else
				{
					// 处理单个时统设置应答帧
					ProcTimeDelayReturnFrameOne(pTimeDelayThread);
				}	
			}		
		}		
	}
}
// 线程函数
DWORD WINAPI RunTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return 1;
	}
	m_oRoutStruct* pRout = NULL;
	CString str = _T("");
	unsigned int uiProcRoutIP = 0;
	while(true)
	{
		if (pTimeDelayThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pTimeDelayThread->m_pThread->m_bWork == true)
		{
			EnterCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
			EnterCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
			if (pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.size() > 0)
			{
				pTimeDelayThread->m_uiCounter++;
				if (pTimeDelayThread->m_uiCounter == 1)
				{
					// 得到时统队列头的路由IP地址作为当前任务处理的路由IP
					uiProcRoutIP = *pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.begin();
					// 当前操作的路由IP在路由索引表中
					if (TRUE == IfIndexExistInRoutMap(uiProcRoutIP, &pTimeDelayThread->m_pRoutList->m_oRoutMap))
					{
						// 得到当前操作的路由指针
						pRout = GetRout(uiProcRoutIP, &pTimeDelayThread->m_pRoutList->m_oRoutMap);
					}
					else
					{
						str.Format(_T("路由IP = 0x%x"), pRout->m_uiRoutIP);
						AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "RunTimeDelayThread", ConvertCStrToStr(str), 
							ErrorType, IDS_ERR_ROUT_NOTEXIT);
					}
					// 处理尾包时刻查询
					ProcTailTimeFrame(pRout, pTimeDelayThread);
				}
				else if (pTimeDelayThread->m_uiCounter == 5)
				{
					// 处理尾包时刻查询应答
					ProcTailTimeReturnFrame(pRout, pTimeDelayThread);
					// 检查尾包时刻查询结果是否接收完全
					if (false == CheckTailTimeReturn(pRout, pTimeDelayThread))
					{
						pTimeDelayThread->m_uiCounter = 0;
					}
					else
					{
						// 处理时统设置
						ProcTimeDelayFrame(pRout, pTimeDelayThread);
					}
				}
				else if (pTimeDelayThread->m_uiCounter == 10)
				{
					// 处理时统设置应答
					ProcTimeDelayReturnFrame(pTimeDelayThread);
					// 判断该路由方向是否完成时统
					// 检查时统设置应答是否接收完全
					if (true == CheckTimeDelayReturnByRout(pRout, pTimeDelayThread, false))
					{
						// 时统任务移到队列尾
						pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.pop_front();
						pTimeDelayThread->m_pRoutList->m_olsTimeDelayTaskQueue.push_back(uiProcRoutIP);
					}
				}
				else if (pTimeDelayThread->m_uiCounter == 15)
				{
					pTimeDelayThread->m_uiCounter = 0;
				}
			}
			LeaveCriticalSection(&pTimeDelayThread->m_pRoutList->m_oSecRoutList);
			LeaveCriticalSection(&pTimeDelayThread->m_pInstrumentList->m_oSecInstrumentList);
		}
		if (pTimeDelayThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitTimeDelayThread(pTimeDelayThread);
	}
	::SetEvent(pTimeDelayThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化时统设置线程
MatrixServerDll_API bool OnInitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	pTimeDelayThread->m_bADCStartSample = false;
	pTimeDelayThread->m_uiCounter = 0;
	if (false == OnInitThread(pTimeDelayThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
	}
	::ResetEvent(pTimeDelayThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pTimeDelayThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunTimeDelayThread,
		pTimeDelayThread, 
		0, 
		&pTimeDelayThread->m_pThread->m_dwThreadID);
	if (pTimeDelayThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", 
			"pTimeDelayThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnInitTimeDelayThread", "时统设置线程创建成功");
	return true;
}
// 初始化时统设置线程
MatrixServerDll_API bool OnInit_TimeDelayThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pTimeDelayThread->m_pLogOutPutTimeDelay = pEnv->m_pLogOutPutTimeDelay;
	pEnv->m_pTimeDelayThread->m_pTailTimeFrame = pEnv->m_pTailTimeFrame;
	pEnv->m_pTimeDelayThread->m_pTimeDelayFrame = pEnv->m_pTimeDelayFrame;
	pEnv->m_pTimeDelayThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pTimeDelayThread->m_pRoutList = pEnv->m_pRoutList;
	return OnInitTimeDelayThread(pEnv->m_pTimeDelayThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭时统设置线程
MatrixServerDll_API bool OnCloseTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pTimeDelayThread->m_pThread))
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", 
			"时统设置线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pTimeDelayThread->m_pThread->m_pLogOutPut, "OnCloseTimeDelayThread", "时统设置线程成功关闭");
		return true;
	}
}
// 释放时统设置线程
MatrixServerDll_API void OnFreeTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread)
{
	if (pTimeDelayThread == NULL)
	{
		return;
	}
	if (pTimeDelayThread->m_pThread != NULL)
	{
		delete pTimeDelayThread->m_pThread;
	}
	delete pTimeDelayThread;
}
// 创建ADC参数设置线程
MatrixServerDll_API m_oADCSetThreadStruct* OnCreateADCSetThread(void)
{
	m_oADCSetThreadStruct* pADCSetThread = NULL;
	pADCSetThread = new m_oADCSetThreadStruct;
	pADCSetThread->m_pThread = new m_oThreadStruct;
	pADCSetThread->m_pADCSetFrame = NULL;
	pADCSetThread->m_pInstrumentList = NULL;
	pADCSetThread->m_pRoutList = NULL;
	pADCSetThread->m_pLogOutPutADCFrameTime = NULL;
	return pADCSetThread;
}
// 发送ADC命令设置帧
MatrixServerDll_API void OnSelectADCSetCmd(m_oADCSetThreadStruct* pADCSetThread, bool bRout, unsigned int uiDstIP)
{
	CString str = _T("");
	unsigned int uiTemp = 0;
	EnterCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	// 查询命令字个数
	pADCSetThread->m_pADCSetFrame->m_usCommandWordNum = 0;
	// 按照路由的广播端口广播发送ADC参数设置命令帧
	if (bRout == true)
	{
		// 目标IP地址
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = pADCSetThread->m_pThread->m_pConstVar->m_iIPBroadcastAddr;
		// 广播端口
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiBroadCastPortSeted = uiDstIP;
		// 查询命令字内容
		// 广播端口
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdBroadCastPortSeted;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		str.Format(_T("向广播端口 = 0x%x 的仪器广播发送ADC参数设置帧，命令序号 = %d"), 
			uiDstIP, pADCSetThread->m_uiADCSetOperationNb);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", ConvertCStrToStr(str));
	}
	// 按照IP地址发送ADC参数设置命令帧
	else
	{
		// 目标IP地址
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiDstIP = uiDstIP;
		str.Format(_T("向IP地址 = 0x%x 的仪器发送ADC参数设置帧，命令序号 = %d"), 
			uiDstIP, pADCSetThread->m_uiADCSetOperationNb);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", ConvertCStrToStr(str));
	}
	// 1~11为ADC参数设置命令，12~18为ADC开始采样命令，19~22为ADC停止采样命令
	switch (pADCSetThread->m_uiADCSetOperationNb)
	{
	case 1:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 2:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置正弦波命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCSetSine;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCSetSineSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 3:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 4:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源低位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCOpenTBPowerLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCOpenTBPowerLowSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 5:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB电源高位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCOpenTBPowerHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCOpenTBPowerHighSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 6:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关低位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCOpenSwitchTBLow;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCOpenSwitchTBLowSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 7:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关高位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCOpenSwitchTBHigh;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCOpenSwitchTBHighSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 8:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 9:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置写寄存器
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCRegisterWrite;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCRegisterWriteSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 10:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置读寄存器
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCRegisterRead;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCRegisterReadSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 11:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置数据返回端口
		// 自动AD返回地址
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCDataReturnAddr;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 自动数据返回端口和命令
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCDataReturnPort;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 端口递增下限和上限
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCDataReturnPortLimit;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 12:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置打开TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCTBSwitchOpen;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCTBSwitchOpenSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 13:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 14:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 15:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置ADC数据采样率等参数
		// @@@需要界面设置采样率等参数，暂选为1K采样率
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 16:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置连续采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCReadContinuous;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCReadContinuousSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 17:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendQueryCmd;
		// 查询仪器的当前系统时间
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 18:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB时间高位
		if (pADCSetThread->m_uiLocalSysTime < pADCSetThread->m_uiTBTimeOld)
		{
			uiTemp = (0xffffffff - pADCSetThread->m_uiTBTimeOld + pADCSetThread->m_uiLocalSysTime) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		else
		{
			uiTemp = (pADCSetThread->m_uiLocalSysTime - pADCSetThread->m_uiTBTimeOld) % (pADCSetThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum * 16);
		}
		if (pADCSetThread->m_uiLocalSysTime < uiTemp)
		{
			pADCSetThread->m_uiLocalSysTime = 0xffffffff - uiTemp + pADCSetThread->m_uiLocalSysTime;
		}
		else
		{
			pADCSetThread->m_uiLocalSysTime -= uiTemp;
		}
		pADCSetThread->m_uiTBTimeOld = pADCSetThread->m_uiLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiTBHigh = 
			pADCSetThread->m_pThread->m_pConstVar->m_uiTBSleepTimeHigh + pADCSetThread->m_uiLocalSysTime;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdTBHigh;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		// 设置TB时间低位
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = pADCSetThread->m_pThread->m_pConstVar->m_usTBSleepTimeLow;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = pADCSetThread->m_pThread->m_pConstVar->m_usCmdTBCtrlStartSample
			+ pADCSetThread->m_pThread->m_pConstVar->m_usCmdCtrlCloseLed;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		str.Format(_T("ADC开始采样：TB时间高位 = 0x%x，TB时间低位 = 0x%x"), 
			pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_uiTBHigh, 
			pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnSelectADCSetCmd", ConvertCStrToStr(str));
		break;
	case 19:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 20:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 21:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// ADC设置停止采样命令
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_pcADCSet = pADCSetThread->m_pThread->m_pConstVar->m_pSetADCStopSample;
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_iADCSetNum = pADCSetThread->m_pThread->m_pConstVar->m_iSetADCStopSampleSize;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdADCSet;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	case 22:
		// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usCommand = pADCSetThread->m_pThread->m_pConstVar->m_usSendSetCmd;
		// 设置TB低位关闭TB开关
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBLow = 0;
		// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
		pADCSetThread->m_pADCSetFrame->m_pCommandStructSet->m_usTBCtrl = 0;
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord[pADCSetThread->m_pADCSetFrame->m_usCommandWordNum] = pADCSetThread->m_pThread->m_pConstVar->m_byCmdTbLow;
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum++;
		break;
	default:
		break;
	}
	MakeInstrumentFrame(pADCSetThread->m_pADCSetFrame->m_pCommandStructSet, 
		pADCSetThread->m_pThread->m_pConstVar, 
		pADCSetThread->m_pADCSetFrame->m_pcSndFrameData, 
		pADCSetThread->m_pADCSetFrame->m_pcCommandWord, 
		pADCSetThread->m_pADCSetFrame->m_usCommandWordNum);
	LeaveCriticalSection(&pADCSetThread->m_pADCSetFrame->m_oSecADCSetFrame);
	// 发送ADC参数设置帧
	SendInstrumentADCSetFrame(pADCSetThread->m_pADCSetFrame, pADCSetThread->m_pThread->m_pConstVar);
}
// 发送ADC命令设置帧
MatrixServerDll_API void OnSendADCSetCmd(m_oADCSetThreadStruct* pADCSetThread)
{
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	for (iter = pADCSetThread->m_pRoutList->m_oADCSetRoutMap.begin();iter != pADCSetThread->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		iter->second->m_bADCSetReturn = false;
		// 重置ADC参数设置应答标志位
		pInstrument = iter->second->m_pHead;
		do 
		{
			pInstrumentNext = GetNextInstrument(iter->second->m_iRoutDirection, pInstrument, pADCSetThread->m_pThread->m_pConstVar);
			pInstrumentNext->m_bADCSetReturn = false;
			pInstrument = pInstrumentNext;
		} while (pInstrument != iter->second->m_pTail);
		if (iter->second->m_bADCSetRout == true)
		{
			// 选择ADC参数设置命令
			OnSelectADCSetCmd(pADCSetThread, true, iter->second->m_pTail->m_uiBroadCastPort);
			pADCSetThread->m_uiADCSetNum += iter->second->m_uiInstrumentNum;
		}
	}
	for (iter2 = pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.begin();iter2 != pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		// 重置ADC参数设置应答标志位
		iter2->second->m_bADCSetReturn = false;
		// 选择ADC参数设置命令
		OnSelectADCSetCmd(pADCSetThread, false, iter2->second->m_uiIP);
		pADCSetThread->m_uiADCSetNum++;
	}
}
// 处理ADC参数设置应答帧
MatrixServerDll_API void ProcADCSetReturnFrameOne(m_oADCSetThreadStruct* pADCSetThread)
{
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 得到仪器IP
	uiIPInstrument = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, &pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap);
		pInstrument->m_bADCSetReturn = true;
		if (pADCSetThread->m_uiADCSetOperationNb == 17)
		{
			pADCSetThread->m_uiLocalSysTime = pADCSetThread->m_pADCSetFrame->m_pCommandStructReturn->m_uiSysTime;
			str.Format(_T("IP地址 = 0x%x的仪器本地时间为 = 0x%x"), uiIPInstrument, pADCSetThread->m_uiLocalSysTime);
			AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne", ConvertCStrToStr(str));
		}
	}
	else
	{
		str.Format(_T("接收到IP地址 = 0x%x的ADC参数设置应答"), uiIPInstrument);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ProcADCSetReturnFrameOne", ConvertCStrToStr(str),
			ErrorType, IDS_ERR_IPMAP_NOTEXIT);
	}
}
// 输出接收和发送帧的统计结果
MatrixServerDll_API void OnOutPutADCDataRecResult(m_oADCSetThreadStruct* pADCSetThread)
{
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	CString str = _T("");
	m_oInstrumentStruct* pInstrument = NULL;

	// 应当接收到的ADC数据帧数
	int iADCDataShouldRecFrameNum = 0;
	// 实际接收到的ADC数据帧数
	int iADCDataActualRecFrameNum = 0;

	for (iter = pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap.begin();
		iter != pADCSetThread->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		pInstrument = iter->second;
		// 仪器类型为采集站
		if (pInstrument->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
		{
			str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，应收ADC数据帧数 %d，实际接收帧数 %d，丢失帧数%d"),
				pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_uiADCDataShouldRecFrameNum, 
				pInstrument->m_uiADCDataActualRecFrameNum, pInstrument->m_uiADCDataShouldRecFrameNum - pInstrument->m_uiADCDataActualRecFrameNum);
			AddMsgToLogOutPutList(pADCSetThread->m_pLogOutPutADCFrameTime, "", ConvertCStrToStr(str));
			iADCDataActualRecFrameNum += pInstrument->m_uiADCDataActualRecFrameNum;
			iADCDataShouldRecFrameNum += pInstrument->m_uiADCDataShouldRecFrameNum;
		}
	}
	// ADC数据接收帧结果统计
	str.Format(_T("应收ADC数据帧数%d， 实际接收帧数%d，丢失帧数%d"), iADCDataShouldRecFrameNum, 
		iADCDataActualRecFrameNum, iADCDataShouldRecFrameNum - iADCDataActualRecFrameNum);
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnOutPutADCDataRecResult", ConvertCStrToStr(str));
	pADCSetThread->m_pLogOutPutADCFrameTime->m_uiLogInfoCount = OutPutLogFileInfoNumLimit;
}
// 命令应答接收完全后的操作
MatrixServerDll_API void OnADCSetNextOpt(m_oADCSetThreadStruct* pADCSetThread)
{
	// 执行下一步ADC命令发送并开启应答监视定时器
	pADCSetThread->m_uiADCSetOperationNb++;
	pADCSetThread->m_uiCounter = 0;
	// 完成ADC参数设置
	if (pADCSetThread->m_uiADCSetOperationNb == 12)
	{
		pADCSetThread->m_uiADCSetOperationNb = 0;
		// ADC参数设置线程停止工作
		pADCSetThread->m_pThread->m_bWork = false;
		return;
	}
	// 完成ADC开始数据采集
	else if (pADCSetThread->m_uiADCSetOperationNb == 19)
	{
		pADCSetThread->m_uiADCSetOperationNb = 0;
		// ADC参数设置线程停止工作
		pADCSetThread->m_pThread->m_bWork = false;
		return;
	}
	// 完成ADC停止数据采集
	else if (pADCSetThread->m_uiADCSetOperationNb == 23)
	{
		pADCSetThread->m_uiADCSetOperationNb = 0;
		// ADC参数设置线程停止工作
		pADCSetThread->m_pThread->m_bWork = false;
		OnOutPutADCDataRecResult(pADCSetThread);
		return;
	}
}
// 判断ADC参数设置应答是否接收完全
MatrixServerDll_API bool CheckADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	hash_map<unsigned int, m_oInstrumentStruct*> ::iterator iter2;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	bool bADCSetRoutReturn = true;
	bool bReturn = true; 
	for (iter = pADCSetThread->m_pRoutList->m_oADCSetRoutMap.begin();iter != pADCSetThread->m_pRoutList->m_oADCSetRoutMap.end(); iter++)
	{
		if (false == iter->second->m_bADCSetReturn)
		{
			bADCSetRoutReturn = true;
			pInstrument = iter->second->m_pHead;
			do 
			{
				pInstrumentNext = GetNextInstrument(iter->second->m_iRoutDirection, pInstrument, pADCSetThread->m_pThread->m_pConstVar);
				if (pInstrumentNext->m_iInstrumentType == pADCSetThread->m_pThread->m_pConstVar->m_iInstrumentTypeFDU)
				{
					if (false == pInstrumentNext->m_bADCSetReturn)
					{
						// 找不到则插入索引表
						// 仪器索引表中已经有的路由不再广播发送ADC参数设置
						if (FALSE == IfIndexExistInMap(pInstrumentNext->m_uiIP, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap))
						{
							AddInstrumentToMap(pInstrumentNext->m_uiIP, pInstrumentNext, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
						}
						bReturn = false;
						bADCSetRoutReturn = false;
					}
					else
					{
						if (pADCSetThread->m_uiADCSetOperationNb == 11)
						{
							pInstrumentNext->m_bADCSet = true;
						}
						else if (pADCSetThread->m_uiADCSetOperationNb == 18)
						{
							pInstrumentNext->m_bADCStartSample = true;
						}
						else if (pADCSetThread->m_uiADCSetOperationNb == 22)
						{
							pInstrumentNext->m_bADCStopSample = true;
						}
						// 如果在索引表中找到该仪器则删除
						DeleteInstrumentFromMap(pInstrumentNext->m_uiIP, &pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap);
					}
				}
				pInstrument = pInstrumentNext;
			} while (pInstrument != iter->second->m_pTail);
			iter->second->m_bADCSetReturn = bADCSetRoutReturn;
			iter->second->m_bADCSetRout = bADCSetRoutReturn;
		}
	}
	for (iter2 = pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.begin();iter2 != pADCSetThread->m_pInstrumentList->m_oADCSetInstrumentMap.end(); iter2++)
	{
		if (false == iter2->second->m_bADCSetReturn)
		{
			bReturn = false;
			break;
		}
		else
		{
			if (pADCSetThread->m_uiADCSetOperationNb == 11)
			{
				iter2->second->m_bADCSet = true;
			}
			else if (pADCSetThread->m_uiADCSetOperationNb == 18)
			{
				iter2->second->m_bADCStartSample = true;
			}
			else if (pADCSetThread->m_uiADCSetOperationNb == 22)
			{
				iter2->second->m_bADCStopSample = true;
			}
		}
	}
	return bReturn;
}
// 处理ADC参数设置应答帧
MatrixServerDll_API void ProcADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	CString str = _T("");
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
				pADCSetThread->m_pADCSetFrame->m_pcRcvFrameData, 
				pADCSetThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCSetThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentADCSetReturnFrame(pADCSetThread->m_pADCSetFrame, 
					pADCSetThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCSetReturnFrame",
						"", ErrorType, IDS_ERR_PARSE_IPSETRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个ADC参数设置应答帧
					ProcADCSetReturnFrameOne(pADCSetThread);
					pADCSetThread->m_uiADCSetReturnNum++;
					LeaveCriticalSection(&pADCSetThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
	// 判断ADC参数设置应答是否接收完全
	if (true == CheckADCSetReturnFrame(pADCSetThread))
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收完全"), pADCSetThread->m_uiADCSetOperationNb);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", ConvertCStrToStr(str));
		// 命令应答接收完全后的操作
		OnADCSetNextOpt(pADCSetThread);
	}
	else
	{
		str.Format(_T("ADC参数设置命令 %d 应答接收不完全"), pADCSetThread->m_uiADCSetOperationNb);
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "CheckADCSetReturnFrame", ConvertCStrToStr(str), WarningType);
	}
}
// 线程等待函数
MatrixServerDll_API void WaitADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCSetThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pADCSetThread->m_pThread->m_bClose == true)
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
		if (pADCSetThread->m_pThread->m_bClose == true)
		{
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
			break;
		}
		WaitADCSetThread(pADCSetThread);
	}
	::SetEvent(pADCSetThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化ADC参数设置线程
MatrixServerDll_API bool OnInitADCSetThread(m_oADCSetThreadStruct* pADCSetThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pADCSetThread == NULL)
	{
		return false;
	}
	pADCSetThread->m_uiADCSetOperationNb = 0;
	pADCSetThread->m_bADCStartSample = false;
	pADCSetThread->m_bADCStopSample = false;
	pADCSetThread->m_uiLocalSysTime = 0;
	pADCSetThread->m_uiTBTimeOld = 0;
	pADCSetThread->m_uiADCSetNum = 0;
	pADCSetThread->m_uiADCSetReturnNum = 0;
	if (false == OnInitThread(pADCSetThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
	}
	::ResetEvent(pADCSetThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pADCSetThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCSetThread,
		pADCSetThread, 
		0, 
		&pADCSetThread->m_pThread->m_dwThreadID);
	if (pADCSetThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", 
			"pADCSetThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnInitADCSetThread", "ADC参数设置线程创建成功");
	return true;
}
// 初始化ADC参数设置线程
MatrixServerDll_API bool OnInit_ADCSetThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pADCSetThread->m_pADCSetFrame = pEnv->m_pADCSetFrame;
	pEnv->m_pADCSetThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	pEnv->m_pADCSetThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pADCSetThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	return OnInitADCSetThread(pEnv->m_pADCSetThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC参数设置线程
MatrixServerDll_API bool OnCloseADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
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
	else
	{
		AddMsgToLogOutPutList(pADCSetThread->m_pThread->m_pLogOutPut, "OnCloseADCSetThread", "ADC参数设置线程成功关闭");
		return true;
	}
}
// 释放ADC参数设置线程
MatrixServerDll_API void OnFreeADCSetThread(m_oADCSetThreadStruct* pADCSetThread)
{
	if (pADCSetThread == NULL)
	{
		return;
	}
	if (pADCSetThread->m_pThread != NULL)
	{
		delete pADCSetThread->m_pThread;
	}
	delete pADCSetThread;
}
// 创建误码查询线程
MatrixServerDll_API m_oErrorCodeThreadStruct* OnCreateErrorCodeThread(void)
{
	m_oErrorCodeThreadStruct* pErrorCodeThread = NULL;
	pErrorCodeThread = new m_oErrorCodeThreadStruct;
	pErrorCodeThread->m_pThread = new m_oThreadStruct;
	pErrorCodeThread->m_pLogOutPutErrorCode = NULL;
	pErrorCodeThread->m_pErrorCodeFrame = NULL;
	pErrorCodeThread->m_pRoutList = NULL;
	pErrorCodeThread->m_pInstrumentList = NULL;
	return pErrorCodeThread;
}
// 线程等待函数
MatrixServerDll_API void WaitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pErrorCodeThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pErrorCodeThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pErrorCodeThread->m_pThread->m_pConstVar->m_iErrorCodeSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 处理单个误码查询应答帧
MatrixServerDll_API void ProcErrorCodeReturnFrameOne(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOutPut = _T("");
	char byTemp = 0;
	int iTemp = 0;
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 得到仪器IP
	uiIPInstrument = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pErrorCodeThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, &pErrorCodeThread->m_pInstrumentList->m_oIPInstrumentMap);
		str.Format(_T("仪器SN = 0x%x，IP = 0x%x；"), pInstrument->m_uiSN, pInstrument->m_uiIP);
		strOutPut += str;
		// 仪器类型为LCI或者交叉站
		if ((pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
			|| (pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLAUX))
		{
			// 交叉站大线A数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byLAUXErrorCodeDataLineACount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byLAUXErrorCodeDataLineACountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byLAUXErrorCodeDataLineACountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byLAUXErrorCodeDataLineACountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLineACount += iTemp;
			}
			pInstrument->m_byLAUXErrorCodeDataLineACountOld = byTemp;
			str.Format(_T("大线A数据故障数=%d，"),iTemp);
			strOutPut += str;
			// 交叉站大线B数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byLAUXErrorCodeDataLineBCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byLAUXErrorCodeDataLineBCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byLAUXErrorCodeDataLineBCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byLAUXErrorCodeDataLineBCountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLineBCount += iTemp;
			}
			pInstrument->m_byLAUXErrorCodeDataLineBCountOld = byTemp;
			str.Format(_T("大线B数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站交叉线A数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byLAUXErrorCodeDataLAUXLineACount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byLAUXErrorCodeDataLAUXLineACountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byLAUXErrorCodeDataLAUXLineACountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byLAUXErrorCodeDataLAUXLineACountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLAUXLineACount += iTemp;
			}
			pInstrument->m_byLAUXErrorCodeDataLAUXLineACountOld = byTemp;
			str.Format(_T("交叉线A数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站交叉线B数据故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byLAUXErrorCodeDataLAUXLineBCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byLAUXErrorCodeDataLAUXLineBCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byLAUXErrorCodeDataLAUXLineBCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byLAUXErrorCodeDataLAUXLineBCountOld;
				}
				pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount += iTemp;
			}
			pInstrument->m_byLAUXErrorCodeDataLAUXLineBCountOld = byTemp;
			str.Format(_T("交叉线B数据故障数=%d，"), iTemp);
			strOutPut += str;
			// 交叉站命令口故障
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byLAUXErrorCodeCmdCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byLAUXErrorCodeCmdCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byLAUXErrorCodeCmdCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byLAUXErrorCodeCmdCountOld;
				}
				// LCI不计算命令误码
				if (pInstrument->m_iInstrumentType == pErrorCodeThread->m_pThread->m_pConstVar->m_iInstrumentTypeLCI)
				{
					iTemp = 0;
				}
				pInstrument->m_iLAUXErrorCodeCmdCount += iTemp;
			}
			pInstrument->m_byLAUXErrorCodeCmdCountOld = byTemp;
			str.Format(_T("命令口故障数=%d；"), iTemp);
			strOutPut += str;
		}
		// 仪器类型为采集站或者电源站
		else
		{
			// 采集站和电源站网络数据错误计数
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byFDUErrorCodeDataCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byFDUErrorCodeDataCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byFDUErrorCodeDataCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byFDUErrorCodeDataCountOld;
				}
				pInstrument->m_iFDUErrorCodeDataCount += iTemp;
			}
			pInstrument->m_byFDUErrorCodeDataCountOld = byTemp;
			str.Format(_T("网络数据错误计数=%d，"), iTemp);
			strOutPut += str;
			// 采集站和电源站命令错误计数
			byTemp = pErrorCodeThread->m_pErrorCodeFrame->m_pCommandStructReturn->m_byFDUErrorCodeCmdCount;
			if (pInstrument->m_uiErrorCodeReturnNum != 0)
			{
				if (pInstrument->m_byFDUErrorCodeCmdCountOld > byTemp)
				{
					iTemp = 256 + byTemp - pInstrument->m_byFDUErrorCodeCmdCountOld;
				}
				else
				{
					iTemp = byTemp - pInstrument->m_byFDUErrorCodeCmdCountOld;
				}
				pInstrument->m_iFDUErrorCodeCmdCount += iTemp;
			}
			pInstrument->m_byFDUErrorCodeCmdCountOld = byTemp;
			str.Format(_T("命令错误计数=%d；"), iTemp);
			strOutPut += str;
		}
		pInstrument->m_uiErrorCodeReturnNum++;
		str.Format(_T("接收帧数=%d"), pInstrument->m_uiErrorCodeReturnNum);
		strOutPut += str;
		AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", ConvertCStrToStr(strOutPut));
	}
	else
	{
		str.Format(_T("接收到IP地址 = 0x%x的误码查询应答"), uiIPInstrument);
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "ProcErrorCodeReturnFrameOne", ConvertCStrToStr(str),
			ErrorType, IDS_ERR_IPSETMAP_NOTEXIT);
	}
}
// 处理误码查询应答帧
MatrixServerDll_API void ProcErrorCodeReturnFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
		pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pErrorCodeThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket,
				pErrorCodeThread->m_pErrorCodeFrame->m_pcRcvFrameData, 
				pErrorCodeThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pErrorCodeThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentErrorCodeReturnFrame(pErrorCodeThread->m_pErrorCodeFrame, 
					pErrorCodeThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "ParseInstrumentErrorCodeReturnFrame",
						"", ErrorType, IDS_ERR_PARSE_IPSETRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个IP地址设置应答帧
					ProcErrorCodeReturnFrameOne(pErrorCodeThread);
					LeaveCriticalSection(&pErrorCodeThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 发送误码查询帧
MatrixServerDll_API void ProcErrorCodeQueryFrame(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	hash_map<unsigned int, m_oRoutStruct*> ::iterator iter;
	m_oRoutStruct* pRout = NULL;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentNext = NULL;
	CString str = _T("");
	EnterCriticalSection(&pErrorCodeThread->m_pRoutList->m_oSecRoutList);
	for (iter = pErrorCodeThread->m_pRoutList->m_oRoutMap.begin();
		iter != pErrorCodeThread->m_pRoutList->m_oRoutMap.end();
		iter++)
	{
		pRout = iter->second;
		if (pRout->m_pTail != NULL)
		{
			pInstrument = pRout->m_pHead;
			do 
			{
				pInstrumentNext = GetNextInstrument(pRout->m_iRoutDirection, pInstrument, 
					pErrorCodeThread->m_pThread->m_pConstVar);
				pInstrumentNext->m_uiErrorCodeQueryNum++;
				pInstrument = pInstrumentNext;
			} while (pInstrumentNext != pRout->m_pTail);
			// 广播发送误码查询帧
			MakeInstrumentErrorCodeQueryFrame(pErrorCodeThread->m_pErrorCodeFrame,
				pErrorCodeThread->m_pThread->m_pConstVar, pRout->m_pTail->m_uiBroadCastPort);
			SendInstrumentErrorCodeFrame(pErrorCodeThread->m_pErrorCodeFrame,
				pErrorCodeThread->m_pThread->m_pConstVar);
			str.Format(_T("向路由IP = 0x%x广播发送误码查询帧"), pRout->m_uiRoutIP);
			AddMsgToLogOutPutList(pErrorCodeThread->m_pLogOutPutErrorCode, "", ConvertCStrToStr(str));
		}
	}
	LeaveCriticalSection(&pErrorCodeThread->m_pRoutList->m_oSecRoutList);
}
// 线程函数
DWORD WINAPI RunErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pErrorCodeThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pErrorCodeThread->m_pThread->m_bWork == true)
		{
			// 处理误码查询应答帧
			ProcErrorCodeReturnFrame(pErrorCodeThread);
			// 发送误码查询帧
			ProcErrorCodeQueryFrame(pErrorCodeThread);
		}
		if (pErrorCodeThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitErrorCodeThread(pErrorCodeThread);
	}
	::SetEvent(pErrorCodeThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化误码查询线程
MatrixServerDll_API bool OnInitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pErrorCodeThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pErrorCodeThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
	}
	::ResetEvent(pErrorCodeThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pErrorCodeThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunErrorCodeThread,
		pErrorCodeThread, 
		0, 
		&pErrorCodeThread->m_pThread->m_dwThreadID);
	if (pErrorCodeThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", 
			"pErrorCodeThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnInitErrorCodeThread", "误码查询线程创建成功");
	return true;
}
// 初始化误码查询线程
MatrixServerDll_API bool OnInit_ErrorCodeThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pErrorCodeThread->m_pLogOutPutErrorCode = pEnv->m_pLogOutPutErrorCode;
	pEnv->m_pErrorCodeThread->m_pErrorCodeFrame = pEnv->m_pErrorCodeFrame;
	pEnv->m_pErrorCodeThread->m_pRoutList = pEnv->m_pRoutList;
	pEnv->m_pErrorCodeThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	return OnInitErrorCodeThread(pEnv->m_pErrorCodeThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭误码查询线程
MatrixServerDll_API bool OnCloseErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pErrorCodeThread->m_pThread))
	{
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", 
			"误码查询线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pErrorCodeThread->m_pThread->m_pLogOutPut, "OnCloseErrorCodeThread", "误码查询线程成功关闭");
		return true;
	}
}
// 释放误码查询线程
MatrixServerDll_API void OnFreeErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread)
{
	if (pErrorCodeThread == NULL)
	{
		return;
	}
	if (pErrorCodeThread->m_pThread != NULL)
	{
		delete pErrorCodeThread->m_pThread;
	}
	delete pErrorCodeThread;
}
// 创建ADC数据接收线程
MatrixServerDll_API m_oADCDataRecThreadStruct* OnCreateADCDataRecThread(void)
{
	m_oADCDataRecThreadStruct* pADCDataRecThread = NULL;
	pADCDataRecThread = new m_oADCDataRecThreadStruct;
	pADCDataRecThread->m_pThread = new m_oThreadStruct;
	pADCDataRecThread->m_pLogOutPutADCFrameTime = NULL;
	pADCDataRecThread->m_pADCDataFrame = NULL;
	pADCDataRecThread->m_pInstrumentList = NULL;
	return pADCDataRecThread;
}
// 线程等待函数
MatrixServerDll_API void WaitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pADCDataRecThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(pADCDataRecThread->m_pThread->m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataRecSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 处理单个ADC数据帧
MatrixServerDll_API void ProcADCDataRecFrameOne(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	unsigned int uiIPInstrument = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	CString str = _T("");
	CString strOut = _T("");
	unsigned short usADCDataFramePointNow = 0;
	unsigned int uiADCDataFramePointMove = 0;
	unsigned int uiADCDataFrameSysTimeNow = 0;
	unsigned int uiADCDataFrameSysTimeMove = 0;
	unsigned int uiLostFrameNum = 0;
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// 得到仪器IP
	uiIPInstrument = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiSrcIP;
	// 仪器在索引表中
	if (TRUE == IfIndexExistInMap(uiIPInstrument, &pADCDataRecThread->m_pInstrumentList->m_oIPInstrumentMap))
	{
		pInstrument = GetInstrumentFromMap(uiIPInstrument, &pADCDataRecThread->m_pInstrumentList->m_oIPInstrumentMap);
		usADCDataFramePointNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_usADCDataPoint;
		uiADCDataFrameSysTimeNow = pADCDataRecThread->m_pADCDataFrame->m_pCommandStructReturn->m_uiADCSampleSysTime;
		if (pInstrument->m_uiADCDataActualRecFrameNum > 0)
		{
			if (usADCDataFramePointNow > pADCDataRecThread->m_pThread->m_pConstVar->m_iADCFramePointLimit)
			{
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", "指针偏移超限", ErrorType);
			}
			if (usADCDataFramePointNow < pInstrument->m_usADCDataFramePoint)
			{
				uiADCDataFramePointMove = pADCDataRecThread->m_pThread->m_pConstVar->m_iADCFramePointLimit 
					- pInstrument->m_usADCDataFramePoint + usADCDataFramePointNow;
			}
			else
			{
				uiADCDataFramePointMove = usADCDataFramePointNow - pInstrument->m_usADCDataFramePoint;
			}
			if (uiADCDataFrameSysTimeNow < pInstrument->m_uiADCDataFrameSysTime)
			{
				uiADCDataFrameSysTimeMove = 0xffffffff - pInstrument->m_uiADCDataFrameSysTime + uiADCDataFrameSysTimeNow;
			}
			else
			{
				uiADCDataFrameSysTimeMove = uiADCDataFrameSysTimeNow - pInstrument->m_uiADCDataFrameSysTime;
			}
			if (uiADCDataFramePointMove % pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum != 0)
			{
				str.Format(_T("指针偏移量出错，偏移量差值为%d"), uiADCDataFramePointMove);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", ConvertCStrToStr(strOut), ErrorType);
				AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", ConvertCStrToStr(strOut), ErrorType);
			}
			else
			{
				uiLostFrameNum = uiADCDataFramePointMove / pADCDataRecThread->m_pThread->m_pConstVar->m_iADCDataInOneFrameNum - 1;
				if (uiLostFrameNum > 0)
				{
					pInstrument->m_uiADCDataShouldRecFrameNum += uiLostFrameNum;
					str.Format(_T("仪器SN = 0x%x，IP = 0x%x，丢失帧数为%d"), pInstrument->m_uiSN, pInstrument->m_uiIP, uiLostFrameNum);
					AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ProcADCDataRecFrameOne", ConvertCStrToStr(str), WarningType);
					AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", ConvertCStrToStr(str), WarningType);
					// 发送ADC重发帧

				}
			}
		}
		pInstrument->m_usADCDataFramePoint = usADCDataFramePointNow;
		pInstrument->m_uiADCDataFrameSysTime = uiADCDataFrameSysTimeNow;
		pInstrument->m_uiADCDataShouldRecFrameNum++;
		pInstrument->m_uiADCDataActualRecFrameNum++; 
		str.Format(_T("仪器SN = 0x%x，IP = 0x%x；"), pInstrument->m_uiSN, pInstrument->m_uiIP);
		strOut += str;
		str.Format(_T("接收帧的指针偏移量为 %d，差值为 %d；"), pInstrument->m_usADCDataFramePoint, uiADCDataFramePointMove);
		strOut += str;
		str.Format(_T("本地时间为 %d，差值为%d"), pInstrument->m_uiADCDataFrameSysTime, uiADCDataFrameSysTimeMove);
		strOut += str;
		AddMsgToLogOutPutList(pADCDataRecThread->m_pLogOutPutADCFrameTime, "", ConvertCStrToStr(strOut));
	}
}
// 处理ADC数据接收帧
MatrixServerDll_API void ProcADCDataRecFrame(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
		pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
		pADCDataRecThread->m_pThread->m_pLogOutPut);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			if (true == GetFrameData(pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket,
				pADCDataRecThread->m_pADCDataFrame->m_pcRcvFrameData, 
				pADCDataRecThread->m_pThread->m_pConstVar->m_iRcvFrameSize, 
				pADCDataRecThread->m_pThread->m_pLogOutPut))
			{
				if (false == ParseInstrumentADCDataRecFrame(pADCDataRecThread->m_pADCDataFrame, 
					pADCDataRecThread->m_pThread->m_pConstVar))
				{
					AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "ParseInstrumentADCDataRecFrame",
						"", ErrorType, IDS_ERR_PARSE_IPSETRETURNFRAME);
				}
				else
				{
					EnterCriticalSection(&pADCDataRecThread->m_pInstrumentList->m_oSecInstrumentList);
					// 处理单个ADC数据帧
					ProcADCDataRecFrameOne(pADCDataRecThread);
					LeaveCriticalSection(&pADCDataRecThread->m_pInstrumentList->m_oSecInstrumentList);
				}	
			}		
		}		
	}
}
// 线程函数
DWORD WINAPI RunADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return 1;
	}
	while(true)
	{
		if (pADCDataRecThread->m_pThread->m_bClose == true)
		{
			break;
		}
		if (pADCDataRecThread->m_pThread->m_bWork == true)
		{
			// 处理ADC数据接收帧
			ProcADCDataRecFrame(pADCDataRecThread);
		}
		if (pADCDataRecThread->m_pThread->m_bClose == true)
		{
			break;
		}
		WaitADCDataRecThread(pADCDataRecThread);
	}
	::SetEvent(pADCDataRecThread->m_pThread->m_hThreadClose); // 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return 1;
}
// 初始化ADC数据接收线程
MatrixServerDll_API bool OnInitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread, m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL)
{
	if (pADCDataRecThread == NULL)
	{
		return false;
	}
	if (false == OnInitThread(pADCDataRecThread->m_pThread, pLogOutPut, pConstVar))
	{
		return false;
	}
	::ResetEvent(pADCDataRecThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pADCDataRecThread->m_pThread->m_hThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunADCDataRecThread,
		pADCDataRecThread, 
		0, 
		&pADCDataRecThread->m_pThread->m_dwThreadID);
	if (pADCDataRecThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnInitADCDataRecThread", 
			"pADCDataRecThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnInitADCDataRecThread", "ADC数据接收线程创建成功");
	return true;
}
// 初始化ADC数据接收线程
MatrixServerDll_API bool OnInit_ADCDataRecThread(m_oEnvironmentStruct* pEnv)
{
	pEnv->m_pADCDataRecThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	pEnv->m_pADCDataRecThread->m_pADCDataFrame = pEnv->m_pADCDataFrame;
	pEnv->m_pADCDataRecThread->m_pInstrumentList = pEnv->m_pInstrumentList;
	return OnInitADCDataRecThread(pEnv->m_pADCDataRecThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭ADC数据接收线程
MatrixServerDll_API bool OnCloseADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return false;
	}
	if (false == OnCloseThread(pADCDataRecThread->m_pThread))
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", 
			"ADC数据接收线程强制关闭", WarningType);
		return false;
	}
	else
	{
		AddMsgToLogOutPutList(pADCDataRecThread->m_pThread->m_pLogOutPut, "OnCloseADCDataRecThread", "ADC数据接收线程成功关闭");
		return true;
	}
}
// 释放ADC数据接收线程
MatrixServerDll_API void OnFreeADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread)
{
	if (pADCDataRecThread == NULL)
	{
		return;
	}
	if (pADCDataRecThread->m_pThread != NULL)
	{
		delete pADCDataRecThread->m_pThread;
	}
	delete pADCDataRecThread;
}
// 输出接收和发送帧的统计结果
MatrixServerDll_API void OnOutPutResult(m_oEnvironmentStruct* pEnv)
{
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	CString str = _T("");
	m_oInstrumentStruct* pInstrument = NULL;
	// 尾包查询帧查询的仪器计数
	int iTailTimeQueryNum = 0;
	// 尾包查询帧应答计数
	int iTailTimeReturnNum = 0;
	// 时统设置帧计数
	int iTimeDelaySetNum = 0;
	// 时统设置应答帧计数
	int iTimeDelayReturnNum = 0;

	// 网络数据错误计数
	int iErrorCodeDataNum = 0;
	// 命令错误计数
	int iErrorCodeCmdNum = 0;
	// 发送误码查询帧数
	int iErrorCodeQueryNum = 0;
	// 接收到误码查询帧数
	int iErrorCodeReturnNum = 0;

	EnterCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
	for (iter = pEnv->m_pInstrumentList->m_oIPInstrumentMap.begin();
		iter != pEnv->m_pInstrumentList->m_oIPInstrumentMap.end(); iter++)
	{
		pInstrument = iter->second;
		// 尾包时刻查询和时统
		str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，发送尾包时刻查询帧次数 %d，应答次数 %d，发送时统次数 %d，应答次数%d"),
			pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_iTailTimeQueryCount, pInstrument->m_iTailTimeReturnCount,
			pInstrument->m_iTimeSetCount, pInstrument->m_iTimeSetReturnCount);
		AddMsgToLogOutPutList(pEnv->m_pLogOutPutTimeDelay, "", ConvertCStrToStr(str));
		iTailTimeQueryNum += pInstrument->m_iTailTimeQueryCount;
		iTailTimeReturnNum += pInstrument->m_iTailTimeReturnCount;
		iTimeDelaySetNum += pInstrument->m_iTimeSetCount;
		iTimeDelayReturnNum += pInstrument->m_iTimeSetReturnCount;

		// 误码查询帧数
		str.Format(_T("仪器SN = 0x%x，仪器IP = 0x%x，发送误码查询帧数 %d，应答次数 %d"),
			pInstrument->m_uiSN, pInstrument->m_uiIP, pInstrument->m_uiErrorCodeQueryNum, 
			pInstrument->m_uiErrorCodeReturnNum);
		AddMsgToLogOutPutList(pEnv->m_pLogOutPutErrorCode, "", ConvertCStrToStr(str));
		iErrorCodeQueryNum += pInstrument->m_uiErrorCodeQueryNum;
		iErrorCodeReturnNum += pInstrument->m_uiErrorCodeReturnNum;
		// 网络数据错误计数
		iErrorCodeDataNum += pInstrument->m_iFDUErrorCodeDataCount + pInstrument->m_iLAUXErrorCodeDataLineACount
			+ pInstrument->m_iLAUXErrorCodeDataLineBCount + pInstrument->m_iLAUXErrorCodeDataLAUXLineACount
			+ pInstrument->m_iLAUXErrorCodeDataLAUXLineBCount;
		// 命令错误计数
		iErrorCodeCmdNum += pInstrument->m_iFDUErrorCodeCmdCount + pInstrument->m_iLAUXErrorCodeCmdCount;
	}
	LeaveCriticalSection(&pEnv->m_pInstrumentList->m_oSecInstrumentList);
	// 尾包时刻
	str.Format(_T("尾包时刻查询仪器的总数%d， 应答帧总数%d"), iTailTimeQueryNum, iTailTimeReturnNum);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", ConvertCStrToStr(str));
	// 时统
	str.Format(_T("时统设置仪器的总数%d， 应答帧总数%d"), iTimeDelaySetNum, iTimeDelayReturnNum);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", ConvertCStrToStr(str));
	// 误码查询
	str.Format(_T("误码查询仪器的总数%d， 应答帧总数%d"), iErrorCodeQueryNum, iErrorCodeReturnNum);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", ConvertCStrToStr(str));
	// 误码查询结果统计
	str.Format(_T("数据误码总数%d， 命令误码总数%d"), iErrorCodeDataNum, iErrorCodeCmdNum);
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnClose", ConvertCStrToStr(str));
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
	pEnv->m_pTimeDelayFrame = NULL;
	pEnv->m_pADCSetFrame = NULL;
	pEnv->m_pErrorCodeFrame = NULL;
	pEnv->m_pADCDataFrame = NULL;
	pEnv->m_pLogOutPutOpt = NULL;
	pEnv->m_pLogOutPutTimeDelay = NULL;
	pEnv->m_pLogOutPutErrorCode = NULL;
	pEnv->m_pLogOutPutADCFrameTime = NULL;
	pEnv->m_pInstrumentList = NULL;
	pEnv->m_pRoutList = NULL;
	pEnv->m_pLogOutPutThread = NULL;
	pEnv->m_pHeartBeatThread = NULL;
	pEnv->m_pHeadFrameThread = NULL;
	pEnv->m_pIPSetFrameThread = NULL;
	pEnv->m_pTailFrameThread = NULL;
	pEnv->m_pMonitorThread = NULL;
	pEnv->m_pTimeDelayThread = NULL;
	pEnv->m_pADCSetThread = NULL;
	pEnv->m_pErrorCodeThread = NULL;
	pEnv->m_pADCDataRecThread = NULL;
	// 创建操作日志输出结构体
	pEnv->m_pLogOutPutOpt = OnCreateLogOutPut();
	// 创建时统日志输出结构体
	pEnv->m_pLogOutPutTimeDelay = OnCreateLogOutPut();
	// 创建误码查询日志输出结构体
	pEnv->m_pLogOutPutErrorCode = OnCreateLogOutPut();
	// 创建ADC数据帧时间日志输出结构体
	pEnv->m_pLogOutPutADCFrameTime = OnCreateLogOutPut();
	// 创建常量信息结构体
	pEnv->m_pConstVar = OnCreateConstVar();
	// 创建仪器通讯信息结构体
	pEnv->m_pInstrumentCommInfo = OnCreateInstrumentCommInfo();
	// 创建心跳帧信息结构体
	pEnv->m_pHeartBeatFrame = OnCreateInstrumentHeartBeat();
	// 创建首包帧信息结构体
	pEnv->m_pHeadFrame = OnCreateInstrumentHeadFrame();
	// 创建IP地址设置帧信息结构体
	pEnv->m_pIPSetFrame = OnCreateInstrumentIPSetFrame();
	// 创建尾包帧信息结构体
	pEnv->m_pTailFrame = OnCreateInstrumentTailFrame();
	// 创建尾包时刻帧信息结构体
	pEnv->m_pTailTimeFrame = OnCreateInstrumentTailTimeFrame();
	// 创建时统设置帧信息结构体
	pEnv->m_pTimeDelayFrame = OnCreateInstrumentTimeDelayFrame();
	// 创建ADC参数设置帧信息结构体
	pEnv->m_pADCSetFrame = OnCreateInstrumentADCSetFrame();
	// 创建误码查询帧信息结构体
	pEnv->m_pErrorCodeFrame = OnCreateInstrumentErrorCodeFrame();
	// 创建ADC数据帧信息结构体
	pEnv->m_pADCDataFrame = OnCreateInstrumentADCDataFrame();
	// 创建仪器队列结构体
	pEnv->m_pInstrumentList = OnCreateInstrumentList();
	// 创建路由队列结构体
	pEnv->m_pRoutList = OnCreateRoutList();
	// 创建日志输出线程
	pEnv->m_pLogOutPutThread = OnCreateLogOutPutThread();
	// 创建心跳线程
	pEnv->m_pHeartBeatThread = OnCreateHeartBeatThread();
	// 创建首包接收线程
	pEnv->m_pHeadFrameThread = OnCreateHeadFrameThread();
	// 创建IP地址设置线程
	pEnv->m_pIPSetFrameThread = OnCreateIPSetFrameThread();
	// 创建尾包接收线程
	pEnv->m_pTailFrameThread = OnCreateTailFrameThread();
	// 创建时统线程
	pEnv->m_pTimeDelayThread = OnCreateTimeDelayThread();
	// 创建路由监视线程
	pEnv->m_pMonitorThread = OnCreateMonitorThread();
	// 创建ADC参数设置线程
	pEnv->m_pADCSetThread = OnCreateADCSetThread();
	// 创建误码查询线程
	pEnv->m_pErrorCodeThread = OnCreateErrorCodeThread();
	// 创建ADC数据接收线程
	pEnv->m_pADCDataRecThread = OnCreateADCDataRecThread();
	return pEnv;
}
// 初始化实例
MatrixServerDll_API void OnInitInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	CString str = _T("");
	CString strPath = _T("");
	SYSTEMTIME  sysTime;
	// 创建程序运行日志文件夹
	CreateDirectory(LogFolderPath, NULL);
	GetLocalTime(&sysTime);
	str.Format(_T("\\%04d年%02d月%02d日_Log"),sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	strPath = LogFolderPath + str;
	CreateDirectory(strPath, NULL);
	str.Format(_T("\\%02d时%02d分%02d秒_log"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strPath += str;
	CreateDirectory(strPath, NULL);

	// 初始化操作日志输出结构体
	pEnv->m_pLogOutPutOpt->m_csSaveLogFilePath = strPath + SysOptLogFolderPath;
	pEnv->m_pLogOutPutOpt->m_byLogFileType = OptLogType;
	OnInitLogOutPut(pEnv->m_pLogOutPutOpt);
	// 初始化时统日志输出结构体
	pEnv->m_pLogOutPutTimeDelay->m_csSaveLogFilePath = strPath + TimeDelayLogFolderPath;
	pEnv->m_pLogOutPutTimeDelay->m_byLogFileType = TimeDelayLogType;
	OnInitLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 初始化误码查询日志输出结构体
	pEnv->m_pLogOutPutErrorCode->m_csSaveLogFilePath = strPath + ErrorCodeLogFolderPath;
	pEnv->m_pLogOutPutErrorCode->m_byLogFileType = ErrorCodeLogType;
	OnInitLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 初始化ADC数据帧时间日志输出结构体
	pEnv->m_pLogOutPutADCFrameTime->m_csSaveLogFilePath = strPath + ADCFrameTimeLogFolderPath;
	pEnv->m_pLogOutPutADCFrameTime->m_byLogFileType = ADCFrameTimeLogType;
	OnInitLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
	// 初始化常量信息结构体
	OnInitConstVar(pEnv->m_pConstVar, pEnv->m_pLogOutPutOpt);
	// 初始化仪器通讯信息结构体
	OnInitInstrumentCommInfo(pEnv->m_pInstrumentCommInfo, pEnv->m_pLogOutPutOpt);
	// 初始化套接字库
	OnInitSocketLib(pEnv->m_pLogOutPutOpt);
	// 初始化日志输出线程
	OnInit_LogOutPutThread(pEnv);
	// 初始化心跳线程
	OnInit_HeartBeatThread(pEnv);
	// 初始化首包接收线程
	OnInit_HeadFrameThread(pEnv);
	// 初始化IP地址设置线程
	OnInit_IPSetFrameThread(pEnv);
	// 初始化尾包接收线程
	OnInit_TailFrameThread(pEnv);
	// 初始化时统设置线程
	OnInit_TimeDelayThread(pEnv);
	// 初始化路由监视线程
	OnInit_MonitorThread(pEnv);
	// 初始化ADC参数设置线程
	OnInit_ADCSetThread(pEnv);
	// 初始化误码查询线程
	OnInit_ErrorCodeThread(pEnv);
	// 初始化ADC数据接收线程
	OnInit_ADCDataRecThread(pEnv);

	// 初始化心跳
	OnInitInstrumentHeartBeat(pEnv->m_pHeartBeatFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化首包
	OnInitInstrumentHeadFrame(pEnv->m_pHeadFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化IP地址设置
	OnInitInstrumentIPSetFrame(pEnv->m_pIPSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包
	OnInitInstrumentTailFrame(pEnv->m_pTailFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化尾包时刻
	OnInitInstrumentTailTimeFrame(pEnv->m_pTailTimeFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化时统设置
	OnInitInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化ADC参数设置
	OnInitInstrumentADCSetFrame(pEnv->m_pADCSetFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化误码查询
	OnInitInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化ADC数据帧
	OnInitInstrumentADCDataFrame(pEnv->m_pADCDataFrame, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化仪器队列结构体
	OnInitInstrumentList(pEnv->m_pInstrumentList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化路由队列结构体
	OnInitRoutList(pEnv->m_pRoutList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 关闭心跳Socket
	OnCloseSocket(pEnv->m_pHeartBeatFrame->m_oHeartBeatSocket);
	// 创建并设置心跳端口
	OnCreateAndSetHeartBeatSocket(pEnv->m_pHeartBeatFrame, pEnv->m_pLogOutPutOpt);
	// 关闭首包Socket
	OnCloseSocket(pEnv->m_pHeadFrame->m_oHeadFrameSocket);
	// 创建并设置首包端口
	OnCreateAndSetHeadFrameSocket(pEnv->m_pHeadFrame, pEnv->m_pLogOutPutOpt);
	// 关闭IP地址设置Socket
	OnCloseSocket(pEnv->m_pIPSetFrame->m_oIPSetFrameSocket);
	// 创建并设置IP地址设置端口
	OnCreateAndSetIPSetFrameSocket(pEnv->m_pIPSetFrame, pEnv->m_pLogOutPutOpt);
	// 关闭尾包Socket
	OnCloseSocket(pEnv->m_pTailFrame->m_oTailFrameSocket);
	// 创建并设置尾包端口
	OnCreateAndSetTailFrameSocket(pEnv->m_pTailFrame, pEnv->m_pLogOutPutOpt);
	// 关闭尾包时刻Socket
	OnCloseSocket(pEnv->m_pTailTimeFrame->m_oTailTimeFrameSocket);
	// 创建并设置尾包时刻查询帧端口
	OnCreateAndSetTailTimeFrameSocket(pEnv->m_pTailTimeFrame, pEnv->m_pLogOutPutOpt);
	// 关闭时统设置Socket
	OnCloseSocket(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket);
	// 创建并设置时统设置帧端口
	OnCreateAndSetTimeDelayFrameSocket(pEnv->m_pTimeDelayFrame, pEnv->m_pLogOutPutOpt);
	// 关闭ADC参数设置Socket
	OnCloseSocket(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket);
	// 创建并设置ADC参数设置帧端口
	OnCreateAndSetADCSetFrameSocket(pEnv->m_pADCSetFrame, pEnv->m_pLogOutPutOpt);
	// 关闭误码查询帧Socket
	OnCloseSocket(pEnv->m_pErrorCodeFrame->m_oErrorCodeFrameSocket);
	// 创建并设置误码查询帧端口
	OnCreateAndSetErrorCodeFrameSocket(pEnv->m_pErrorCodeFrame, pEnv->m_pLogOutPutOpt);
	// 关闭ADC数据帧Socket
	OnCloseSocket(pEnv->m_pADCDataFrame->m_oADCDataFrameSocket);
	// 创建并设置ADC数据帧端口
	OnCreateAndSetADCDataFrameSocket(pEnv->m_pADCDataFrame, pEnv->m_pLogOutPutOpt);
}
// 初始化
MatrixServerDll_API void OnInit(m_oEnvironmentStruct* pEnv)
{
	OnInitInstance(pEnv);
}
// 关闭
MatrixServerDll_API void OnClose(m_oEnvironmentStruct* pEnv)
{
	CString str = _T("");
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
	pEnv->m_pMonitorThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pTimeDelayThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pADCSetThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pErrorCodeThread->m_pThread->m_bClose = true;
	// 线程关闭标志位为true
	pEnv->m_pADCDataRecThread->m_pThread->m_bClose = true;
	// 关闭心跳线程
	OnCloseHeartBeatThread(pEnv->m_pHeartBeatThread);
	// 关闭首包接收线程
	OnCloseHeadFrameThread(pEnv->m_pHeadFrameThread);
	// 关闭IP地址设置线程
	OnCloseIPSetFrameThread(pEnv->m_pIPSetFrameThread);
	// 关闭尾包接收线程
	OnCloseTailFrameThread(pEnv->m_pTailFrameThread);
	// 关闭路由监视线程
	OnCloseMonitorThread(pEnv->m_pMonitorThread);
	// 关闭时统设置线程
	OnCloseTimeDelayThread(pEnv->m_pTimeDelayThread);
	// 关闭ADC参数设置线程
	OnCloseADCSetThread(pEnv->m_pADCSetThread);
	// 关闭误码查询线程
	OnCloseErrorCodeThread(pEnv->m_pErrorCodeThread);
	// 关闭ADC数据接收线程
	OnCloseADCDataRecThread(pEnv->m_pADCDataRecThread);
	// 关闭日志输出线程
	OnCloseLogOutPutThread(pEnv->m_pLogOutPutThread);

	// 输出接收和发送帧的统计结果
	OnOutPutResult(pEnv);

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
	// 关闭时统设置Socket
	OnCloseSocket(pEnv->m_pTimeDelayFrame->m_oTimeDelayFrameSocket);
	// 关闭ADC参数设置Socket
	OnCloseSocket(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket);
	// 关闭误码查询帧Socket
	OnCloseSocket(pEnv->m_pErrorCodeFrame->m_oErrorCodeFrameSocket);
	// 关闭ADC数据帧Socket
	OnCloseSocket(pEnv->m_pADCDataFrame->m_oADCDataFrameSocket);
	// 释放套接字库
	OnCloseSocketLib(pEnv->m_pLogOutPutOpt);
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
	// 释放时统设置资源
	OnCloseInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame);
	// 释放ADC参数设置资源
	OnCloseInstrumentADCSetFrame(pEnv->m_pADCSetFrame);
	// 释放误码查询资源
	OnCloseInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame);
	// 释放ADC数据帧资源
	OnCloseInstrumentADCDataFrame(pEnv->m_pADCDataFrame);
	// 释放仪器队列资源
	OnCloseInstrumentList(pEnv->m_pInstrumentList);
	// 释放路由队列资源
	OnCloseRoutList(pEnv->m_pRoutList);

	// 关闭操作日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutOpt);
	// 关闭时统日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 关闭误码查询日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 关闭ADC数据帧时间日志文件
	OnCloseLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
}
// 工作
MatrixServerDll_API void OnWork(m_oEnvironmentStruct* pEnv)
{
	// 初始化仪器队列结构体
	OnInitInstrumentList(pEnv->m_pInstrumentList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 初始化路由队列结构体
	OnInitRoutList(pEnv->m_pRoutList, pEnv->m_pInstrumentCommInfo, pEnv->m_pConstVar);
	// 日志输出线程开始工作
	pEnv->m_pLogOutPutThread->m_pThread->m_bWork = true;
	// 心跳线程开始工作
	pEnv->m_pHeartBeatThread->m_pThread->m_bWork = true;
	// 清空首包帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pHeadFrameThread->m_pHeadFrame->m_oHeadFrameSocket, pEnv->m_pConstVar);
	// 首包接收线程开始工作
	pEnv->m_pHeadFrameThread->m_pThread->m_bWork = true;
	// 清空IP地址设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pIPSetFrameThread->m_pIPSetFrame->m_oIPSetFrameSocket, pEnv->m_pConstVar);
	// IP地址设置线程开始工作
	pEnv->m_pIPSetFrameThread->m_pThread->m_bWork = true;
	// 清空尾包帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pTailFrameThread->m_pTailFrame->m_oTailFrameSocket, pEnv->m_pConstVar);
	// 尾包接收线程开始工作
	pEnv->m_pTailFrameThread->m_pThread->m_bWork = true;
	// 路由监视线程开始工作
	pEnv->m_pMonitorThread->m_pThread->m_bWork = true;
 	// 清空误码查询接收缓冲区
 	OnClearSocketRcvBuf(pEnv->m_pErrorCodeThread->m_pErrorCodeFrame->m_oErrorCodeFrameSocket, pEnv->m_pConstVar);
	// 清空ADC参数设置应答帧接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pADCSetFrame->m_oADCSetFrameSocket, pEnv->m_pConstVar);
	// 清空ADC数据接收缓冲区
	OnClearSocketRcvBuf(pEnv->m_pADCDataRecThread->m_pADCDataFrame->m_oADCDataFrameSocket, pEnv->m_pConstVar);
	// ADC数据接收线程开始工作
	pEnv->m_pADCDataRecThread->m_pThread->m_bWork = true;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnWork", "开始工作");
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
	pEnv->m_pMonitorThread->m_pThread->m_bWork = false;
	// 时统设置线程停止工作
	pEnv->m_pTimeDelayThread->m_pThread->m_bWork = false;
	// ADC参数设置线程停止工作
	pEnv->m_pADCSetThread->m_pThread->m_bWork = false;
	// 误码查询线程停止工作
	pEnv->m_pErrorCodeThread->m_pThread->m_bWork = false;
	// ADC数据接收线程停止工作
	pEnv->m_pADCDataRecThread->m_pThread->m_bWork = false;
	AddMsgToLogOutPutList(pEnv->m_pLogOutPutOpt, "OnStop", "停止工作");
}
// 释放实例资源
MatrixServerDll_API void OnFreeInstance(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return;
	}
	// 释放操作日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutOpt);
	// 释放时统日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutTimeDelay);
	// 释放误码查询日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutErrorCode);
	// 释放ADC数据帧时间日志输出结构体资源
	OnFreeLogOutPut(pEnv->m_pLogOutPutADCFrameTime);
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
	// 释放时统设置帧结构体资源
	OnFreeInstrumentTimeDelayFrame(pEnv->m_pTimeDelayFrame);
	// 释放ADC参数设置帧结构体资源
	OnFreeInstrumentADCSetFrame(pEnv->m_pADCSetFrame);
	// 释放误码帧结构体资源
	OnFreeInstrumentErrorCodeFrame(pEnv->m_pErrorCodeFrame);
	// 释放ADC数据帧结构体资源
	OnFreeInstrumentADCDataFrame(pEnv->m_pADCDataFrame);
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
	OnFreeMonitorThread(pEnv->m_pMonitorThread);
	// 释放时统设置线程
	OnFreeTimeDelayThread(pEnv->m_pTimeDelayThread);
	// 释放ADC参数设置线程
	OnFreeADCSetThread(pEnv->m_pADCSetThread);
	// 释放误码查询线程
	OnFreeErrorCodeThread(pEnv->m_pErrorCodeThread);
	// 释放ADC数据接收线程
	OnFreeADCDataRecThread(pEnv->m_pADCDataRecThread);
	delete pEnv;
	pEnv = NULL;
}