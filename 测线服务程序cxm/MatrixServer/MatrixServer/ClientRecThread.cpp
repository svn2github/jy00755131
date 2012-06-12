/**
@brief 该文件为服务端与客户端通讯接收线程的实现文件
@details
@file ClientRecThread.cpp
@note 
@see	
@version	
@author cxm
@bug
*/
#include "StdAfx.h"
#include "ClientRecThread.h"
#include "CommClient.h"
CClientRecThread::CClientRecThread(void)
	: m_fInstrumentNoiseLimit(0)
	, m_fInstrumentDistortionLimit(0)
	, m_fInstrumentCrosstalkLimit(0)
	, m_fInstrumentGainPhaseLimit(0)
	, m_fInstrumentCMRRLimit(0)
	, m_fSensorResistanceLimitMin(0)
	, m_fSensorResistanceLimitMax(0)
	, m_fSensorLeakageLimit(0)
	, m_fSensorNoiseLimit(0)
	, m_fSensorTiltLimit(0)
	, m_fSensorTiltModelLimit(0)
	, m_fSeisMonitorLimit(0)
{
	m_pClientRecFrame = NULL;
	m_pClientSndFrame = NULL;
	m_pMatrixDllCall = NULL;
	m_pComClientMap = NULL;
	m_uiLineNum = 0;
	m_uiColumnNum = 0;
	m_bCheckConnected = false;
	m_oInstrumentWholeTableMap.clear();
	m_oInstrumentUpdateArea.clear();
	m_uiClientActiveCount = 0;
	m_uiClientCheckCount = 0;
	m_bClientClose = false;
}


CClientRecThread::~CClientRecThread(void)
{
	m_oInstrumentWholeTableMap.clear();
	m_oInstrumentUpdateArea.clear();
}

// 处理函数
void CClientRecThread::OnProc(void)
{
	int iFrameNum = 0;
	bool bActive = false;
	m_oCommFrameStructPtr ptrFrameRec = NULL;
	m_oCommFrameStructPtr ptrFrameSnd = NULL;
	EnterCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
	EnterCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	iFrameNum = m_pClientRecFrame->m_olsCommWorkFrame.size();
	if (iFrameNum != 0)
	{
		bActive = true;
	}
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrameRec = *m_pClientRecFrame->m_olsCommWorkFrame.begin();
		// 如果为设置帧
		if (ptrFrameRec->m_cCmdType == CmdTypeSet)
		{
			// 发送应答帧
			m_pClientSndFrame->MakeReturnFrame(ptrFrameRec);
			// 如果为新命令则清除未完成的旧命令待处理任务
			if (m_pClientRecFrame->m_uiCmdIndex < ptrFrameRec->m_uiCmdIndex)
			{
				m_pClientRecFrame->m_uiCmdIndex = ptrFrameRec->m_uiCmdIndex;
				m_pClientRecFrame->OnResetProcBuf();
			}
			// 将帧内容加入待处理任务中
			if (m_pClientRecFrame->m_uiCmdIndex == ptrFrameRec->m_uiCmdIndex)
			{
				SaveRecFrameToTask(ptrFrameRec);
			}
			// 如果为过期命令则不处理只返回应答
			else
			{
			}
		}
		// 如果为应答帧
		else if (ptrFrameRec->m_cCmdType == CmdTypeReturn)
		{
			// 在已发送帧索引中找到该帧并删除
			if (TRUE == m_pClientSndFrame->IfFramePtrExistInSndMap(ptrFrameRec->m_usCmd, 
				ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
				&m_pClientSndFrame->m_oSndFrameMap))
			{
				ptrFrameSnd = m_pClientSndFrame->GetFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pClientSndFrame->m_oSndFrameMap);
				m_pClientSndFrame->DeleteFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pClientSndFrame->m_oSndFrameMap);
				m_pClientSndFrame->AddFreeFrameStruct(ptrFrameSnd);
			}
		}
		m_pClientRecFrame->m_olsCommWorkFrame.pop_front();
		m_pClientRecFrame->AddFreeFrameStruct(ptrFrameRec);
	}
	LeaveCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	LeaveCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
	// 监视客户端是否活跃
	MonitorClientActive(bActive);
}
// 将帧内容加入待处理任务中
void CClientRecThread::SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame)
{
	unsigned int uiSaveBegin = 0;
	uiSaveBegin = (ptrFrame->m_uiFrameIndex - 1) * FrameInfoSizeLimit;
	memcpy(&m_pClientRecFrame->m_cProcBuf[uiSaveBegin], ptrFrame->m_pcFrameInfo, ptrFrame->m_usFrameInfoSize);
	// 命令帧接收完全
	if (ptrFrame->m_uiFrameIndex == ptrFrame->m_uiFrameNum)
	{
		// 进行命令处理
		OnProcRecCmd(ptrFrame->m_usCmd, m_pClientRecFrame->m_cProcBuf, uiSaveBegin + ptrFrame->m_usFrameInfoSize);
	}
}

