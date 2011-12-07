#pragma once

#include "MarkerData.h"

/**
*@brief 迂回道数据类
*/
class CDetourData
{
public:
	CDetourData();
	~CDetourData();

public: //属性
	/** 低端仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiBoxTypeLow;
	/** 低端仪器SN号*/
	unsigned int m_uiSNLow;
	/** 低端测道号*/
	unsigned int m_uiChannelNbLow;
	/** 高端仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiBoxTypeHigh;
	/** 高端仪器SN号*/
	unsigned int m_uiSNHigh;
	/** 高端测道号*/
	unsigned int m_uiChannelNbHigh;
	/** 停止分配IP地址标记 0:继续；1停止*/
	unsigned int m_uiStopMarking;

	/** 低端标记点*/
	CMarkerData m_oMarkerLow;
	/** 高端标记点*/
	CMarkerData m_oMarkerHigh;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
};
