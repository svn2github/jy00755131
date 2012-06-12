#include "stdafx.h"
#include "GraphInstrumentList.h"

CGraphInstrumentList::CGraphInstrumentList(void)
{
	m_pInstrumentList = NULL;	// 仪器队列指针	

	m_pArrayGraphInstrument = NULL;	// 仪器图形单元数组指针	
	m_uiCountAll = 0;	// 总数	
	m_uiCountUsed = 0;	// 使用中数量	
	m_uiCountFree = 0;	// 空闲数量

	m_oPaperColor = RGB(255, 255, 255);	// 背景颜色
	m_oGridColor = RGB(0, 0, 128);	// 网格线颜色

	m_uiRowCount = 0;	// 行数
	m_uiColumnCount = 0;	// 列数

	m_iPaperX = 0;	// 图形区X方向尺寸
	m_iPaperY = 0;	// 图形区Y方向尺寸

	m_iPaperHeadLeft = 50;	// 图形区左边预留尺寸
	m_iPaperHeadTop = 50;	// 图形区左边预留尺寸

	m_iGridXMin = 10;	// 单元格X方向最小尺寸
	m_iGridYMin = 40;	// 单元格Y方向最小尺寸
	m_iGridXMax = 400;	// 单元格X方向最大尺寸
	m_iGridYMax = 200;	// 单元格Y方向最大尺寸

	m_iSizeModeMin = 40;	// 图形单元最小尺寸模式时的门限尺寸
	m_iSizeModeMax = 50;	// 图形单元最大尺寸模式时的门限尺寸

	m_iUnitWidthMin = 10;	// 图形单元宽度，最小尺寸模式
	m_iUnitHightMin = 24;	// 图形单元高度，最小尺寸模式

	m_iUnitWidthNormal = 16;	// 图形单元宽度，正常尺寸模式
	m_iUnitHightNormal = 32;	// 图形单元高度，正常尺寸模式

	m_iUnitWidthMax = 32;	// 图形单元宽度，最大尺寸模式	
	m_iUnitHightMax = 32;	// 图形单元高度，最大尺寸模式

	m_iGridX = m_iGridXMin;	// 单元格X方向尺寸
	m_iGridY = m_iGridYMin;	// 单元格Y方向尺寸

	m_oRectArea.SetRectEmpty();	// 图形区位置
}

CGraphInstrumentList::~CGraphInstrumentList(void)
{
}

// 初始化
void CGraphInstrumentList::OnInit()
{
	// 生成数组
	m_pArrayGraphInstrument = new CGraphInstrumentData[m_uiCountAll];
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		m_pArrayGraphInstrument[i].OnReset();
	}	
	m_uiCountUsed = 0;	// 使用中数量	
	m_uiCountFree = m_uiCountAll;	// 空闲数量
}

// 关闭
void CGraphInstrumentList::OnClose()
{
	// 删除测道数组
	delete[] m_pArrayGraphInstrument;
	m_oRectArea.SetRectEmpty();	// 图形区位置
}

// 重置
void CGraphInstrumentList::OnReset()
{	
	m_uiCountUsed = 0;	// 使用中数量	
	m_uiCountFree = m_uiCountAll;	// 空闲数量
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置
		m_pArrayGraphInstrument[i].OnReset();
	}
	m_oGraphInstrumentMap.RemoveAll();	// 图形单元索引表
	m_olsRowLineNb.RemoveAll();	// 单元格行-测线号对应列表
	m_olsColumnPointNb.RemoveAll();	// 单元格列-测点号对应列表

	m_uiRowCount = 0;	// 行数
	m_uiColumnCount = 0;	// 列数
	m_iPaperX = 0;	// 图形区X方向尺寸
	m_iPaperY = 0;	// 图形区Y方向尺寸
	m_iGridX = m_iGridXMin;	// 单元格X方向尺寸
	m_iGridY = m_iGridYMin;	// 单元格Y方向尺寸
	m_oRectArea.SetRectEmpty();	// 图形区位置
}

