#include "StdAfx.h"
#include "SiteData.h"
#include "Parameter.h"
CSiteData::CSiteData()
{

}

CSiteData::~CSiteData()
{
}
/**
* 初始化
* @param void
* @return void
*/
void CSiteData::OnInit()
{
// 	// 首个主交叉站指针
// 	m_pFirstMainCross = NULL;
 	// 仪器队列仪器总数
 	m_oInstrumentList.m_uiCountAll = m_uiInstrumentCountAll;
	// 初始化仪器队列
	m_oInstrumentList.OnInit();
// 	// 初始化SN仪器索引表
// 	m_oSNInstrumentMap.OnInit();
// 	// 初始化IP地址仪器索引表
// 	m_oIPInstrumentMap.OnInit();
	// 路由队列路由总数
	m_oRoutList.m_uiCountAll = m_uiInstrumentCountAll;
	// 初始化路由队列
	m_oRoutList.OnInit();
// 	// 初始化路由地址路由索引表
// 	m_oRoutIPRoutMap.OnInit();
// 	// IP地址队列临时IP地址总数
// 	m_oIPList.m_uiCountAll = m_uiIPTempCountAll;
// 	// 初始化IP地址队列
// 	m_oIPList.OnInit();

	m_uiSystemTimeSite = 0;	// 野外设备系统时间

	// 删除仪器或路由处理线程正在运行
	m_bProcDeleteRunning = false;
	// 现场数据输出线程正在运行
	m_bProcSiteDataOutputRunning = false;

	// 暂停要求，要求仪器IP地址分配线程暂停
	m_bPauseRequestForProcIPDistribute = false;

	// 尾包处理线程暂停
	m_bProcTailFramePause = false;
	// 路由监视线程暂停
	m_bProcMonitorRoutPause = false;
	// 首包处理线程暂停
	m_bProcHeadFramePause = false;
	// 仪器IP地址分配线程暂停
	m_bProcIPDistributePause = false;
	// 仪器IP地址设置线程暂停
	m_bProcIPSetPause = false;
	// 现场数据输出线程线程暂停
	m_bProcSiteDataOutputPause = false;

	// 心跳处理线程停止
	m_bProcHeartBeatStop = true;
	// 首包处理线程停止
	m_bProcHeadFrameStop = true;
	// 尾包处理线程停止
	m_bProcTailFrameStop = true;
	// 路由监视线程停止
	m_bProcMonitorRoutStop = true;
	// 仪器IP地址分配线程停止
	m_bProcIPDistributeStop = true;
	// 仪器时延处理线程停止
	m_bProcTimeDelayStop = true;
	// 仪器IP地址设置线程停止
	m_bProcIPSetStop = true;
	// 删除仪器或路由处理线程停止
	m_bProcDeleteStop = true;
	m_bProcTimeDelayON_stop=false;
	iDelayfrist=0;
	// 现场数据输出线程线程停止
	m_bProcSiteDataOutputStop = true;
	// 测试线程线程停止，基本测试
	m_bProcTestBaseStop = true;
	// 测试线程线程停止，噪声监测
	m_bProcTestNoiseStop = true;

	// 心跳处理线程关闭
	m_bProcHeartBeatClose = false;
	// 首包处理线程关闭
	m_bProcHeadFrameClose = false;
	// 尾包处理线程关闭
	m_bProcTailFrameClose = false;
	// 路由监视线程关闭
	m_bProcMonitorRoutClose = false;
	// 仪器IP地址分配线程关闭
	m_bProcIPDistributeClose = false;
	// 仪器时延处理线程关闭
	m_bProcTimeDelayClose = false;
	// 仪器IP地址设置线程关闭
	m_bProcIPSetClose = false;
	// 删除仪器或路由处理线程关闭
	m_bProcDeleteClose = false;
	// 现场数据输出线程线程关闭
	m_bProcSiteDataOutputClose = false;
	// 测试线程线程关闭，基本测试
	m_bProcTestBaseClose = false;
	// 测试线程线程关闭，噪声监测
	m_bProcTestNoiseClose = false;

	// 现场数据是否变化
	m_bSiteDataChanged = false;
	// 上次现场数据变化时刻
	m_uiSiteDataChangedTime = 0;
	// 测试数据是否变化
	m_bTestDataChanged = false;

// 	NetDataCount=0;
// 	NetOrderCount=0;
}

