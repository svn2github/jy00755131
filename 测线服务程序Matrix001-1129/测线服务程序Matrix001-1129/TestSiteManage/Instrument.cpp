#include "StdAfx.h"
#include "Instrument.h"
#include "Matrixline.h"

CInstrument::CInstrument()
{
}

CInstrument::~CInstrument()
{
}

/**
* 重置
* @param void
* @return void
*/
void CInstrument::OnReset()
{
	// 仪器是否使用中
	m_bInUsed = false;

	// 仪器设备号
	m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_uiInstrumentType = 0;

	// 仪器IP地址
	m_uiIP = 0;
	// 仪器路由IP地址
	m_uiRoutIP = 0;
	// 路由方向 1-上；2-下；3-左；4右
	m_uiRoutDirection = 0;
	// 路由地址 交叉线方向 上方
	m_uiRoutIPTop = 0;
	// 路由地址 交叉线方向 下方
	m_uiRoutIPDown = 0;
	// 路由地址 测线线方向 左方
	m_uiRoutIPLeft = 0;
	// 路由地址 测线线方向 右方
	m_uiRoutIPRight = 0;

	// 链接的下一个主交叉站
	m_pMainCrossNext = NULL;
	// 链接的上一个主交叉站
	m_pMainCrossPreview = NULL;
	// 链接的交叉站 上方
	m_pCrossTop = NULL;
	// 链接的交叉站 下方
	m_pCrossDown = NULL;
	// 链接的仪器 左方
	m_pInstrumentLeft = NULL;
	// 链接的仪器 右方
	m_pInstrumentRight = NULL;

	// 链接的下一个主交叉站
	m_uiSNMainCrossNext = 0;
	// 链接的上一个主交叉站
	m_uiSNMainCrossPreview = 0;
	// 链接的交叉站 上方
	m_uiSNCrossTop = 0;
	// 链接的交叉站 下方
	m_uiSNCrossDown = 0;
	// 链接的仪器 左方
	m_uiSNInstrumentLeft = 0;
	// 链接的仪器 右方
	m_uiSNInstrumentRight = 0;

	// 链接的下一个主交叉站
	m_iIndexMainCrossNext = -1;
	// 链接的上一个主交叉站
	m_iIndexMainCrossPreview = -1;
	// 链接的交叉站 上方
	m_iIndexCrossTop = -1;
	// 链接的交叉站 下方
	m_iIndexCrossDown = -1;
	// 链接的仪器 左方
	m_iIndexInstrumentLeft = -1;
	// 链接的仪器 右方
	m_iIndexInstrumentRight = -1;

	// 首包时刻
	m_uiTimeHeadFrame = 0;
	// 尾包计数
	m_uiTailFrameCount = 0;
	// 仪器时延
	m_uiTimeDelay = 0;

	m_usReceiveTime = 0;	// 16bits时间，接收时刻低位
	m_usSendTime = 0;	// 16bits时间，发送时刻低位

	m_usLineLeftReceiveTime = 0;	// 16bits 测线方向左面尾包接收时刻
	m_usLineRightReceiveTime = 0;	// 16bits 测线方向右面尾包接收时刻

	m_usCrossTopReceiveTime = 0;	// 16bits 交叉线方向上面尾包接收时刻
	m_usCrossDownReceiveTime = 0;	// 16bits 交叉线方下面尾包接收时刻

	// 测线
	m_uiLineNb = 0;
	// 测点
	m_uiPointNb = 0;
	// 测道
	m_uiChannelNb = 0;

	// 是否跳过道
	m_bJumpedChannel = false;
	// 是否连接检波器
	m_bSensor = false;

	/** 仪器IP地址分配时间*/
	m_uiIPDistributeTime = 0;	

	// 是否辅助道
	m_bAux = false;
	// 辅助道标记时间
	m_uiAuxTime = 0;

	// 是否连接爆炸机
	m_bBlastMachine = false;
	// 爆炸机标记时间
	m_uiBlastMachineTime = 0;

	// 是否迂回道
	m_bDetour = false;
	// 是否迂回道低端标记点
	m_bDetourMarkerLow = false;
	// 是否迂回道高端标记点
	m_bDetourMarkerHigh = false;
	// 是否迂回道停止标记
	m_uiStopMarking = false;
	// 迂回道标记时间
	m_uiDetourLogoTime = 0;
	// 迂回道标记时间
	m_uiDetourTime = 0;

	// 是否标记点
	m_bMarker = false;
	// 标记点标记时间
	m_uiMarkerLogoTime = 0;
	// 标记点标记时间
	m_uiMarkerTime = 0;

	// 第几次设置IP地址
	m_iIPSetCount = 0;
	// IP地址设置是否成功
	m_bIPSetOK = false;

	// 第几次尾包时刻查询
	m_iTailTimeQueryCount = 0;
	// 尾包时刻查询是否成功
	m_bTailTimeQueryOK = false;

	// 本地时间修正高位
	m_uiTimeHigh = 0;
	// 本地时间修正低位
	m_uiTimeLow = 0;
	// 第几次设置仪器时间
	m_iTimeSetCount = 0;
	// 仪器时间设置是否成功
	m_bTimeSetOK = false;

	// 仪器被删除时的删除方向 1-上；2-下；3-左；4右
	m_uiDeleteDirection = 0;

	// 仪器噪声测试值
	m_fNoiseValue = 0.0;
	// 仪器噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byNoiseCheck = 0;

	// 仪器畸变测试值
	m_fDistortionValue = 0.0;
	// 仪器畸变测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byDistortionCheck = 0;

	// 仪器串音测试值
	m_fCrosstalkValue = 0.0;
	// 仪器串音测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byCrosstalkCheck = 0;

	// 仪器增益测试值
	m_fGainValue = 0.0;
	// 仪器增益测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byGainCheck = 0;
	// 仪器相位测试值
	m_fPhaseValue = 0.0;
	// 仪器相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byPhaseCheck = 0;
	// 仪器增益和相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byGainAndPhaseCheck = 0;

	// 仪器共模测试值
	m_fCMRRValue = 0.0;
	// 仪器共模测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byCMRRCheck = 0;

	/** 仪器测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	m_byCheck = 0;
}

/**
* 根据链接方向，得到连接的下一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
*/
CInstrument* CInstrument::GetNextInstrument(unsigned int uiRoutDirection)
{
	CInstrument* pInstrumentNext = NULL;

	// 判断方向
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			pInstrumentNext = m_pCrossTop;
			break;
		}
	case 2:	// 下
		{
			pInstrumentNext = m_pCrossDown;
			break;
		}
	case 3:	// 左
		{
			pInstrumentNext = m_pInstrumentLeft;
			break;
		}
	case 4:	// 右
		{
			pInstrumentNext = m_pInstrumentRight;
			break;
		}
	}
	return pInstrumentNext;
}

