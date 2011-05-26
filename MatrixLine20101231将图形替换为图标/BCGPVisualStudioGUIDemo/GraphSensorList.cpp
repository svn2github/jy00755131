#include "stdafx.h"
#include "GraphSensorList.h"

CGraphSensorList::CGraphSensorList()
{
	m_pChannelList = NULL;	// 测道队列指针	

	m_pArrayGraphSensor = NULL;	// 检波器图形单元数组指针	
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
}

CGraphSensorList::~CGraphSensorList()
{
}

// 初始化
void CGraphSensorList::OnInit()
{
	// 生成数组
	m_pArrayGraphSensor = new CGraphSensorData[m_uiCountAll];
	// 使用中数量
	m_uiCountUsed = 0;
	// 空闲数量
	m_uiCountFree = m_uiCountAll;
}

// 关闭
void CGraphSensorList::OnClose()
{
	// 删除测道数组
	delete[] m_pArrayGraphSensor;
}

// 重置
void CGraphSensorList::OnReset()
{
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测道数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置
		m_pArrayGraphSensor[i].OnReset();
	}

	m_uiRowCount = 0;	// 行数
	m_uiColumnCount = 0;	// 列数
	m_iPaperX = 0;	// 图形区X方向尺寸
	m_iPaperY = 0;	// 图形区Y方向尺寸
	m_iGridX = m_iGridXMin;	// 单元格X方向尺寸
	m_iGridY = m_iGridYMin;	// 单元格Y方向尺寸
}

// 得到测线号在图形区所在的行索引
unsigned int CGraphSensorList::GetUintRowIndex(unsigned int uiNbLine)
{
	unsigned int uiPosX;
	m_pChannelList->m_oNbLineMap.Lookup(uiNbLine, uiPosX);
	return uiPosX;
}

// 得到测道号在图形区所在的列索引
unsigned int CGraphSensorList::GetUintColumnIndex(unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	unsigned int uiPosY = (uiNbPoint - m_pChannelList->m_uiNbPointMin) * m_pChannelList->m_uiPointChannelCountMax + uiNbChannel - 1;
	return uiPosY;
}

// 得到图形区行索引对应的测线号
unsigned int CGraphSensorList::GetUintLineNb(unsigned int uiRowIndex)
{
	POSITION pos = m_pChannelList->m_olsNbLine.FindIndex(uiRowIndex);
	unsigned int uiLineNb = m_pChannelList->m_olsNbLine.GetAt(pos);
	return uiLineNb;
}

// 得到图形区列索引对应的测点号
unsigned int CGraphSensorList::GetUintPointNb(unsigned int uiColumIndex)
{
	unsigned int uiPointNb = (uiColumIndex / m_pChannelList->m_uiPointChannelCountMax) + m_pChannelList->m_uiNbPointMin;
	return uiPointNb;
}

// 得到图形区列索引对应的测道号
unsigned int CGraphSensorList::GetUintChannelNb(unsigned int uiColumIndex)
{
	unsigned int uiChannelNb = uiColumIndex % m_pChannelList->m_uiPointChannelCountMax + 1;
	return uiChannelNb;
}

// 得到图形区行列索引对应的测道
CChannelSetupData* CGraphSensorList::GetChannelSetupData(unsigned int uiRowIndex, unsigned int uiColumIndex)
{
	CChannelSetupData* pChannelSetupData = NULL;
	unsigned int uiLineNb = GetUintLineNb(uiRowIndex);
	unsigned int uiPointNb = GetUintPointNb(uiColumIndex);
	unsigned int uiChannelNb = GetUintChannelNb(uiColumIndex);
	// 得到测道
	pChannelSetupData = m_pChannelList->GetChannel(uiLineNb, uiPointNb, uiChannelNb);
	return pChannelSetupData;
}

// 加载检波器图形视图检波器图形单元队列
void CGraphSensorList::LoadSiteData()
{
	m_uiRowCount = m_pChannelList->m_uiNbLineCount;	// 行数
	unsigned int uiPointCount = m_pChannelList->m_uiNbPointMax - m_pChannelList->m_uiNbPointMin + 1;	// 测点数量
	m_uiColumnCount = uiPointCount * m_pChannelList->m_uiPointChannelCountMax;	// 列数

	CChannelSetupData* pChannelSetupData = NULL;
	for(unsigned int i = 0; i < m_pChannelList->m_uiCountUsed; i++)
	{
		// 得到一个测道
		pChannelSetupData = m_pChannelList->GetChannelByIndex(i);
		m_pArrayGraphSensor[i].m_pChannelSetupData = pChannelSetupData;
		// 得到测道号在图形区所在的列索引
		m_pArrayGraphSensor[i].m_iPosX = GetUintColumnIndex(pChannelSetupData->m_uiNbPoint, pChannelSetupData->m_uiNbChannel);
		// 得到测线号在图形区所在的行索引
		m_pArrayGraphSensor[i].m_iPosY = GetUintRowIndex(pChannelSetupData->m_uiNbLine);	
	}
	m_uiCountUsed = m_pChannelList->m_uiCountUsed;
	m_uiCountFree = m_pChannelList->m_uiCountFree;

	m_iPaperX = (m_uiColumnCount - 1) * m_iGridX;	// X方向尺寸
	m_iPaperY = (m_uiRowCount - 1) * m_iGridY;	// Y方向尺寸
}

