#pragma once
#include "../common/OperaStruct.h"
/******************************************************************************
    类    型：CProcessTypes
        
    功    能：

    修改历史：

*******************************************************************************/
/**
 * @class CProcessTypes
 * @brief 处理类型表对象
 *
 * 封装所有的处理类型对象。
 */
class CProcessTypes
{

// 属性
public:
	CArray<CProcessType*,CProcessType*>	m_AllProcess;
public:
	CProcessTypes(void);
	~CProcessTypes(void);

	// 得到指定索引的对象，索引从0开始
	CProcessType* GetProcessType(int iIndex);
	int GetCount(void);
	// 添加一个对象
	int Add(CProcessType* pProcess);
	// 
	CProcessType* GetProcessTypeByNb(DWORD dwNb);
	int  GetProcessIndexByNb(DWORD dwNb);
	void RemoveAll(void);
	// 完全复制所有的处理类型，包括对象数据
	// void Replicate(CProcessTypes* pProcessTypes);
};
