#include "StdAfx.h"
#include "MemMappingBase.h"

CMemMappingBase::CMemMappingBase(void)
: m_pMutex(NULL)
, m_strMemName(_T("内存对象"))
, m_hMemMap(NULL)
, m_pData(NULL)
, m_dwMessage(0)
, m_dwSize(1024*1024)
, m_pObjArea(NULL)
, m_pUpdateArea(NULL)
, m_dwUpdateAreaSize(2044)
{
}

CMemMappingBase::~CMemMappingBase(void)
{
	Destroy();
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：创建内存映射区，内存映射区名称 m_strMemName ，大小为****

    修改历史：

*******************************************************************************/
bool CMemMappingBase::Create(void)
{
	if(NULL == m_pMutex)
	{
		m_pMutex = new CMutex(FALSE,m_strMemName);
	}
	m_dwMessage = RegisterWindowMessage(m_strMemName);
	CSingleLock  ObjLock(m_pMutex,TRUE);
	m_hMemMap = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,m_dwSize, m_strMemName);
	if(NULL == m_hMemMap)
	{
		return false;
	}
	 
	m_pData = (BYTE*)MapViewOfFile(m_hMemMap,FILE_MAP_WRITE,0,0,0);
	if(NULL==m_pData)
	{
		CloseHandle(m_hMemMap);
		m_hMemMap = NULL;
		return false;
	}
	
	// 首先是数据更新区，大小为m_dwUpdateAreaSize
	m_pUpdateArea = m_pData;
	// 对象区在数据更新区和对象个数后面
	m_pObjArea = m_pData + m_dwUpdateAreaSize + sizeof(ULONG);
    // 一定要清零，至少数据更新区和对象个数要清零
	memset(m_pData,0,m_dwSize);
	ObjLock.Unlock();
	return true;
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：释放内存映射区，清理内存

    修改历史：

*******************************************************************************/
bool CMemMappingBase::Destroy(void)
{
	if(m_pMutex)
	{
		delete m_pMutex;
		m_pMutex = NULL;
	}
	if(m_pData)
	{
		UnmapViewOfFile(m_pData);
		m_pData = NULL;
	}
	if(m_hMemMap)
	{
		CloseHandle(m_hMemMap);
		m_hMemMap = NULL;
	}

	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：  向更新环形区追加一条记录

    修改历史：

*******************************************************************************/
bool CMemMappingBase::AppendUpdate(unsigned long dwIndex)
{
	ULONG  dwUpdateWrite = sizeof(ULONG);
	// 读对象的总数	
	memcpy_s(&dwUpdateWrite,sizeof(ULONG),m_pUpdateArea,sizeof(ULONG));
	if(dwUpdateWrite<=m_dwUpdateAreaSize-sizeof(ULONG))
	{
		memcpy_s(m_pUpdateArea+dwUpdateWrite,sizeof(ULONG),&dwIndex,sizeof(ULONG));
		dwUpdateWrite+=sizeof(ULONG);
	}
	else
	{
		// 更新环形缓冲区溢出处理
	}

	return true;
}
