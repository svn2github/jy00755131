#include "StdAfx.h"
#include "Instrument.h"

CInstrument::CInstrument(void)
{
}

CInstrument::~CInstrument(void)
{
}

/**
* 得到测试结果字符串
* @param unsigned int uiType 测试类型
* @param unsigned int iSubItem 测试项
* @param CString strUnit 单位
* @return CString 测试结果字符串
*/
CString CInstrument::GetTestResultString(unsigned int uiType, int iSubItem, CString strUnit)
{
	CString strTestResult = "";
	switch(uiType)
	{
	case 4:	// 仪器噪声。 Noise
		strTestResult = GetTestResultString(m_byNoiseCheck, m_fNoiseValue, strUnit);
		break;
	case 5:	// 仪器畸变。 Distortion
		strTestResult = GetTestResultString(m_byDistortionCheck, m_fDistortionValue, strUnit);
		break;
	case 7:	// 仪器串音。 Crosstalk
		strTestResult = GetTestResultString(m_byCrosstalkCheck, m_fCrosstalkValue, strUnit);
		break;
	case 6:	// 仪器增益/ 相位。 Gain/Phase
		if(1 == iSubItem)	// 仪器增益
		{
			strTestResult = GetTestResultString(m_byGainCheck, m_fGainValue, strUnit);
		}
		else if(2 == iSubItem)	// 仪器相位
		{
			strTestResult = GetTestResultString(m_byPhaseCheck, m_fPhaseValue, strUnit);
		}		
		break;
	case 8:	// 仪器共模。CMRR
		strTestResult = GetTestResultString(m_byCMRRCheck, m_fCMRRValue, strUnit);
		break;
	case 30:	// 所有仪器测试
		strTestResult = GetTestResultString(m_byCheck);
		break;
	}
	return strTestResult;
}

/**
* 得到测试结果字符串
* @param unsigned byte byCheck 测试结果
* @param unsigned float m_fValue 测试值
* @param CString strUnit 单位
* @return CString 测试结果字符串
*/
CString CInstrument::GetTestResultString(byte byCheck, float m_fValue, CString strUnit)
{
	CString strTestResult = "";
	switch(byCheck)
	{
	case 0:	// 0：未测试
		strTestResult = "No test";
		break;
	case 1:	// 1：测试合格
	case 2:	// 2：测试不合格
		strTestResult.Format("%3.2f%s", m_fValue, strUnit);
		break;
	case 3:	// 3：无信号
		strTestResult = "No signal";
		break;
	case 4:	// 4：未判读
		strTestResult = "No Judge";
		break;
	}
	return strTestResult;
}

/**
* 得到测试结果字符串
* @param unsigned int uiType 测试类型
* @param unsigned int iSubItem 测试项
* @return CString 测试结果字符串
*/
CString CInstrument::GetTestResultString(unsigned int uiType, int iSubItem)
{
	CString strTestResult = "";
	switch(uiType)
	{
	case 4:	// 仪器噪声。 Noise
		strTestResult = GetTestResultString(m_byNoiseCheck, m_fNoiseValue);
		break;
	case 5:	// 仪器畸变。 Distortion
		strTestResult = GetTestResultString(m_byDistortionCheck, m_fDistortionValue);
		break;
	case 7:	// 仪器串音。 Crosstalk
		strTestResult = GetTestResultString(m_byCrosstalkCheck, m_fCrosstalkValue);
		break;
	case 6:	// 仪器增益/ 相位。 Gain/Phase
		if(1 == iSubItem)	// 仪器增益
		{
			strTestResult = GetTestResultString(m_byGainCheck, m_fGainValue);
		}
		else if(2 == iSubItem)	// 仪器相位
		{
			strTestResult = GetTestResultString(m_byPhaseCheck, m_fPhaseValue);
		}		
		break;
	case 8:	// 仪器共模。CMRR
		strTestResult = GetTestResultString(m_byCMRRCheck, m_fCMRRValue);
		break;
	case 30:	// 所有仪器测试
		strTestResult = GetTestResultString(m_byCheck);
		break;
	}
	return strTestResult;
}

/**
* 得到测试结果字符串
* @param unsigned byte byCheck 测试结果
* @param unsigned float m_fValue 测试值
* @return CString 测试结果字符串
*/
CString CInstrument::GetTestResultString(byte byCheck, float m_fValue)
{
	CString strTestResult = "";
	switch(byCheck)
	{
	case 0:	// 0：未测试
		strTestResult = "No test";
		break;
	case 1:	// 1：测试合格
	case 2:	// 2：测试不合格
		strTestResult.Format("%3.2f", m_fValue);
		break;
	case 3:	// 3：无信号
		strTestResult = "No signal";
		break;
	case 4:	// 4：未判读
		strTestResult = "No Judge";
		break;
	}
	return strTestResult;
}

/**
* 得到测试结果字符串
* @param unsigned byte byCheck 测试结果
* @return CString 测试结果字符串
*/
CString CInstrument::GetTestResultString(byte byCheck)
{
	CString strTestResult = "";
	switch(byCheck)
	{
	case 0:	// 0：未测试
		strTestResult = "No test";
		break;
	case 1:	// 1：测试合格
		strTestResult = "OK";
		break;
	case 2:	// 2：测试不合格
		strTestResult = "Fail";
		break;
	case 3:	// 3：无信号
		strTestResult = "No signal";
		break;
	case 4:	// 4：未判读
		strTestResult = "No Judge";
		break;
	}
	return strTestResult;
}

/**
* 得到测试结果字符串
* @param unsigned int uiType 测试类型
* @return byte 测试结果
*/
byte CInstrument::GetTestResultCheck(unsigned int uiType)
{
	byte byCheck = 0;
	switch(uiType)
	{
	case 4:	// 仪器噪声。 Noise
		byCheck = m_byNoiseCheck;
		break;
	case 5:	// 仪器畸变。 Distortion
		byCheck = m_byDistortionCheck;
		break;
	case 7:	// 仪器串音。 Crosstalk
		byCheck = m_byCrosstalkCheck;
		break;
	case 6:	// 仪器增益/ 相位。 Gain/Phase
		byCheck = m_byGainAndPhaseCheck;
		break;
	case 8:	// 仪器共模。CMRR
		byCheck = m_byCMRRCheck;
		break;
	case 30:	// 所有仪器测试
		byCheck = m_byCheck;
		break;
	}
	return byCheck;
}