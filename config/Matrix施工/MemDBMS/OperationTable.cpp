#include "StdAfx.h"
#include "OperationTable.h"



CMemOperationTable::CMemOperationTable(void)
{
	m_strMemName=_T("放炮表2010");
	m_dwSize = 1024*1024;
}

CMemOperationTable::~CMemOperationTable(void)
{
}
bool CMemOperationTable::Save(unsigned char* pData,SOURCESHOT* pVP)
{
	if(NULL==pData || NULL==pVP)
		return false;

	int ret;
	
	CSingleLock ObjLock(m_pMutex,TRUE);
	ret=memcpy_s(pData,sizeof(SOURCESHOT),pVP,sizeof(SOURCESHOT));	
	ObjLock.Unlock();

	if(!ret)
		return false;
    
	return true;
}

bool CMemOperationTable::Load(unsigned char* pData,SOURCESHOT* pVP)
{
	if(NULL==pData || NULL==pVP)
		return false;
	
	int ret;
	CSingleLock ObjLock(m_pMutex,TRUE);
	ret = memcpy_s(pVP,sizeof(SOURCESHOT),pData,sizeof(SOURCESHOT) );
	ObjLock.Unlock();
	if(!ret)
		return false;

	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 读取内存中保存的对象个数

    修改历史：

*******************************************************************************/
ULONG CMemOperationTable::GetObjCount(void)
{
	ULONG  dwCount=0;
	CSingleLock ObjLock(m_pMutex,TRUE);
	memcpy_s(&dwCount,sizeof(ULONG),m_pUpdateArea+m_dwUpdateAreaSize,sizeof(ULONG));
	ObjLock.Unlock();
	return dwCount;
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：  修改一个对象，不增加对象的总数

    修改历史：

*******************************************************************************/
bool CMemOperationTable::Modify(unsigned long ulIndex,SOURCESHOT* pVP)
{
	if(NULL==pVP)
		return false;
	if(Save(m_pObjArea+ulIndex*GetObjectSize(),pVP))
	{
		PostMessage(HWND_BROADCAST,m_dwMessage,0,ulIndex);
		return true;
	}
	return false;
	
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：SOURCESHOT* pVP；
  
    返 回 值：
    
    功    能：  读取一个对象

    修改历史：

*******************************************************************************/
bool CMemOperationTable::Load(unsigned long ulIndex,SOURCESHOT* pVP)
{
	if(NULL==pVP)
		return false;

	return Load(m_pObjArea+ulIndex*GetObjectSize(),pVP);

}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值： unsigned long, 添加对象的索引号
    
    功    能： 在当前对象区的最后追加一条放炮记录,同时要内存对象的总数要加1

    修改历史：

*******************************************************************************/
unsigned long  CMemOperationTable::Append(SOURCESHOT* pShot)
{
	ULONG  dwCount=0;
	ULONG  dwUpdateWrite = sizeof(ULONG);
	CSingleLock ObjLock(m_pMutex,TRUE);
	// 读对象的总数
	memcpy_s(&dwCount,sizeof(ULONG),m_pUpdateArea+m_dwUpdateAreaSize,sizeof(ULONG));
	// 写入到对象区最后的地址空间
    memcpy_s(m_pObjArea+ dwCount* GetObjectSize() ,sizeof(SOURCESHOT), pShot , sizeof(SOURCESHOT));
    dwCount++;
	// 对象的总数加 1
	memcpy_s(m_pUpdateArea+m_dwUpdateAreaSize,sizeof(ULONG),&dwCount,sizeof(ULONG));
	
	// 读对象的总数	
	memcpy_s(&dwUpdateWrite,sizeof(ULONG),m_pUpdateArea,sizeof(ULONG));
	if(dwUpdateWrite<=m_dwUpdateAreaSize-sizeof(ULONG))
	{
		memcpy_s(m_pUpdateArea+dwUpdateWrite,sizeof(ULONG),&dwCount,sizeof(ULONG));
		dwUpdateWrite+=sizeof(ULONG);
		memcpy_s(m_pUpdateArea,sizeof(ULONG),&dwUpdateWrite,sizeof(ULONG));
	}
	else
	{
		// 更新环形缓冲区溢出处理
	}
	ObjLock.Unlock();
	PostMessage(HWND_BROADCAST,m_dwMessage,0,dwCount);
	return dwCount;
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：  更新数据后写更新标记

    修改历史：

*******************************************************************************/
bool CMemOperationTable::AppendUpdateArea(ULONG dwIndex)
{
	return false;
}