/**
* 根据链接方向，得到连接的采集站
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的采集站不存在
*/
CInstrument* CInstrument::GetInstrumentCollect(unsigned int uiRoutDirection)
{
	// 判断路由方向
	switch(uiRoutDirection)
	{
	case 3:	// 左
		{
			// 迂回道高端标记点，迂回道停止标记，非采集站
			if((true == m_bDetourMarkerHigh) && (1 == m_uiStopMarking) && (3 != m_uiInstrumentType))
			{
				return NULL;
			}
			// 迂回道低端标记点，迂回道停止标记
			if((true == m_bDetourMarkerLow) && (1 == m_uiStopMarking))
			{
				return NULL;
			}
			// 辅助道，或迂回道，或爆炸机控制器，或非采集站
			if((true == m_bAux) || (true == m_bBlastMachine) || (true == m_bDetour) || (3 != m_uiInstrumentType))
			{
				// 左指针为空
				if(NULL == m_pInstrumentLeft)
				{
					return NULL;
				}
				else	// 左指针不为空
				{
					// 向左查找
					return m_pInstrumentLeft->GetInstrumentCollect(3);
				}
			}
			return this;
			break;
		}
	case 4:	// 右
		{
			// 迂回道低端标记点，迂回道停止标记，非采集站
			if((true == m_bDetourMarkerLow) && (1 == m_uiStopMarking) && (3 != m_uiInstrumentType))
			{
				return NULL;
			}
			// 迂回道高端标记点，迂回道停止标记
			if((true == m_bDetourMarkerHigh) && (1 == m_uiStopMarking))
			{
				return NULL;
			}
			// 辅助道，或迂回道，或爆炸机控制器，或非采集站
			if((true == m_bAux) || (true == m_bBlastMachine) || (true == m_bDetour) || (3 != m_uiInstrumentType))
			{
				// 右指针为空
				if(NULL == m_pInstrumentRight)
				{
					return NULL;
				}
				else	// 右指针不为空
				{
					// 向右查找
					return m_pInstrumentRight->GetInstrumentCollect(4);
				}
			}
			return this;
			break;
		}
	}
	return NULL;
}

