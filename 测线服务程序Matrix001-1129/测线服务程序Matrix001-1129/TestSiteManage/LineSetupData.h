#pragma once

#include "PointSetupData.h"

/**
*@brief 测线数据类
*/
class CLineSetupData
{
public:
	CLineSetupData();
	~CLineSetupData();

public: // 属性
	/** 在测线数组中的位置*/
	unsigned int m_uiIndex;

	/** 测线是否使用中*/
	bool m_bInUsed;

	/** 测线号*/
	unsigned int m_uiNbLine;

	/** 上面链接的测线*/
	CLineSetupData* m_pLinelTop;
	/** 下面链接的测线*/
	CLineSetupData* m_pLinelDown;

	/** 测点数量*/
	unsigned int m_uiPointCount;
	/** 测点头指针*/
	CPointSetupData* m_pPointHead;
	/** 测点尾指针*/
	CPointSetupData* m_pPointTail;

	/** 测道数量*/
	unsigned int m_uiChannelCount;
	/** 测道头指针*/
	CChannelSetupData* m_pChannelHead;
	/** 测道尾指针*/
	CChannelSetupData* m_pChannelTail;

	/** 测线上的测点定义*/
	CString m_strReceiverSection;

public: // 方法
	// 重置
	void OnReset();
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 得到测点连接的测点
	CPointSetupData* GetPointByDirection(CPointSetupData* pPointSetupData, unsigned int uiDirection);
	// 得到测点左边连接的测点
	CPointSetupData* GetPointLeft(CPointSetupData* pPointSetupData);
	// 得到测点右边连接的测点
	CPointSetupData* GetPointRight(CPointSetupData* pPointSetupData);
};
