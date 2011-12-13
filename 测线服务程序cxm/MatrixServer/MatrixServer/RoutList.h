#pragma once

#include "Rout.h"
#include <hash_map>
#include <list>
using stdext::hash_map;
using std::list;
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
	list<CRout*> m_olsRoutFree;
	// 仪器路由地址索引表
	hash_map<unsigned int, CRout*> m_oRoutMap;
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
	// 增加一个路由
	void AddRout(unsigned int uiIndex, CRout* pRout);
	// 得到一个路由
	BOOL GetRout(unsigned int uiIndex, CRout* &pRout);
	// 删除一个路由
	void DeleteRout(unsigned int uiIndex);
	// 路由地址是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiRoutIP);
};
