#include "stdafx.h"
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
			}
			// 判断：采集站为采集道
			else
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
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					break;
				case 1:	// 1：测试合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
					break;
				case 2:	// 2：测试不合格
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
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
					break;
// 				case 5: // 5：鼠标中键选中测试区域
// 					oPenUnit.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
// 					oBrushUnit.CreateSolidBrush(RGB(190, 190, 190));	// 灰色
// 					pOldPen = pDC->SelectObject(&oPenUnit);
// 					pOldPenBrush = pDC->SelectObject(&oBrushUnit);
// 
// 					pDC->Ellipse(&oRect);
// 					// 画中心小椭圆
// 					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
// 					break;
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
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldPenBrush);
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
// cxm 2011-09-19
// 点击GO按钮则清空鼠标中键选中设备的测试结果
void CGraphInstrumentData::OnClearCheckInstrumentTestResult(unsigned int uiTestType)
{
	switch(uiTestType)
	{
	case 4:	// 仪器噪声。 Noise
		m_pInstrument->m_byNoiseCheck = 0;
		break;
	case 5:	// 仪器畸变。 Distortion
		m_pInstrument->m_byDistortionCheck = 0;
		break;
	case 7:	// 仪器串音。 Crosstalk
		m_pInstrument->m_byCrosstalkCheck = 0;
		break;
	case 6:	// 仪器增益/ 相位。 Gain/Phase
		m_pInstrument->m_byGainAndPhaseCheck = 0;
		break;
	case 8:	// 仪器共模。CMRR
		m_pInstrument->m_byCMRRCheck = 0;
		break;
	case 30:	// 所有仪器测试
		m_pInstrument->m_byCheck = 0;
		break;
	}
}