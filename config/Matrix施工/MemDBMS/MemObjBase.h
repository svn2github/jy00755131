#pragma once

class CMemObjBase
{
public:
	CMemObjBase(void);
	~CMemObjBase(void);
	// 将对象保存到共享内存中
	virtual bool Save(unsigned char* pData) = 0;
	// 从共享内存中装载对象数据
	virtual bool Load(unsigned char* pData) = 0;
};
