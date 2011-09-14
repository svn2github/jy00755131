#pragma once

#include "ChannelSetupData.h"

/**
*@brief 测点数据类
*/
class CPointSetupData
{
public:
	CPointSetupData();
	~CPointSetupData();

public: // 属性
	/** 在测点数组中的位置*/
	unsigned int m_uiIndex;

	/** 测点是否使用中*/
	bool m_bInUsed;
	/** 点代码*/
	int m_iPointCode;

	/** 测线号*/
	unsigned int m_uiNbLine;
	/** 测点号*/
	unsigned int m_uiNbPoint;
	/** 测点索引号*/
	unsigned int m_uiIndexPoint;
	/** 是否哑点*/
	bool m_bMute;

	/** 左侧链接的测点*/
	CPointSetupData* m_pPointLeft;
	/** 左侧链接的测点*/
	CPointSetupData* m_pPointRight;
	/** 上面链接的测点*/
	CPointSetupData* m_pPointlTop;
	/** 下面链接的测点*/
	CPointSetupData* m_pPointlDown;

	/** 测道数量*/
	unsigned int m_uiChannelCount;
	/** 测道头指针*/
	CChannelSetupData* m_pChannelHead;
	/** 测道尾指针*/
	CChannelSetupData* m_pChannelTail;

public: // 方法
	// 重置
	void OnReset();

	// 根据测线号和测点号，生成测点索引号
	void CreateIndexPoint();
	// 根据测线号和测点号，生成测点索引号
	static unsigned int CreateIndexPoint(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 得到测道连接的测道
	CChannelSetupData* GetChannelRight(CChannelSetupData* pChannelSetupData, unsigned int uiDirection);
	// 得到测道左边连接的测道
	CChannelSetupData* GetChannelLeft(CChannelSetupData* pChannelSetupData);
	// 得到测道右边连接的测道
	CChannelSetupData* GetChannelRight(CChannelSetupData* pChannelSetupData);
};
