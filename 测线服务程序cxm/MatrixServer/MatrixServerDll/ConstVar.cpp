#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建常量信息结构体
m_oConstVarStruct* OnCreateConstVar(void)
{
	m_oConstVarStruct* pConstVar = NULL;
	pConstVar = new m_oConstVarStruct;
	pConstVar->m_cpFrameHeadCheck = NULL;
	pConstVar->m_cpSetADCSetSine = NULL;
	pConstVar->m_cpSetADCStopSample = NULL;
	pConstVar->m_cpSetADCOpenTBPowerLow = NULL;
	pConstVar->m_cpSetADCOpenTBPowerHigh = NULL;
	pConstVar->m_cpSetADCOpenSwitchTBLow = NULL;
	pConstVar->m_cpSetADCOpenSwitchTBHigh = NULL;
	pConstVar->m_cpSetADCRegisterRead = NULL;
	pConstVar->m_cpSetADCRegisterWrite = NULL;
	pConstVar->m_cpSetADCTBSwitchOpen = NULL;
	pConstVar->m_cpSetADCSample = NULL;
	pConstVar->m_cpSetADCReadContinuous = NULL;
	pConstVar->m_pLogOutPut = NULL;
	InitializeCriticalSection(&pConstVar->m_oSecConstVar);
	return pConstVar;
}
// 载入INI文件
void LoadIniFile(m_oConstVarStruct* pConstVar, string strINIFilePath)
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
	int iTemp = 0;
	int iReadNum = 0;
	strFilePath = strINIFilePath.c_str();
	if (false == IfFileExist(strFilePath))
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "LoadIniFile", "", 
			ErrorType, IDS_ERR_FILE_NOEXIST);
		return;
	}
	try
	{
		EnterCriticalSection(&pConstVar->m_oSecConstVar);
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
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_iBroadcastPortStart, sizeof(int));

		strSectionKey=_T("IPBroadcastAddr");		// 设置为广播IP
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_iIPBroadcastAddr, sizeof(int));

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
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cSndFrameBufInit = static_cast<char>(iTemp);

		strSectionKey=_T("RcvFrameSize");		// 接收的网络数据帧帧长度
		pConstVar->m_iRcvFrameSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

		strSectionKey=_T("SndFrameSize");		// 发送的网络数据帧帧长度
		pConstVar->m_iSndFrameSize = GetPrivateProfileInt(strSection,strSectionKey,NULL,strFilePath);

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
		pConstVar->m_cCmdSn = static_cast<char>(iTemp);

		strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdHeadFrameTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &pConstVar->m_cCmdLocalIPAddr, sizeof(char));

		strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLocalSysTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLocalTimeFixedHigh = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLocalTimeFixedLow = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdADCDataReturnAddr = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdADCDataReturnPort = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdADCDataReturnPortLimit = static_cast<char>(iTemp);

		strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdBroadCastPortSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdFDUErrorCode = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTBHigh");				// TB时刻高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdTBHigh = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTbLow");				// TB时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdTbLow = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUXRoutOpenQuery = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUXRoutOpenSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdTailRecSndTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdBroadCastPortSeted = static_cast<char>(iTemp);

		strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdADCSet = static_cast<char>(iTemp);

		strSectionKey=_T("CmdNetTime");				// 网络时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdNetTime = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLineTailRecTimeLAUX = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUTailRecTimeLAUX = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUXErrorCode1 = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUXErrorCode2 = static_cast<char>(iTemp);

		strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdLAUXSetRout = static_cast<char>(iTemp);

		strSectionKey=_T("CmdReturnRout");			// 返回路由
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdReturnRout = static_cast<char>(iTemp);

		strSectionKey=_T("CmdEnd");					// 命令解析结束命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		_stscanf_s(strValue, _T("0x%x"), &iTemp, sizeof(int));
		pConstVar->m_cCmdEnd = static_cast<char>(iTemp);

		//读取ini文件中相应字段的内容
		strSection = _T("ADC参数设置");			// 获取当前区域

		strSectionKey=_T("SetADCSetSineSize");	// ADC设置正弦波命令大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCSetSineSize = _ttoi(strValue);

		strSectionKey=_T("SetADCSetSine");		// ADC设置正弦波命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCSetSine, pConstVar->m_iSetADCSetSineSize, strValue);

		strSectionKey=_T("SetADCStopSampleSize");	// ADC设置停止采样命令大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCStopSampleSize = _ttoi(strValue);

		strSectionKey=_T("SetADCStopSample");		// ADC设置停止采样命令
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCStopSample, 
			pConstVar->m_iSetADCStopSampleSize, strValue);

		strSectionKey=_T("SetADCOpenTBPowerLowSize");	// ADC设置打开TB电源低位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenTBPowerLowSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenTBPowerLow");		// ADC设置打开TB电源低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCOpenTBPowerLow, 
			pConstVar->m_iSetADCOpenTBPowerLowSize, strValue);

		strSectionKey=_T("SetADCOpenTBPowerHighSize");	// ADC设置打开TB电源高位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenTBPowerHighSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenTBPowerHigh");		// ADC设置打开TB电源高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCOpenTBPowerHigh, 
			pConstVar->m_iSetADCOpenTBPowerHighSize, strValue);

		strSectionKey=_T("SetADCOpenSwitchTBLowSize");	// ADC设置打开TB开关低位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenSwitchTBLowSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenSwitchTBLow");		// ADC设置打开TB开关低位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCOpenSwitchTBLow, 
			pConstVar->m_iSetADCOpenSwitchTBLowSize, strValue);

		strSectionKey=_T("SetADCOpenSwitchTBHighSize");	// ADC设置打开TB开关高位大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCOpenSwitchTBHighSize = _ttoi(strValue);

		strSectionKey=_T("SetADCOpenSwitchTBHigh");		// ADC设置打开TB开关高位
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCOpenSwitchTBHigh, 
			pConstVar->m_iSetADCOpenSwitchTBHighSize, strValue);

		strSectionKey=_T("SetADCRegisterReadSize");	// ADC设置读寄存器大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCRegisterReadSize = _ttoi(strValue);

		strSectionKey=_T("SetADCRegisterRead");		// ADC设置读寄存器
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCRegisterRead, 
			pConstVar->m_iSetADCRegisterReadSize, strValue);

		strSectionKey=_T("SetADCRegisterWriteSize");	// ADC设置写寄存器大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCRegisterWriteSize = _ttoi(strValue);

		strSectionKey=_T("SetADCRegisterWrite");		// ADC设置写寄存器
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCRegisterWrite, 
			pConstVar->m_iSetADCRegisterWriteSize, strValue);

		strSectionKey=_T("SetADCTBSwitchOpenSize");	// ADC设置打开TB开关大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCTBSwitchOpenSize = _ttoi(strValue);

		strSectionKey=_T("SetADCTBSwitchOpen");		// ADC设置打开TB开关
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCTBSwitchOpen, 
			pConstVar->m_iSetADCTBSwitchOpenSize, strValue);

		strSectionKey=_T("SetADCSampleSize");	// ADC采样设置大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCSampleSize = _ttoi(strValue);

		strSectionKey=_T("SetADCSample");		// ADC采样设置
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCSample, pConstVar->m_iSetADCSampleSize, strValue);

		strSectionKey=_T("SetADCReadContinuousSize");	// ADC设置连续采样大小
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		pConstVar->m_iSetADCReadContinuousSize = _ttoi(strValue);

		strSectionKey=_T("SetADCReadContinuous");		// ADC设置连续采样
		GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,sizeof(strBuff) / 2,strFilePath);
		strValue = strBuff;
		ParseCStringToArray(&pConstVar->m_cpSetADCReadContinuous, 
			pConstVar->m_iSetADCReadContinuousSize, strValue);

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

		LeaveCriticalSection(&pConstVar->m_oSecConstVar);
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
void OnInitConstVar(m_oConstVarStruct* pConstVar, string strINIFilePath, m_oLogOutPutStruct* pLogOutPut)
{
	if (pConstVar == NULL)
	{
		return;
	}
	EnterCriticalSection(&pConstVar->m_oSecConstVar);
	pConstVar->m_pLogOutPut = pLogOutPut;
	LoadIniFile(pConstVar, strINIFilePath);
	LeaveCriticalSection(&pConstVar->m_oSecConstVar);
}
// 关闭常量信息结构体
void OnCloseConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	EnterCriticalSection(&pConstVar->m_oSecConstVar);
	if (pConstVar->m_cpFrameHeadCheck != NULL)
	{
		delete[] pConstVar->m_cpFrameHeadCheck;
		pConstVar->m_cpFrameHeadCheck = NULL;
	}
	if (pConstVar->m_cpSetADCSetSine != NULL)
	{
		delete[] pConstVar->m_cpSetADCSetSine;
		pConstVar->m_cpSetADCSetSine = NULL;
	}
	if (pConstVar->m_cpSetADCStopSample != NULL)
	{
		delete[] pConstVar->m_cpSetADCStopSample;
		pConstVar->m_cpSetADCStopSample = NULL;
	}
	if (pConstVar->m_cpSetADCOpenTBPowerLow != NULL)
	{
		delete[] pConstVar->m_cpSetADCOpenTBPowerLow;
		pConstVar->m_cpSetADCOpenTBPowerLow = NULL;
	}
	if (pConstVar->m_cpSetADCOpenTBPowerHigh != NULL)
	{
		delete[] pConstVar->m_cpSetADCOpenTBPowerHigh;
		pConstVar->m_cpSetADCOpenTBPowerHigh = NULL;
	}
	if (pConstVar->m_cpSetADCOpenSwitchTBLow != NULL)
	{
		delete[] pConstVar->m_cpSetADCOpenSwitchTBLow;
		pConstVar->m_cpSetADCOpenSwitchTBLow = NULL;
	}
	if (pConstVar->m_cpSetADCOpenSwitchTBHigh != NULL)
	{
		delete[] pConstVar->m_cpSetADCOpenSwitchTBHigh;
		pConstVar->m_cpSetADCOpenSwitchTBHigh = NULL;
	}
	if (pConstVar->m_cpSetADCRegisterRead != NULL)
	{
		delete[] pConstVar->m_cpSetADCRegisterRead;
		pConstVar->m_cpSetADCRegisterRead = NULL;
	}
	if (pConstVar->m_cpSetADCRegisterWrite != NULL)
	{
		delete[] pConstVar->m_cpSetADCRegisterWrite;
		pConstVar->m_cpSetADCRegisterWrite = NULL;
	}
	if (pConstVar->m_cpSetADCTBSwitchOpen != NULL)
	{
		delete[] pConstVar->m_cpSetADCTBSwitchOpen;
		pConstVar->m_cpSetADCTBSwitchOpen = NULL;
	}
	if (pConstVar->m_cpSetADCSample != NULL)
	{
		delete[] pConstVar->m_cpSetADCSample;
		pConstVar->m_cpSetADCSample = NULL;
	}
	if (pConstVar->m_cpSetADCReadContinuous != NULL)
	{
		delete[] pConstVar->m_cpSetADCReadContinuous;
		pConstVar->m_cpSetADCReadContinuous = NULL;
	}
	LeaveCriticalSection(&pConstVar->m_oSecConstVar);
}
// 释放常量信息结构体
void OnFreeConstVar(m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pConstVar->m_oSecConstVar);
	delete pConstVar;
}