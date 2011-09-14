#pragma once

#include "Instrument.h"

class CGraphInstrumentData
{
public:
	CGraphInstrumentData(void);
	~CGraphInstrumentData(void);

public: //属性	
	bool m_bInUsed;	// 图形单元是否使用中

	int m_iPosX;	// X方向位置
	int m_iPosY;	// Y方向位置
	int m_iPosIndex;	// 仪器位置索引

	CInstrument* m_pInstrument;	// 仪器指针

public: // 方法
	// 重置
	void OnReset();
	// 生成仪器位置索引
	static int CreatePosIndex(int iPosX, int iPosY);
	// 设置图形参数
	void SetDrawParam();

	// 画图，单元连接线
	void DrawUnitLinkLine(CDC* pDC, int iGridX, int iGridY);
	// 画图
	void DrawUnit(CDC* pDC, int iGridX, int iGridY, int iUnitWidth, int iUnitHight, int iUnitSizeMode, unsigned int uiTestType);
};

/*
单元绘制规则：
未分配IP地址的，显示为黄十字


属于QC范围的采集站
测试合格 绿色
测试不合格 红色
未测试 蓝色
无信号或未判读 黄色

*/