#include "StdAfx.h"
#include "ChannelSetupData.h"

CChannelSetupData::CChannelSetupData(void)
{
}

CChannelSetupData::~CChannelSetupData(void)
{
}

// 根据测线号、测点号、测道号，生成IP地址
unsigned int CChannelSetupData::CreateIP(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	// IP地址
	return  uiNbLine * 1000000 + uiNbPoint * 10 + uiNbChannel;
}

/**
* 得到测试结果字符串
* @param unsigned int uiType 测试类型
* @return CString 测试结果字符串
*/
CString CChannelSetupData::GetTestResultString(unsigned int uiType)
{
	CString strTestResult = "";
	switch(uiType)
	{
	case 15:	// 检波器电阻。 Resistance
		strTestResult = GetTestResultString(m_byResistanceCheck, m_fResistanceValue);
		break;
	case 14:	// 检波器漏电。 Leakage
		strTestResult = GetTestResultString(m_byLeakageCheck, m_fLeakageValue);
		break;
	case 1:	// 检波器噪声。 Noise
		strTestResult = GetTestResultString(m_byNoiseCheck, m_fNoiseValue);
		break;
	case 20:	// 检波器倾斜度。 Tilt
		strTestResult = GetTestResultString(m_byTiltCheck, m_fTiltValue);	
		break;
	case 21:	// 检波器倾斜度模型。Tilt Model
		strTestResult = GetTestResultString(m_byTiltModelCheck, m_fTiltModelValue);
		break;
	case 22:	// 检波器倾斜度模型。Tilt Model
		strTestResult = GetTestResultString(m_byNoiseMonitorCheck, m_fNoiseMonitorValue);
		break;
	case 40:	// 所有检波器测试
		strTestResult = "";
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
CString CChannelSetupData::GetTestResultString(byte byCheck, float m_fValue)
{
	CString strTestResult = "";
	switch(byCheck)
	{
	case 0:	// 0：未测试
		strTestResult = "Not test";
		break;
	case 1:	// 1：测试合格
	case 2:	// 2：测试不合格
		strTestResult.Format("%f", m_fValue);
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
* @param float fSeismonitorLimit 噪声监测判据
* @return byte 测试结果
*/
byte CChannelSetupData::GetTestResultCheck(unsigned int uiType, float fSeismonitorLimit)
{
	byte byCheck = 0;
	switch(uiType)
	{
	case 15:	// 检波器电阻。 Resistance
		byCheck = m_byResistanceCheck;
		break;
	case 14:	// 检波器漏电。 Leakage
		byCheck = m_byLeakageCheck;
		break;
	case 1:	// 检波器噪声。 Noise
		byCheck = m_byNoiseCheck;
		break;
	case 20:	// 检波器倾斜度。 Tilt
		byCheck = m_byTiltCheck;
		break;
	case 21:	// 检波器倾斜度模型。Tilt Model
		byCheck = m_byTiltModelCheck;
		break;
	case 22:	// 检波器噪声监测
		// 判断：4：未判读
		if(4 == m_byNoiseMonitorCheck)
		{
			m_byNoiseMonitorCheck = 2;	// 2：测试不合格
			if(m_fNoiseMonitorValue < fSeismonitorLimit)
			{
				m_byNoiseMonitorCheck = 1;	// 1：测试合格
			}
		}
		byCheck = m_byNoiseMonitorCheck;
		break;
	case 40:	// 所有检波器测试
		byCheck = m_byCheck;
		break;
	}
	return byCheck;
}