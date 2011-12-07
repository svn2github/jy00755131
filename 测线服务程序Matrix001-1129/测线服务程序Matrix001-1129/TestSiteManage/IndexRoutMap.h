#pragma once

#include "Rout.h"
#include "Instrument.h"

/**
*@brief 路由索引表类
*/
class CIndexRoutMap
{
public:
	CIndexRoutMap();
	~CIndexRoutMap();

public: //属性
	/** 路由索引表*/
	CMap<unsigned int, unsigned int, CRout*, CRout*> m_oRoutMap;

	CRout* m_pRout;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 增加一个路由
	void AddRout(unsigned int uiIndex, CRout* pRout);
	// 得到一个路由
	BOOL GetRout(unsigned int uiIndex, CRout* &pRout);
	// 删除一个路由
	void DeleteRout(unsigned int uiIndex);
	// 删除所有路由
	void DeleteRoutAll();
	// 路由地址是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiRoutIP);
	// 判断路由是否是最久没有时延修正的路由
	bool JudgeRoutIsLongestTime(CRout* pRout);
};
