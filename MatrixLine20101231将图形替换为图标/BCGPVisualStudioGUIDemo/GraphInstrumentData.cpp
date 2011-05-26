#include "stdafx.h"
#include "resource.h"
#include "GraphInstrumentData.h"

CGraphInstrumentData::CGraphInstrumentData(void)
{
}

CGraphInstrumentData::~CGraphInstrumentData(void)
{
}

// 设置图形参数
void CGraphInstrumentData::SetDrawParam()
{

}

// 重置
void CGraphInstrumentData::OnReset()
{
	m_bInUsed = false;	// 图形单元是否使用中
	
	m_iPosX = 0;	// X方向位置
	m_iPosY = 0;	// Y方向位置
	m_iPosIndex = 0;	// 仪器位置索引

	m_pInstrument = NULL;	// 仪器指针
}
// 生成仪器位置索引
int CGraphInstrumentData::CreatePosIndex(int iPosX, int iPosY)
{
	return iPosX * 100000 + iPosY;
}

// 画图
void CGraphInstrumentData::DrawUnit(CDC* pDC, int iGridX, int iGridY, int iUnitWidth, int iUnitHight,  int iUnitSizeMode, unsigned int uiTestType)
{
	CPen oPenUnit;
	CPen* pOldPen;
	CBrush oBrushUnit;
	CBrush* pOldPenBrush;

	int iCenterX = m_iPosX * iGridX;
	int iCenterY = m_iPosY * iGridY;
	CRect oRect;
	oRect.left = iCenterX - iUnitWidth / 2;
	oRect.top = iCenterY - iUnitWidth / 2;
	oRect.right = oRect.left + iUnitWidth;
	oRect.bottom = oRect.top + iUnitWidth;

	// by vivi 2011-01-12
	CDC m_MemDc;//定义一个DC
	CBitmap m_BkBmp;//定义个位图对象
	BITMAP m_BmpInfo;//定义一个位图信息结构体

	// 画仪器
	if(0 < m_pInstrument->m_uiIP)
	{
		if(1 == m_pInstrument->m_uiInstrumentType)	// 交叉站
		{
			oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
			oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
			pOldPen = pDC->SelectObject(&oPenUnit);
			pOldPenBrush = pDC->SelectObject(&oBrushUnit);
			pDC->FillRect(&oRect, &oBrushUnit);
			pDC->Rectangle(&oRect);
			// 画X字
			pDC->MoveTo(CPoint(oRect.left, oRect.top));
			pDC->LineTo(CPoint(oRect.right, oRect.bottom));
			pDC->MoveTo(CPoint(oRect.left, oRect.bottom));
			pDC->LineTo(CPoint(oRect.right, oRect.top));

			// by vivi 2011-01-12
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldPenBrush);
		}
		else if(2 == m_pInstrument->m_uiInstrumentType)	// 电源站
		{
			oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
			oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
			pOldPen = pDC->SelectObject(&oPenUnit);
			pOldPenBrush = pDC->SelectObject(&oBrushUnit);
			pDC->FillRect(&oRect, &oBrushUnit);
			pDC->Rectangle(&oRect);
			// 画中心小矩形
			pDC->Rectangle(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
			// by vivi 2011-01-12
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldPenBrush);
		}
		// 判断：采集站
		else
		{
			// 判断：采集站链接爆炸机控制器
			if(true == m_pInstrument->m_bBlastMachine)
			{
				oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
				oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
				pOldPen = pDC->SelectObject(&oPenUnit);
				pOldPenBrush = pDC->SelectObject(&oBrushUnit);

				pDC->Ellipse(&oRect);
				// 画中心小矩形
				pDC->Rectangle(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
				// by vivi 2011-01-12
				pDC->SelectObject(pOldPen);
				pDC->SelectObject(pOldPenBrush);
			}
			// 判断：采集站为跳过道
			else if(true == m_pInstrument->m_bJumpedChannel)
			{
				oPenUnit.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	// 绿色
				oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
				pOldPen = pDC->SelectObject(&oPenUnit);
				pOldPenBrush = pDC->SelectObject(&oBrushUnit);

				// 画十字
				pDC->MoveTo(CPoint(oRect.left, iCenterY));
				pDC->LineTo(CPoint(oRect.right, iCenterY));
				pDC->MoveTo(CPoint(iCenterX, oRect.top));
				pDC->LineTo(CPoint(iCenterX, oRect.bottom));
				// by vivi 2011-01-12
				pDC->SelectObject(pOldPen);
				pDC->SelectObject(pOldPenBrush);
			}
			// 判断：采集站为辅助道
			else if(true == m_pInstrument->m_bAux)
			{
				byte byCheck = m_pInstrument->GetTestResultCheck(uiTestType);
				switch(byCheck)
				{
				case 0:	// 0：未测试
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(0, 0, 255));	// 蓝色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					break;
				case 1:	// 1：测试合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					break;
				case 2:	// 2：测试不合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					break;
				case 3:	// 3：无信号
					oPenUnit.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	// 蓝色
					oBrushUnit.CreateSolidBrush(RGB(0, 0, 255));	// 蓝色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					// 画十字
					pDC->MoveTo(CPoint(oRect.left, iCenterY));
					pDC->LineTo(CPoint(oRect.right, iCenterY));
					pDC->MoveTo(CPoint(iCenterX, oRect.top));
					pDC->LineTo(CPoint(iCenterX, oRect.bottom));
					break;
				}
				// by vivi 2011-01-12
				pDC->SelectObject(pOldPen);
				pDC->SelectObject(pOldPenBrush);
			}
			// 判断：采集站为采集道
			else
			{
				byte byCheck = m_pInstrument->GetTestResultCheck(uiTestType);
				switch(byCheck)
				{
				case 0:	// 0：未测试
					// by vivi 2011-01-12
// 					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
// 					oBrushUnit.CreateSolidBrush(RGB(0, 0, 255));	// 蓝色
// 					pOldPen = pDC->SelectObject(&oPenUnit);
// 					pOldPenBrush = pDC->SelectObject(&oBrushUnit);
// 
// 					pDC->Ellipse(&oRect);
// 					// 画中心小椭圆
// 					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					
					if (iUnitSizeMode == 1)
					{
						m_BkBmp.LoadBitmap(IDB_BITMAP_FDU_1);	// 此处为刚刚添加进来的位图资源ID
					}
					else if (iUnitSizeMode == 2)
					{
						m_BkBmp.LoadBitmap(IDB_BITMAP_FDU_2);	// 此处为刚刚添加进来的位图资源ID
					} 
					else if (iUnitSizeMode == 3)
					{
						m_BkBmp.LoadBitmap(IDB_BITMAP_FDU_3);	// 此处为刚刚添加进来的位图资源ID
					}
					else
					{
						break;
					}
					m_BkBmp.GetBitmap(&m_BmpInfo);				// 获取位图高宽等信息，保存在位图结构体中
					m_MemDc.CreateCompatibleDC(pDC);			// 创建一个兼容屏幕DC的内存DC：m_MemDc。
					m_MemDc.SelectObject(&m_BkBmp);				// 将该位图选择到刚创建的内存DC中。

					/*将内存DC里的东西贴到屏幕DC上去*/
					pDC->BitBlt(oRect.left,oRect.top,m_BmpInfo.bmWidth,m_BmpInfo.bmHeight,&m_MemDc,0,0,SRCCOPY);
					break;
				case 1:	// 1：测试合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					// by vivi 2011-01-12
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldPenBrush);
					break;
				case 2:	// 2：测试不合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					// by vivi 2011-01-12
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldPenBrush);
					break;
				case 3:	// 3：无信号
				case 4:	// 4：未判读
					oPenUnit.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 255, 0));	// 黄色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					// by vivi 2011-01-12
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldPenBrush);
					break;
				}
			}
		}
	}
	// 判断：未分配IP地址
	else
	{
		oPenUnit.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));	// 黄色
		oBrushUnit.CreateSolidBrush(RGB(255, 255, 0));	// 黄色
		pOldPen = pDC->SelectObject(&oPenUnit);
		pOldPenBrush = pDC->SelectObject(&oBrushUnit);

		// 画十字
		pDC->MoveTo(CPoint(oRect.left, iCenterY));
		pDC->LineTo(CPoint(oRect.right, iCenterY));
		pDC->MoveTo(CPoint(iCenterX, oRect.top));
		pDC->LineTo(CPoint(iCenterX, oRect.bottom));
		// by vivi 2011-01-12
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldPenBrush);
	}

	// by vivi 2011-01-12
