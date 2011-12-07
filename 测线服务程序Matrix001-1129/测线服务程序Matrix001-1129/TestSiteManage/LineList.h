#pragma once

#include "LineSetupData.h"

/**
*@brief 测线数据队列类
*/
class CLineList
{
public:
	CLineList();
	~CLineList();

public: //属性
	/** 测线数组指针*/
	CLineSetupData* m_pArrayLine;
	/** 测线总数*/
	unsigned int m_uiCountAll;
	/** 使用中测线数量*/
	unsigned int m_uiCountUsed;
	/** 空闲测线数量*/
	unsigned int m_uiCountFree;
	/** 测线索引表*/
	CMap<unsigned int, unsigned int, CLineSetupData*, CLineSetupData*> m_oLineMap;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 得到一个空闲测线
	CLineSetupData* GetFreeLine();
	// 增加一个测线到索引表
	void AddLineToMap(unsigned int uiIndex, CLineSetupData* pLine);
	// 由索引表得到一个测线
	BOOL GetLineFromMap(unsigned int uiIndex, CLineSetupData* &pLine);
	// 测线索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 得到一个测线
	CLineSetupData* GetLineByIndex(unsigned int uiIndex);
	// 得到测线
	CLineSetupData* GetLine(unsigned int uiNbLine);

};