// 处理接收命令函数
void CClientRecThread::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	// 客户端登陆验证（帧内容为验证码）
	if (usCmd == CmdClientConnect)
	{
		// 与连接校验码相匹配
		if (0 == strcmp(pChar, CommCheck))
		{
			m_bCheckConnected = true;
		}
	}
	else
	{
		if (m_bCheckConnected == false)
		{
			return;
		}
		switch (usCmd)
		{
		case CmdQueryRevSection:
			OnProcQueryRevSection();
			break;
			// 客户端心跳命令（帧内容为空）
		case CmdSetHeartBeat:
			OnProcInstrumentTableUpdate();
			break;
		case CmdQueryWholeTable:
			m_oInstrumentWholeTableMap.clear();
			OnProcInstrumentTableUpdate();
			break;
			// 上电（命令字后帧内容为空，返回值为执行FieldOn剩余时间，为0表示无需等待）
		case CmdSetFieldOn:
			OnProcSetFieldOn();
			break;
			// 断电（命令字后帧内容为空）
		case CmdSetFieldOff:
			OnProcSetFieldOff();
			break;
			// 查询所选仪器全部信息（帧内容为行号+区域号）
		case CmdQueryInstrumentInfo:
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrumentInfoByArea);
			break;
			// 查询全部仪器的全部信息（应答帧内容为仪器结构体）
		case CmdQueryInstrumentInfoAll:
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrumentInfoByArea);
			break;

			// 查询 SurveyXML 文件信息（帧内容为空）
		case CmdQuerySurveyXMLInfo:
			OnProcQuerySurveyXMLInfo(usCmd);
			break;
			// 设置 SurveyXML 文件信息（帧内容为信息结构体）
		case CmdSetSurveyXMLInfo:
			m_pMatrixDllCall->Dll_SetSurverySetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 PointCode XML文件信息（帧内容为空）
		case CmdQueryPointCodeXMLInfo:
			OnProcQueryPointCodeXMLInfo(usCmd);
			break;
			// 设置 PointCode XML文件信息（帧内容为信息结构体）
		case CmdSetPointCodeXMLInfo:
			m_pMatrixDllCall->Dll_SetPointCodeSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Sensor XML文件信息（帧内容为空）
		case CmdQuerySensorXMLInfo:
			OnProcQuerySensorXMLInfo(usCmd);
			break;
			// 设置 Sensor XML文件信息（帧内容为信息结构体）
		case CmdSetSensorXMLInfo:
			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Marker XML文件信息（帧内容为空）
		case CmdQueryMarkerXMLInfo:
			OnProcQueryMarkerXMLInfo(usCmd);
			break;
			// 设置 Marker XML文件信息（帧内容为信息结构体）
		case CmdSetMarkerXMLInfo:
			m_pMatrixDllCall->Dll_SetMarkerSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Aux XML文件信息（帧内容为空）
		case CmdQueryAuxXMLInfo:
			OnProcQueryAuxXMLInfo(usCmd);
			break;
			// 设置 Aux XML文件信息（帧内容为信息结构体）
		case CmdSetAuxXMLInfo:
			m_pMatrixDllCall->Dll_SetAuxSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Detour XML文件信息（帧内容为空）
		case CmdQueryDetourXMLInfo:
			OnProcQueryDetourXMLInfo(usCmd);
			break;
			// 设置 Detour XML文件信息（帧内容为信息结构体）
		case CmdSetDetourXMLInfo:
			m_pMatrixDllCall->Dll_SetDetourSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Mute XML文件信息（帧内容为空）
		case CmdQueryMuteXMLInfo:
			OnProcQueryMuteXMLInfo(usCmd);
			break;
			// 设置 Mute XML文件信息（帧内容为信息结构体）
		case CmdSetMuteXMLInfo:
			m_pMatrixDllCall->Dll_SetMuteSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 BlastMachine XML文件信息（帧内容为空）
		case CmdQueryBlastMachineXMLInfo:
			OnProcQueryBlastMachineXMLInfo(usCmd);
			break;
			// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
		case CmdSetBlastMachineXMLInfo:
			m_pMatrixDllCall->Dll_SetBlastMachineSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Absolute XML文件信息（帧内容为空）
		case CmdQueryAbsoluteXMLInfo:
			OnProcQueryAbsoluteXMLInfo(usCmd);
			break;
			// 设置 Absolute XML文件信息（帧内容为信息结构体）
		case CmdSetAbsoluteXMLInfo:
			m_pMatrixDllCall->Dll_SetAbsoluteSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break; 
			// 查询 Generic XML文件信息（帧内容为空）
		case CmdQueryGenericXMLInfo:
			OnProcQueryGenericXMLInfo(usCmd);
			break;
			// 设置 Generic XML文件信息（帧内容为信息结构体）
		case CmdSetGenericXMLInfo:
			m_pMatrixDllCall->Dll_SetGenericSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Look XML文件信息（帧内容为空）
		case CmdQueryLookXMLInfo:
			OnProcQueryLookXMLInfo(usCmd);
			break;
			// 设置 Look XML文件信息（帧内容为信息结构体）
		case CmdSetLookXMLInfo:
			m_pMatrixDllCall->Dll_SetLookSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestBase XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestBaseXMLInfo:
			OnProcQueryInstrumentTestBaseXMLInfo(usCmd);
			break;
			// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestBaseXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, true);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestBase XML文件信息（帧内容为空）
		case CmdQuerySensorTestBaseXMLInfo:
			OnProcQuerySensorTestBaseXMLInfo(usCmd);
			break;
			// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestBaseXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, false);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestLimit XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestLimitXMLInfo:
			OnProcQueryInstrumentTestLimitXMLInfo(usCmd);
			break;
			// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestLimitXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, true);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestLimit XML文件信息（帧内容为空）
		case CmdQuerySensorTestLimitXMLInfo:
			OnProcQuerySensorTestLimitXMLInfo(usCmd);
			break;
			// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestLimitXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, false);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTest XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestXMLInfo:
			OnProcQueryInstrumentTestXMLInfo(usCmd);
			break;
			// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrumentTestSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTest XML文件信息（帧内容为空）
		case CmdQuerySensorTestXMLInfo:
			OnProcQuerySensorTestXMLInfo(usCmd);
			break;
			// SensorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestXMLInfo:
			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 MultipleTest XML文件信息（帧内容为空）
		case CmdQueryMultipleTestXMLInfo:
			OnProcQueryMultipleTestXMLInfo(usCmd);
			break;
			// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
		case CmdSetMultipleTestXMLInfo:
			m_pMatrixDllCall->Dll_SetMultipleTestSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SeisMonitorTest XML文件信息（帧内容为空）
		case CmdQuerySeisMonitorTestXMLInfo:
			OnProcQuerySeisMonitorTestXMLInfo(usCmd);
			break;
			// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSeisMonitorTestXMLInfo:
			m_pMatrixDllCall->Dll_SetSeisMonitorSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 LAULeakage XML文件信息（帧内容为空）
		case CmdQueryLAULeakageXMLInfo:
			OnProcQueryLAULeakageXMLInfo(usCmd);
			break;
			// 设置 LAULeakage XML文件信息（帧内容为信息结构体）
		case CmdSetLAULeakageXMLInfo:
			m_pMatrixDllCall->Dll_SetLAULeakageSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 FormLine XML文件信息（帧内容为空）
		case CmdQueryFormLineXMLInfo:
			OnProcQueryFormLineXMLInfo(usCmd);
			break;
			// 设置 FormLine XML文件信息（帧内容为信息结构体）
		case CmdSetFormLineXMLInfo:
			m_pMatrixDllCall->Dll_SetFormLineSetupData(pChar, uiSize);
			BroadCastXMLChange(usCmd, pChar, uiSize);
			break;

			// 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrNoiseTestArea:
			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrNoiseTestByArea);
			break;
			// 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）
		case CmdQueryInstrNoiseTestAll:
			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrNoiseTestByArea);
			break;
			// 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrDistortionTestArea:
			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrDistortionTestByArea);
			break;
			// 查询全部仪器失真测试数据和测试结果（帧内容为空）
		case CmdQueryInstrDistortionTestAll:
			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrDistortionTestByArea);
			break;
			// 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCrosstalkTestArea:
			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrCrosstalkTestByArea);
			break;
			// 查询全部仪器串扰测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCrosstalkTestAll:
			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrCrosstalkTestByArea);
			break;
			// 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCMRRTestArea:
			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrCMRRTestByArea);
			break;
			// 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCMRRTestAll:
			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrCMRRTestByArea);
			break;
			// 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrGainPhaseTestArea:
			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrGainPhaseTestByArea);
			break;
			// 查询全部仪器增益相位测试数据和测试结果（帧内容为空）
		case CmdQueryInstrGainPhaseTestAll:
			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
			OnProcQueryInfoAll(&CClientRecThread::QueryInstrGainPhaseTestByArea);
			break;
			// 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorResistanceTestArea:
			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorResistanceTestByArea);
			break;
			// 查询全部检波器阻抗测试数据和测试结果（帧内容为空）
		case CmdQuerySensorResistanceTestAll:
			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
			OnProcQueryInfoAll(&CClientRecThread::QuerySensorResistanceTestByArea);
			break;
			// 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorLeakageTestArea:
			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorLeakageTestByArea);
			break;
			// 查询全部检波器漏电测试数据和测试结果（帧内容为空）
		case CmdQuerySensorLeakageTestAll:
			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
			OnProcQueryInfoAll(&CClientRecThread::QuerySensorLeakageTestByArea);
			break;
			// 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorNoiseTestArea:
			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorNoiseTestByArea);
			break;
			// 查询全部检波器噪声测试数据和测试结果（帧内容为空）
		case CmdQuerySensorNoiseTestAll:
			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
			OnProcQueryInfoAll(&CClientRecThread::QuerySensorNoiseTestByArea);
			break;
			// 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltTestArea:
			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorTiltTestByArea);
			break;
			// 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltTestAll:
			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
			OnProcQueryInfoAll(&CClientRecThread::QuerySensorTiltTestByArea);
			break;
			// 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltModelTestArea:
			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorTiltModelTestByArea);
			break;
			// 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltModelTestAll:
			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
			OnProcQueryInfoAll(&CClientRecThread::QuerySensorTiltModelTestByArea);
			break;
			// 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySeisMonitorTestArea:
			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySeisMonitorTestByArea);
			break;
			// 查询全部地震监测测试数据和测试结果（帧内容为空）
		case CmdQuerySeisMonitorTestAll:
			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
			OnProcQueryInfoAll(&CClientRecThread::QuerySeisMonitorTestByArea);
			break;
		default:
			break;
		}
	}
}
// 由线号和点号得到区域位置
void CClientRecThread::GetAreaFromPoint(int iLineIndex, int iPointIndex, m_oAreaStruct* pAreaStruct)
{
	pAreaStruct->m_uiLineNb = m_uiLineNum + iLineIndex;
	pAreaStruct->m_uiAreaNb = m_uiColumnNum + iPointIndex;
	if ((pAreaStruct->m_uiAreaNb % InstrumentTableWindowSize) == 0)
	{
		pAreaStruct->m_uiAreaNb /= InstrumentTableWindowSize;
	}
	else
	{
		pAreaStruct->m_uiAreaNb /= InstrumentTableWindowSize;
		pAreaStruct->m_uiAreaNb += 1;
	}
}
// 由区域号得到线号及点号范围
void CClientRecThread::GetPointRangeFromArea(int* iLineIndex, int* iPointMinIndex, int* iPointMaxIndex, m_oAreaStruct* pAreaStruct)
{
	*iLineIndex = (int)pAreaStruct->m_uiLineNb - m_uiLineNum;
	*iPointMinIndex = ((int)pAreaStruct->m_uiAreaNb - 1) * InstrumentTableWindowSize + 1 - m_uiColumnNum;
	*iPointMaxIndex = ((int)pAreaStruct->m_uiAreaNb) * InstrumentTableWindowSize - m_uiColumnNum;
}
// 判断仪器更新区域是否已加入索引表
BOOL CClientRecThread::IfAreaExistInMap(m_oAreaStruct* pAreaStruct,
	map<m_oAreaStruct, m_oAreaStruct>* pMap)
{
	if ((pMap == NULL) || (pAreaStruct == NULL))
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	map<m_oAreaStruct, m_oAreaStruct>::iterator iter;
	iter = pMap->find(*pAreaStruct);
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
// 增加对象到索引表
void CClientRecThread::AddAreaToMap(int iLineIndex, int iPointIndex, 
	map<m_oAreaStruct, m_oAreaStruct>* pMap)
{
	if (pMap == NULL)
	{
		return;
	}
	m_oAreaStruct oAreaStruct;
	GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
	if (FALSE == IfAreaExistInMap(&oAreaStruct, pMap))
	{
		pMap->insert(map<m_oAreaStruct, m_oAreaStruct>::value_type (oAreaStruct, oAreaStruct));
	}
}
// 处理仪器设备表更新
void CClientRecThread::OnProcInstrumentTableUpdate(void)
{
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::iterator iterLocation;
	map<m_oAreaStruct, m_oAreaStruct>::iterator iterArea;
	int iLineIndex = 0;
	int iPointIndex = 0;
	int iAddr = 0;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	int iPos = 0;
	int iSize = 0;
	m_oInstrumentStruct* pInstrument = NULL;
	m_oInstrumentStruct* pInstrumentClient = NULL;
	m_oInstrumentUpdateArea.clear();
	// 将SN索引表与客户端的仪器位置索引表相对照
	m_pMatrixDllCall->Dll_QueryInstrumentLocation(m_pClientSndFrame->m_cProcBuf, iSize);
	for (int i=0; i<iSize;)
	{
		memcpy(&iLineIndex, &m_pClientSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&iPointIndex, &m_pClientSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&iAddr, &m_pClientSndFrame->m_cProcBuf[i], 4);
		i += 4;
		pInstrument = (m_oInstrumentStruct*)iAddr;
		// 客户端仪器索引表中找不到该设备
		if (FALSE == IfLocationExistInMap(iLineIndex, iPointIndex, &m_oInstrumentWholeTableMap))
		{
			AddLocationToMap(iLineIndex, iPointIndex, pInstrument, &m_oInstrumentWholeTableMap);
			// 增加对象到索引表
			AddAreaToMap(iLineIndex, iPointIndex, &m_oInstrumentUpdateArea);
		}
		else
		{
			pInstrumentClient = GetInstrumentFromLocationMap(iLineIndex, iPointIndex, &m_oInstrumentWholeTableMap);
			if (pInstrument != pInstrumentClient)
			{
				pInstrumentClient = pInstrument;
				// 增加对象到索引表
				AddAreaToMap(iLineIndex, iPointIndex, &m_oInstrumentUpdateArea);
			}
		}
	}
	EnterCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	// 将客户端的仪器位置索引表与SN索引表相对照
	for (iterLocation = m_oInstrumentWholeTableMap.begin(); iterLocation != m_oInstrumentWholeTableMap.end();)
	{
		// 仪器IP地址索引表找不到该设备
		if (FALSE == m_pMatrixDllCall->Dll_IfLocationExistInMap(iterLocation->first.m_iLineIndex, 
			iterLocation->first.m_iPointIndex, &m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
		{
			// 增加对象到索引表
			AddAreaToMap(iterLocation->first.m_iLineIndex, iterLocation->first.m_iPointIndex, &m_oInstrumentUpdateArea);
			m_oInstrumentWholeTableMap.erase(iterLocation++);
		}
		else
		{
			pInstrument = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iterLocation->first.m_iLineIndex, 
				iterLocation->first.m_iPointIndex, &m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
			// SN不同则删除该仪器
			if (pInstrument != iterLocation->second)
			{
				iterLocation->second = pInstrument;
			}
			iterLocation++;
		}
	}
	LeaveCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	// 对照后客户端仪器索引表与服务端仪器SN索引表一致，将变化区域发送到客户端
	for (iterArea = m_oInstrumentUpdateArea.begin(); iterArea != m_oInstrumentUpdateArea.end(); iterArea++)
	{
		// 由区域号得到线号及点号范围
		GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, &iterArea->second);
		memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiLineNb, 4);
		iPos += 4;
		memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiAreaNb, 4);
		iPos += 4;
		for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
		{
			pInstrument = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iLineIndex, i, &m_oInstrumentWholeTableMap);
			if (pInstrument != NULL)
			{
				memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiSN, 4);
				iPos += 4;
				memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiIP, 4);
				iPos += 4;
			}
			else
			{
				memset(&m_pClientSndFrame->m_cProcBuf[iPos], 0, 8);
				iPos += 8;
			}
		}
	}
	if (iPos > 0)
	{
		m_pClientSndFrame->MakeSetFrame(CmdQueryUpdateTable, m_pClientSndFrame->m_cProcBuf, iPos);
	}
}