// 加载仪器图形视图仪器图形单元队列
void CGraphInstrumentList::LoadSiteData()
{
	m_uiCountUsed = m_pInstrumentList->m_uiCountUsed;	// 使用中数量
	unsigned int i;
	for(i = 0; i < m_uiCountAll; i++)
	{
		// 映射仪器数组到单元数组
		if(true == m_pInstrumentList->m_pArrayInstrument[i].m_bInUsed)
		{
			m_pArrayGraphInstrument[i].m_bInUsed = true;	// 图形单元是否使用中
			m_pArrayGraphInstrument[i].m_pInstrument = &m_pInstrumentList->m_pArrayInstrument[i];
		}
	}
	// 设置仪器的位置，递归；主交叉站为原点
	SetInstrumentPos(m_pInstrumentList->m_pFirstMainCross, 0);
	// 得到需要显示的单元
	for(i = 0; i < m_uiCountAll; i++)
	{
		if(true == m_pInstrumentList->m_pArrayInstrument[i].m_bInUsed)	// 使用中
		{
			// 不是迂回道；或不是采集站
			if((false == m_pInstrumentList->m_pArrayInstrument[i].m_bDetour) || (1 == m_pInstrumentList->m_pArrayInstrument[i].m_uiInstrumentType) || (2 == m_pInstrumentList->m_pArrayInstrument[i].m_uiInstrumentType))
			{
				// 坐标转换，坐标原点由主交叉站移到区域左上角
				m_pArrayGraphInstrument[i].m_iPosX = m_pArrayGraphInstrument[i].m_iPosX  - m_oRectArea.left;
				m_pArrayGraphInstrument[i].m_iPosY = m_pArrayGraphInstrument[i].m_iPosY  - m_oRectArea.top;
				// 生成索引
				m_pArrayGraphInstrument[i].m_iPosIndex = m_pArrayGraphInstrument[i].CreatePosIndex(m_pArrayGraphInstrument[i].m_iPosX, m_pArrayGraphInstrument[i].m_iPosY);
				// 加入索引表
				m_oGraphInstrumentMap.SetAt(m_pArrayGraphInstrument[i].m_iPosIndex, &m_pArrayGraphInstrument[i]);
			}
		}
	}
	// 坐标原点移到区域左上角
	m_oRectArea.right = m_oRectArea.right - m_oRectArea.left;
	m_oRectArea.bottom = m_oRectArea.bottom - m_oRectArea.top;
	m_oRectArea.left = 0;
	m_oRectArea.top = 0;

	// 得到行数和列数
	if((m_oRectArea.bottom > 0) || (m_oRectArea.right > 0))
	{
		m_uiRowCount = m_oRectArea.bottom + 1;	// 行数
		m_uiColumnCount = m_oRectArea.right + 1;	// 列数
	}
	if(1 == m_uiCountUsed)
	{
		m_uiRowCount = 1;	// 行数
		m_uiColumnCount = 1;	// 列数
	}
	
	// 得到画图区域大小
	if(m_uiColumnCount > 0)
	{
		m_iPaperX = (m_uiColumnCount - 1) * m_iGridX;	// X方向尺寸
	}
	if(m_uiRowCount > 0)
	{
		m_iPaperY = (m_uiRowCount - 1) * m_iGridY;	// Y方向尺寸
	}
	// 设置单元格行坐标和测线对应关系
	SetLineNbList();
	// 设置单元格列坐标和测点对应关系
	SetPointNbList();
}

// 得到图形单元，根据图形单元数组索引
CGraphInstrumentData* CGraphInstrumentList::GetGraphInstrumentDataByIndex(unsigned int uiIndex)
{
	return &m_pArrayGraphInstrument[uiIndex];
}

// 得到仪器对象指针，根据图形单元数组索引
CInstrument* CGraphInstrumentList::GetInstrumentByIndex(unsigned int uiIndex)
{
	CGraphInstrumentData* pGraphInstrumentData = GetGraphInstrumentDataByIndex(uiIndex);
	return pGraphInstrumentData->m_pInstrument;
}

