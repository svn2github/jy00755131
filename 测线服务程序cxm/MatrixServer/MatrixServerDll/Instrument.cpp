#include "stdafx.h"
#include "MatrixServerDll.h"

// 仪器信息重置
void OnInstrumentReset(m_oInstrumentStruct* pInstrument, bool bSetByHand)
{
	ASSERT(pInstrument != NULL);
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
	// 链接的仪器 上方
	pInstrument->m_pInstrumentTop = NULL;
	// 链接的仪器 下方
	pInstrument->m_pInstrumentDown = NULL;
	// 链接的仪器 左方
	pInstrument->m_pInstrumentLeft = NULL;
	// 链接的仪器 右方
	pInstrument->m_pInstrumentRight = NULL;
	// @@@时间48位
// 	/** 仪器本地系统时间*/
// 	pInstrument->m_uiSystemTime = 0;
// 	/** 仪器网络时间*/
// 	pInstrument->m_uiNetTime = 0;
	/** 仪器网络状态*/
	pInstrument->m_uiNetState = 0;
	/** 仪器参数备用1*/
	pInstrument->m_uiNetOrder = 0;

	// 首包时刻
	pInstrument->m_uiTimeHeadFrame = 0;
	// 仪器软件版本号
	pInstrument->m_uiVersion = 0;
	// 	// 尾包时刻
	// 	pInstrument->m_uiTailSysTime = 0;
	// 尾包计数
	pInstrument->m_iTailFrameCount = 0;
	// 仪器时延
	pInstrument->m_uiDelayTime = 0;

// 	pInstrument->m_usReceiveTime = 0;	// 16bits时间，接收时刻低位
// 	pInstrument->m_usSendTime = 0;	// 16bits时间，发送时刻低位

// 	/** 16bits时间，接收时刻低位*/
// 	pInstrument->m_usReceiveTimeLow = 0;
	/** 32bits时间，接收时刻高位*/
	pInstrument->m_uiReceiveTime = 0;
// 	/** 16bits时间，发送时刻低位*/
// 	pInstrument->m_usSendTimeLow = 0;
	/** 32bits时间，发送时刻高位*/
	pInstrument->m_uiSendTime = 0;

	pInstrument->m_usLineLeftReceiveTime = 0;	// 16bits 测线方向左面尾包接收时刻
	pInstrument->m_usLineRightReceiveTime = 0;	// 16bits 测线方向右面尾包接收时刻

	pInstrument->m_usCrossTopReceiveTime = 0;	// 16bits 交叉线方向上面尾包接收时刻
	pInstrument->m_usCrossDownReceiveTime = 0;	// 16bits 交叉线方下面尾包接收时刻

	pInstrument->m_uiBroadCastPort = 0;			// 采集站或电源站的广播端口
	// 测线
	pInstrument->m_iLineIndex = 0;
	// 测点号
	pInstrument->m_iPointIndex = 0;
	// 标记点号
	pInstrument->m_uiPointNb = 0;
	// 标记线号
	pInstrument->m_uiLineNb = 0;
	// 测道
	pInstrument->m_uiChannelNb = 0;

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
	pInstrument->m_bStopMarking = false;
	// 是否标记点
	pInstrument->m_bMarker = false;
	// 是否哑道
	pInstrument->m_bMute = false;
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
	if (bSetByHand == true)
	{
		// 仪器是否进行了ADC参数设置
		pInstrument->m_bADCSet = true;
		// 仪器开始ADC数据采集设置成功
		pInstrument->m_bADCStartSample = true;
		// 仪器停止ADC数据采集设置成功
		pInstrument->m_bADCStopSample = true;
		// 路由开关
		pInstrument->m_ucLAUXRoutOpenSet = 0xf0;
	}
	else
	{
		// 仪器是否进行了ADC参数设置
		pInstrument->m_bADCSet = false;
		// 仪器开始ADC数据采集设置成功
		pInstrument->m_bADCStartSample = false;
		// 仪器停止ADC数据采集设置成功
		pInstrument->m_bADCStopSample = false;
		// 路由开关
		pInstrument->m_ucLAUXRoutOpenSet = 0;
	}
	// 误码查询发送帧数
	pInstrument->m_uiErrorCodeQueryNum = 0;
	// 误码查询应答帧数
	pInstrument->m_uiErrorCodeReturnNum = 0;
	// 采集站和电源站网络数据错误计数
	pInstrument->m_iFDUErrorCodeDataCount = 0;
	// 采集站和电源站命令错误计数
	pInstrument->m_iFDUErrorCodeCmdCount = 0;
	// 采集站和电源站网络数据错误计数
	pInstrument->m_cFDUErrorCodeDataCountOld = 0;
	// 采集站和电源站命令错误计数
	pInstrument->m_cFDUErrorCodeCmdCountOld = 0;

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
	pInstrument->m_cLAUXErrorCodeDataLineACountOld = 0;
	// 交叉站大线B数据故障
	pInstrument->m_cLAUXErrorCodeDataLineBCountOld = 0;
	// 交叉站交叉线A数据故障
	pInstrument->m_cLAUXErrorCodeDataLAUXLineACountOld = 0;
	// 交叉站交叉线B数据故障
	pInstrument->m_cLAUXErrorCodeDataLAUXLineBCountOld = 0;
	// 交叉站命令口故障
	pInstrument->m_cLAUXErrorCodeCmdCountOld = 0;

	// 实际接收ADC数据帧数
	pInstrument->m_uiADCDataActualRecFrameNum = 0;
	// 重发查询帧得到的应答帧数
	pInstrument->m_uiADCDataRetransmissionFrameNum = 0;
	// 应该接收ADC数据帧数（含丢帧）
	pInstrument->m_uiADCDataShouldRecFrameNum = 0;
	// ADC采样数据（取每帧的第一个数据用于计算）
	pInstrument->m_olsADCDataSave.clear();
	// ADC数据帧的指针偏移量
	pInstrument->m_usADCDataFramePointNow = 0;
	// ADC数据帧发送时的本地时间
	pInstrument->m_uiADCDataFrameSysTimeNow = 0;
	// ADC数据帧的指针偏移量
	pInstrument->m_usADCDataFramePointOld = 0;
	// ADC数据帧发送时的本地时间
	pInstrument->m_uiADCDataFrameSysTimeOld = 0;
	// 仪器开始采样的TB时刻高位
	pInstrument->m_uiTBHigh = 0;
	// 检查是否晚或者没有收到ADC数据帧
	pInstrument->m_bCheckADCFrameLate = true;
	// 仪器存活时间
	pInstrument->m_uiActiveTime = 0;
}
// 判断索引号是否已加入索引表
BOOL IfIndexExistInMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
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
// 增加对象到索引表
void AddInstrumentToMap(unsigned int uiIndex, m_oInstrumentStruct* pInstrument, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pInstrument != NULL);
	ASSERT(pMap != NULL);
	if (FALSE == IfIndexExistInMap(uiIndex, pMap))
	{
		pMap->insert(hash_map<unsigned int, m_oInstrumentStruct*>::value_type (uiIndex, pInstrument));
	}
}
// 根据输入索引号，由索引表得到仪器指针
m_oInstrumentStruct* GetInstrumentFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned int, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 从索引表删除索引号指向的仪器指针
BOOL DeleteInstrumentFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
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