// 处理上电
void CClientRecThread::OnProcSetFieldOn(void)
{
	unsigned int uiFieldOnNeedTime = 0;;
	uiFieldOnNeedTime = m_pMatrixDllCall->Dll_Work();
	memcpy(m_pClientSndFrame->m_cProcBuf, &uiFieldOnNeedTime, 4);
	m_pClientSndFrame->MakeSetFrame(CmdFieldOnWaitTime, m_pClientSndFrame->m_cProcBuf, 4);
}


// 处理断电
void CClientRecThread::OnProcSetFieldOff(void)
{
	m_pMatrixDllCall->Dll_Stop();
	m_oInstrumentWholeTableMap.clear();
	m_oInstrumentUpdateArea.clear();
}


// 查询 SurveyXML 文件信息
void CClientRecThread::OnProcQuerySurveyXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySurverySetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 PointCode XML文件信息
void CClientRecThread::OnProcQueryPointCodeXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryPointCodeSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Sensor XML文件信息
void CClientRecThread::OnProcQuerySensorXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySensorSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Marker XML文件信息
void CClientRecThread::OnProcQueryMarkerXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMarkerSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Aux XML文件信息
void CClientRecThread::OnProcQueryAuxXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryAuxSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Detour XML文件信息
void CClientRecThread::OnProcQueryDetourXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryDetourSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Mute XML文件信息
void CClientRecThread::OnProcQueryMuteXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMuteSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 BlastMachine XML文件信息
void CClientRecThread::OnProcQueryBlastMachineXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryBlastMachineSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Absolute XML文件信息
// 炮点+排列个数+排列
void CClientRecThread::OnProcQueryAbsoluteXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryAbsoluteSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Generic XML文件信息
void CClientRecThread::OnProcQueryGenericXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryGenericSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 Look XML文件信息
void CClientRecThread::OnProcQueryLookXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryLookSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTestBase XML文件信息
void CClientRecThread::OnProcQueryInstrumentTestBaseXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestBaseSetupData(m_pClientSndFrame->m_cProcBuf, iPos, true);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTestBase XML文件信息
void CClientRecThread::OnProcQuerySensorTestBaseXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestBaseSetupData(m_pClientSndFrame->m_cProcBuf, iPos, false);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTestLimit XML文件信息
void CClientRecThread::OnProcQueryInstrumentTestLimitXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestLimitSetupData(m_pClientSndFrame->m_cProcBuf, iPos, true);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTestLimit XML文件信息
void CClientRecThread::OnProcQuerySensorTestLimitXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestLimitSetupData(m_pClientSndFrame->m_cProcBuf, iPos, false);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTest XML文件信息
void CClientRecThread::OnProcQueryInstrumentTestXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrumentTestSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTest XML文件信息
void CClientRecThread::OnProcQuerySensorTestXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySensorTestSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 MultipleTest XML文件信息
// 关键字结构体+排列个数+排列
void CClientRecThread::OnProcQueryMultipleTestXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMultipleTestSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 SeisMonitorTest XML文件信息
void CClientRecThread::OnProcQuerySeisMonitorTestXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySeisMonitorSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 LAULeakage XML文件信息
void CClientRecThread::OnProcQueryLAULeakageXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryLAULeakageSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 查询 FormLine XML文件信息
void CClientRecThread::OnProcQueryFormLineXMLInfo(unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryFormLineSetupData(m_pClientSndFrame->m_cProcBuf, iPos);
	m_pClientSndFrame->MakeSetFrame(usCmd, m_pClientSndFrame->m_cProcBuf, iPos);
}


