#include "stdafx.h"
#include "GraphSensorData.h"

CGraphSensorData::CGraphSensorData(void)
{
	// 重置
	OnReset();
}

CGraphSensorData::~CGraphSensorData(void)
{
}

// 设置图形参数
void CGraphSensorData::SetDrawParam()
{

}

// 重置
void CGraphSensorData::OnReset()
{
	m_iPosX = 0;	// X方向位置
	m_iPosY = 0;	// Y方向位置
	m_pChannelSetupData = NULL;	// 测道指针
}

// 画图
void CGraphSensorData::DrawUnit(CDC* pDC, int iGridX, int iGridY, int iUnitWidth, int iUnitHight,  int iUnitSizeMode, unsigned int uiTestType, bool bDrawSensorNb, float fSeismonitorLimit)
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

	// 画检波器
	if(0 < m_pChannelSetupData->m_uiSN)
	{
		// 判断：采集站为跳过道
		if(true == m_pChannelSetupData->m_bJumped)
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
		// 判断：采集站为哑道
		else if(true == m_pChannelSetupData->m_bMute)
		{
			oPenUnit.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));	// 红色
			oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
			pOldPen = pDC->SelectObject(&oPenUnit);
			pOldPenBrush = pDC->SelectObject(&oBrushUnit);

			// 画十字
			pDC->MoveTo(CPoint(oRect.left, iCenterY));
			pDC->LineTo(CPoint(oRect.right, iCenterY));
			pDC->MoveTo(CPoint(iCenterX, oRect.top));
			pDC->LineTo(CPoint(iCenterX, oRect.bottom));
		}
		// 判断：采集站为采集道
		else
		{
			byte byCheck = m_pChannelSetupData->GetTestResultCheck(uiTestType, fSeismonitorLimit);
			switch(byCheck)
			{
			case 0:	// 0：未测试
				oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
				oBrushUnit.CreateSolidBrush(RGB(0, 0, 255));	// 蓝色
				pOldPen = pDC->SelectObject(&oPenUnit);
				pOldPenBrush = pDC->SelectObject(&oBrushUnit);

				pDC->Ellipse(&oRect);
				// 画中心小椭圆
				pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitHight / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitHight / 4);
				break;
			case 1:	// 1：测试合格
				if(22 == uiTestType)
				{
					// 得到噪声监测图形单元高度
					int iGraphHight = GetSeismonitorGraphHight(iUnitHight, fSeismonitorLimit);
					oRect.left = iCenterX - iUnitWidth / 2;
					oRect.top = iCenterY - iGraphHight / 2;
					oRect.right = oRect.left + iUnitWidth;
					oRect.bottom = oRect.top + iGraphHight;

					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					GetSeismonitorGraphColour(&oBrushUnit, fSeismonitorLimit);
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->FillRect(&oRect, &oBrushUnit);
//					pDC->Rectangle(&oRect);
				}
				else
				{
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(0, 255, 0));	// 绿色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
				}
				break;
			case 2:	// 2：测试不合格
				if(22 == uiTestType)
				{
					// 得到噪声监测图形单元高度
					int iGraphHight = GetSeismonitorGraphHight(iUnitHight, fSeismonitorLimit);
					oRect.left = iCenterX - iUnitWidth / 2;
					oRect.top = iCenterY - iGraphHight / 2;
					oRect.right = oRect.left + iUnitWidth;
					oRect.bottom = oRect.top + iGraphHight;

					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->FillRect(&oRect, &oBrushUnit);
//					pDC->Rectangle(&oRect);
				}
				else
				{
					oPenUnit.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
					oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
					pOldPen = pDC->SelectObject(&oPenUnit);
					pOldPenBrush = pDC->SelectObject(&oBrushUnit);

					pDC->Ellipse(&oRect);
					// 画中心小椭圆
					pDC->Ellipse(oRect.left + iUnitWidth / 4, oRect.top + iUnitWidth / 4, oRect.right - iUnitWidth / 4, oRect.bottom - iUnitWidth / 4);
				}
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
			}
		}
	}
	// 判断：未分配IP地址
	else
	{
		// 判断：采集站为哑道
		if(true == m_pChannelSetupData->m_bMute)
		{
			oPenUnit.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));	// 红色
			oBrushUnit.CreateSolidBrush(RGB(255, 0, 0));	// 红色
			pOldPen = pDC->SelectObject(&oPenUnit);
			pOldPenBrush = pDC->SelectObject(&oBrushUnit);

			// 画十字
			pDC->MoveTo(CPoint(oRect.left, iCenterY));
			pDC->LineTo(CPoint(oRect.right, iCenterY));
			pDC->MoveTo(CPoint(iCenterX, oRect.top));
			pDC->LineTo(CPoint(iCenterX, oRect.bottom));
		}
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

	}

	if(true == bDrawSensorNb)
	{
		oRect.left = iCenterX - iUnitWidth / 2;
		oRect.top = iCenterY - iUnitHight / 2;
		oRect.right = oRect.left + iUnitWidth;
		oRect.bottom = oRect.top + iUnitHight;

		CString strSensorNb;
		strSensorNb.Format("%d", m_pChannelSetupData->m_uiSensorNb);
		int nPrevMode = pDC->SetBkMode(TRANSPARENT); 		
		pDC->DrawText(strSensorNb, &oRect, DT_CENTER | DT_VCENTER);
		pDC->SetBkMode(nPrevMode);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldPenBrush);
}

