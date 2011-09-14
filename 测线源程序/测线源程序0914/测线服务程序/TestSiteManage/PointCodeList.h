#pragma once

#include "PointCodeData.h"

/**
*@brief 点代码数据队列类
*/
class CPointCodeList
{
public:
	CPointCodeList();
	~CPointCodeList();

	
public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 检波器队列指针*/
	CSensorList* m_pSensorList;

	/** 点代码队列*/
	CList<CPointCodeData*, CPointCodeData*> m_olsPointCode;
	/** 点代码索引表*/
	CMap<unsigned int, unsigned int, CPointCodeData*, CPointCodeData*> m_oPointCodeMap;
	/** 点代码总数*/
	unsigned int m_uiCountAll;

	CPointCodeData* m_pPointCodeData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到点代码
	CPointCodeData* GetPointCode(unsigned int uiNb);
};
