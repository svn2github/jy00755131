#pragma once

/**
*@brief 标记点数据类
*/
class CMarkerData
{
public:
	CMarkerData();
	~CMarkerData();

public: //属性
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 测线号*/
	unsigned int m_uiLineName;
	/** 测点号*/
	unsigned int m_uiPointNb;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 标记点增量*/
	unsigned int m_uiMarkerIncr;
	/** 翻转标记方向标记 0:不翻转；1：翻转*/
	unsigned int m_uiReversed;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