// 得到噪声监测图形单元相对高度
int CGraphSensorData::GetSeismonitorGraphHight(float fSeismonitorLimit)
{
	float fDis = (fSeismonitorLimit - m_pChannelSetupData->m_fNoiseMonitorValue) / 6.0;
	int iDis = abs((int)fDis);
	if(0 == iDis)
	{
		iDis = 1;
	}
	else if(8 < iDis)
	{
		iDis = 8;
	}
	return iDis;
}

// 得到噪声监测图形单元高度
int CGraphSensorData::GetSeismonitorGraphHight(int iUnitHight, float fSeismonitorLimit)
{
	int iDis = GetSeismonitorGraphHight(fSeismonitorLimit);
	int iGraphHight = iUnitHight * iDis / 8;

	return iGraphHight;
}

// 得到噪声监测图形单元颜色
void CGraphSensorData::GetSeismonitorGraphColour(CBrush *oBrushUnit, float fSeismonitorLimit)
{
	int iDis = GetSeismonitorGraphHight(fSeismonitorLimit);
	switch (iDis)
	{
	case 1:
		oBrushUnit->CreateSolidBrush(RGB(255, 0, 0));	// 红色
		break;
	case 2:
		oBrushUnit->CreateSolidBrush(RGB(255, 128, 0));	// 色6
		break;
	case 3:
		oBrushUnit->CreateSolidBrush(RGB(255, 128, 64));	// 色5
		break;
	case 4:
		oBrushUnit->CreateSolidBrush(RGB(255, 192, 64));	// 色4
		break;
	case 5:
		oBrushUnit->CreateSolidBrush(RGB(255, 255, 0));	// 色3，黄色
		break;
	case 6:
		oBrushUnit->CreateSolidBrush(RGB(255, 255, 200));	// 色2
		break;
	case 7:
		oBrushUnit->CreateSolidBrush(RGB(255, 255, 255));	// 色1，白色
		break;
	case 8:
		oBrushUnit->CreateSolidBrush(RGB(0, 255, 0));	// 绿色
		break;
	}
}
// cxm 2011-09-19
// 点击GO按钮则清空鼠标中键选中设备的测试结果
void CGraphSensorData::OnClearCheckInstrumentTestResult(unsigned int uiTestType)
{
	switch(uiTestType)
	{
	case 15:	// 检波器电阻。 Resistance
		m_pChannelSetupData->m_byResistanceCheck = 0;
		break;
	case 14:	// 检波器漏电。 Leakage
		m_pChannelSetupData->m_byLeakageCheck = 0;
		break;
	case 1:	// 检波器噪声。 Noise
		m_pChannelSetupData->m_byNoiseCheck = 0;
		break;
	case 20:	// 检波器倾斜度。 Tilt
		m_pChannelSetupData->m_byTiltCheck = 0;
		break;
	case 21:	// 检波器倾斜度模型。Tilt Model
		m_pChannelSetupData->m_byTiltModelCheck = 0;
		break;
	case 22:	// 检波器噪声监测
		m_pChannelSetupData->m_byNoiseMonitorCheck = 0;
		break;
	case 40:	// 所有检波器测试
		m_pChannelSetupData->m_byCheck = 0;
		break;
	}
}