/**
* 根据链接方向，重新设置交叉站尾包接收时刻
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return void
*/
void CInstrument::ResetInstrumentCrossTailFrameReceiveTime(unsigned int uiRoutDirection)
{	
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	if(1 != m_uiInstrumentType)
	{
		return;
	}

	// 判断方向
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			m_usReceiveTime = m_usCrossTopReceiveTime;	// 16bits 交叉线方向上面尾包接收时刻
			break;
		}
	case 2:	// 下
		{
			m_usReceiveTime = m_usCrossDownReceiveTime;	// 16bits 交叉线方下面尾包接收时刻
			break;
		}
	case 3:	// 左
		{
			m_usReceiveTime = m_usLineLeftReceiveTime;	// 16bits 测线方向左面尾包接收时刻
			break;
		}
	case 4:	// 右
		{
			m_usReceiveTime = m_usLineRightReceiveTime;	// 16bits 测线方向右面尾包接收时刻
			break;
		}
	}
}

/**
* 重新设置尾包接收、发送时刻，屏蔽高2位
* @param void
* @return void
*/
void CInstrument::ResetInstrumentTailFrameTime()
{
	m_usSendTime = m_usSendTime % TLOffset;
	m_usReceiveTime	= m_usReceiveTime % TLOffset;
}

/**
* 根据方向得到交叉站路由地址
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return unsigned int 交叉站路由地址 0：仪器不是交叉站，或此方向路由地址为0
*/
unsigned int CInstrument::GetRoutIPByRoutDirection(unsigned int uiRoutDirection)
{	
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	if(1 != m_uiInstrumentType)
	{
		return 0;
	}

	// 判断方向
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			return m_uiRoutIPTop;
			break;
		}
	case 2:	// 下
		{
			return m_uiRoutIPDown;
			break;
		}
	case 3:	// 左
		{
			return m_uiRoutIPLeft;
			break;
		}
	case 4:	// 右
		{
			return m_uiRoutIPRight;
			break;
		}
	}
	return 0;
}

/**
* 根据方向设置交叉站路由地址
* @param unsigned int uiRoutIP 路由IP地址
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return void
*/
void CInstrument::SetRoutIPByRoutDirection(unsigned int uiRoutIP, unsigned int uiRoutDirection)
{	
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	if(1 != m_uiInstrumentType)
	{
		return;
	}

	// 判断方向
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			m_uiRoutIPTop = uiRoutIP;
			break;
		}
	case 2:	// 下
		{
			m_uiRoutIPDown = uiRoutIP;
			break;
		}
	case 3:	// 左
		{
			m_uiRoutIPLeft = uiRoutIP;
			break;
		}
	case 4:	// 右
		{
			m_uiRoutIPRight = uiRoutIP;
			break;
		}
	}
	return;
}

/**
* 检查测试结果
* @param void
* @return void
*/
void CInstrument::CheckResultAll()
{
	m_byCheck = 0;
	// 所有仪器测试
	if((1 == m_byNoiseCheck) && (1 == m_byDistortionCheck) && (1 == m_byCrosstalkCheck) && (1 == m_byGainAndPhaseCheck) && (1 == m_byCrosstalkCheck))
	{
		m_byCheck = 1;
	}
	else if((2 == m_byNoiseCheck) || (2 == m_byDistortionCheck) || (2 == m_byCrosstalkCheck) || (2 == m_byGainAndPhaseCheck) || (2 == m_byCrosstalkCheck))
	{
		m_byCheck = 2;
	}
	else if((3 == m_byNoiseCheck) || (3 == m_byDistortionCheck) || (3 == m_byCrosstalkCheck) || (3 == m_byGainAndPhaseCheck) || (3 == m_byCrosstalkCheck))
	{
		m_byCheck = 3;
	}
}