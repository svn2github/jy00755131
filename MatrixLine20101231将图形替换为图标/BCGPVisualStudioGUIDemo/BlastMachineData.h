#pragma once

/**
*@brief 爆炸机控制器数据类
*/
class CBlastMachineData
{
public:
	CBlastMachineData();
	~CBlastMachineData();

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 标签*/
	CString m_strLabel;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN号*/
	unsigned int m_uiSN;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 增益*/
	unsigned int m_uiGain;
	/** 地震队号*/
	unsigned int m_uiDPGNb;
	/** 注释*/
	CString m_strComments;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
