#include "StdAfx.h"
#include "CommServerDll.h"


CCommServerDll::CCommServerDll(void)
	: m_uiLineNum(0)
	, m_uiColumnNum(0)
	, m_fInstrumentNoiseLimit(0)
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
	m_pMatrixServer = NULL;
	m_pLineSetupData = NULL;
	m_pOptSetupData = NULL;
}


CCommServerDll::~CCommServerDll(void)
{
}

// 处理上电
void CCommServerDll::OnProcSetFieldOn(CCommRecThread* pRecThread)
{
	unsigned int uiFieldOnNeedTime = 0;;
	uiFieldOnNeedTime = m_pMatrixServer->Dll_Work();
	memcpy(pRecThread->m_pCommSndFrame->m_cProcBuf, &uiFieldOnNeedTime, 4);
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdFieldOnWaitTime, 
		pRecThread->m_pCommSndFrame->m_cProcBuf, 4);
}
// 处理断电
void CCommServerDll::OnProcSetFieldOff(CCommRecThread* pRecThread)
{
	m_pMatrixServer->Dll_Stop();
	pRecThread->m_oInstrumentWholeTableMap.clear();
}
// 处理查询接收区域命令
void CCommServerDll::OnProcQueryRevSection(CCommRecThread* pRecThread)
{
	int iPos = 0;
	m_pMatrixServer->Dll_GetLineRevSection(m_uiLineNum, m_uiColumnNum, m_pLineSetupData);
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiLineNum, 4);
	iPos += 4;
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiColumnNum, 4);
	iPos += 4;
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryRevSection, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 从XML配置文件得到测试数据限制值
float CCommServerDll::QueryTestDataLimitFromXML(bool bInstrument, string str)
{
	return m_pMatrixServer->Dll_QueryTestDataLimit(bInstrument, str, m_pLineSetupData);
}

// 查询 SurveyXML 文件信息
void CCommServerDll::OnProcQuerySurveyXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QuerySurverySetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 PointCode XML文件信息
void CCommServerDll::OnProcQueryPointCodeXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryPointCodeSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Sensor XML文件信息
void CCommServerDll::OnProcQuerySensorXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QuerySensorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Marker XML文件信息
void CCommServerDll::OnProcQueryMarkerXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryMarkerSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Aux XML文件信息
void CCommServerDll::OnProcQueryAuxXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryAuxSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Detour XML文件信息
void CCommServerDll::OnProcQueryDetourXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryDetourSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Mute XML文件信息
void CCommServerDll::OnProcQueryMuteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryMuteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 BlastMachine XML文件信息
void CCommServerDll::OnProcQueryBlastMachineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryBlastMachineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Absolute XML文件信息
// 炮点+排列个数+排列
void CCommServerDll::OnProcQueryAbsoluteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryAbsoluteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Generic XML文件信息
void CCommServerDll::OnProcQueryGenericXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryGenericSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Look XML文件信息
void CCommServerDll::OnProcQueryLookXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryLookSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTestBase XML文件信息
void CCommServerDll::OnProcQueryInstrumentTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTestBase XML文件信息
void CCommServerDll::OnProcQuerySensorTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTestLimit XML文件信息
void CCommServerDll::OnProcQueryInstrumentTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTestLimit XML文件信息
void CCommServerDll::OnProcQuerySensorTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTest XML文件信息
void CCommServerDll::OnProcQueryInstrumentTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryInstrumentTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTest XML文件信息
void CCommServerDll::OnProcQuerySensorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QuerySensorTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 MultipleTest XML文件信息
// 关键字结构体+排列个数+排列
void CCommServerDll::OnProcQueryMultipleTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryMultipleTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SeisMonitorTest XML文件信息
void CCommServerDll::OnProcQuerySeisMonitorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QuerySeisMonitorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 LAULeakage XML文件信息
void CCommServerDll::OnProcQueryLAULeakageXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryLAULeakageSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 FormLine XML文件信息
void CCommServerDll::OnProcQueryFormLineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryFormLineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询所选区域仪器噪声测试数据和测试结果
unsigned int CCommServerDll::QueryInstrNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器失真测试数据和测试结果
unsigned int CCommServerDll::QueryInstrDistortionTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器串扰测试数据和测试结果
unsigned int CCommServerDll::QueryInstrCrosstalkTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器共模抑制比测试数据和测试结果
unsigned int CCommServerDll::QueryInstrCMRRTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器增益相位测试数据和测试结果
unsigned int CCommServerDll::QueryInstrGainPhaseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器阻抗测试数据和测试结果
unsigned int CCommServerDll::QuerySensorResistanceTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器漏电测试数据和测试结果
unsigned int CCommServerDll::QuerySensorLeakageTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器噪声测试数据和测试结果
unsigned int CCommServerDll::QuerySensorNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度测试数据和测试结果
unsigned int CCommServerDll::QuerySensorTiltTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度模式测试数据和测试结果
unsigned int CCommServerDll::QuerySensorTiltModelTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域地震监测测试数据和测试结果
unsigned int CCommServerDll::QuerySeisMonitorTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
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
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 计算测试数据的算术均方根
float CCommServerDll::CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument)
{
	return (float)m_pMatrixServer->Dll_CalMeanSquare(pInstrument);
}