// 处理仪器信息查询
void CClientRecThread::OnProcQueryByArea(char* pChar, unsigned int uiSize, 
	unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos))
{
	unsigned int uiPos = 0;
	m_oAreaStruct oAreaStruct;
	while(uiPos < uiSize)
	{
		memcpy(&oAreaStruct.m_uiLineNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oAreaStruct.m_uiAreaNb, &pChar[uiPos], 4);
		uiPos += 4;
		uiPos = QueryByArea(&oAreaStruct, uiPos, ptrFun);
	}
	m_pClientSndFrame->MakeSetFrame(CmdQueryInstrumentInfo, m_pClientSndFrame->m_cProcBuf, uiPos);
}


// 处理全部信息查询
void CClientRecThread::OnProcQueryInfoAll(unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos))
{
	int iLineNum = 0;
	int iColumn = 0;
	int iAreaNum = 0;
	int iPos = 0;
	iLineNum = m_uiLineNum * 2 - 1;
	iColumn = m_uiColumnNum * 2 - 1;
	iAreaNum = iColumn / InstrumentTableWindowSize;
	if ((iColumn % InstrumentTableWindowSize) != 0)
	{
		iAreaNum += 1;
	}
	if ((iAreaNum < 1) || (iLineNum < 1))
	{
		return;
	}
	char* pChar = new char[iLineNum * iAreaNum * 8];
	for (int i=1; i<= iLineNum; i++)
	{
		for (int j=1; j<=iAreaNum; j++)
		{
			memcpy(&pChar[iPos], &i, 4);
			iPos += 4;
			memcpy(&pChar[iPos], &j, 4);
			iPos += 4;
		}
	}
	OnProcQueryByArea(pChar, iPos, ptrFun);
	delete[] pChar;
	pChar = NULL;
}


