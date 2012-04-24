#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建常量信息结构体
m_oConstVarStruct* OnCreateConstVar(void)
{
	m_oConstVarStruct* pConstVar = NULL;
	pConstVar = new m_oConstVarStruct;
	pConstVar->m_cpFrameHeadCheck = NULL;
	pConstVar->m_pLogOutPut = NULL;
	pConstVar->m_strINIFilePath = "";
	return pConstVar;
}
// 载入INI文件
void LoadIniFile(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	CString strSection	= _T("");
	CString strSectionKey = _T("");
	CString strValue = _T("");
	CString strFilePath	= _T("");
	wchar_t strBuff[INIFileStrBufSize];
	strFilePath = pConstVar->m_strINIFilePath.c_str();
	if (false == IfFileExist(strFilePath))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "LoadIniFile", "", 
			ErrorType, IDS_ERR_FILE_NOEXIST);
		return;
	}
	try
	{
		//读取ini文件中相应字段的内容
		strSection = _T("常量设置");					// 获取当前区域

		strSectionKey=_T("InstrumentCountAll");			// 仪器设备个数
		pConstVar->m_iInstrumentNum = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataCountAll");			// ADC数据缓冲区个数
		pConstVar->m_iADCDataCountAll = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("OptTaskCountAll");			// 施工任务个数
		pConstVar->m_iOptTaskCountAll = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("OneSleepTime");				// 一次休眠的时间
		pConstVar->m_iOneSleepTime = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("LogOutPutSleepTimes");		// 日志输出线程写日志的延时次数
		pConstVar->m_iLogOutPutSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("HertBeatSleepTimes");			// 心跳线程发送心跳帧延时次数
		pConstVar->m_iHeartBeatSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("HeadFrameSleepTimes");		// 首包线程接收首包延时次数
		pConstVar->m_iHeadFrameSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("IPSetFrameSleepTimes");		// IP地址设置线程延时次数
		pConstVar->m_iIPSetFrameSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("TailFrameSleepTimes");		// 尾包线程延时次数
		pConstVar->m_iTailFrameSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("MonitorSleepTimes");		// 路由监视线程延时次数
		pConstVar->m_iMonitorSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("TimeDelaySleepTimes");		// 时统设置线程延时次数
		pConstVar->m_iTimeDelaySleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSetSleepTimes");			// ADC参数设置线程延时次数
		pConstVar->m_iADCSetSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ErrorCodeSleepTimes");		// 误码查询线程延时次数
		pConstVar->m_iErrorCodeSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataRecSleepTimes");		// ADC数据接收线程延时次数
		pConstVar->m_iADCDataRecSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataSaveSleepTimes");		// ADC数据存储线程延时次数
		pConstVar->m_iADCDataSaveSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("CloseThreadSleepTimes");		// 等待线程关闭的延时次数
		pConstVar->m_iCloseThreadSleepTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("HeadFrameStableNum");			// 首包稳定计数
		pConstVar->m_iHeadFrameStableTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("IPAddrResetTimes");		// IP地址重设次数
		pConstVar->m_iIPAddrResetTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("TailFrameStableTimes");		// 尾包稳定计数
		pConstVar->m_iTailFrameStableTimes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("MonitorStableTime");		// 路由监视稳定时间
		pConstVar->m_iMonitorStableTime = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("LineSysStableTime");		// 测网系统达到稳定状态时间
		pConstVar->m_iLineSysStableTime = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSetOptNb");			// ADC参数设置操作序号
		pConstVar->m_iADCSetOptNb = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCStartSampleOptNb");	// ADC开始采集操作序号
		pConstVar->m_iADCStartSampleOptNb = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCStopSampleOptNb");		// ADC停止采集操作序号
		pConstVar->m_iADCStopSampleOptNb = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("InstrumentTypeLAUX");		// 仪器类型-交叉站
		pConstVar->m_iInstrumentTypeLAUX = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("InstrumentTypeLAUL");		// 仪器类型-电源站
		pConstVar->m_iInstrumentTypeLAUL = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("InstrumentTypeFDU");		// 仪器类型-采集站
		pConstVar->m_iInstrumentTypeFDU = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("InstrumentTypeLCI");		// 仪器类型-LCI
		pConstVar->m_iInstrumentTypeLCI= GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("DirectionTop");			// 方向上方
		pConstVar->m_iDirectionTop = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("DirectionDown");			// 方向下方
		pConstVar->m_iDirectionDown = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("DirectionLeft");			// 方向左方
		pConstVar->m_iDirectionLeft = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("DirectionRight");			// 方向右方
		pConstVar->m_iDirectionRight = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("DirectionCenter");		// 方向正中
		pConstVar->m_iDirectionCenter = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("IPSetAddrStart");			// IP地址设置的起始地址
		pConstVar->m_iIPSetAddrStart = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("IPSetAddrInterval");		// IP地址设置的间隔
		pConstVar->m_iIPSetAddrInterval = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("RoutSetAddrStart");		// 路由地址设置的起始地址
		pConstVar->m_iRoutSetAddrStart = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("RoutSetAddrInterval");	// 路由地址设置的间隔
		pConstVar->m_iRoutSetAddrInterval = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("BroadcastPortStart");		// 设置广播端口起始地址
		pConstVar->m_iBroadcastPortStart = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("IPBroadcastAddr");		// 设置为广播IP
		pConstVar->m_uiIPBroadcastAddr = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCFrameSaveInOneFileNum");	// 一个文件内存储单个设备ADC数据帧数
		pConstVar->m_iADCFrameSaveInOneFileNum = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSaveHeadLineNum");		// 存储ADC数据的文件头行数
		pConstVar->m_iADCSaveHeadLineNum = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSaveLeftInfoBytes");		// 存储ADC数据的左侧预留信息字节数
		pConstVar->m_iADCSaveLeftInfoBytes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSaveDataBytes");		// 存储ADC数据的字节数
		pConstVar->m_iADCSaveDataBytes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCSaveDataIntervalBytes");		// 存储ADC数据之间的间隔字节数
		pConstVar->m_iADCSaveDataIntervalBytes = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataBufSize");		// 设备ADC数据缓冲区大小
		pConstVar->m_iADCDataBufSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("TBSleepTimeHigh");			// TB设置延时高位
		pConstVar->m_uiTBSleepTimeHigh = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("TBSleepTimeLow");				// TB设置延时低位
		pConstVar->m_usTBSleepTimeLow = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		//读取ini文件中相应字段的内容
		strSection = _T("帧格式设置");			// 获取当前区域
		strSectionKey=_T("FrameHeadSize");		// 帧头长度
		pConstVar->m_iFrameHeadSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("FrameHeadCheck");		// 同步帧头
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpFrameHeadCheck, pConstVar->m_iFrameHeadSize, strValue);

		strSectionKey=_T("FrameCmdSize1B");		// 命令字长度1字节
		pConstVar->m_iFrameCmdSize1B = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("FramePacketSize1B");	// 命令包长度1字节
		pConstVar->m_iFramePacketSize1B = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("FramePacketSize2B");	// 命令包长度2字节
		pConstVar->m_iFramePacketSize2B = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("FramePacketSize4B");	// 命令包长度4字节
		pConstVar->m_iFramePacketSize4B = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataSize3B");		// ADC数据所占字节数
		pConstVar->m_iADCDataSize3B = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCDataInOneFrameNum");	// 一帧内ADC数据个数
		pConstVar->m_iADCDataInOneFrameNum = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("ADCFramePointLimit");	// ADC数据帧指针偏移量上限
		pConstVar->m_usADCFramePointLimit = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CommandWordMaxNum");	// 命令字个数最大值
		pConstVar->m_iCommandWordMaxNum = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);
		pConstVar->m_iADCSetCommandMaxByte = pConstVar->m_iCommandWordMaxNum 
			* pConstVar->m_iFramePacketSize4B;

		strSectionKey=_T("SndFrameBufInit");	// 发送帧缓冲区初值设定
		pConstVar->m_cSndFrameBufInit = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("RcvFrameSize");		// 接收的网络数据帧帧长度
		pConstVar->m_iRcvFrameSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("SndFrameSize");		// 发送的网络数据帧帧长度
		pConstVar->m_iSndFrameSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		/////////////////////////////////////////////////////////////////////////
		strSection = _T("服务器与设备命令字设置");		// 获取当前区域
		strSectionKey=_T("SendSetCmd");			// 发送设置命令
		pConstVar->m_usSendSetCmd = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("SendQueryCmd");		// 发送查询命令
		pConstVar->m_usSendQueryCmd = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("SendADCCmd");			// 发送ADC采样数据重发命令
		pConstVar->m_usSendADCCmd = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdSn");				// 串号
		pConstVar->m_cCmdSn = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
		pConstVar->m_cCmdHeadFrameTime = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
		pConstVar->m_cCmdLocalIPAddr = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
		pConstVar->m_cCmdLocalSysTime = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
		pConstVar->m_cCmdLocalTimeFixedHigh = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
		pConstVar->m_cCmdLocalTimeFixedLow = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
		pConstVar->m_cCmdADCDataReturnAddr = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
		pConstVar->m_cCmdADCDataReturnPort = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
		pConstVar->m_cCmdADCDataReturnPortLimit = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
		pConstVar->m_cCmdBroadCastPortSet = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
		pConstVar->m_cCmdFDUErrorCode = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTBHigh");				// TB时刻高位
		pConstVar->m_cCmdTBHigh = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTbLow");				// TB时刻低位
		pConstVar->m_cCmdTbLow = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
		pConstVar->m_cCmdLAUXRoutOpenQuery = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
		pConstVar->m_cCmdLAUXRoutOpenSet = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
		pConstVar->m_cCmdTailRecSndTime = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
		pConstVar->m_cCmdBroadCastPortSeted = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
		pConstVar->m_cCmdADCSet = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdNetTime");				// 网络时刻
		pConstVar->m_cCmdNetTime = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
		pConstVar->m_cCmdLineTailRecTimeLAUX = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
		pConstVar->m_cCmdLAUTailRecTimeLAUX = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
		pConstVar->m_cCmdLAUXErrorCode1 = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
		pConstVar->m_cCmdLAUXErrorCode2 = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
		pConstVar->m_cCmdLAUXSetRout = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdReturnRout");			// 返回路由
		pConstVar->m_cCmdReturnRout = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdEnd");					// 命令解析结束命令
		pConstVar->m_cCmdEnd = static_cast<char>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTBCtrlStartSample");		// TB开关控制ADC数据采集命令
		pConstVar->m_usCmdTBCtrlStartSample = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTBLoseCtrlStartSample");	// 无需TB开关控制ADC数据采集命令
		pConstVar->m_usCmdTBLoseCtrlStartSample = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdTBCtrlStopSample");		// TB开关控制ADC数据停止采集命令
		pConstVar->m_usCmdTBCtrlStopSample = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));

		strSectionKey=_T("CmdCtrlCloseLed");			// LED灯灭
		pConstVar->m_usCmdCtrlCloseLed = static_cast<unsigned short>(GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath));
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
void OnInitConstVar(m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut)
{
	if (pConstVar == NULL)
	{
		return;
	}
	pConstVar->m_pLogOutPut = pLogOutPut;
	LoadIniFile(pConstVar);
}
// 关闭常量信息结构体
void OnCloseConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pConstVar->m_cpFrameHeadCheck != NULL)
	{
		delete[] pConstVar->m_cpFrameHeadCheck;
		pConstVar->m_cpFrameHeadCheck = NULL;
	}
}
// 释放常量信息结构体
void OnFreeConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	delete pConstVar;
}