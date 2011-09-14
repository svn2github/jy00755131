#include "StdAfx.h"
#include "IPList.h"

CIPList::CIPList()
{
}

CIPList::~CIPList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CIPList::OnInit()
{
	// 空闲IP地址数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// IP地址加在空闲IP地址队列尾部
		m_olsIPFree.AddTail( i + 2000000000 + 200);
	}
}

/**
* 关闭
* @param void
* @return void
*/
void CIPList::OnClose()
{
	// 清空空闲IP地址队列
	m_olsIPFree.RemoveAll();
}

/**
* 重置
* @param void
* @return void
*/
void CIPList::OnReset()
{
	// 清空空闲IP地址队列
	m_olsIPFree.RemoveAll();
	// 空闲IP地址数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// IP地址加在空闲IP地址队列尾部
		m_olsIPFree.AddTail( i + 2000000000 + 200);
	}
}

/**
* 得到一个空闲仪器IP地址
* @param void
* @return unsigned int 空闲IP地址 0：没有空闲IP地址
*/
unsigned int CIPList::GetFreeInstrumentIP()
{
	unsigned int uiIP = 0;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		uiIP = m_olsIPFree.RemoveHead();
		m_uiCountFree--;
	}

	return uiIP;
}

/**
* 增加一个空闲仪器
* @param unsigned int uiIP IP地址
* @return void
*/
void CIPList::AddFreeInstrumentIP(unsigned int uiIP)
{
	//加入空闲队列
	m_olsIPFree.AddTail(uiIP);
	m_uiCountFree++;
}

/**
* 得到一个空闲交叉站IP地址
* @param void
* @return unsigned int 空闲IP地址 0：没有空闲IP地址
*/
unsigned int CIPList::GetFreeCrossIP()
{
	unsigned int uiIP = 0;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		uiIP = m_olsIPFree.RemoveHead();
		m_uiCountFree--;
	}

	return uiIP;
}

/**
* 增加一个空闲交叉站IP地址
* @param unsigned int uiIP IP地址
* @return void
*/
void CIPList::AddFreeCrossIP(unsigned int uiIP)
{
	//加入空闲队列
	m_olsIPFree.AddTail(uiIP);
	m_uiCountFree++;
}

/**
* 得到一个空闲电源站IP地址
* @param void
* @return unsigned int 空闲IP地址 0：没有空闲IP地址
*/
unsigned int CIPList::GetFreePowerIP()
{
	unsigned int uiIP = 0;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		uiIP = m_olsIPFree.RemoveHead();
		m_uiCountFree--;
	}

	return uiIP;
}

/**
* 增加一个空闲电源站IP地址
* @param unsigned int uiIP IP地址
* @return void
*/
void CIPList::AddFreePowerIP(unsigned int uiIP)
{
	//加入空闲队列
	m_olsIPFree.AddTail(uiIP);
	m_uiCountFree++;
}

/**
* 得到一个空闲迂回道IP地址
* @param void
* @return unsigned int 空闲IP地址 0：没有空闲IP地址
*/
unsigned int CIPList::GetDetourIP()
{
	unsigned int uiIP = 0;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		uiIP = m_olsIPFree.RemoveHead();
		m_uiCountFree--;
	}

	return uiIP;
}

/**
* 增加一个空闲迂回道IP地址
* @param unsigned int uiIP IP地址
* @return void
*/
void CIPList::AddDetourIP(unsigned int uiIP)
{
	//加入空闲队列
	m_olsIPFree.AddTail(uiIP);
	m_uiCountFree++;
}

/**
* 得到一个采集站IP地址
* @param unsigned int uiLineNb 测线号
* @param unsigned int uiPointNb 测点号
* @param unsigned int uiChannelNb 测道号
* @return unsigned int IP地址
*/
unsigned int CIPList::GetCollectIP(unsigned int uiLineNb, unsigned int uiPointNb, unsigned int uiChannelNb)
{
	//测道号小于10；测点号小于100000；测线号小于2000
	return uiChannelNb + uiPointNb * 10 + uiLineNb * 1000000;
}

/**
* 得到一个爆炸机IP地址
* @param unsigned int uiNb 标识号
* @return unsigned int IP地址
*/
unsigned int CIPList::GetBlastMachineIP(unsigned int uiNb)
{
	//20亿 + 爆炸机号
	return uiNb + 2000000000;
}

/**
* 得到一个辅助道IP地址
* @param unsigned int uiNb 标识号
* @return unsigned int IP地址
*/
unsigned int CIPList::GetAuxIP(unsigned int uiNb)
{
	//20亿 + 100 + 辅助道号
	return uiNb + 2000000000 + 100;
}

/**
* 字符串转换为无符号整数
* @param CString strData 字符串
* @return unsigned int 无符号整数
*/
unsigned int CIPList::ConvertStringToUnsignedInt(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_UINT);
	unsigned int uiData = oVariant.uintVal;
	return uiData;
}

/**
* 数字IP地址转换为字符串IP地址
* @param UINT uiIP 数字IP地址
* @return CString 字符串IP地址
*/
CString CIPList::IPToString(UINT uiIP)
{
	CString strIP;
	UINT uiPart1, uiPart2, uiPart3, uiPart4;
	UINT uiRemain;
	strIP = "";
	uiPart1 = uiIP / 0x1000000;
	uiRemain = uiIP % 0x1000000;

	uiPart2 = uiRemain / 0x10000;
	uiRemain = uiRemain % 0x10000;;

	uiPart3 = uiRemain / 0x100;
	uiRemain = uiRemain % 0x100;

	uiPart4 = uiRemain;

	strIP.Format(("%d.%d.%d.%d"), uiPart4, uiPart3, uiPart2, uiPart1);
	return strIP;
}

/**
* 字符串IP地址转换为数字IP地址
* @param CString strIP 字符串IP地址
* @return UINT 数字IP地址
*/
UINT CIPList::StringToIP(CString strIP)
{
	return inet_addr(strIP);
}

/**
* 得到仪器IP地址的类型	1-固定IP地址；2-浮动IP地址
* @param unsigned int uiIP IP地址
* @return int IP地址的类型	1-固定IP地址；2-浮动IP地址
*/
int CIPList::GetInstrumentIPType(unsigned int uiIP)
{
	int iType = 1;
	if(uiIP >= 2000000200)
	{
		iType = 2;
	}
	return iType;
}