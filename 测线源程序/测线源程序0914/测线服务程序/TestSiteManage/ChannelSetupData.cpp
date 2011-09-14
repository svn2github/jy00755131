#include "StdAfx.h"
#include "ChannelSetupData.h"

CChannelSetupData::CChannelSetupData()
{
}

CChannelSetupData::~CChannelSetupData()
{
}

/**
* 重置
* @param void
* @return void
*/
void CChannelSetupData::OnReset()
{
	// 仪器设备号
	m_uiSN = 0;

	// 野外电阻测试值
	m_fResistanceValue = 0.0;
	// 野外电阻测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byResistanceCheck = 0;
	// 野外电阻测试判据，下限
	m_fResistanceLimitMin = 0.0;
	// 野外电阻测试判据，上限
	m_fResistanceLimitMax = 0.0;

	// 野外漏电测试值
	m_fLeakageValue = 0.0;
	// 野外漏电测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byLeakageCheck = 0;
	// 野外漏电测试判据
	m_fLeakageLimit = 0.0;

	// 野外噪声测试值
	m_fNoiseValue = 0.0;
	// 野外噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byNoiseCheck = 0;
	// 野外噪声测试判据
	m_fNoiseLimit = 0.0;

	// 野外倾斜度测试值
	m_fTiltValue = 0.0;
	// 野外倾斜度测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byTiltCheck = 0;
	// 野外倾斜度测试判据
	m_fTiltLimit = 0.0;

	// 野外倾斜度模型测试值
	m_fTiltModelValue = 0.0;
	// 野外倾斜度模型测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byTiltModelCheck = 0;
	// 野外倾斜度模型测试判据
	m_fTiltModelLimit = 0.0;

	// 测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byCheck = 0;

	// 噪声监测测试值
	m_fNoiseMonitorValue = 0.0;
	// 噪声监测测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byNoiseMonitorCheck = 0;

	// 检波器是否进行了初始测试
	m_byTestOnField = 0;
	// 初始测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byOnFieldCheck = 0;
}

/**
* 重新加载
* @param void
* @return void
*/
void CChannelSetupData::OnReload()
{
	// 测道是否使用中
	m_bInUsed = false;

	// 测线号
	m_uiNbLine = 0;
	// 测点号
	m_uiNbPoint = 0;
	// 测点索引号
	m_uiIndexPoint = 0;
	// 测道号
	m_uiNbChannel = 0;
	// IP地址
	m_uiIP = 0;
	// 检波器号
	m_uiSensorNb = 0;
	// SegdCode
	m_uiSegdCode = 0;

	// 是否跳过道
	m_bJumped = false;
	// 是否哑点
	m_bMute = false;

	// 仪器设备号
	m_uiSN = 0;

	// 左侧链接的测道
	m_pChannelLeft = NULL;
	// 左侧链接的测道
	m_pChannelRight = NULL;
	// 上面链接的测道
	m_pChannelTop = NULL;
	// 下面链接的测道
	m_pChannelDown = NULL;

	// 野外电阻测试值
	m_fResistanceValue = 0.0;
	// 野外电阻测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byResistanceCheck = 0;
	// 野外电阻测试判据，下限
	m_fResistanceLimitMin = 0.0;
	// 野外电阻测试判据，上限
	m_fResistanceLimitMax = 0.0;

	// 野外漏电测试值
	m_fLeakageValue = 0.0;
	// 野外漏电测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byLeakageCheck = 0;
	// 野外漏电测试判据
	m_fLeakageLimit = 0.0;

	// 野外噪声测试值
	m_fNoiseValue = 0.0;
	// 野外噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byNoiseCheck = 0;
	// 野外噪声测试判据
	m_fNoiseLimit = 0.0;

	// 野外倾斜度测试值
	m_fTiltValue = 0.0;
	// 野外倾斜度测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byTiltCheck = 0;
	// 野外倾斜度测试判据
	m_fTiltLimit = 0.0;

	// 野外倾斜度模型测试值
	m_fTiltModelValue = 0.0;
	// 野外倾斜度模型测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byTiltModelCheck = 0;
	// 野外倾斜度模型测试判据
	m_fTiltModelLimit = 0.0;

	// 测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byCheck = 0;

	// 噪声监测测试值
	m_fNoiseMonitorValue = 0.0;
	// 噪声监测测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byNoiseMonitorCheck = 0;

	// 检波器是否进行了初始测试
	m_byTestOnField = 0;
	// 初始测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byOnFieldCheck = 0;
}

