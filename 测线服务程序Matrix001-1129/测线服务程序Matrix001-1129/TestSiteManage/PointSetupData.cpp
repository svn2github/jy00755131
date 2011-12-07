#include "StdAfx.h"
#include "PointSetupData.h"

CPointSetupData::CPointSetupData()
{
}

CPointSetupData::~CPointSetupData()
{
}

/**
* 重新加载
* @param void
* @return void
*/
void CPointSetupData::OnReset()
{
	// 测点是否使用中
	m_bInUsed = false;
	// 点代码
	m_iPointCode = 0;

	// 测线号
	m_uiNbLine = 0;
	// 测点号
	m_uiNbPoint = 0;
	// 测点索引号
	m_uiIndexPoint = 0;
	// 是否哑点
	m_bMute = false;

	// 左侧链接的测点
	m_pPointLeft = NULL;
	// 左侧链接的测点
	m_pPointRight = NULL;
	// 上面链接的测点
	m_pPointlTop = NULL;
	// 下面链接的测点
	m_pPointlDown = NULL;

	// 测道数量
	m_uiChannelCount = 0;
	// 测道头指针
	m_pChannelHead = NULL;
	// 测道尾指针
	m_pChannelTail = NULL;
}

/**
* 根据测线号和测点号，生成测点索引号
* @param void
* @return void
*/
void CPointSetupData::CreateIndexPoint()
{
	// 测点索引号
	m_uiIndexPoint = CreateIndexPoint(m_uiNbLine, m_uiNbPoint);
}

/**
* 根据测线号和测点号，生成测点索引号
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return unsigned int 测点索引号
*/
// 
unsigned int CPointSetupData::CreateIndexPoint(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	// 测点索引号
	return uiNbLine * 1000000 + uiNbPoint * 10;
}

/**
* 根据链接方向，得到测道连接下一个的测道
* @param CChannelSetupData* pChannelSetupData 测道
* @param unsigned int uiDirection 链接方向 3：左；4：右
* @return CChannelSetupData* 测道连接下一个的测道 NULL：测道连接下一个的测道不存在
*/
CChannelSetupData* CPointSetupData::GetChannelRight(CChannelSetupData* pChannelSetupData, unsigned int uiDirection)
{
	// 判断方向
	switch(uiDirection)
	{
	case 3:	// 左
		{
			return GetChannelLeft(pChannelSetupData);
			break;
		}
	case 4:	// 右
		{
			return GetChannelRight(pChannelSetupData);
			break;
		}
	}
	return NULL;
}

/**
* 得到测道左边连接的测道
* @param CChannelSetupData* pChannelSetupData 测道
* @return CChannelSetupData* 测道连接下一个的测道 NULL：测道连接下一个的测道不存在
*/
CChannelSetupData* CPointSetupData::GetChannelLeft(CChannelSetupData* pChannelSetupData)
{
	// 测道为空
	if(NULL == pChannelSetupData)
	{
		return NULL;
	}
	// 测道为测点的测道头
	if(m_pChannelHead == pChannelSetupData)
	{
		return NULL;
	}
	// 得到测道左边连接的测道
	return pChannelSetupData->m_pChannelLeft;
}

/**
* 得到测道右边连接的测道
* @param CChannelSetupData* pChannelSetupData 测道
* @return CChannelSetupData* 测道连接下一个的测道 NULL：测道连接下一个的测道不存在
*/
CChannelSetupData* CPointSetupData::GetChannelRight(CChannelSetupData* pChannelSetupData)
{
	CChannelSetupData* pChannelSetupDataRight = NULL;
	// 测道为空
	if(NULL == pChannelSetupData)
	{
		return NULL;
	}
	// 测道为测点的测道尾
	if(m_pChannelTail == pChannelSetupData)
	{
		return NULL;
	}
	// 得到测道右边连接的测道
	return pChannelSetupData->m_pChannelRight;
}