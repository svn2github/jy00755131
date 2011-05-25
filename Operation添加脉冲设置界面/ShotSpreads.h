#pragma once
#include "../common/OperaStruct.h"
/******************************************************************************
    类    型：CShotSpreadEx
        
    功    能：从CShotSpread继承，增加采集排列的

    修改历史：

******************************************************************************
class CShotSpreadEx : CShotSpread
{
public:
	CArray<CString,LPCTSTR>	m_arrSpreadDesc;
public:
	CShotSpreadEx(void);
	~CShotSpreadEx(void);
};
*/
/******************************************************************************
    类    型：CShotSpreads
        
    功    能：所有的采集排列

    修改历史：

*******************************************************************************/
/**
 * @class CShotSpreads
 * @brief 采集排列表
 *
 * 定义采集排列表。
 */
class CShotSpreads
{
	// 属性
public:
	CArray<CShotSpread*,CShotSpread*>	m_AllSpread;
public:
	CShotSpreads(void);
	~CShotSpreads(void);

	// 得到指定索引的采集排列，索引从0开始
	CShotSpread* GetShotSpread(int iIndex);
	CShotSpread* GetShotSpreadBySN(DWORD dwSN);
	int GetCount(void);
	// 增加一个对象
	int Add(CShotSpread* pSpread);	

};
