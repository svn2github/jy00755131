#include "stdafx.h"
#include "MatrixServerDll.h"

// 得到帧的帧内容
void GetFrameInfo(char* pFrameData, int iSize, string* strFrameData)
{
	CString cstr = _T("");
	string strConv = "";
	*strFrameData = "";
	ASSERT(pFrameData != NULL);
	for (int i=0; i<iSize; i++)
	{
		cstr.Format(_T("%02x"), (unsigned char)pFrameData[i]);
		strConv = (CStringA)cstr;
		*strFrameData += strConv;
		*strFrameData += ' ';
	}
}
// 从CString转换为char数组
void ParseCStringToArray(char** pData, int iSize, CString str)
{
	if ((*pData) != NULL)
	{
		delete[] (*pData);
		(*pData) = NULL;
	}
	*pData = new char[iSize];
	int iDirectionOld = 0;
	int iDirectionNew = 0;
	CString strTemp = _T("");
	int iTemp = 0;
	for (int i=0; i<iSize; i++)
	{
		iDirectionNew = str.Find(_T("0x"), iDirectionOld);
		iDirectionNew += 2;
		strTemp = str.Mid(iDirectionNew, 2);
		_stscanf_s(strTemp, _T("%x"), &iTemp, sizeof(int));
		(*pData)[i] = iTemp;
		iDirectionOld = iDirectionNew;
	}
}
// 判断文件是否存在
bool IfFileExist(CString str)
{
	if (_taccess(str, 0) == -1)
	{
		return false;
	}
	return true;
}
// 校验帧的同步码
bool CheckInstrFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize)
{
	ASSERT(pFrameData != NULL);
	ASSERT(pFrameHeadCheck != NULL);
	for (int i=0; i<iCheckSize; i++)
	{
		if (pFrameData[i] != pFrameHeadCheck[i])
		{
			return false;
		}
	}
	return true;
}
// 生成帧的同步码
bool MakeInstrFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize)
{
	ASSERT(pFrameData != NULL);
	ASSERT(pFrameHeadCheck != NULL);
	memcpy(pFrameData, pFrameHeadCheck, iCheckSize);
	return true;
}
// 重置帧内容解析变量
bool ResetInstrFramePacket(m_oInstrumentCommandStruct* pCommand)
{
	ASSERT(pCommand != NULL);
	// SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站
	pCommand->m_uiSN = 0;
	// 首包时刻
	pCommand->m_uiTimeHeadFrame = 0;
	// 仪器IP地址
	pCommand->m_uiInstrumentIP = 0;
	// 本地时间
	pCommand->m_usSysTimeNewLow = 0;
	// 本地时间
	pCommand->m_uiSysTimeNewHigh = 0;
	// 本地时间
	pCommand->m_usSysTimeOldLow = 0;
	// 本地时间
	pCommand->m_uiSysTimeOldHigh = 0;
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
	pCommand->m_cFDUErrorCodeDataCount = 0;
	// 命令错误计数
	pCommand->m_cFDUErrorCodeCmdCount = 0;
	// 时间报文状态
	pCommand->m_cTimeStatus = 0;
	// 控制状态
	pCommand->m_cCtrlStatus = 0;
	// TB时刻高位
	pCommand->m_uiTBHigh = 0;
	// 硬件设备软件版本号
	pCommand->m_uiVersion = 0;
	// TB时刻低位
	pCommand->m_usTBLow = 0;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	pCommand->m_usTBCtrl = 0;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	pCommand->m_cLAUXRoutOpenQuery = 0;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	pCommand->m_cLAUXRoutOpenSet = 0;
//	// 尾包接收时刻，低14位有效
//	pCommand->m_usTailRecTime = 0;
//	// 尾包发送时刻//交叉站尾包发送时刻，低14位有效
//	pCommand->m_usTailSndTime = 0;
// 	/** 尾包接收时刻低位*/
// 	pCommand->m_usTailRecTimeLow = 0;
	/** 尾包接收时刻高位*/
	pCommand->m_uiTailRecTime = 0;
// 	/** 尾包发送时刻/交叉站尾包发送时刻低位*/
// 	pCommand->m_usTailSndTimeLow = 0;
	/** 尾包发送时刻/交叉站尾包发送时刻高位*/
	pCommand->m_uiTailSndTime = 0;
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
	pCommand->m_cLAUXErrorCodeDataLineACount = 0;
	// 交叉站大线B数据故障
	pCommand->m_cLAUXErrorCodeDataLineBCount = 0;
	// 交叉站交叉线A数据故障
	pCommand->m_cLAUXErrorCodeDataLAUXLineACount = 0;
	// 交叉站交叉线B数据故障
	pCommand->m_cLAUXErrorCodeDataLAUXLineBCount = 0;
	// 交叉站命令口故障
	pCommand->m_cLAUXErrorCodeCmdCount = 0;
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
	// ADC数据采集时仪器本地时间高位
	pCommand->m_uiADCSampleSysTimeHigh = 0;
	// ADC数据采集时仪器本地时间低位
	pCommand->m_usADCSampleSysTimeLow = 0;
	return true;
}
// 解析与设备通讯接收帧内容
bool ParseInstrFrame(m_oInstrumentCommandStruct* pCommand, 
	char* pFrameData, m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut)
{
	string strFrameData = "";
	ASSERT(pLogOutPut != NULL);
	ASSERT(pCommand != NULL);
	ASSERT(pFrameData != NULL);
	ASSERT(pConstVar != NULL);
	unsigned char	byCommandWord = 0;
	unsigned int uiRoutAddrNum = 0;
	int iPos = 0;
	int iFramePacketSize4B = 0;
	int iFramePacketSize2B = 0;
	int iFramePacketSize1B = 0;
	int iFrameCmdSize1B = 0;
	int iADCDataSize3B = 0;
	int iBufLen = 0;
	iFramePacketSize4B = pConstVar->m_iFramePacketSize4B;
	iFramePacketSize2B = pConstVar->m_iFramePacketSize2B;
	iFramePacketSize1B = pConstVar->m_iFramePacketSize1B;
	iFrameCmdSize1B = pConstVar->m_iFrameCmdSize1B;
	iADCDataSize3B = pConstVar->m_iADCDataSize3B;

	// 校验帧的同步码
	if (false == CheckInstrFrameHead(pFrameData, pConstVar->m_cpFrameHeadCheck, 4))
	{
		GetFrameInfo(pFrameData, pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pLogOutPut, "ParseInstrumentFrame", 
			strFrameData, ErrorType, IDS_ERR_CHECKFRAMEHEAD);
		return false;
	}
	// 重置帧内容解析变量
	ResetInstrFramePacket(pCommand);
	iPos += pConstVar->m_iFrameHeadSize;
	// 源IP地址
	memcpy(&pCommand->m_uiSrcIP, &pFrameData[iPos], iFramePacketSize4B);
	iPos += iFramePacketSize4B;
	// 目标IP地址
	memcpy(&pCommand->m_uiDstIP, &pFrameData[iPos], iFramePacketSize4B);
	iPos += iFramePacketSize4B;
	// 目标端口号
	memcpy(&pCommand->m_usReturnPort, &pFrameData[iPos], iFramePacketSize2B);
	iPos += iFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pCommand->m_usCommand, &pFrameData[iPos], iFramePacketSize2B);
	iPos += iFramePacketSize2B;
	// 如果帧命令号不属于设置、查询和AD数据则返回
	if (!((pCommand->m_usCommand == pConstVar->m_usSendSetCmd)
		|| (pCommand->m_usCommand == pConstVar->m_usSendQueryCmd)
		|| (pCommand->m_usCommand == pConstVar->m_usSendADCCmd)))
	{
		GetFrameInfo(pFrameData, pConstVar->m_iRcvFrameSize, &strFrameData);
		AddMsgToLogOutPutList(pLogOutPut, "ParseInstrumentFrame", 
			strFrameData, ErrorType, IDS_ERR_CHECKFRAMECMD);
		return false;
	}
	// 如果为ADC采样数据帧
	if (pCommand->m_usCommand == pConstVar->m_usSendADCCmd)
	{
		// ADC数据指针偏移量
		memcpy(&pCommand->m_usADCDataPoint, &pFrameData[iPos], iFramePacketSize2B);
		iPos += iFramePacketSize2B;
		// ADC数据采集时仪器本地时间
		memcpy(&pCommand->m_usADCSampleSysTimeLow, &pFrameData[iPos], iFramePacketSize2B);
		iPos += iFramePacketSize2B;
		memcpy(&pCommand->m_uiADCSampleSysTimeHigh, &pFrameData[iPos], iFramePacketSize4B);
		iPos += iFramePacketSize4B;
		pCommand->m_uiADCSampleSysTimeHigh = (pCommand->m_uiADCSampleSysTimeHigh << 2) 
			+ (pCommand->m_usADCSampleSysTimeLow >> 14);
		pCommand->m_usADCSampleSysTimeLow &= 0x3fff;
		iBufLen = pConstVar->m_iADCDataInOneFrameNum * iADCDataSize3B;
		memcpy(pCommand->m_pADCDataBuf, &pFrameData[iPos], iBufLen);
		iPos += iBufLen;
		return true;
	}
	// 解析帧内容
	while(true)
	{
		memcpy(&byCommandWord, &pFrameData[iPos], iFrameCmdSize1B);
		iPos += iFrameCmdSize1B;
		if (byCommandWord == pConstVar->m_byCmdSn)
		{
			memcpy(&pCommand->m_uiSN, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdHeadFrameTime)
		{
			memcpy(&pCommand->m_uiTimeHeadFrame, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalIPAddr)
		{
			memcpy(&pCommand->m_uiInstrumentIP, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalSysTime1)
		{
			// @@@时间48位
// 			memcpy(&pCommand->m_uiSysTimeNewHigh, &pFrameData[iPos], iFramePacketSize4B);
// 			iPos += iFramePacketSize4B;
			memcpy(&pCommand->m_usSysTimeNewLow, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_uiSysTimeNewHigh, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalSysTime2)
		{
			unsigned short usTemp = 0;
			unsigned int uiTemp = 0;
			memcpy(&usTemp, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			uiTemp = usTemp;
			uiTemp = uiTemp << 16;
			pCommand->m_uiSysTimeNewHigh += uiTemp;
			pCommand->m_uiSysTimeNewHigh <<= 2;
			pCommand->m_uiSysTimeNewHigh += (pCommand->m_usSysTimeNewLow >> 14);
			pCommand->m_usSysTimeNewLow &= 0x3fff;
			memcpy(&pCommand->m_usSysTimeOldLow, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalSysTime3)
		{
			memcpy(&pCommand->m_uiSysTimeOldHigh, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
			pCommand->m_uiSysTimeOldHigh <<= 2;
			pCommand->m_uiSysTimeOldHigh += (pCommand->m_usSysTimeOldLow >> 14);
			pCommand->m_usSysTimeOldLow &= 0x3fff;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalTimeFixedHigh)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedHigh, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLocalTimeFixedLow)
		{
			memcpy(&pCommand->m_uiLocalTimeFixedLow, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnAddr)
		{
			memcpy(&pCommand->m_uiADCDataReturnAddr, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnPort)
		{
			memcpy(&pCommand->m_usADCDataReturnPort, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnCmd, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCDataReturnPortLimit)
		{
			memcpy(&pCommand->m_usADCDataReturnPortLimitLow, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_usADCDataReturnPortLimitHigh, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pCommand->m_uiBroadCastPortSet, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdFDUErrorCode)
		{
			memcpy(&pCommand->m_cFDUErrorCodeDataCount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cFDUErrorCodeCmdCount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cTimeStatus, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cCtrlStatus, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTBHigh)
		{
			memcpy(&pCommand->m_uiTBHigh, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTbLow)
		{
			memcpy(&pCommand->m_usTBLow, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_usTBCtrl, &pFrameData[iPos], iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXRoutOpenQuery)
		{
			memcpy(&pCommand->m_cLAUXRoutOpenQuery, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXRoutOpenSet)
		{
			memcpy(&pCommand->m_cLAUXRoutOpenSet, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdVersion)
		{
			memcpy(&pCommand->m_uiVersion, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTailRecSndTime1)
		{
// 			memcpy(&pCommand->m_usTailRecTime, &pFrameData[iPos], iFramePacketSize2B);
// 			pCommand->m_usTailRecTime &= 0x3fff;
// 			iPos += iFramePacketSize2B;
// 			memcpy(&pCommand->m_usTailSndTime, &pFrameData[iPos], iFramePacketSize2B);
// 			pCommand->m_usTailSndTime &= 0x3fff;
// 			iPos += iFramePacketSize2B;
// 			memcpy(&pCommand->m_usTailRecTimeLow, &pFrameData[iPos], iFramePacketSize2B);
// 			iPos += iFramePacketSize2B;
// 			memcpy(&pCommand->m_uiTailRecTime, &pFrameData[iPos], iFramePacketSize2B);
// 			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_uiTailRecTime, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdTailRecSndTime2)
		{
// 			unsigned short usTemp = 0;
// 			unsigned int uiTemp = 0;
// 			memcpy(&usTemp, &pFrameData[iPos], iFramePacketSize2B);
// 			iPos += iFramePacketSize2B;
// 			uiTemp = usTemp;
// 			uiTemp = uiTemp << 16;
// 			pCommand->m_uiTailRecTime += uiTemp;
// 			pCommand->m_uiTailRecTime <<= 2;
// 			pCommand->m_uiTailRecTime += (pCommand->m_usTailRecTimeLow >> 14);
// 			pCommand->m_usTailRecTimeLow &= 0x3fff;
// 			memcpy(&pCommand->m_usTailSndTimeLow, &pFrameData[iPos], iFramePacketSize2B);
// 			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_uiTailSndTime, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
// 		else if (byCommandWord == pConstVar->m_byCmdTailRecSndTime3)
// 		{
// 			memcpy(&pCommand->m_uiTailSndTime, &pFrameData[iPos], iFramePacketSize4B);
// 			iPos += iFramePacketSize4B;
// 			pCommand->m_uiTailSndTime <<= 2;
// 			pCommand->m_uiTailSndTime += (pCommand->m_usTailSndTimeLow >> 14);
// 			pCommand->m_usTailSndTimeLow &= 0x3fff;
// 		}
		else if (byCommandWord == pConstVar->m_byCmdBroadCastPortSeted)
		{
			memcpy(&pCommand->m_uiBroadCastPortSeted, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdADCSet)
		{
			memcpy(&pCommand->m_cpADCSet[pCommand->m_iADCSetNum], &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
			pCommand->m_iADCSetNum += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdNetTime)
		{
			memcpy(&pCommand->m_uiNetTime, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLineTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLineA, &pFrameData[iPos], iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLineA &= 0x3fff;
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLineB, &pFrameData[iPos], iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLineB &= 0x3fff;
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineA, &pFrameData[iPos], iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLAUXLineA &= 0x3fff;
			iPos += iFramePacketSize2B;
			memcpy(&pCommand->m_usLAUXTailRecTimeLAUXLineB, &pFrameData[iPos], iFramePacketSize2B);
			pCommand->m_usLAUXTailRecTimeLAUXLineB &= 0x3fff;
			iPos += iFramePacketSize2B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXErrorCode1)
		{
			memcpy(&pCommand->m_cLAUXErrorCodeDataLineACount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cLAUXErrorCodeDataLineBCount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cLAUXErrorCodeDataLAUXLineACount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pCommand->m_cLAUXErrorCodeDataLAUXLineBCount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXErrorCode2)
		{
			memcpy(&pCommand->m_cLAUXErrorCodeCmdCount, &pFrameData[iPos], iFramePacketSize1B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdLAUXSetRout)
		{
			if (uiRoutAddrNum == 0)
			{
				memcpy(&pCommand->m_uiRoutIPRight, &pFrameData[iPos], iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 1)
			{
				memcpy(&pCommand->m_uiRoutIPLeft, &pFrameData[iPos], iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 2)
			{
				memcpy(&pCommand->m_uiRoutIPTop, &pFrameData[iPos], iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 3)
			{
				memcpy(&pCommand->m_uiRoutIPDown, &pFrameData[iPos], iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			uiRoutAddrNum++;
		}
		else if (byCommandWord == pConstVar->m_byCmdReturnRout)
		{
			memcpy(&pCommand->m_uiRoutIP, &pFrameData[iPos], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (byCommandWord == pConstVar->m_byCmdEnd)
		{
			break;
		}
		else
		{
			iPos += iFramePacketSize4B;
		}
	}
	return true;
}
// 生成与设备通讯帧
bool MakeInstrFrame(m_oInstrumentCommandStruct* pCommand, m_oConstVarStruct* pConstVar,
	char* pFrameData, BYTE* pCommandWord, unsigned short usCommandWordNum)
{
	ASSERT(pCommand != NULL);
	ASSERT(pFrameData != NULL);
	ASSERT(pConstVar != NULL);
	int iFramePacketSize4B = 0;
	int iFramePacketSize2B = 0;
	int iFramePacketSize1B = 0;
	int iFrameCmdSize1B = 0;
	int iPos = 0;
	unsigned int uiRoutAddrNum = 0;

	iFramePacketSize4B = pConstVar->m_iFramePacketSize4B;
	iFramePacketSize2B = pConstVar->m_iFramePacketSize2B;
	iFramePacketSize1B = pConstVar->m_iFramePacketSize1B;
	iFrameCmdSize1B = pConstVar->m_iFrameCmdSize1B;
	// 生成帧的同步码
	MakeInstrFrameHead(pFrameData, pConstVar->m_cpFrameHeadCheck, pConstVar->m_iFrameHeadSize);
	iPos += pConstVar->m_iFrameHeadSize;
	// 源IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiSrcIP, iFramePacketSize4B);
	iPos += iFramePacketSize4B;
	// 目标IP地址
	memcpy(&pFrameData[iPos], &pCommand->m_uiDstIP, iFramePacketSize4B);
	iPos += iFramePacketSize4B;
	// 目标端口号
	memcpy(&pFrameData[iPos], &pCommand->m_usReturnPort, iFramePacketSize2B);
	iPos += iFramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&pFrameData[iPos], &pCommand->m_usCommand, iFramePacketSize2B);
	iPos += iFramePacketSize2B;
	// 包时间
	iPos += iFramePacketSize4B;
	// 如果为ADC数据重发帧
	if (pCommand->m_usCommand == pConstVar->m_usSendADCCmd)
	{
		// 指针偏移量
		memcpy(&pFrameData[iPos], &pCommand->m_usADCDataPoint, iFramePacketSize2B);
		iPos += iFramePacketSize2B;
		// 截止
		pFrameData[iPos] = pConstVar->m_byCmdEnd;
		return true;
	}
	// 生成帧内容
	for (unsigned short i=0; i<usCommandWordNum; i++)
	{
		memcpy(&pFrameData[iPos], &pCommandWord[i], iFrameCmdSize1B);
		iPos += iFrameCmdSize1B;
		if (pCommandWord[i] == pConstVar->m_byCmdSn)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiSN, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdHeadFrameTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTimeHeadFrame, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalIPAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiInstrumentIP, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalSysTime1)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usSysTimeNewLow, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_uiSysTimeNewHigh, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalTimeFixedHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedHigh, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLocalTimeFixedLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiLocalTimeFixedLow, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnAddr)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiADCDataReturnAddr, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnPort)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPort, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnCmd, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCDataReturnPortLimit)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitLow, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usADCDataReturnPortLimitHigh, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdBroadCastPortSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiBroadCastPortSet, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdFDUErrorCode)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cFDUErrorCodeDataCount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cFDUErrorCodeCmdCount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cTimeStatus, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cCtrlStatus, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTBHigh)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTBHigh, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTbLow)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usTBLow, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usTBCtrl, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXRoutOpenQuery)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXRoutOpenQuery, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXRoutOpenSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXRoutOpenSet, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pConstVar->m_cSndFrameBufInit, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTailRecSndTime1)
		{
// 			memcpy(&pFrameData[iPos], &pCommand->m_usTailRecTimeLow, iFramePacketSize2B);
// 			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_uiTailRecTime, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdTailRecSndTime2)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiTailSndTime, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdBroadCastPortSeted)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiBroadCastPortSeted, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdADCSet)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cpADCSet[0], iFramePacketSize4B);
			iPos += iFramePacketSize4B;
			for (int j = iFramePacketSize4B; j < pCommand->m_iADCSetNum; j += iFramePacketSize4B)
			{
				memcpy(&pFrameData[iPos], &pConstVar->m_byCmdADCSet, iFrameCmdSize1B);
				iPos += iFrameCmdSize1B;
				memcpy(&pFrameData[iPos], &pCommand->m_cpADCSet[j], iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdNetTime)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiNetTime, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLineTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineA, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLineB, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUTailRecTimeLAUX)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineA, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
			memcpy(&pFrameData[iPos], &pCommand->m_usLAUXTailRecTimeLAUXLineB, iFramePacketSize2B);
			iPos += iFramePacketSize2B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXErrorCode1)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXErrorCodeDataLineACount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXErrorCodeDataLineBCount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXErrorCodeDataLAUXLineACount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXErrorCodeDataLAUXLineBCount, iFramePacketSize1B);
			iPos += iFramePacketSize1B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXErrorCode2)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_cLAUXErrorCodeCmdCount, iFramePacketSize1B);
			iPos += iFramePacketSize4B;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdLAUXSetRout)
		{
			if (uiRoutAddrNum == 0)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPLeft, iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 1)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPRight, iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 2)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPTop, iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			else if (uiRoutAddrNum == 3)
			{
				memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIPDown, iFramePacketSize4B);
				iPos += iFramePacketSize4B;
			}
			uiRoutAddrNum++;
		}
		else if (pCommandWord[i] == pConstVar->m_byCmdReturnRout)
		{
			memcpy(&pFrameData[iPos], &pCommand->m_uiRoutIP, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
		else
		{
			memset(&pFrameData[iPos], pConstVar->m_cSndFrameBufInit, iFramePacketSize4B);
			iPos += iFramePacketSize4B;
		}
	}
	pFrameData[iPos] = pConstVar->m_byCmdEnd;
	return true;
}
// 创建CSocket接收端口并绑定端口和IP地址
SOCKET CreateInstrSocket(unsigned short usPort, unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut)
{
	SOCKET oSocket = INVALID_SOCKET;
	CString str = _T("");
	string strConv = "";
	sockaddr_in oAddr;
	// 填充套接字地址结构
	oSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	oAddr.sin_family = AF_INET;
	oAddr.sin_port = htons(usPort);
	oAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	// 绑定本地地址
	if (SOCKET_ERROR == bind(oSocket, reinterpret_cast<sockaddr*>(&oAddr), 
		sizeof(oAddr)))
	{
		AddMsgToLogOutPutList(pLogOutPut, "CreateInstrumentSocket", "bind", 
			ErrorType, WSAGetLastError());
		str.Format(_T("bind端口 = 0x%x, IP = 0x%x"), usPort, uiIP);
		strConv = (CStringA)str;
		AddMsgToLogOutPutList(pLogOutPut, "CreateInstrumentSocket", strConv);
	}
	else
	{
		listen(oSocket, 2);
	}
	return oSocket;
}
// 设置广播模式
void SetInstrSocketBroadCast(SOCKET oSocket, m_oLogOutPutStruct* pLogOutPut)
{
	//设置广播模式
	int	iOptval = 0;
	int iOptlen = 0;
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
void SetRcvBufferSize(SOCKET oSocket, int iRcvBufferSize, m_oLogOutPutStruct* pLogOutPut)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_RCVBUF,  
		reinterpret_cast<const char *>(&iRcvBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, "SetRcvBufferSize", "setsockopt", 
			ErrorType, WSAGetLastError());
	}
}
// 设置端口发送缓冲区发送帧数量
void SetSndBufferSize(SOCKET oSocket, int iSndBufferSize, m_oLogOutPutStruct* pLogOutPut)
{
	if (SOCKET_ERROR == setsockopt(oSocket, SOL_SOCKET, SO_SNDBUF,  
		reinterpret_cast<const char *>(&iSndBufferSize), sizeof(int)))
	{
		AddMsgToLogOutPutList(pLogOutPut, "SetSndBufferSize", "setsockopt", 
			ErrorType, WSAGetLastError());
	}
}
// 得到网络接收缓冲区收到的帧数量
DWORD GetFrameCount(SOCKET oSocket, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut)
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
bool GetFrameData(SOCKET oSocket, char* pFrameData, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut)
{
	bool bReturn = false;
	sockaddr_in AddrFrom;
	int iSize = sizeof(AddrFrom);
	int iCount = 0;
	string strFrameData = "";
	iCount = recvfrom(oSocket, pFrameData, iRcvFrameSize, 0, 
		reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
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
		GetFrameInfo(pFrameData, iCount, &strFrameData);
		AddMsgToLogOutPutList(pLogOutPut, "GetFrameData", strFrameData, 
			ErrorType, IDS_ERR_RCVFRAMESIZE);
	}
	return bReturn;
}
// 清空接收帧缓冲区
void ClearRcvFrameBuf(SOCKET oSocket, char* pFrameData, int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut)
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
				iCount = recvfrom(oSocket, pFrameData, iRcvFrameSize, 0, 
					reinterpret_cast<sockaddr*>(&AddrFrom), &iSize);
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
bool SendFrame(SOCKET oSocket, char* pFrameData,int iSndFrameSize, 
	unsigned short usPort, unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut)
{
	bool bReturn = false;
	string strFrameData = "";
	// 填充套接字地址结构
	sockaddr_in addrSend;
	addrSend.sin_family = AF_INET;
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
		AddMsgToLogOutPutList(pLogOutPut, "SendFrame", "sendto", 
			ErrorType, WSAGetLastError());
	}
	else
	{
		GetFrameInfo(pFrameData, iCount, &strFrameData);
		AddMsgToLogOutPutList(pLogOutPut, "SendFrame", strFrameData, 
			ErrorType, IDS_ERR_SNDFRAMESIZE);
	}
	return bReturn;
}
// 关闭Socket套接字
void OnCloseSocket(SOCKET oSocket)
{
	if (oSocket == INVALID_SOCKET)
	{
		return;
	}
	shutdown(oSocket, SD_BOTH);
	closesocket(oSocket);
	oSocket = INVALID_SOCKET;
}
// 清空接收缓冲区
void OnClearSocketRcvBuf(SOCKET oSocket, int iRcvFrameSize)
{
	// 帧数量设置为0
	int iFrameCount = 0;
	// 接收缓冲区
	char* pcRcvFrameData = NULL;
	pcRcvFrameData = new char[iRcvFrameSize];
	// 得到首包接收网络端口帧数量
	iFrameCount = GetFrameCount(oSocket,iRcvFrameSize);
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			GetFrameData(oSocket, pcRcvFrameData, iRcvFrameSize);		
		}		
	}
	delete[] pcRcvFrameData;
	pcRcvFrameData = NULL;
}
// 得到路由方向上仪器个数
bool OnGetRoutInstrNum(int iLineIndex, int iPointIndex, int iDirection, 
	m_oEnvironmentStruct* pEnv, unsigned int& uiInstrumentNum)
{
	m_oRoutStruct* pRout = NULL;
	unsigned int uiRoutIP = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	EnterCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	if (false == IfLocationExistInMap(iLineIndex, iPointIndex, &pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return false;
	}
	pInstrument = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, &pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
	if (false == GetRoutIPBySn(pInstrument->m_uiSN, iDirection, pEnv->m_pLineList->m_pInstrumentList, uiRoutIP))
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return false;
	}
	
	if (false == GetRoutByRoutIP(uiRoutIP, pEnv->m_pLineList->m_pRoutList, &pRout))
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return false;
	}
	uiInstrumentNum = 0;
	pInstrument = pRout->m_pHead;
	if ((pInstrument->m_bIPSetOK) && (iDirection == DirectionCenter))
	{
		uiInstrumentNum = 1;
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return true;
	}
	if (pRout->m_pTail == NULL)
	{
		LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
		return true;
	}
	while(pInstrument != pRout->m_pTail)
	{
		pInstrument = GetNextInstrAlongRout(pInstrument, pRout->m_iRoutDirection);
		if (pInstrument == NULL)
		{
			break;
		}
		if (pInstrument->m_iTimeSetReturnCount > 0)
		{
			uiInstrumentNum++;
		}
	}
	LeaveCriticalSection(&pEnv->m_pLineList->m_oSecLineList);
	return true;
}

// 计算测试数据的算术均方根
double CalMeanSquare(m_oInstrumentStruct* pInstrument)
{
	double dbData = 0.0;
	double dbAvg = 0.0;
	int iSize = 0;
	list<int>::iterator iter;
	iSize = pInstrument->m_olsADCDataSave.size();
	if (iSize == 0)
	{
		return 0;
	}
	for (iter = pInstrument->m_olsADCDataSave.begin();
		iter != pInstrument->m_olsADCDataSave.end(); iter++)
	{
		dbAvg += *iter;
	}
	dbAvg /= iSize;
	for (iter = pInstrument->m_olsADCDataSave.begin();
		iter != pInstrument->m_olsADCDataSave.end(); iter++)
	{
		dbData += pow((*iter - dbAvg), 2);
	}
	dbData /= iSize;
	dbData = pow(dbData, 0.5);
	return dbData;
}