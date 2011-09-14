#pragma once

#include "Rout.h"

/**
*@brief 路由数据队列类
*/
class CRoutList
{
public:
	CRoutList();
	~CRoutList();

public: //属性
	/** 路由数组指针*/
	CRout* m_pArrayRout;
	/** 路由数组指针，为现场数据输出准备*/
	CRout* m_pArrayRoutOutput;
	/** 空闲路由队列*/
	CList<CRout*, CRout*> m_olsRoutFree;
	/** 路由总数*/
	unsigned int m_uiCountAll;
	/** 空闲路由数量*/
	unsigned int m_uiCountFree;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 得到一个空闲路由
	CRout* GetFreeRout();
	// 增加一个空闲路由
	void AddFreeRout(CRout* pRout);
	// 复制路由数组到输出数组
	void CopyRoutArrayForOutput();
};