/**
* 重置
* @param void
* @return void
*/
void CSiteData::OnReset()
{
// 	// 首个主交叉站指针
// 	m_pFirstMainCross = NULL;
	// 重置仪器队列
	m_oInstrumentList.OnReset();
// 	// 重置SN仪器索引表
// 	m_oSNInstrumentMap.OnReset();
// 	// 重置IP地址仪器索引表
// 	m_oIPInstrumentMap.OnReset();
	// 重置路由队列
	m_oRoutList.OnReset();
// 	// 重置路由地址路由索引表
// 	m_oRoutIPRoutMap.OnReset();
// 	// 重置IP地址队列
// 	m_oIPList.OnReset();

	m_uiSystemTimeSite = 0;	// 野外设备系统时间

	// 删除仪器或路由处理线程正在运行
	m_bProcDeleteRunning = false;
	// 现场数据输出线程正在运行
	m_bProcSiteDataOutputRunning = false;
	// 尾包处理线程暂停
	m_bProcTailFramePause = false;
	// 路由监视线程暂停
	m_bProcMonitorRoutPause = false;
	// 首包处理线程暂停
	m_bProcHeadFramePause = false;
	// 仪器IP地址分配线程暂停
	m_bProcIPDistributePause = false;
	// 仪器IP地址设置线程暂停
	m_bProcIPSetPause = false;
	// 现场数据输出线程线程暂停
	m_bProcSiteDataOutputPause = false;

	// 心跳处理线程停止
	m_bProcHeartBeatStop = true;
	// 首包处理线程停止
	m_bProcHeadFrameStop = true;
	// 尾包处理线程停止
	m_bProcTailFrameStop = true;
	// 路由监视线程停止
	m_bProcMonitorRoutStop = true;
	// 仪器IP地址分配线程停止
	m_bProcIPDistributeStop = true;
	// 仪器时延处理线程停止
	m_bProcTimeDelayStop = true;
	// 仪器IP地址设置线程停止
	m_bProcIPSetStop = true;
	// 删除仪器或路由处理线程停止
	m_bProcDeleteStop = true;
	// 现场数据输出线程线程停止
	m_bProcSiteDataOutputStop = true;
	// 测试线程线程停止，基本测试
	m_bProcTestBaseStop = true;
	// 测试线程线程停止，噪声监测
	m_bProcTestNoiseStop = true;

	// 现场数据是否变化
	m_bSiteDataChanged = false;
	// 上次现场数据变化时刻
	m_uiSiteDataChangedTime = 0;
	// 测试数据是否变化
	m_bTestDataChanged = false;
}

/**
* 关闭
* @param void
* @return void
*/
void CSiteData::OnClose()
{
// 	// 首个主交叉站指针
// 	m_pFirstMainCross = NULL;
	// 关闭仪器队列
	m_oInstrumentList.OnClose();
	// 关闭路由队列
	m_oRoutList.OnClose();
// 	// 关闭路由地址路由索引表
// 	m_oRoutIPRoutMap.OnClose();
// 	// 关闭IP地址队列
// 	m_oIPList.OnClose();
}

// /**
// * 仪器SN号是否已加入SN索引表
// * @param unsigned int uiSN 仪器SN
// * @return BOOL TRUE：是；FALSE：否
// */
// BOOL CSiteData::IfSNExistInSNMap(unsigned int uiSN)
// {
// 	return m_oInstrumentList.IfIndexExistInSNMap(uiSN);
// 
// }
// 
// /**
// * 由仪器SN索引表得到一个仪器
// * @param unsigned int uiIndex 仪器SN号
// * @param CInstrument* &pInstrument 仪器指针，引用方式调用
// * @return BOOL TRUE：成功；FALSE：失败
// */
// BOOL CSiteData::GetInstrumentFromSNMapBySN(unsigned int uiIndex, CInstrument* &pInstrument)
// {
// 	return m_oInstrumentList.GetInstrumentFromSNMap(uiIndex, pInstrument);
// }
// 
// /**
// * 仪器IP地址是否已加入IP地址索引表
// * @param unsigned int uiIP 仪器IP地址
// * @return BOOL TRUE：是；FALSE：否
// */
// BOOL CSiteData::IfIPExistInIPMap(unsigned int uiIP)
// {
// 	return m_oInstrumentList.IfIndexExistInIPMap(uiIP);
// }
// 
// /**
// * 由仪器IP地址索引表得到一个仪器
// * @param unsigned int uiIndex 仪器IP地址
// * @param CInstrument* &pInstrument 仪器指针，引用方式调用
// * @return BOOL TRUE：成功；FALSE：失败
// */
// BOOL CSiteData::GetInstrumentFromIPMapByIP(unsigned int uiIndex, CInstrument* &pInstrument)
// {
// 	return m_oInstrumentList.GetInstrumentFromIPMap(uiIndex, pInstrument);
// }

/**
* 更新路由对象的路由时刻
* @param unsigned int uiRoutIP 路由IP地址
* @return void
*/
void CSiteData::UpdateRoutTime(unsigned int uiRoutIP)
{
// 	if(0 == uiRoutIP)	// 没有路由地址
// 	{	
// 		return;
// 	}

	CRout* pRout = NULL;
	if(TRUE == m_oRoutList.GetRout(uiRoutIP, pRout))
	{
		pRout->UpdateRoutTime();
	}
}

