#include "StdAfx.h"
#include "ChannelList.h"

CChannelList::CChannelList(void)
{
}

CChannelList::~CChannelList(void)
{
}

// 初始化
void CChannelList::OnInit()
{
	// 生成测道数组
	m_pArrayChannel = new CChannelSetupData[m_uiCountAll];
	// 使用中测道数量
	m_uiCountUsed = 0;
	// 空闲测道数量
	m_uiCountFree = m_uiCountAll;

	m_uiNbPointMin = 100000;	// 最小测点号
	m_uiNbPointMax = 0;	// 最大测点号
	m_uiPointChannelCountMax = 0;	// 测点上最大测道数量
	m_uiNbLineCount = 0;	// 测线数量
	m_olsNbLine.RemoveAll();	// 测线号队列
}

// 关闭
void CChannelList::OnClose()
{
	// 删除测道数组
	delete[] m_pArrayChannel;
	// 清空索引表
	m_oChannelMap.RemoveAll();
}

// 重置
void CChannelList::OnReset()
{
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测道数量
	m_uiCountFree = m_uiCountAll;
	// 清空索引表
	m_oChannelMap.RemoveAll();

	m_uiNbPointMin = 100000;	// 最小测点号
	m_uiNbPointMax = 0;	// 最大测点号
	m_uiPointChannelCountMax = 0;	// 测点上最大测道数量
	m_uiNbLineCount = 0;	// 测线数量
	m_olsNbLine.RemoveAll();	// 测线号队列
	m_oNbLineMap.RemoveAll();	// 测线号索引表
}

// 增加一个测道到索引表
void CChannelList::AddChannelToMap(unsigned int uiIndex, CChannelSetupData* pChannel)
{
	m_oChannelMap.SetAt(uiIndex, pChannel);
}

// 测道索引号是否已加入索引表
BOOL CChannelList::IfIndexExistInMap(unsigned int uiIndex)
{
	CChannelSetupData* pChannel = NULL;
	return m_oChannelMap.Lookup(uiIndex, pChannel);
}

// 由索引表得到一个测道
BOOL CChannelList::GetChannelFromMap(unsigned int uiIndex, CChannelSetupData* &pChannel)
{
	return m_oChannelMap.Lookup(uiIndex, pChannel);
}

// 得到一个测道
CChannelSetupData* CChannelList::GetChannelByIndex(unsigned int uiIndex)
{
	return &m_pArrayChannel[uiIndex];
}

// 得到测道
CChannelSetupData* CChannelList::GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	CChannelSetupData* pChannel = NULL;
	unsigned int uiIP = CChannelSetupData::CreateIP(uiNbLine, uiNbPoint, uiNbChannel);
	if(TRUE == GetChannelFromMap(uiIP, pChannel))
	{
		return pChannel;
	}
	return NULL;
}

// 加载现场测道数据队列
void CChannelList::LoadSiteData()
{
	CString strFileName = "FileChannel.dat";
	CFile oFile;
	BOOL bData;
	bData = oFile.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	ULONGLONG iByteCount = oFile.GetLength();
	unsigned int iChannelSize = sizeof(CChannelSetupData);
	if(iByteCount != iChannelSize * m_uiCountAll)
	{
		// 错误SOS******
		oFile.Close();
		return;
	}
	iByteCount = oFile.Read(m_pArrayChannel, iChannelSize * m_uiCountAll);
	oFile.Close();

	m_uiCountUsed = 0;	// 使用中测道数量
	m_uiNbLineCount = 0;	// 测线数量
	unsigned int uiNbLinePreview = 0;	//上一个测线号
	CChannelSetupData* pChannelSetupData;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pChannelSetupData = &m_pArrayChannel[i];
		if(true == pChannelSetupData->m_bInUsed)
		{
			if(pChannelSetupData->m_uiNbPoint < m_uiNbPointMin)
			{
				m_uiNbPointMin = pChannelSetupData->m_uiNbPoint;	// 最小测点号
			}
			if(pChannelSetupData->m_uiNbPoint > m_uiNbPointMax)
			{
				m_uiNbPointMax = pChannelSetupData->m_uiNbPoint;	// 最大测点号
			}
			if(pChannelSetupData->m_uiNbChannel > m_uiPointChannelCountMax)
			{
				m_uiPointChannelCountMax = pChannelSetupData->m_uiNbChannel;	// 测点上最大测道数量
			}
			if(uiNbLinePreview != pChannelSetupData->m_uiNbLine)
			{			
				m_olsNbLine.AddTail(pChannelSetupData->m_uiNbLine);		// 测线号队列
				m_oNbLineMap.SetAt(pChannelSetupData->m_uiNbLine, m_uiNbLineCount);	// 测线号索引表
				m_uiNbLineCount++;	// 测线数量
				uiNbLinePreview = pChannelSetupData->m_uiNbLine;				
			}

			m_oChannelMap.SetAt(pChannelSetupData->m_uiIP, pChannelSetupData);	// 加入索引表
			m_uiCountUsed++;	// 使用中测道数量
		}
		else
		{
			break;
		}
	}	
	m_uiCountFree = m_uiCountAll - m_uiCountUsed;	// 空闲测道数量
}