/**
* 根据测线号、测点号、测道号，生成IP地址
* @param void
* @return void
*/
void CChannelSetupData::CreateIP()
{
	// IP地址
	m_uiIP = CreateIP(m_uiNbLine, m_uiNbPoint, m_uiNbChannel);
}

/**
* 根据测线号、测点号、测道号，生成IP地址
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @param unsigned int uiNbChannel 测道号
* @return unsigned int IP地址
*/
unsigned int CChannelSetupData::CreateIP(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	// IP地址
	return  uiNbLine * 1000000 + uiNbPoint * 10 + uiNbChannel;
}

/**
* 检查测试结果，检波器
* @param void
* @return void
*/
void CChannelSetupData::CheckResultAll()
{
	m_byCheck = 0;
	// 判断：所有仪器测试项目合格
	if((1 == m_byResistanceCheck) && (1 == m_byLeakageCheck) && (1 == m_byNoiseCheck) && (1 == m_byTiltCheck) && (1 == m_byTiltModelCheck))
	{
		m_byCheck = 1;
	}
	// 判断：至少有一项仪器测试项目不合格
	else if((2 == m_byResistanceCheck) || (2 == m_byLeakageCheck) || (2 == m_byNoiseCheck) || (2 == m_byTiltCheck) || (2 == m_byTiltModelCheck))
	{
		m_byCheck = 2;
	}
	// 判断：至少有一项仪器测试项目无信号
	else if((3 == m_byResistanceCheck) || (3 == m_byLeakageCheck) || (3 == m_byNoiseCheck) || (3 == m_byTiltCheck) || (3 == m_byTiltModelCheck))
	{
		m_byCheck = 3;
	}
}

/**
* 检查初始测试结果
* @param UINT uiTestResistance 新仪器是否测试Resistance
* @param UINT uiTestTilt 新仪器是否测试Tilt
* @param UINT uiTestLeakage 新仪器是否测试Leakage
* @return void
*/
void CChannelSetupData::CheckResultOnField(UINT uiTestResistance, UINT uiTestTilt, UINT uiTestLeakage)
{
	m_byOnFieldCheck = 0;
	// 判断：仪器测试项目合格
	if(((0 == uiTestResistance) || ((1 == uiTestResistance) && (1 == m_byResistanceCheck)))
		&& ((0 == uiTestTilt) || ((1 == uiTestTilt) && (1 == m_byTiltCheck)))
		&& ((0 == uiTestLeakage) || ((1 == uiTestLeakage) && (1 == m_byLeakageCheck))))
	{
		m_byOnFieldCheck = 1;
	}
	// 判断：至少有一项仪器测试项目不合格
	else if(((1 == uiTestResistance) && (2 == m_byResistanceCheck))
		|| ((1 == uiTestTilt) && (2 == m_byTiltCheck))
		|| ((1 == uiTestLeakage) && (2 == m_byLeakageCheck)))
	{
		m_byOnFieldCheck = 2;
	}
	// 判断：至少有一项仪器测试项目无信号
	else if(((1 == uiTestResistance) && (3 == m_byResistanceCheck))
		|| ((1 == uiTestTilt) && (3 == m_byTiltCheck))
		|| ((1 == uiTestLeakage) && (3 == m_byLeakageCheck)))
	{
		m_byOnFieldCheck = 3;
	}
}