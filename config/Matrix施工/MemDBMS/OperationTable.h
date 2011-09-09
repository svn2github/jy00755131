#pragma once
#include "memobjbase.h"
#include "memmappingbase.h"

enum  VPSTATUS : unsigned char{ ToDO =0, Ready, Doing, Suspended , Done};

typedef struct tagSOURCESHOT
{
	
    tagSOURCESHOT()
		: m_VPStatus(ToDO)
		, m_dwShotNb(0)
		, m_byBreakPoint(0)
		, m_dwSwathNb(1)
		, m_dwSourcePointIndex(1)
		, m_fSourceLine(0)
		, m_fSourceNb(0)
		, m_bySpreadType(0)
		, m_dwSpreadSFL(0)
		, m_dwSpreadSFN(0)
		, m_bySuperSpread(0)
		, m_dwProcessTypeID(0)
	{

	}
	VPSTATUS	   m_VPStatus;
	unsigned char  m_byBreakPoint;
	unsigned long  m_dwShotNb;    
	unsigned long  m_dwSwathNb;
	unsigned long  m_dwSourcePointIndex;
	float          m_fSourceLine;        // SourceLine
	float          m_fSourceNb;          // SourceReceiver    
	unsigned long  m_dwSpreadSFL;
	unsigned long  m_dwSpreadSFN;
	unsigned char  m_bySpreadType;       // Absolute or Generic
	unsigned char  m_bySuperSpread;	     // 是否超级排列
	unsigned long  m_dwProcessTypeID;
	TCHAR          m_szComments[256];

}SOURCESHOT;

// typedef struct tagSOURCESHOT_MEM
// {
// 	tagSOURCESHOT_MEM()
// 	{
// 		m_dwPrevIndex=0;
// 		m_dwNextIndex=0;
// 	}
// 	SOURCESHOT		m_Shot;
// 	ULONG			m_dwPrevIndex;
// 	ULONG			m_dwNextIndex;
// }SOURCESHOT_MEM;

// class COperationTable :
// 	public CMemObjBase
// {
// 	
// public:
//     VPRECORD    m_VP;
// public:
// 	COperationTable(void);
// 	~COperationTable(void);
// 	// 将对象保存到共享内存中
// 	virtual bool Save(unsigned char* pData);
// 	// 从共享内存中装载对象数据
// 	virtual bool Load(unsigned char* pData);
// };

class CMemOperationTable :
	public CMemMappingBase
{
public:
	CMemOperationTable(void);
	~CMemOperationTable(void);
private:
	// 将对象保存到共享内存中
	bool Save(unsigned char* pData, SOURCESHOT* pVP);
	// 从共享内存中装载对象数据
	bool Load(unsigned char* pData, SOURCESHOT* pVP);

public:
	// 将对象保存到共享内存中
	bool Modify(unsigned long ulIndex,SOURCESHOT* pVP);
	// 从共享内存中装载对象数据
	bool Load(unsigned long ulIndex,SOURCESHOT* pVP);
	// 获得对象大小
	virtual ULONG GetObjectSize() { return (sizeof(SOURCESHOT)); }; // +sizeof(ULONG)*2)

	// 添加一条放炮记录
	unsigned long Append(SOURCESHOT* pShot);
	ULONG GetObjCount(void);
private:
	bool AppendUpdateArea(ULONG dwIndex);
};
