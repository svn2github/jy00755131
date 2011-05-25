#pragma once
#include "../common/OperaStruct.h"

/**
 * @class CShotSources
 * @brief 震源表
 *
 * 封装所有的震源对象，包括脉冲和可控震源。
 */
class CShotSources
{
// 属性
public:
	CArray<CShotSource*,CShotSource*>	m_AllSrc;
// 方法
public:
	CShotSources(void);
	~CShotSources(void);

	// 得到指定索引的震源，索引从0开始
	CShotSource* GetShotSource(int iIndex);
	int GetCount(void);
	// 增加一个震源
	int Add(CShotSource* pSrc);
	// 通过Nb查找震源对象
	CShotSource* GetShotSourceByNb(DWORD dwNb);
	// 删除所有震源对象
	void RemoveAll(void);
	// 将另一个震源表数组对象拷贝到本对象中，不另外复制震源对象
	void Copy(CShotSources* pShotSrcs);
	// 设置某一位置的对象
	void SetAt(int nIndex,CShotSource* pShotSrc);
};