//  	pDC->SelectObject(pOldPen);
//  	pDC->SelectObject(pOldPenBrush);
}

// 画图，单元连接线
void CGraphInstrumentData::DrawUnitLinkLine(CDC* pDC, int iGridX, int iGridY)
{
	if((m_pInstrument->m_uiSNInstrumentRight == 0) && (m_pInstrument->m_uiSNCrossDown == 0))
	{
		return;
	}

	CPen oPenLine;
	CPen* pOldPen;

	int iCenterX = m_iPosX * iGridX;
	int iCenterY = m_iPosY * iGridY;

	// 画连接线，向右画
	// 判断：迂回道低端
	if(true == m_pInstrument->m_bDetourMarkerLow)
	{
		oPenLine.CreatePen(PS_DOT, 5, RGB(0, 0, 255));	// 蓝色
	}
	else
	{
		oPenLine.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	// 绿色
	}
	
	pOldPen = pDC->SelectObject(&oPenLine);
	if(m_pInstrument->m_uiSNInstrumentRight > 0)
	{
		pDC->MoveTo(CPoint(iCenterX, iCenterY));
		pDC->LineTo(CPoint(iCenterX + iGridX, iCenterY));
	}
	// 画连接线，向下画
	if(m_pInstrument->m_uiSNCrossDown > 0)
	{
		pDC->MoveTo(CPoint(iCenterX, iCenterY));
		pDC->LineTo(CPoint(iCenterX, iCenterY + iGridY));
	}
	pDC->SelectObject(pOldPen);
}