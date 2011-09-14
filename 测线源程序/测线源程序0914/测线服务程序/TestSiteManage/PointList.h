#pragma once

#include "PointSetupData.h"

/**
*@brief 测点数据队列类
*/
class CPointList
{
public:
	CPointList();
	~CPointList();

public: //属性
	/** 测点数组指针*/
	CPointSetupData* m_pArrayPoint;
	/** 测点总数*/
	unsigned int m_uiCountAll;
	/** 使用中测点数量*/
	unsigned int m_uiCountUsed;
	/** 空闲测点数量*/
	unsigned int m_uiCountFree;
	/** 测点索引表*/
	CMap<unsigned int, unsigned int, CPointSetupData*, CPointSetupData*> m_oPointMap;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 得到一个空闲测点
	CPointSetupData* GetFreePoint();
	// 增加一个测点到索引表
	void AddPointToMap(unsigned int uiIndex, CPointSetupData* pPoint);
	// 由索引表得到一个测点
	BOOL GetPointFromMap(unsigned int uiIndex, CPointSetupData* &pPoint);
	// 测点索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 得到一个测点
	CPointSetupData* GetPointByIndex(unsigned int uiIndex);
	// 得到测道
	CPointSetupData* GetPoint(unsigned int uiNbLine, unsigned int uiNbPoint);
};