/**
* 得到一个空闲仪器
* @param void
* @return CInstrument* NULL：失败
*/
CInstrument* CSiteData::GetFreeInstrument()
{
	CInstrument* pInstrument = NULL;
	pInstrument = m_oInstrumentList.GetFreeInstrument();
	return pInstrument;
}

/**
* 得到仪器连接的下一个仪器
* @param CInstrument* pInstrument 仪器指针
* @param unsigned int uiRoutDirection 连接方向  1-上；2-下；3-左；4右
* @return CInstrument* NULL：失败
*/
CInstrument* CSiteData::GetNextInstrument(CInstrument* pInstrument, unsigned int uiRoutDirection)
{
	CInstrument* pInstrumentNext = NULL;

	pInstrumentNext = pInstrument->GetNextInstrument(uiRoutDirection);

	return pInstrumentNext;
}
// 仪器位置按照首包时刻排序
void CSiteData::InstrumentLocationSort(CInstrument* pInstrument, CRout* pRout, unsigned int uiRoutDirection)
{
	bool bLocation = false;
	bool bStable = true;
	CInstrument* pInstrumentNext = NULL;
	CInstrument* pInstrumentLeft = NULL;
	CInstrument* pInstrumentRight = NULL;

	// 该路由方向尾仪器为空
	if (pRout->m_pTail == NULL)
	{
		pRout->m_pTail = pInstrument;
		if (uiRoutDirection == DirectionLeft)
		{
			pRout->m_pHead->m_pInstrumentLeft = pInstrument;
			pInstrument->m_pInstrumentRight = pRout->m_pHead;
		}
		else if (uiRoutDirection == DirectionRight)
		{
			pRout->m_pHead->m_pInstrumentRight = pInstrument;
			pInstrument->m_pInstrumentLeft = pRout->m_pHead;
		}
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		// 该路由方向尾仪器首包时刻小于该仪器的首包时刻
		if (pRout->m_pTail->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
		{
			if (uiRoutDirection == DirectionLeft)
			{
				pRout->m_pTail->m_pInstrumentLeft = pInstrument;
				pInstrument->m_pInstrumentRight = pRout->m_pTail;
			}
			else if (uiRoutDirection == DirectionRight)
			{
				pRout->m_pTail->m_pInstrumentRight = pInstrument;
				pInstrument->m_pInstrumentLeft = pRout->m_pTail;
			}
			pRout->m_pTail = pInstrument;
			pInstrument->m_iHeadFrameStableNum++;
		} 
		// 仪器位于路由链中
		else
		{
			pInstrumentNext = pRout->m_pHead;
			while(pInstrumentNext != NULL)
			{
				if (pInstrumentNext->m_uiTimeHeadFrame < pInstrument->m_uiTimeHeadFrame)
				{
					if (uiRoutDirection == DirectionLeft)
					{
						pInstrumentRight = pInstrumentNext;
					}
					else if (uiRoutDirection == DirectionRight)
					{
						pInstrumentLeft = pInstrumentNext;
					}
				}
				else
				{
					// 找到路由表第一个首包时刻大于等于仪器首包时刻的仪器
					if (bLocation == false)
					{
						bLocation = true;
						if (uiRoutDirection == DirectionLeft)
						{
							pInstrumentLeft = pInstrumentNext;
							if (pInstrumentLeft == pInstrument)
							{
								pInstrument->m_iHeadFrameStableNum++;
							}
							// 在路由表中两个仪器中间插入仪器
							else
							{
								pInstrumentRight->m_pInstrumentLeft = pInstrument;
								pInstrument->m_pInstrumentRight = pInstrumentRight;
								pInstrumentLeft->m_pInstrumentRight = pInstrument;
								pInstrument->m_pInstrumentLeft = pInstrumentLeft;
								pInstrumentLeft->m_iHeadFrameStableNum = 0;
								pInstrument->m_iHeadFrameStableNum = 0;
								bStable = false;	
							}
						}
						else if (uiRoutDirection == DirectionRight)
						{
							pInstrumentRight = pInstrumentNext;
							if (pInstrumentRight == pInstrument)
							{
								pInstrument->m_iHeadFrameStableNum++;
							}
							// 在路由表中两个仪器中间插入仪器
							else
							{
								pInstrumentRight->m_pInstrumentLeft = pInstrument;
								pInstrument->m_pInstrumentRight = pInstrumentRight;
								pInstrumentLeft->m_pInstrumentRight = pInstrument;
								pInstrument->m_pInstrumentLeft = pInstrumentLeft;
								pInstrumentRight->m_iHeadFrameStableNum = 0;
								pInstrument->m_iHeadFrameStableNum = 0;
								bStable = false;	
							}
						}
					}
					else
					{
						if (bStable == false)
						{
							pInstrumentNext->m_iHeadFrameStableNum = 0;
						}
					}
				}
				pInstrumentNext = GetNextInstrument(pInstrumentNext, uiRoutDirection);
			}
		}
	}
}
// 设置仪器的位置
void CSiteData::SetInstrumentLocation(CInstrument* pInstrument)
{
	// 在路由索引表中找到路由头仪器
	// 如果是LCI及交叉线方向的交叉站
	if ((pInstrument->m_uiRoutDirection == DirectionCenter)
		|| (pInstrument->m_uiRoutDirection == DirectionTop)
		|| (pInstrument->m_uiRoutDirection == DirectionDown))
	{
		pInstrument->m_iHeadFrameStableNum++;
	}
	else
	{
		CRout* pRout = NULL;
		if (TRUE == m_oRoutList.GetRout(pInstrument->m_uiRoutIP, pRout))
		{
			// 大线左方
			if (pInstrument->m_uiRoutDirection == DirectionLeft)
			{
				InstrumentLocationSort(pInstrument, pRout, DirectionLeft);
			}
			// 大线右方
			else
			{
				InstrumentLocationSort(pInstrument, pRout, DirectionRight);
			}
		}
		else
		{

		}
	}
	// 如果仪器在其路由方向上位置稳定次数超过设定次数
	// 则将该仪器加入IP地址设置队列
	if (pInstrument->m_iHeadFrameStableNum >= HeadFrameStableNum)
	{
		if (FALSE == m_oInstrumentList.IfIndexExistInIPSetMap(pInstrument->m_uiIP))
		{
			m_oInstrumentList.AddInstrumentToIPSetMap(pInstrument->m_uiIP, pInstrument);
		}
	}
}
// /**
// * 删除仪器串，包括仪器本身；删除前设置删除来源仪器的连接指针
// * @param CInstrument* pInstrument 仪器指针
// * @param unsigned int uiRoutDirection 连接方向  1-上；2-下；3-左；4右
// * @return void
// */
// void CSiteData::DeleteInstrumentLink(CInstrument* pInstrument, unsigned int uiDirection)
// {
// 	// 删除来源仪器
// 	CInstrument* pInstrumentRoot = NULL;
// 	// 删除来源仪器对应的路由
// 	CRout* pRout = NULL;
// 
// 	// 判断路由方向
// 	switch(uiDirection)
// 	{
// 	case 1:	// 上
// 		{
// 			// 删除来源仪器为交叉站
// 			pInstrumentRoot = pInstrument->m_pCrossTop;
// 			// 仪器下指针为空
// 			pInstrumentRoot->m_pCrossDown = NULL;
// 			pInstrumentRoot->m_uiSNCrossDown = 0;
// 			pInstrumentRoot->m_iIndexCrossDown = -1;
// 			// 找到路由对象
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout))
// 			{	// 路由对象尾指针为空
// 				pRout->m_pTail = NULL;
// 				pRout->m_uiSNTail = 0;
// 				pRout->m_iIndexTail = -1;
// 			}
// 			break;
// 		}
// 	case 2:	// 下
// 		{
// 			// 删除来源仪器为交叉站
// 			pInstrumentRoot = pInstrument->m_pCrossDown;
// 			// 仪器上指针为空
// 			pInstrumentRoot->m_pCrossTop = NULL;
// 			pInstrumentRoot->m_uiSNCrossTop = 0;
// 			pInstrumentRoot->m_iIndexCrossTop = -1;
// 			// 找到路由对象
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout))
// 			{
// 				// 路由对象尾指针为空
// 				pRout->m_pTail = NULL;
// 				pRout->m_uiSNTail = 0;
// 				pRout->m_iIndexTail = -1;
// 			}
// 			break;
// 		}
// 	case 3:	// 左
// 		{
// 			// 删除来源仪器
// 			pInstrumentRoot = pInstrument->m_pInstrumentLeft;
// 			// 仪器指针为空
// 			pInstrumentRoot->m_pInstrumentRight = NULL;
// 			pInstrumentRoot->m_uiSNInstrumentRight = 0;
// 			pInstrumentRoot->m_iIndexInstrumentRight = -1;
// 			// 找到路由对象
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout))
// 			{
// 				// 删除来源仪器为交叉站
// 				if(1 == pInstrumentRoot->m_uiInstrumentType)
// 				{
// 					// 路由对象尾指针为空
// 					pRout->m_pTail = NULL;
// 					pRout->m_uiSNTail = 0;
// 					pRout->m_iIndexTail = -1;
// 				}
// 				else	// 删除来源仪器不是交叉站
// 				{
// 					// 路由尾指针指向仪器
// 					pRout->m_pTail = pInstrumentRoot;
// 					pRout->m_uiSNTail = pInstrumentRoot->m_uiSN;
// 					pRout->m_iIndexTail = pInstrumentRoot->m_uiIndex;
// 				}
// 			}
// 			break;
// 		}
// 	case 4:	// 右
// 		{
// 			// 删除来源仪器
// 			pInstrumentRoot = pInstrument->m_pInstrumentRight;
// 			// 仪器指针为空
// 			// 仪器左指针为空
// 			pInstrumentRoot->m_pInstrumentLeft = NULL;
// 			pInstrumentRoot->m_uiSNInstrumentLeft = 0;
// 			pInstrumentRoot->m_iIndexInstrumentLeft = -1;
// 			// 找到路由对象
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout))
// 			{
// 				// 删除来源仪器为交叉站
// 				if(1 == pInstrumentRoot->m_uiInstrumentType)
// 				{
// 					// 路由对象尾指针为空
// 					pRout->m_pTail = NULL;
// 					pRout->m_uiSNTail = 0;
// 					pRout->m_iIndexTail = -1;
// 				}
// 				else	// 删除来源仪器不是交叉站
// 				{
// 					// 路由尾指针指向仪器
// 					pRout->m_pTail = pInstrumentRoot;
// 					pRout->m_uiSNTail = pInstrumentRoot->m_uiSN;
// 					pRout->m_iIndexTail = pInstrumentRoot->m_uiIndex;
// 				}
// 			}
// 			break;
// 		}
// 	}
// 
// 	// 删除仪器，递归
// 	DeleteInstrument(pInstrument, uiDirection);
// }
// 
// /**
// * 删除仪器，递归，包括仪器本身
// * @param CInstrument* pInstrument 仪器指针
// * @param unsigned int uiRoutDirection 连接方向  1-上；2-下；3-左；4右
// * @return void
// */
// void CSiteData::DeleteInstrument(CInstrument* pInstrument, unsigned int uiDirection)
// {
// 	if(NULL == pInstrument)
// 	{
// 		return;
// 	}
// 	if(false == pInstrument->m_bInUsed)
// 	{
// 		return;
// 	}
// 
// 	// 删除链接的仪器，来自删除方向的除外
// 	// 向上删除
// 	if((1 != uiDirection) && (NULL != pInstrument->m_pCrossTop))	// 不是来自上方的删除，且上指针不为空
// 	{
// 		// 删除上面链接的仪器
// 		DeleteInstrument(pInstrument->m_pCrossTop, 2);
// 	}
// 	// 向下删除
// 	if((2 != uiDirection) && (NULL != pInstrument->m_pCrossDown))	// 不是来自下方的删除，且下指针不为空
// 	{
// 		// 删除下面链接的仪器
// 		DeleteInstrument(pInstrument->m_pCrossDown, 1);
// 	}
// 	// 向左删除
// 	if((3 != uiDirection) && (NULL != pInstrument->m_pInstrumentLeft))	// 不是来自左方的删除，且左指针不为空
// 	{
// 		// 删除左面链接的仪器
// 		DeleteInstrument(pInstrument->m_pInstrumentLeft, 4);
// 	}
// 	// 向右删除
// 	if((4 != uiDirection) && (NULL != pInstrument->m_pInstrumentRight))	// 不是来自右方的删除，且右指针不为空
// 	{
// 		// 删除右面链接的仪器
// 		DeleteInstrument(pInstrument->m_pInstrumentRight, 3);
// 	}
// 
// 	// 从SN索引表中删除自己
// 	m_oSNInstrumentMap.DeleteInstrument(pInstrument->m_uiSN);	
// 	// 从IP索引表中删除自己
// 	m_oIPInstrumentMap.DeleteInstrument(pInstrument->m_uiIP);
// 
// 	// 清除被删除仪器对应测道的仪器SN号
// 	m_pLogicData->ClearSNInChannelWhenInstrumentDelete(pInstrument->m_uiIP);
// 
// 	// 从路由索引表中删除自己
// 	DeleteRout(pInstrument);
// 	// 加入空闲仪器队列
// 	m_oInstrumentList.AddFreeInstrument(pInstrument);
// }
// 
// /**
// * 删除路由链接的仪器
// * @param CRout* pRout 路由对象指针
// * @return bool true：仪器删除成功；false：没有要删除的仪器
// */
// bool CSiteData::DeleteRoutLinkInstrument(CRout* pRout)
// {
// 	bool bReturn = false;
// 
// 	// 判断路由方向是否链接仪器
// 	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
// 	{
// 		return bReturn;
// 	}
// 
// 	// 判断路由方向
// 	switch(pRout->m_uiRoutDirection)
// 	{
// 	case 1:	// 上
// 		{
// 			// 删除交叉站上方链接的所有仪器
// 			DeleteInstrument(pRout->m_pHead->m_pCrossTop, 2);
// 			break;
// 		}
// 	case 2:	// 下
// 		{
// 			// 删除交叉站下方链接的所有仪器
// 			DeleteInstrument(pRout->m_pHead->m_pCrossDown, 1);
// 			break;
// 		}
// 	case 3:	// 左
// 		{
// 			// 删除交叉站左方链接的所有仪器
// 			DeleteInstrument(pRout->m_pHead->m_pInstrumentLeft, 4);
// 			break;
// 		}
// 	case 4:	// 右
// 		{
// 			// 删除交叉站右方链接的所有仪器
// 			DeleteInstrument(pRout->m_pHead->m_pInstrumentRight, 3);
// 			break;
// 		}
// 	}
// 	// 路由对象尾指针为空
// 	pRout->m_pTail = NULL;
// 	bReturn = true;
// 	return bReturn;
// }
// 
// /**
// * 删除路由IP地址被禁止的路由链接的仪器
// * @param CRout* pRout 路由对象指针
// * @return bool true：仪器删除成功；false：没有要删除的仪器
// */
// bool CSiteData::DeleteRoutLinkInstrumentWhenRoutIPDisable(CRout* pRout)
// {
// 	bool bReturn = false;
// 
// 	// 判断路由方向是否链接仪器
// 	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
// 	{
// 		return bReturn;
// 	}
// 
// 	// 判断路由方向
// 	switch(pRout->m_uiRoutDirection)
// 	{
// 	case 1:	// 上
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPTop)
// 			{
// 				// 删除路由链接的仪器
// 				bReturn = DeleteRoutLinkInstrument(pRout);
// 			}
// 			break;
// 		}
// 	case 2:	// 下
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPDown)
// 			{
// 				// 删除路由链接的仪器
// 				bReturn = DeleteRoutLinkInstrument(pRout);
// 			}
// 			break;
// 		}
// 	case 3:	// 左
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPLeft)
// 			{
// 				// 删除路由链接的仪器
// 				bReturn = DeleteRoutLinkInstrument(pRout);
// 			}
// 			break;
// 		}
// 	case 4:	// 右
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPRight)
// 			{
// 				// 删除路由链接的仪器
// 				bReturn = DeleteRoutLinkInstrument(pRout);
// 			}
// 			break;
// 		}
// 	}
// 	return bReturn;
// }
// 
// /**
// * 删除路由时间过期的路由对象所链接的仪器
// * @param CRout* pRout 路由对象指针
// * @return bool true：仪器删除成功；false：没有要删除的仪器
// */
// bool CSiteData::DeleteRoutLinkInstrumentWhenRoutTimeExpired(CRout* pRout)
// {
// 	// 是否发生删除路由链接的仪器操作
// 	bool bDelete = false;
// 
// 	// 判断路由方向是否链接了仪器
// 	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
// 	{
// 		return bDelete;
// 	}
// 
// 	// 得到当前系统时间
// 	unsigned int uiTimeNow = GetTickCount();
// 
// 	// 判断路由对象的路由时间是否过期
// 	if(5000 < (uiTimeNow - pRout->m_uiRoutTime))	// 过期
// 	{
// 		// 删除路由链接的仪器
// 		bDelete = DeleteRoutLinkInstrument(pRout);
// 	}
// 	return bDelete;
// }
// 
// /**
// * 删除仪器4个路由方向路由IP地址对应的路由对象
// * @param CInstrument* pInstrument 仪器对象指针
// * @return void
// */
// void CSiteData::DeleteRout(CInstrument* pInstrument)
// {
// 	// 从路由索引表中删除自己
// 	if(1 == pInstrument->m_uiInstrumentType)	// 交叉站
// 	{
// 		CRout* pRout = NULL;
// 		// 上路由
// 		if(0 != pInstrument->m_uiRoutIPTop)	
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPTop, pRout))	// 找到路由对象
// 			{
// 				// 删除路由
// 				DeleteRout(pRout);
// 			}
// 		}
// 		// 下路由
// 		if(0 != pInstrument->m_uiRoutIPDown)	
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPDown, pRout))	// 找到路由对象
// 			{
// 				// 删除路由
// 				DeleteRout(pRout);
// 			}
// 		}
// 		// 左路由
// 		if(0 != pInstrument->m_uiRoutIPLeft)	
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPLeft, pRout))	// 找到路由对象
// 			{
// 				// 删除路由
// 				DeleteRout(pRout);
// 			}
// 		}
// 		// 右路由
// 		if(0 != pInstrument->m_uiRoutIPRight)	
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPRight, pRout))	// 找到路由对象
// 			{
// 				// 删除路由
// 				DeleteRout(pRout);
// 			}
// 		}
// 	}
// }
// 
// /**
// * 删除路由
// * @param CRout* pRout 路由对象指针
// * @return void
// */
// void CSiteData::DeleteRout(CRout* pRout)
// {
// 	if(NULL == pRout)
// 	{
// 		return;
// 	}
// 	if(false == pRout->m_bInUsed)
// 	{
// 		return;
// 	}
// 	// 删除路由
// 	m_oRoutIPRoutMap.DeleteRout(pRout->m_uiRoutIP);	
// 	// 加入空闲路由队列
// 	m_oRoutList.AddFreeRout(pRout);	
// 
// }
// 
// /**
// * 删除路由IP地址被禁止的路由
// * @param CRout* pRout 路由对象指针
// * @return bool true：路由删除成功；false：没有要删除的路由
// */
// bool CSiteData::DeleteRoutWhenRoutIPDisable(CRout* pRout)
// {
// 	bool bReturn = false;
// 
// 	// 判断路由方向
// 	switch(pRout->m_uiRoutDirection)
// 	{
// 	case 1:	// 上
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPTop)
// 			{
// 				DeleteRout(pRout);
// 				bReturn = true;
// 			}
// 			break;
// 		}
// 	case 2:	// 下
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPDown)
// 			{
// 				DeleteRout(pRout);
// 				bReturn = true;
// 			}
// 			break;
// 		}
// 	case 3:	// 左
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPLeft)
// 			{
// 				DeleteRout(pRout);
// 				bReturn = true;
// 			}
// 			break;
// 		}
// 	case 4:	// 右
// 		{
// 			if(0 == pRout->m_pHead->m_uiRoutIPRight)
// 			{
// 				DeleteRout(pRout);
// 				bReturn = true;
// 			}
// 			break;
// 		}
// 	}
// 	return bReturn;
// }
// 
// /**
// * 得到仪器链接方向的第一个采集站
// * @param CInstrument* pInstrument 仪器指针
// * @param unsigned int uiRoutDirection 连接方向  1-上；2-下；3-左；4右
// * @return CInstrument* 采集站指针 NULL：仪器链接方向没有采集站
// */
// CInstrument* CSiteData::GetInstrumentCollect(CInstrument* pInstrument, unsigned int uiRoutDirection)
// {
// 	if(NULL == pInstrument)
// 	{
// 		return NULL;
// 	}
// 	return pInstrument->GetInstrumentCollect(uiRoutDirection);
// }
// 
// /**
// * 释放仪器IP地址
// * @param unsigned int uiIP 仪器IP地址
// * @param unsigned unsigned int uiIPDistributeTime 仪器IP地址分配时间
// * @return void
// */
// void CSiteData::ReleaseInstrument(unsigned int uiIP, unsigned int uiIPDistributeTime)
// {
// 	CInstrument* pInstrument = NULL;
// 	// 判断：仪器在仪器IP地址索引表中不存在
// 	if(FALSE == m_oIPInstrumentMap.GetInstrument(uiIP, pInstrument))
// 	{
// 		return;
// 	}
// 	// 判断：不是采集站
// 	if(3 != pInstrument->m_uiInstrumentType)
// 	{
// 		return;
// 	}
// 	// 判断：仪器IP地址不能释放
// 	if(uiIPDistributeTime == pInstrument->m_uiIPDistributeTime)
// 	{
// 		return;
// 	}	
// 
// 	// 得到测道，根据IP地址
// 	CChannelSetupData* phannelSetupData = m_pLogicData->m_oSurveySetupData.GetChannel(uiIP);
// 	if(NULL != phannelSetupData)
// 	{
// 		phannelSetupData->m_uiSN = 0;
// 	}	
// 
// 	// 得到仪器IP地址的类型	1-固定IP地址；2-浮动IP地址
// 	int iType = CIPList::GetInstrumentIPType(uiIP);
// 	// IP地址仪器索引表
// 	m_oIPInstrumentMap.DeleteInstrument(uiIP);
// 	// 2-浮动IP地址
// 	if(2 == iType)
// 	{
// 		m_oIPList.AddFreeInstrumentIP(uiIP);
// 	}
// }

