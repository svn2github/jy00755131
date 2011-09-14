#pragma once

#include "SensorData.h"

/**
*@brief 检波器队列类
*/
class CSensorList
{
public:
	CSensorList();
	~CSensorList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 检波器队列*/
	CList<CSensorData*, CSensorData*> m_olsSensor;
	/** 检波器索引表*/
	CMap<unsigned int, unsigned int, CSensorData*, CSensorData*> m_oSensorMap;
	/** 检波器总数*/
	unsigned int m_uiCountAll;

	CSensorData* m_pSensorData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();
	// 处理配置文件更改	SurveySetup的ApplySensor
	void OnSetupDataChangeForSurveySetupApplySensor();

	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到检波器
	CSensorData* GetSensor(unsigned int uiNb);
};
