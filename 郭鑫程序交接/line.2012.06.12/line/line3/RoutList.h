#pragma once

#include "Rout.h"

class CRoutList
{
public:
	CRoutList(void);
	~CRoutList(void);

public: //属性
	
	CRout* m_pArrayRout;	// 路由数组指针	
	unsigned int m_uiCountAll;	// 路由总数	
	unsigned int m_uiCountUsed;	// 使用路由数量
	
	CMap<unsigned int, unsigned int, CRout*, CRout*> m_oRoutMap;	// 路由地址索引表

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();

	// 加载现场路由数据队列
	void LoadSiteData();
};