// 设置仪器的位置，递归
void CGraphInstrumentList::SetInstrumentPos(CInstrument* pInstrument, unsigned int uiDirection)
{
	if(NULL == pInstrument)
	{
		return;
	}
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	CGraphInstrumentData* pGraphInstrumentDataPreview = NULL;
	CInstrument* pInstrumentNext = NULL;
	pGraphInstrumentData = GetGraphInstrumentDataByIndex(pInstrument->m_uiIndex);
	// 判断方向
	switch(uiDirection)
	{
	case 1:	// 上
		{
			pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexCrossTop);
			pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY + 1;
			pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX;
			if(m_oRectArea.bottom < pGraphInstrumentData->m_iPosY)
			{
				m_oRectArea.bottom = pGraphInstrumentData->m_iPosY;
			}
			break;
		}
	case 2:	// 下
		{
			pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexCrossDown);
			pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY - 1;
			pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX;
			if(m_oRectArea.top > pGraphInstrumentData->m_iPosY)
			{
				m_oRectArea.top = pGraphInstrumentData->m_iPosY;
			}
			break;
		}
	case 3:	// 左
		{
			if((false == pInstrument->m_bDetour) || (1 == pInstrument->m_uiInstrumentType) || (2 == pInstrument->m_uiInstrumentType))
			{
				pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexInstrumentLeft);
				pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX + 1;
				pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY;
				if(m_oRectArea.right < pGraphInstrumentData->m_iPosX)
				{
					m_oRectArea.right = pGraphInstrumentData->m_iPosX;
				}
			}
			else if(true == pInstrument->m_bDetour)
			{
				pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexInstrumentLeft);
				pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX;
				pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY;
				if(m_oRectArea.right < pGraphInstrumentData->m_iPosX)
				{
					m_oRectArea.right = pGraphInstrumentData->m_iPosX;
				}
			}
			break;
		}
	case 4:	// 右
		{
			if((false == pInstrument->m_bDetour) || (1 == pInstrument->m_uiInstrumentType) || (2 == pInstrument->m_uiInstrumentType))
			{
				pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexInstrumentRight);
				pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX - 1;
				pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY;
				if(m_oRectArea.left > pGraphInstrumentData->m_iPosX)
				{
					m_oRectArea.left = pGraphInstrumentData->m_iPosX;
				}
			}
			else if(true == pInstrument->m_bDetour)
			{
				pGraphInstrumentDataPreview = GetGraphInstrumentDataByIndex(pInstrument->m_iIndexInstrumentRight);
				pGraphInstrumentData->m_iPosX = pGraphInstrumentDataPreview->m_iPosX;
				pGraphInstrumentData->m_iPosY = pGraphInstrumentDataPreview->m_iPosY;
				if(m_oRectArea.left > pGraphInstrumentData->m_iPosX)
				{
					m_oRectArea.left = pGraphInstrumentData->m_iPosX;
				}
			}
			break;
		}
	}

	// 向上
	if((1 != uiDirection) && (-1 != pInstrument->m_iIndexCrossTop))	// 不是来自上方的，且上有索引
	{
		pInstrumentNext = GetInstrumentByIndex(pInstrument->m_iIndexCrossTop);
		// 上面链接的仪器
		SetInstrumentPos(pInstrumentNext, 2);
	}
	// 向下
	if((2 != uiDirection) && (-1 != pInstrument->m_iIndexCrossDown))	// 不是来自下方的，且下有索引
	{
		pInstrumentNext = GetInstrumentByIndex(pInstrument->m_iIndexCrossDown);
		// 下面链接的仪器
		SetInstrumentPos(pInstrumentNext, 1);
	}
	// 向左
	if((3 != uiDirection) && (-1 != pInstrument->m_iIndexInstrumentLeft))	// 不是来自左方的，且左有索引
	{
		pInstrumentNext = GetInstrumentByIndex(pInstrument->m_iIndexInstrumentLeft);
		// 左面链接的仪器
		SetInstrumentPos(pInstrumentNext, 4);
	}
	// 向右
	if((4 != uiDirection) && (-1 != pInstrument->m_iIndexInstrumentRight))	// 不是来自右方的，且右有索引
	{
		pInstrumentNext = GetInstrumentByIndex(pInstrument->m_iIndexInstrumentRight);
		// 右面链接的仪器
		SetInstrumentPos(pInstrumentNext, 3);
	}
}

