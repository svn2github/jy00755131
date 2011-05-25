#pragma once
#include "../common/OperaStruct.h"
/**
 * @class CShotPoints
 * @brief 放炮表
 *
 * 描述放炮表以及对放炮表的一些访问操作。
 */
class CShotPoints
{
// 属性
public:
	CArray<CShotPoint*,CShotPoint*>	m_AllVP;
// 方法
public:
	CShotPoints(void);
	~CShotPoints(void);
	// 得到指定索引的炮号，索引从0开始
	CShotPoint* GetShotPoint(int iIndex);
	int GetCount(void);
	// 增加一个炮号
	int Add(CShotPoint* pVP);

	CShotPoint* GetShotPointByNb(DWORD dwNb);

	void   RemoveAll(void);
	void   Copy(CShotPoints*  pPoints);
};