/**
* 更新上次现场数据变化时刻
* @param void
* @return void
*/
void CSiteData::UpdateSiteDataChangedTime()
{
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Lock();
		// 现场数据是否变化
		m_bSiteDataChanged = true;
		// 上次现场数据变化时刻
		m_uiSiteDataChangedTime = GetTickCount();
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Unlock();
}

/**
* 判断现场数据是否在规定时间内无新变化
* @param void
* @return bool true：是；false：否
*/
bool CSiteData::JudgeSiteDataChangedTime()
{
	bool bReturn = false;
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Lock();
		unsigned int uiTimeNow = GetTickCount();
		// // 现场数据有变化，且5秒内无新变化
		if((true == m_bSiteDataChanged) && (2000 < (uiTimeNow - m_uiSiteDataChangedTime)))
		{
			bReturn = true;
		}	
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Unlock();
	return bReturn;
}

/**
* 重置上次现场数据变化时刻
* @param void
* @return void
*/
void CSiteData::ResetSiteDataChangedTime()
{
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Lock();
		// 现场数据无变化
		m_bSiteDataChanged = false;
	// 为现场数据变化的线程同步对象
	m_oCriticalSectionSiteDataChanged.Unlock();
}

// /**
// * 得到仪器链接方向上对应的路由对象指针
// * @param CInstrument* pInstrument 仪器指针
// * @param unsigned int uiRoutDirection 连接方向  1-上；2-下；3-左；4右
// * @return CRout* NULL：仪器链接方向无对应的路由对象
// */
// CRout* CSiteData::GetRout(CInstrument* pInstrument, unsigned int uiRoutDirection)
// {
// 	CRout* pRout = NULL;
// 	if(NULL == pInstrument)
// 	{
// 		return NULL;
// 	}
// 	if(false == pInstrument->m_bInUsed)
// 	{
// 		return NULL;
// 	}
// 	// 判断路由方向
// 	switch(uiRoutDirection)
// 	{
// 	case 1:	// 上
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPTop, pRout))
// 			{
// 				return pRout;
// 			}
// 			break;
// 		}
// 	case 2:	// 下
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPDown, pRout))
// 			{
// 				return pRout;
// 			}
// 			break;
// 		}
// 	case 3:	// 左
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPLeft, pRout))
// 			{
// 				return pRout;
// 			}
// 			break;
// 		}
// 	case 4:	// 右
// 		{
// 			if(TRUE == m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIPRight, pRout))
// 			{
// 				return pRout;
// 			}
// 			break;
// 		}
// 	}
// 	return pRout;
// }
// 
// /**
// * 设置测试结果，基本测试
// * @param void
// * @return void
// */
// void CSiteData::SetTestResultBase(int iTestType, CTestElementList* pTestElementList)
// {
// 	CTestElementData* pTestElementData = NULL;
// 	CInstrument* pInstrument = NULL;
// 	for(unsigned int i = 0; i < pTestElementList->m_uiCountUsed; i++)
// 	{
// 		pTestElementData = pTestElementList->GetTestElementDataByIndex(i);
// 		if(TRUE == m_oIPInstrumentMap.GetInstrument(pTestElementData->m_uiIP, pInstrument))
// 		{
// 			switch(iTestType)
// 			{
// 				// 仪器测试
// 				// 仪器噪声
// 			case 4:
// 				pInstrument->m_fNoiseValue = pTestElementData->m_fInstrumentNoiseValue;
// 				pInstrument->m_byNoiseCheck = pTestElementData->m_byInstrumentNoiseCheck;
// 				break;
// 				// 	仪器畸变
// 			case 5:
// 				pInstrument->m_fDistortionValue = pTestElementData->m_fInstrumentDistortionValue;
// 				pInstrument->m_byDistortionCheck = pTestElementData->m_byInstrumentDistortionCheck;
// 				break;
// 				// 仪器串音
// 			case 7:	
// 				pInstrument->m_fCrosstalkValue = pTestElementData->m_fInstrumentCrosstalkValue;
// 				pInstrument->m_byCrosstalkCheck = pTestElementData->m_byInstrumentCrosstalkCheck;
// 				break;
// 				// 仪器增益和相位
// 			case 6:
// 				pInstrument->m_fGainValue = pTestElementData->m_fInstrumentGainValue;
// 				pInstrument->m_byGainCheck = pTestElementData->m_byInstrumentGainCheck;
// 				pInstrument->m_fPhaseValue = pTestElementData->m_fInstrumentPhaseValue;
// 				pInstrument->m_byPhaseCheck = pTestElementData->m_byInstrumentPhaseCheck;
// 				pInstrument->m_byGainAndPhaseCheck = pTestElementData->m_byInstrumentGainAndPhaseCheck;
// 				break;
// 				// 仪器共模
// 			case 8:
// 				pInstrument->m_fCMRRValue = pTestElementData->m_fInstrumentCMRRValue;
// 				pInstrument->m_byCMRRCheck = pTestElementData->m_byInstrumentCMRRCheck;
// 				break;
// 			}
// 			pInstrument->CheckResultAll();
// 			pTestElementData->m_byInstrumentCheck = pInstrument->m_byCheck;
// 		}
// 	}
// }