// 由线号和点号得到区域位置
void CCommServerDll::GetAreaFromPoint(int iLineIndex, int iPointIndex, m_oAreaStruct* pAreaStruct)
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
void CCommServerDll::GetPointRangeFromArea(int* iLineIndex, int* iPointMinIndex, 
	int* iPointMaxIndex, m_oAreaStruct* pAreaStruct)
{
	*iLineIndex = (int)pAreaStruct->m_uiLineNb - m_uiLineNum;
	*iPointMinIndex = ((int)pAreaStruct->m_uiAreaNb - 1) * InstrumentTableWindowSize + 1 - m_uiColumnNum;
	*iPointMaxIndex = ((int)pAreaStruct->m_uiAreaNb) * InstrumentTableWindowSize - m_uiColumnNum;
}
// 处理仪器设备表更新
void CCommServerDll::OnProcInstrumentTableUpdate(CCommRecThread* pRecThread)
{
	map<m_oLocationStruct, unsigned int>::iterator iterLocation;
	map<m_oAreaStruct, m_oAreaStruct>::iterator iterArea;
	/** 客户端设备位置索引表*/
	map<m_oLocationStruct, unsigned int> InstrumentWholeTableMap;
	/** 客户端设备更新区域索引表*/
	map<m_oAreaStruct, m_oAreaStruct> InstrumentUpdateArea;
	int iLineIndex = 0;
	int iPointIndex = 0;
	unsigned int uiSN = 0;
	unsigned int* uipSNClient = NULL;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	int iPos = 0;
	int iSize = 0;
	m_oAreaStruct oAreaStruct;
	m_oInstrumentStruct* pInstrument = NULL;
	InstrumentWholeTableMap.clear();
	InstrumentUpdateArea.clear();
	// 将SN索引表与客户端的仪器位置索引表相对照
	m_pMatrixServer->Dll_QueryInstrumentLocation(pRecThread->m_pCommSndFrame->m_cProcBuf, iSize);
	for (int i=0; i<iSize;)
	{
		memcpy(&iLineIndex, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&iPointIndex, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&uiSN, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		// 客户端仪器索引表中找不到该设备
		if (FALSE == pRecThread->IfLocationExistInMap(iLineIndex, iPointIndex))
		{
			pRecThread->AddLocationToMap(iLineIndex, iPointIndex, uiSN);
			GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
			// 增加对象到索引表
			AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
		}
		else
		{
			uipSNClient = pRecThread->GetSnPtrFromLocationMap(iLineIndex, iPointIndex);
			if (uiSN != *uipSNClient)
			{
				*uipSNClient = uiSN;
				GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
				// 增加对象到索引表
				AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
			}
		}
	}
	InstrumentWholeTableMap.swap(pRecThread->m_oInstrumentWholeTableMap);
	EnterCriticalSection(&m_pMatrixServer->m_pEnv->m_pLineList->m_oSecLineList);
	// 将客户端的仪器位置索引表与SN索引表相对照
	for (iterLocation = InstrumentWholeTableMap.begin(); iterLocation != InstrumentWholeTableMap.end();)
	{
		// 仪器位置索引表找不到该设备
		if (FALSE == m_pMatrixServer->Dll_IfLocationExistInMap(iterLocation->first.m_iLineIndex, 
			iterLocation->first.m_iPointIndex, &m_pMatrixServer->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
		{
			// 增加对象到索引表
			GetAreaFromPoint(iterLocation->first.m_iLineIndex, iterLocation->first.m_iPointIndex, &oAreaStruct);
			AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
			InstrumentWholeTableMap.erase(iterLocation++);
		}
		else
		{
			uiSN = m_pMatrixServer->Dll_GetInstrumentFromLocationMap(iterLocation->first.m_iLineIndex, 
				iterLocation->first.m_iPointIndex, &m_pMatrixServer->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap)->m_uiSN;
			// SN不同则更新
			if (uiSN != iterLocation->second)
			{
				iterLocation->second = uiSN;
			}
			iterLocation++;
		}
	}
	LeaveCriticalSection(&m_pMatrixServer->m_pEnv->m_pLineList->m_oSecLineList);
	pRecThread->m_oInstrumentWholeTableMap.swap(InstrumentWholeTableMap);

	// 对照后客户端仪器索引表与服务端仪器SN索引表一致，将变化区域发送到客户端
	for (iterArea = InstrumentUpdateArea.begin(); iterArea != InstrumentUpdateArea.end(); iterArea++)
	{
		// 由区域号得到线号及点号范围
		GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, &iterArea->second);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiLineNb, 4);
		iPos += 4;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiAreaNb, 4);
		iPos += 4;
		for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
		{
			pInstrument = m_pMatrixServer->Dll_GetInstrumentFromLocationMap(iLineIndex, i, 
				&m_pMatrixServer->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
			if (pInstrument != NULL)
			{
				memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiSN, 4);
				iPos += 4;
				memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiIP, 4);
				iPos += 4;
			}
			else
			{
				memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], 0, 8);
				iPos += 8;
			}
		}
	}
	if (iPos > 0)
	{
		pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryUpdateTable, 
			pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	}
}
// 判断仪器更新区域是否已加入索引表
BOOL CCommServerDll::IfAreaExistInMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct)
{
	BOOL bResult = FALSE;
	map<m_oAreaStruct, m_oAreaStruct>::iterator iter;
	iter = pmap->find(oAreaStruct);
	if (iter != pmap->end())
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
void CCommServerDll::AddAreaToMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct)
{
	if (FALSE == IfAreaExistInMap(pmap, oAreaStruct))
	{
		pmap->insert(map<m_oAreaStruct, m_oAreaStruct>::value_type (oAreaStruct, oAreaStruct));
	}
}

