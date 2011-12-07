#include "StdAfx.h"
#include "LayoutSetupData.h"

CLayoutSetupData::CLayoutSetupData()
{
}

CLayoutSetupData::~CLayoutSetupData()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CLayoutSetupData::OnInit()
{
	// 标记点列表对象
	m_oMarkerList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oMarkerList.OnInit();

	// 辅助道列表对象
	m_oAuxList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oAuxList.OnInit();

	// 爆炸机控制器列表对象
	m_oBlastMachineList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oBlastMachineList.OnInit();

	// 迂回道列表对象
	m_oDetourList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oDetourList.OnInit();

	// 哑道列表对象
	m_oMuteList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oMuteList.OnInit();
}

/**
* 重新加载
* @param void
* @return void
*/
void CLayoutSetupData::OnReload()
{
	// 标记点列表对象
	m_oMarkerList.OnReload();
	// 辅助道列表对象
	m_oAuxList.OnReload();
	// 爆炸机控制器列表对象
	m_oBlastMachineList.OnReload();
	// 迂回道列表对象
	m_oDetourList.OnReload();
	// 哑道列表对象
	m_oMuteList.OnReload();
}

/**
* 关闭
* @param void
* @return void
*/
void CLayoutSetupData::OnClose()
{
	// 标记点列表对象
	m_oMarkerList.OnClose();
	// 辅助道列表对象
	m_oAuxList.OnClose();
	// 爆炸机控制器列表对象
	m_oBlastMachineList.OnClose();
	// 迂回道列表对象
	m_oDetourList.OnClose();
	// 哑道列表对象
	m_oMuteList.OnClose();
}

/**
* 解析辅助道排列定义字符串，得到IP地址队列
* @param CList<unsigned int, unsigned int>* plsIP IP地址队列指针
* @param CString strAuxiliaryDescr 辅助道排列定义字符串
* @return bool true：成功；false：失败
*/
bool CLayoutSetupData::ParseSpreadToIPListByAuxiliaryDescr(CList<unsigned int, unsigned int>* plsIP, CString strAuxiliaryDescr)
{
	// 解析辅助道定义
	// 判断：没有定义辅助道
	if(strAuxiliaryDescr == "")
	{
		return true;		
	}
	// 判断：所有辅助道加入队列
	else if(strAuxiliaryDescr == "all")
	{
		for(unsigned int  i = 0; i < m_oAuxList.m_uiCountAll; i++)
		{
			plsIP->AddTail(m_oAuxList.GetAux(i)->m_uiIP);
		}		
	}
	else
	{
		// 转换为小写
		strAuxiliaryDescr.MakeLower();
		// 过滤字符'a'
		strAuxiliaryDescr.Replace(_T("a"), _T(""));

		CString strPart;	// 线段字符串
		POSITION pos;	// 位置
		int iPos, iPosStart, iPosStartEnd;	// 位置
		int iCount, i;
		CList<int, int> lsPos;	// 位置队列

		lsPos.AddTail(-1);
		iPosStart = 0;
		while(true)
		{
			// 查找分割标记","
			iPos = strAuxiliaryDescr.Find(',', iPosStart);
			if(-1 == iPos)
			{
				break;
			}
			lsPos.AddTail(iPos);
			iPosStart = iPos + 1;
		}
		lsPos.AddTail(strAuxiliaryDescr.GetAllocLength());

		// 得到标记点 "," 数量
		iCount = lsPos.GetCount();
		for(i = 0; i < iCount - 1; i++)
		{
			pos = lsPos.FindIndex(i);
			iPosStart = lsPos.GetAt(pos) + 1;
			pos = lsPos.FindIndex(i + 1);
			iPosStartEnd = lsPos.GetAt(pos);
			// 得到线段字符串
			strPart = strAuxiliaryDescr.Mid(iPosStart, iPosStartEnd - iPosStart);
			ParseSpreadToIPListByAuxiliaryDescrPart(plsIP, strPart);
		}
	}
	return true;
}

/**
* 解析辅助道定义字符串，得到IP地址队列
* @param CList<unsigned int, unsigned int>* plsIP IP地址队列指针
* @param CCString strPart 辅助道定义字符串
* @return bool true：成功；false：失败
*/
bool CLayoutSetupData::ParseSpreadToIPListByAuxiliaryDescrPart(CList<unsigned int, unsigned int>* plsIP, CString strPart)
{
	int iPos;	// 位置
	int iStep = 1;	// 步长
	int iStart = 1;	// 起始
	int iEnd = 1;	// 终止
	CString strData;

	// 得到起始点
	iPos = strPart.Find('-', 0);
	// 无分隔符
	if(-1 == iPos)
	{
		// 起始测点
		iStart = CXMLDOMTool::ConvertStringToInt(strPart);
		// 终止测点
		iEnd = iStart;
	}
	else	// 有分隔符
	{
		strData = strPart.Mid(0, iPos);
		// 起始测点
		iStart = CXMLDOMTool::ConvertStringToInt(strData);

		iPos++;
		strData = strPart.Mid(iPos, strPart.GetLength() - iPos);
		// 终止测点
		iEnd = CXMLDOMTool::ConvertStringToInt(strData);
	}
	CAuxData* pAuxData = NULL;
	for(int i = iStart; i <= iEnd; i = i + iStep)
	{
		// 根据标识号，得到辅助道
		pAuxData = m_oAuxList.GetAuxByNb(i);
		if(NULL != pAuxData)
		{
			plsIP->AddTail(pAuxData->m_uiIP);
		}
	}
	return true;
}