// 按区域查询仪器信息
unsigned int CClientRecThread::QueryByArea(m_oAreaStruct* pArea, unsigned int uiStartPos, 
	unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos))
{
	unsigned int uiPos = uiStartPos;
	m_oInstrumentStruct* pInstrument = NULL;
	int iLineIndex = 0;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	// 由区域号得到线号及点号范围
	GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, pArea);
	EnterCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
	{
		pInstrument = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iLineIndex, i, 
			&m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
		uiPos = (this->*ptrFun)(pInstrument, uiPos);
	}
	LeaveCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	return uiPos;
}


// 按区域查询仪器信息
unsigned int CClientRecThread::QueryInstrumentInfoByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	if (pInstrument != NULL)
	{
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiSN, 4);
		uiPos += 4;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiIP, 4);
		uiPos += 4;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 8);
		uiPos += 8;
	}
	return uiPos;
}


// 查询所选区域仪器噪声测试数据和测试结果
unsigned int CClientRecThread::QueryInstrNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentNoiseLimit) || (fData < -m_fInstrumentNoiseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器失真测试数据和测试结果
unsigned int CClientRecThread::QueryInstrDistortionTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentDistortionLimit) || (fData < -m_fInstrumentDistortionLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器串扰测试数据和测试结果
unsigned int CClientRecThread::QueryInstrCrosstalkTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentCrosstalkLimit) || (fData < -m_fInstrumentCrosstalkLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器共模抑制比测试数据和测试结果
unsigned int CClientRecThread::QueryInstrCMRRTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentCMRRLimit) || (fData < -m_fInstrumentCMRRLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器增益相位测试数据和测试结果
unsigned int CClientRecThread::QueryInstrGainPhaseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentGainPhaseLimit) || (fData < -m_fInstrumentGainPhaseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器阻抗测试数据和测试结果
unsigned int CClientRecThread::QuerySensorResistanceTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorResistanceLimitMax) || (fData < m_fSensorResistanceLimitMin))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器漏电测试数据和测试结果
unsigned int CClientRecThread::QuerySensorLeakageTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorLeakageLimit) || (fData < -m_fSensorLeakageLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器噪声测试数据和测试结果
unsigned int CClientRecThread::QuerySensorNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorNoiseLimit) || (fData < -m_fSensorNoiseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度测试数据和测试结果
unsigned int CClientRecThread::QuerySensorTiltTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorTiltLimit) || (fData < -m_fSensorTiltLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度模式测试数据和测试结果
unsigned int CClientRecThread::QuerySensorTiltModelTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorTiltModelLimit) || (fData < -m_fSensorTiltModelLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域地震监测测试数据和测试结果
unsigned int CClientRecThread::QuerySeisMonitorTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSeisMonitorLimit) || (fData < -m_fSeisMonitorLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&m_pClientSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&m_pClientSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 从XML配置文件得到测试数据限制值
float CClientRecThread::GetTestDataLimitFromXML(bool bInstrument, string str)
{
	return m_pMatrixDllCall->Dll_QueryTestDataLimit(bInstrument, str);
}


// 计算测试数据的算术均方根
float CClientRecThread::CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument)
{
	return (float)m_pMatrixDllCall->Dll_CalMeanSquare(pInstrument);
}