// 按区域查询仪器信息
unsigned int CCommServerDll::QueryByArea(CCommRecThread* pRecThread, m_oAreaStruct* pArea, 
	unsigned int uiStartPos, unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
	unsigned int uiStartPos, CCommRecThread* pRecThread))
{
	unsigned int uiPos = uiStartPos;
	m_oInstrumentStruct* pInstrument = NULL;
	int iLineIndex = 0;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	// 由区域号得到线号及点号范围
	GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, pArea);
	EnterCriticalSection(&m_pMatrixServer->m_pEnv->m_pLineList->m_oSecLineList);
	for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
	{
		pInstrument = m_pMatrixServer->Dll_GetInstrumentFromLocationMap(iLineIndex, i, 
			&m_pMatrixServer->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
		uiPos = (this->*ptrFun)(pInstrument, uiPos, pRecThread);
	}
	LeaveCriticalSection(&m_pMatrixServer->m_pEnv->m_pLineList->m_oSecLineList);
	return uiPos;
}

// 处理仪器信息查询
void CCommServerDll::OnProcQueryByArea(CCommRecThread* pRecThread, char* pChar, unsigned int uiSize, 
	unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread))
{
	unsigned int uiPos = 0;
	m_oAreaStruct oAreaStruct;
	while(uiPos < uiSize)
	{
		memcpy(&oAreaStruct.m_uiLineNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oAreaStruct.m_uiAreaNb, &pChar[uiPos], 4);
		uiPos += 4;
		uiPos = QueryByArea(pRecThread, &oAreaStruct, uiPos, ptrFun);
	}
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryInstrumentInfo, 
		pRecThread->m_pCommSndFrame->m_cProcBuf, uiPos);
}

// 处理全部信息查询
void CCommServerDll::OnProcQueryInfoAll(CCommRecThread* pRecThread, 
	unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread))
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
	OnProcQueryByArea(pRecThread, pChar, iPos, ptrFun);
	delete[] pChar;
	pChar = NULL;
}

// 按区域查询仪器信息
unsigned int CCommServerDll::QueryInstrumentInfoByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	if (pInstrument != NULL)
	{
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiSN, 4);
		uiPos += 4;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiIP, 4);
		uiPos += 4;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 8);
		uiPos += 8;
	}
	return uiPos;
}
// 查询 OperationDelay XML文件信息
void CCommServerDll::OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryDelaySetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 炮表 XML文件信息
void CCommServerDll::OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QuerySourceShotSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 Explo震源类型 XML文件信息
void CCommServerDll::OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryExploSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 Vibro震源类型 XML文件信息
void CCommServerDll::OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryVibroSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessRecord XML文件信息
void CCommServerDll::OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryProcessRecordSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessAux XML文件信息
void CCommServerDll::OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryProcessAuxSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessAcq XML文件信息
void CCommServerDll::OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryProcessAcqSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessType XML文件信息
void CCommServerDll::OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryProcessTypeSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 注释 XML文件信息
void CCommServerDll::OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixServer->Dll_QueryCommentsSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}