// 得到图形单元，根据图形区坐标
CGraphInstrumentData* CGraphInstrumentList::GetGraphInstrumentDataByPosIndex(unsigned int uiPosX, unsigned int uiPosY)
{
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	unsigned int uiPosIndex = CGraphInstrumentData::CreatePosIndex(uiPosX, uiPosY);

	if(TRUE == m_oGraphInstrumentMap.Lookup(uiPosIndex, pGraphInstrumentData))
	{
		return pGraphInstrumentData;		
	}	
	return NULL;
}

// 得到仪器对象指针，根据图形区坐标
CInstrument* CGraphInstrumentList::GetInstrumentByPosIndex(unsigned int uiPosX, unsigned int uiPosY)
{
	CGraphInstrumentData* pGraphInstrumentData = GetGraphInstrumentDataByPosIndex(uiPosX, uiPosY);
	if(NULL != pGraphInstrumentData)
	{
		return pGraphInstrumentData->m_pInstrument;
	}
	return NULL;
}

// 设置单元格行坐标和测线对应关系
void CGraphInstrumentList::SetLineNbList()
{
	unsigned int i, j;
	unsigned int uiLineNb;
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	for(i = 0; i < m_uiRowCount; i++)
	{
		m_olsRowLineNb.AddTail(-1);
		for(j = 0; j < m_uiColumnCount; j++)
		{
			pGraphInstrumentData = GetGraphInstrumentDataByPosIndex(j, i);
			if(NULL != pGraphInstrumentData)
			{
				if(0 != pGraphInstrumentData->m_pInstrument->m_uiLineNb)
				{
					uiLineNb = pGraphInstrumentData->m_pInstrument->m_uiLineNb;
					m_olsRowLineNb.RemoveTail();
					m_olsRowLineNb.AddTail(uiLineNb);
					break;
				}
			}
		}
	}
}

// 设置单元格列坐标和测点对应关系
void CGraphInstrumentList::SetPointNbList()
{
	unsigned int i, j;
	unsigned int uiPointNb;
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	for(i = 0; i < m_uiColumnCount; i++)
		{m_olsColumnPointNb.AddTail(-1);	
		for(j = 0; j < m_uiRowCount; j++)
		{
			pGraphInstrumentData = GetGraphInstrumentDataByPosIndex(i, j);
			if(NULL != pGraphInstrumentData)
			{
				if(0 != pGraphInstrumentData->m_pInstrument->m_uiLineNb)
				{
					uiPointNb = pGraphInstrumentData->m_pInstrument->m_uiPointNb;
					m_olsColumnPointNb.RemoveTail();
					m_olsColumnPointNb.AddTail(uiPointNb);
					break;
				}
			}
		}
	}
}

// 得到行索引对应的测线号
int CGraphInstrumentList::GetLineNbByRowIndex(unsigned int uiRowIndex)
{
	POSITION pos = m_olsRowLineNb.FindIndex(uiRowIndex);
	int iLineNb = m_olsRowLineNb.GetAt(pos);
	return iLineNb;
}

// 得到列索引对应的测点号
int CGraphInstrumentList::GetPointNbByColumnIndex(unsigned int uiColumnIndex)
{
	POSITION pos = m_olsColumnPointNb.FindIndex(uiColumnIndex);
	int iPointNb = m_olsColumnPointNb.GetAt(pos);
	return iPointNb;
}

