#pragma once

#include "SensorList.h"

/**
*@brief 点代码数据类
*/
class CPointCodeData
{
public:
	CPointCodeData();
	~CPointCodeData();

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 标签*/
	CString m_strLabel;
	/** 检波器类型*/
	CString m_strSensorType;

	/** 点代码对应的检波器数量*/
	int m_iSensorCount;
	/** 检波器列表*/
	CList<CSensorData*, CSensorData*> m_olsSensor;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 解析检波器类型
	void ParseSensorType(CSensorList* pSensorList);
	// 根据索引号，得到检波器
	CSensorData* GetSensorData(int iIndex);
};
