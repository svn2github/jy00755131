#pragma once

#include "GraphSensorData.h"
#include "ChannelList.h"

class CGraphSensorList
{
public:
	CGraphSensorList();
	~CGraphSensorList();

public: // 属性
	CChannelList* m_pChannelList;	// 测道队列指针	
	
	CGraphSensorData* m_pArrayGraphSensor;	// 检波器图形单元数组指针	
	unsigned int m_uiCountAll;	// 总数	
	unsigned int m_uiCountUsed;	// 使用中数量	
	unsigned int m_uiCountFree;	// 空闲数量

	COLORREF m_oPaperColor;	// 背景颜色
	COLORREF m_oGridColor;	// 网格线颜色

	unsigned int m_uiRowCount;	// 行数
	unsigned int m_uiColumnCount;	// 列数

	int m_iGridX;	// 单元格X方向尺寸
	int m_iGridY;	// 单元格Y方向尺寸

	int m_iPaperX;	// 图形区X方向尺寸
	int m_iPaperY;	// 图形区Y方向尺寸

	int m_iPaperHeadLeft;	// 图形区左边预留尺寸
	int m_iPaperHeadTop;	// 图形区左边预留尺寸

	int m_iGridXMin;	// 单元格X方向最小尺寸
	int m_iGridYMin;	// 单元格Y方向最小尺寸
	int m_iGridXMax;	// 单元格X方向最大尺寸
	int m_iGridYMax;	// 单元格Y方向最大尺寸

	int m_iSizeModeMin;	// 图形单元最小尺寸模式时的门限尺寸
	int m_iSizeModeMax;	// 图形单元最大尺寸模式时的门限尺寸

	int m_iUnitWidthMin;	// 图形单元宽度，最小尺寸模式
	int m_iUnitHightMin;	// 图形单元高度，最小尺寸模式

	int m_iUnitWidthNormal;	// 图形单元宽度，正常尺寸模式
	int m_iUnitHightNormal;	// 图形单元高度，正常尺寸模式

	int m_iUnitWidthMax;	// 图形单元宽度，最大尺寸模式	
	int m_iUnitHightMax;	// 图形单元高度，最大尺寸模式

public: // 方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();
	// 加载检波器图形视图检波器图形单元队列
	void LoadSiteData();
	// 得到测线号在图形区所在的行索引
	unsigned int GetUintRowIndex(unsigned int uiNbLine);
	// 得到测道号在图形区所在的列索引
	unsigned int GetUintColumnIndex(unsigned int uiNbPoint, unsigned int uiNbChannel);
	// 得到图形区行索引对应的测线号
	unsigned int GetUintLineNb(unsigned int uiRowIndex);
	// 得到图形区列索引对应的测点号
	unsigned int GetUintPointNb(unsigned int uiColumnIndex);
	// 得到图形区列索引对应的测道号
	unsigned int GetUintChannelNb(unsigned int uiColumIndex);
	// 得到图形区行列索引对应的测道
	CChannelSetupData* GetChannelSetupData(unsigned int uiRowIndex, unsigned int uiColumIndex);
	// 把选中区域转换为绝对排列字符串
	CString ConvertSelectAreaToAbsoluteSpreadString(CRect* pSelectAreaIndex);

	// 画网格图
	void DrawGrid(CDC* pDC, int iPaperX, int iPaperY, int iGridX, int iGridY);
	// 画测线标尺和测点标尺
	void DrawMarkLineNbAndPointNb(CDC* pDC, int iGridX, int iGridY);
	// 画测线标尺
	void DrawMarkLineNb(CDC* pDC, int iGridY);
	// 画测点标尺
	void DrawMarkPointNb(CDC* pDC, int iGridX);
	// 画图形单元
	void DrawUnitAll(CDC* pDC, int iGridX, int iGridY, unsigned int uiTestType, unsigned int uiSensorNbSelest, CMap<unsigned int, unsigned int, unsigned int, unsigned int>& oSensorNbMap, float fSeismonitorLimit);
	// 得到图形单元尺寸
	void GetUnitSize(int iGridX, int &iUnitWidth, int &iUnitHight, int &iUnitSizeMode);
};