// 把选中区域转换为绝对排列字符串
CString CGraphInstrumentList::ConvertSelectAreaToAbsoluteSpreadString(CRect* pSelectAreaIndex)
{
	CString strAbsoluteSpread = "";
	CString strLine, strPoint;
	int iPointNb, iPointNbStart, iPointNbEnd;
	int iLineNb;
	CMap<int, int, CString, CString> oLineNbMap;

	for(int i = pSelectAreaIndex->top; i <= pSelectAreaIndex->bottom; i++)
	{
		// 得到图形区列索引对应的测线号
		iLineNb = GetLineNbByRowIndex(i);
		if(iLineNb > 0)	// 测线有效
		{
			iPointNbStart = GetPointNbByColumnIndex(pSelectAreaIndex->left);	// 得到起始测点
			iPointNbEnd = GetPointNbByColumnIndex(pSelectAreaIndex->right);	// 得到终止测点
			if((iPointNbStart > 0) && (iPointNbEnd > 0))	// 测点有效
			{
				if(iPointNbStart > iPointNbEnd)
				{
					iPointNb = iPointNbStart;
					iPointNbStart = iPointNbEnd;
					iPointNbEnd = iPointNb;
				}
				strPoint.Format("%d-%d", iPointNbStart, iPointNbEnd);	// 测点字符串
				strLine.Format("%d:%s;", iLineNb, strPoint);	// 测线字符串
				oLineNbMap.SetAt(iLineNb, strLine);	// 加入索引表
			}
		}
	}
	POSITION pos = oLineNbMap.GetStartPosition();
	while(NULL != pos)
	{
		oLineNbMap.GetNextAssoc(pos, iLineNb, strLine);
		strAbsoluteSpread = strAbsoluteSpread + strLine;	// 链接测线字符串
	}
	if(strAbsoluteSpread.GetLength() > 0)
	{
		strAbsoluteSpread = strAbsoluteSpread.Left(strAbsoluteSpread.GetLength() - 1);	// 去掉最后一个字符“;”
	}
	oLineNbMap.RemoveAll();
	return strAbsoluteSpread;
}

// 画网格图
void CGraphInstrumentList::DrawGrid(CDC* pDC, int iPaperX, int iPaperY, int iGridX, int iGridY)
{

	COLORREF oldBkColor = m_oPaperColor;

	CRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = iPaperX;
	rect.bottom = iPaperY;

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_DASH, 0, m_oGridColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	pDC->MoveTo(0, rect.top);
	pDC->LineTo(0, rect.bottom);
	pDC->MoveTo(rect.left, 0);
	pDC->LineTo(rect.right, 0);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_DOT, 0, m_oGridColor);
	pDC->SelectObject(&penDot);

	for (int x = 0; x < rect.right; x += iGridX)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	for (int y = 0; y < rect.bottom; y += iGridY)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 0, m_oGridColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldBkColor);
}

// 画测线标尺
void CGraphInstrumentList::DrawMarkLineNbAndPointNb(CDC* pDC, int iGridX, int iGridY)
{
	// 画测线标尺
	DrawMarkLineNb(pDC, iGridY);
	// 画测点标尺
	DrawMarkPointNb(pDC, iGridX);
}

// 画测线标尺和测点标尺
void CGraphInstrumentList::DrawMarkLineNb(CDC* pDC, int iGridY)
{
	CPen oPen;
	CPen* pOldPen;

	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iLineNb;
	CString strLineNb;
	CRect oRect;
	for(unsigned int i = 0; i < m_uiRowCount; i++)
	{
		iLineNb = GetLineNbByRowIndex(i);
		strLineNb.Format("%d", iLineNb);

		oRect.left = -50;
		oRect.right = 0;
		oRect.top = i * iGridY - m_iUnitHightMin / 2;
		oRect.bottom = oRect.top + m_iUnitHightMax;

		pDC->DrawText(strLineNb, &oRect, DT_LEFT | DT_VCENTER);

	}
	pDC->SelectObject(pOldPen);
}

