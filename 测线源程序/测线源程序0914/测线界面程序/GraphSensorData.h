#pragma once

#include "ChannelSetupData.h"

class CGraphSensorData
{
public:
	CGraphSensorData(void);
	~CGraphSensorData(void);

public: //属性
	int m_iPosX;	// X方向位置
	int m_iPosY;	// Y方向位置
	CChannelSetupData* m_pChannelSetupData;	// 测道指针

public: // 方法
	// 重置
	void OnReset();
	// 设置图形参数
	void SetDrawParam();
	// 画图
	void DrawUnit(CDC* pDC, int iGridX, int iGridY, int iUnitWidth, int iUnitHight, int iUnitSizeMode, unsigned int uiTestType, bool bDrawSensorNb, float fSeismonitorLimit);
	// 得到噪声监测图形单元相对高度
	int GetSeismonitorGraphHight(float fSeismonitorLimit);
	// 得到噪声监测图形单元高度
	int GetSeismonitorGraphHight(int iUnitHight, float fSeismonitorLimit);
	// 得到噪声监测图形单元颜色
	void GetSeismonitorGraphColour(CBrush *oBrushUnit, float fSeismonitorLimit);
};

/*
单元绘制规则：
未分配IP地址的，显示为黄十字
跳过道，显示为绿十字
哑道，显示为红十字

属于QC范围的采集道
测试合格 绿色
测试不合格 红色
未测试 蓝色
无信号 蓝十字

不属于QC范围的采集道
采集道 灰色
*/