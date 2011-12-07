#include "StdAfx.h"
#include "LineSetupData.h"

CLineSetupData::CLineSetupData()
{
}

CLineSetupData::~CLineSetupData()
{
}

/**
* 重置
* @param void
* @return void
*/
void CLineSetupData::OnReset()
{
	// 测线是否使用中
	m_bInUsed = false;

	// 测线号
	m_uiNbLine = 0;

	// 上面链接的测线
	m_pLinelTop = NULL;
	// 下面链接的测线
	m_pLinelDown = NULL;

	// 测点数量
	m_uiPointCount = 0;
	// 测点头指针
	m_pPointHead = NULL;
	// 测点尾指针
	m_pPointTail = NULL;

	// 测道数量
	m_uiChannelCount = 0;
	// 测道头指针
	m_pChannelHead = NULL;
	// 测道尾指针
	m_pChannelTail = NULL;

	// 测线上的测点定义
	m_strReceiverSection = "";
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CLineSetupData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名	
	// 得到测线号
	strKey = "LineName";
	m_uiNbLine = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 得到测线的测点定义
	strKey = "ReceiverSection";
	m_strReceiverSection = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 根据链接方向，得到测点连接下一个的测点
* @param CPointSetupData* pPointSetupData 测点
* @param unsigned int uiDirection 链接方向 3：左；4：右
* @return CPointSetupData* 测点连接下一个的测点 NULL：测点连接下一个的测点不存在
*/
CPointSetupData* CLineSetupData::GetPointByDirection(CPointSetupData* pPointSetupData, unsigned int uiDirection)
{
	// 判断方向
	switch(uiDirection)
	{
	case 3:	// 左
		{
			return GetPointLeft(pPointSetupData);
			break;
		}
	case 4:	// 右
		{
			return GetPointRight(pPointSetupData);
			break;
		}
	}
	return NULL;
}

/**
* 得到测点左边连接的测点
* @param CPointSetupData* pPointSetupData 测点
* @return CPointSetupData* 测点连接下一个的测点 NULL：测点连接下一个的测点不存在
*/
CPointSetupData* CLineSetupData::GetPointLeft(CPointSetupData* pPointSetupData)
{
	// 测点为空
	if(NULL == pPointSetupData)
	{
		return NULL;
	}
	// 测点为测点的测点头
	if(m_pPointHead == pPointSetupData)
	{
		return NULL;
	}
	// 得到测点左边连接的测点
	return pPointSetupData->m_pPointLeft;
}

/**
* 得到测点右边连接的测点
* @param CPointSetupData* pPointSetupData 测点
* @return CPointSetupData* 测点连接下一个的测点 NULL：测点连接下一个的测点不存在
*/
CPointSetupData* CLineSetupData::GetPointRight(CPointSetupData* pPointSetupData)
{
	CPointSetupData* pPointSetupDataRight = NULL;
	// 测点为空
	if(NULL == pPointSetupData)
	{
		return NULL;
	}
	// 测点为测点的测点尾
	if(m_pPointTail == pPointSetupData)
	{
		return NULL;
	}
	// 得到测点右边连接的测点
	return pPointSetupData->m_pPointRight;
}