// 画测点标尺
void CGraphInstrumentList::DrawMarkPointNb(CDC* pDC, int iGridX)
{
	int iStep = 1;
	if(iGridX <= 2 * m_iGridXMin)
	{
		iStep = 5;
	}

	CPen oPen;
	CPen* pOldPen;

	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iPointNb;
	CString strPointNb;
	CRect oRect;
	for(unsigned int i = 0; i < m_uiColumnCount; i++)
	{
		if((((i + 1) % iStep) == 0) || (i == 0))
		{
			iPointNb = GetPointNbByColumnIndex(i);
			strPointNb.Format("%d", iPointNb);

			oRect.left = i * iGridX - m_iGridXMin / 2;
			oRect.right = oRect.left + m_iGridXMax;
			oRect.top = -50;
			oRect.bottom = 0;
		}

		pDC->DrawText(strPointNb, &oRect, DT_LEFT);

	}
	pDC->SelectObject(pOldPen);
}

// 画图形单元连接线
void CGraphInstrumentList::DrawUnitLinkLineAll(CDC* pDC, int iGridX, int iGridY)
{
	POSITION pos = m_oGraphInstrumentMap.GetStartPosition();
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	unsigned int uiPosIndex;	// 仪器位置索引
	while(NULL != pos)
	{
		// 得到一个绘图单元
		m_oGraphInstrumentMap.GetNextAssoc(pos, uiPosIndex, pGraphInstrumentData);
		pGraphInstrumentData->DrawUnitLinkLine(pDC, iGridX, iGridY);
	}
}

// 画图形单元
void CGraphInstrumentList::DrawUnitAll(CDC* pDC, int iGridX, int iGridY, unsigned int uiTestType, bool bShowCross, bool bShowPower, bool bShowCollect)
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	GetUnitSize(iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);

	POSITION pos = m_oGraphInstrumentMap.GetStartPosition();
	CGraphInstrumentData* pGraphInstrumentData = NULL;
	unsigned int uiPosIndex;	// 仪器位置索引
	while(NULL != pos)
	{
		// 得到一个绘图单元
		m_oGraphInstrumentMap.GetNextAssoc(pos, uiPosIndex, pGraphInstrumentData);
		if((pGraphInstrumentData->m_pInstrument->m_uiInstrumentType == 1) && (bShowCross == true))
		{
			pGraphInstrumentData->DrawUnit(pDC, iGridX, iGridY, iUnitWidth, iUnitHight, iUnitSizeMode, uiTestType);
		}
		else if((pGraphInstrumentData->m_pInstrument->m_uiInstrumentType == 2) && (bShowPower == true))
		{
			pGraphInstrumentData->DrawUnit(pDC, iGridX, iGridY, iUnitWidth, iUnitHight, iUnitSizeMode, uiTestType);
		}
		else if((pGraphInstrumentData->m_pInstrument->m_uiInstrumentType == 3) && (bShowCollect == true))
		{
			pGraphInstrumentData->DrawUnit(pDC, iGridX, iGridY, iUnitWidth, iUnitHight, iUnitSizeMode, uiTestType);
		}
	}
}

// 得到图形单元尺寸
void CGraphInstrumentList::GetUnitSize(int iGridX, int &iUnitWidth, int &iUnitHight, int &iUnitSizeMode)
{
	if(iGridX < m_iSizeModeMin)	// 图形单元最小尺寸模式时的门限尺寸
	{
		iUnitSizeMode = 1;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
		iUnitWidth = m_iUnitWidthMin;	// 图形单元宽度，最小尺寸模式
		iUnitHight = m_iUnitHightMin;	// 图形单元高度，最小尺寸模式
	}
	else if(iGridX > m_iSizeModeMax)	// 图形单元最大尺寸模式时的门限尺寸
	{
		iUnitSizeMode = 3;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
		iUnitWidth = m_iUnitWidthMax;	// 图形单元宽度，最小尺寸模式
		iUnitHight = m_iUnitHightMax;	// 图形单元高度，最小尺寸模式
	}
	else
	{
		iUnitSizeMode = 2;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
		iUnitWidth = m_iUnitWidthNormal;	// 图形单元宽度，最小尺寸模式
		iUnitHight = m_iUnitHightNormal;	// 图形单元高度，最小尺寸模式
	}
}

