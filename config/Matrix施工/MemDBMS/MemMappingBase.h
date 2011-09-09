#pragma once
#include <afxmt.h>

class CMemMappingBase
{
public:
	CMemMappingBase(void);
	~CMemMappingBase(void);

protected:	
	// 自定义更新消息
	UINT	  m_dwMessage;
	// 多进程访问同步对象
	CMutex*   m_pMutex;
	// 内存映射区全局名称
	CString   m_strMemName;
	//内存映射区句柄
	HANDLE    m_hMemMap;
    
	// 指向内存映射区首地址
	BYTE*     m_pData;	
	// 内存映射区字节大小
	ULONG     m_dwSize;
	// 指向内存映射区内保存对象的首地址
	BYTE*     m_pObjArea;

	// 数据更新标志区首地址，为环形缓冲区
	BYTE*     m_pUpdateArea;
	// 内存映射区内分配的数据更新区字节大小
	ULONG     m_dwUpdateAreaSize;
	// 写更新标志区指针
	ULONG     m_dwUpdateWrite;
	// 读更新标志区指针
	ULONG     m_dwUpdateRead;


public:
	// 创建内存映射区
	bool Create(void);
	// 释放内存映射区
	bool Destroy(void);	
	// 得到保存的对象大小
	virtual ULONG GetObjectSize()=0; 
	
protected:
	bool AppendUpdate(unsigned long dwIndex);
};