// 把选中区域转换为绝对排列字符串
CString CGraphSensorList::ConvertSelectAreaToAbsoluteSpreadString(CRect* pSelectAreaIndex)
{
	CString strAbsoluteSpread = "";
	CString strLine, strPoint;
	unsigned int uiPointNbStart, uiPointNbEnd;
	unsigned int uiLineNb;

	// 得到图形区行索引对应的测线号
	uiPointNbStart = GetUintPointNb(pSelectAreaIndex->left);
	uiPointNbEnd = GetUintPointNb(pSelectAreaIndex->right);
	strPoint.Format("%d-%d", uiPointNbStart, uiPointNbEnd);
	for(int i = pSelectAreaIndex->top; i <= pSelectAreaIndex->bottom; i++)
	{
		// 得到图形区列索引对应的测点号
		uiLineNb = GetUintLineNb(i);
		strLine.Format("%d:%s;", uiLineNb, strPoint);
		strAbsoluteSpread = strAbsoluteSpread + strLine;
	}
	strAbsoluteSpread = strAbsoluteSpread.Left(strAbsoluteSpread.GetLength() - 1);
	return strAbsoluteSpread;
}

// 画网格图
void CGraphSensorList::DrawGrid(CDC* pDC, int iPaperX, int iPaperY, int iGridX, int iGridY)
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
void CGraphSensorList::DrawMarkLineNbAndPointNb(CDC* pDC, int iGridX, int iGridY)
{
	// 画测线标尺
	DrawMarkLineNb(pDC, iGridY);
	// 画测点标尺
	DrawMarkPointNb(pDC, iGridX);
}

// 画测线标尺和测点标尺
void CGraphSensorList::DrawMarkLineNb(CDC* pDC, int iGridY)
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
		iLineNb = GetUintLineNb(i);
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
void CGraphSensorList::DrawMarkPointNb(CDC* pDC, int iGridX)
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
			iPointNb = GetUintPointNb(i);
			strPointNb.Format("%d", iPointNb);

			oRect.left = i * iGridX - m_iGridXMin / 2;
			oRect.right = oRect.left + m_iGridXMax;
			oRect.top = -50;
			oRect.bottom = 0;

			pDC->DrawText(strPointNb, &oRect, DT_LEFT);
		}
	}
	pDC->SelectObject(pOldPen);
}

// 画图形单元
void CGraphSensorList::DrawUnitAll(CDC* pDC, int iGridX, int iGridY, unsigned int uiTestType, unsigned int uiSensorNbSelest, CMap<unsigned int, unsigned int, unsigned int, unsigned int>& oSensorNbMap, float fSeismonitorLimit)
{
	int iUnitWidth;	// 图形单元宽度
	int iUnitHight;	// 图形单元高度
	int iUnitSizeMode;	// 图形单元尺寸模式；1-最小；2-正常；3-最大
	// 得到图形单元尺寸
	GetUnitSize(iGridX, iUnitWidth, iUnitHight, iUnitSizeMode);

	CGraphSensorData* pGraphSensorData = NULL;
	bool bDrawSensorNb;
	unsigned int uiSensorNb;
	for(unsigned int i = 0; i < m_uiCountUsed; i++)
	{
		// 得到一个绘图单元
		pGraphSensorData = &m_pArrayGraphSensor[i];
		if((100 == uiSensorNbSelest) || (pGraphSensorData->m_pChannelSetupData->m_uiSensorNb == uiSensorNbSelest))
		{
			bDrawSensorNb = false;
			if(TRUE == oSensorNbMap.Lookup(pGraphSensorData->m_pChannelSetupData->m_uiSensorNb, uiSensorNb))
			{
				bDrawSensorNb = true;
			}
			pGraphSensorData->DrawUnit(pDC, iGridX, iGridY, iUnitWidth, iUnitHight, iUnitSizeMode, uiTestType, bDrawSensorNb, fSeismonitorLimit);
		}		
	}
}

// 得到图形单元尺寸
void CGraphSensorList::GetUnitSize(int iGridX, int &iUnitWidth, int &iUnitHight, int &iUnitSizeMode)
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