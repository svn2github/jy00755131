#pragma once

/**
*@brief FormLine数据类
*/
class CFormLineData
{
public:
	CFormLineData();
	~CFormLineData();

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN号*/
	unsigned int m_uiSN;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
