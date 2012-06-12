#include "stdafx.h"
#include "RoutList.h"

CRoutList::CRoutList(void)
{
}

CRoutList::~CRoutList(void)
{
}

// 初始化
void CRoutList::OnInit()
{
	m_uiCountAll = 2048;	// 路由总数
	m_uiCountUsed = 0;	// 使用路由数量	
	m_pArrayRout = new CRout[m_uiCountAll];	// 生成路由数组
}
// 关闭
void CRoutList::OnClose()
{
	// 删除路由数组
	delete[] m_pArrayRout;
}

// 重置
void CRoutList::OnReset()
{
	m_uiCountUsed = 0;	// 使用路由数量	
	m_oRoutMap.RemoveAll();	// 路由索引表
}

// 加载现场路由数据队列
void CRoutList::LoadSiteData()
{
	CString strFileName = "..\\data\\FileRout.dat";
	CFile oFile;
	BOOL bData;
	bData = oFile.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	ULONGLONG iByteCount = oFile.GetLength();
	unsigned int iRoutSize = sizeof(CRout);
	if(iByteCount != iRoutSize * m_uiCountAll)
	{
		// 错误SOS******
		oFile.Close();
		return;
	}
	iByteCount = oFile.Read(m_pArrayRout, iRoutSize * m_uiCountAll);
	oFile.Close();

	m_uiCountUsed = 0;	// 使用中路由数量
	CRout* pRout;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pRout = &m_pArrayRout[i];
		if(true == pRout->m_bInUsed)
		{
			m_oRoutMap.SetAt(pRout->m_uiRoutIP, pRout);	// 加入路由地址索引表
			m_uiCountUsed++;	// 使用中路由数量
		}
	}	
}