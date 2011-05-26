#pragma once

/**
*@brief 检波器数据类
*/
class CSensorData
{
public:
	CSensorData();
	~CSensorData();

public: //属性
	/** 检波器号*/
	unsigned int m_uiNb;
	/** SegdCode*/
	unsigned int m_uiSegdCode;
	/** 标签*/
	CString m_strLabel;
	/** ContinuityMin*/
	float m_fContinuityMin;
	/** ContinuityMax*/
	float m_fContinuityMax;
	/** Tilt*/
	float m_fTilt;
	/** Nois*/
	float m_fNoise;
	/** LeaKage*/
	float m_fLeaKage;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