// 处理查询接收区域命令
void CClientRecThread::OnProcQueryRevSection(void)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_GetLineRevSection(m_uiLineNum, m_uiColumnNum);
	memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &m_uiLineNum, 4);
	iPos += 4;
	memcpy(&m_pClientSndFrame->m_cProcBuf[iPos], &m_uiColumnNum, 4);
	iPos += 4;
	m_pClientSndFrame->MakeSetFrame(CmdQueryRevSection, m_pClientSndFrame->m_cProcBuf, iPos);
}

// 判断仪器位置索引号是否已加入索引表
BOOL CClientRecThread::IfLocationExistInMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	if (pMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(Location);
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

// 增加对象到索引表
void CClientRecThread::AddLocationToMap(int iLineIndex, int iPointIndex, m_oInstrumentStruct* pInstrument, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	if ((pInstrument == NULL) || (pMap == NULL))
	{
		return;
	}
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	if (false == IfLocationExistInMap(iLineIndex, iPointIndex, pMap))
	{
		pMap->insert(map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::value_type (Location, pInstrument));
	}
}

// 根据输入索引号，由索引表得到仪器指针
m_oInstrumentStruct* CClientRecThread::GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	if (pMap == NULL)
	{
		return NULL;
	}
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(Location);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}

/** 向所有在线客户端广播配置文件变更*/
void CClientRecThread::BroadCastXMLChange(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	for (iter = m_pComClientMap->begin(); iter != m_pComClientMap->end(); iter++)
	{
		if (iter->second->m_oClientRecThread.m_bCheckConnected == true)
		{
			iter->second->m_oClientSndFrame.MakeSetFrame(usCmd, pChar, uiSize);
		}
	}
}

/** 监视客户端是否活跃*/
void CClientRecThread::MonitorClientActive(bool bActive)
{
	bool bClose = false;
	if (bActive == true)
	{
		m_uiClientActiveCount = 0;
	}
	else
	{
		m_uiClientActiveCount++;
		if (m_uiClientActiveCount >= ClientActiveCountNum)
		{
			bClose = true;
		}
	}
	if (m_bCheckConnected == false)
	{
		m_uiClientCheckCount++;
		if (m_uiClientCheckCount >= ClientCheckCountNum)
		{
			bClose = true;
		}
	}
	if (bClose == true)
	{
		if (m_bClientClose == false)
		{
			PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, CloseClientMsg, (DWORD)m_pClientSndFrame->m_pClientSocket->m_pComClient, 0);
			m_bClientClose = true;
		}
	}
}