// 判断仪器位置索引号是否已加入索引表
BOOL IfLocationExistInMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
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
void AddLocationToMap(int iLineIndex, int iPointIndex, m_oInstrumentStruct* pInstrument, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pInstrument != NULL);
	ASSERT(pMap != NULL);
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	if (false == IfLocationExistInMap(iLineIndex, iPointIndex, pMap))
	{
		pMap->insert(map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::value_type (Location, pInstrument));
	}
}
// 根据输入索引号，由索引表得到仪器指针
m_oInstrumentStruct* GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(Location);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 从索引表删除索引号指向的仪器指针
BOOL DeleteInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	m_oInstrumentLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>::iterator iter;
	iter = pMap->find(Location);
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

// 得到仪器在某一方向上的路由IP
bool GetRoutIPBySn(unsigned int uiSN, int iDirection, 
	m_oInstrumentListStruct* pInstrumentList, unsigned int& uiRoutIP)
{
	m_oInstrumentStruct* pInstrument = NULL;
	pInstrument = GetInstrumentFromMap(uiSN, &pInstrumentList->m_oSNInstrumentMap);
	if (pInstrument == NULL)
	{
		return false;
	}
	if (iDirection == DirectionLeft)
	{
		uiRoutIP = pInstrument->m_uiRoutIPLeft;
	}
	else if (iDirection == DirectionRight)
	{
		uiRoutIP = pInstrument->m_uiRoutIPRight;
	}
	else if (iDirection == DirectionTop)
	{
		uiRoutIP = pInstrument->m_uiRoutIPTop;
	}
	else if (iDirection == DirectionDown)
	{
		uiRoutIP = pInstrument->m_uiRoutIPDown;
	}
	else if (iDirection == DirectionCenter)
	{
		uiRoutIP = pInstrument->m_uiRoutIP;
	}
	else
	{
		return false;
	}
	return true;
}
/**
* 根据链接方向，得到连接的下一个仪器
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
*/
m_oInstrumentStruct* GetNextInstrAlongRout(m_oInstrumentStruct* pInstrument, int iRoutDirection)
{
	ASSERT(pInstrument != NULL);
	m_oInstrumentStruct* pInstrumentNext = NULL;
	// 判断方向
	if (iRoutDirection == DirectionTop)
	{
		pInstrumentNext = pInstrument->m_pInstrumentTop;
	}
	else if (iRoutDirection == DirectionDown)
	{
		pInstrumentNext = pInstrument->m_pInstrumentDown;
	}
	else if (iRoutDirection == DirectionLeft)
	{
		pInstrumentNext = pInstrument->m_pInstrumentLeft;
	}
	else if (iRoutDirection == DirectionRight)
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
* @return CInstrument* 仪器指针 NLLL：连接的前一个仪器不存在
*/
m_oInstrumentStruct* GetPreviousInstr(m_oInstrumentStruct* pInstrument)
{
	ASSERT(pInstrument != NULL);
	m_oInstrumentStruct* pInstrumentPrevious = NULL;
	// 判断方向
	if (pInstrument->m_iRoutDirection == DirectionTop)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentDown;
	}
	else if (pInstrument->m_iRoutDirection == DirectionDown)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentTop;
	}
	else if (pInstrument->m_iRoutDirection == DirectionLeft)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentRight;
	}
	else if (pInstrument->m_iRoutDirection == DirectionRight)
	{
		pInstrumentPrevious = pInstrument->m_pInstrumentLeft;
	}
	else
	{
		return NULL;
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
// 更新仪器的存活时间
void UpdateInstrActiveTime(m_oInstrumentStruct* pInstrument)
{
	ASSERT(pInstrument != NULL);
	unsigned int uiActiveTime = 0;
	m_oInstrumentStruct* pInstrumentPrevious = pInstrument;
	uiActiveTime = GetTickCount();
	do 
	{
		pInstrumentPrevious->m_uiActiveTime = uiActiveTime;
		pInstrumentPrevious->m_iTailFrameCount = 0;
		pInstrumentPrevious = GetPreviousInstr(pInstrumentPrevious);
	} while (pInstrumentPrevious != NULL);
}