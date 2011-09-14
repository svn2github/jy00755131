#pragma once

#include "BlastMachineData.h"

/**
*@brief 爆炸机控制器数据队列类
*/
class CBlastMachineList
{
public:
	CBlastMachineList();
	~CBlastMachineList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 爆炸机控制器队列*/
	CList<CBlastMachineData*, CBlastMachineData*> m_olsBlastMachine;
	/** 爆炸机控制器总数*/
	unsigned int m_uiCountAll;

	/** 爆炸机控制器数据指针*/
	CBlastMachineData* m_pBlastMachineData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();
	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到爆炸机控制器
	CBlastMachineData* GetBlastMachine(unsigned int uiIndex);
};
