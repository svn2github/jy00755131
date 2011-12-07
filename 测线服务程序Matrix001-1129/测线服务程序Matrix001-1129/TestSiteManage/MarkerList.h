#pragma once

#include "MarkerData.h"

/**
*@brief 标记点队列类
*/
class CMarkerList
{
public:
	CMarkerList();
	~CMarkerList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 标记点队列*/
	CList<CMarkerData*, CMarkerData*> m_olsMarker;
	/** 标记点总数*/
	unsigned int m_uiCountAll;

	CMarkerData* m_pMarkerData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到标记点
	CMarkerData* GetMarker(unsigned int uiIndex);
};
