#pragma once

#include "GraphInstrumentData.h"
#include "InstrumentList.h"

class CGraphInstrumentList
{
public:
	CGraphInstrumentList(void);
	~CGraphInstrumentList(void);
public: // 属性
	CInstrumentList* m_pInstrumentList;	// 仪器队列指针	

	CGraphInstrumentData* m_pArrayGraphInstrument;	// 仪器图形单元数组指针	
	unsigned int m_uiCountAll;	// 总数	
	unsigned int m_uiCountUsed;	// 使用中数量	
	unsigned int m_uiCountFree;	// 空闲数量

	CMap<unsigned int, unsigned int, CGraphInstrumentData*, CGraphInstrumentData*> m_oGraphInstrumentMap;	// 图形单元索引表
	CList<int, int> m_olsRowLineNb;	// 单元格行-测线号对应列表
	CList<int, int> m_olsColumnPointNb;	// 单元格列-测点号对应列表

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

	CRect m_oRectArea;	// 图形区位置

public: // 方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();

	// 得到图形单元，根据图形单元数组索引
	CGraphInstrumentData* GetGraphInstrumentDataByIndex(unsigned int uiIndex);
	// 得到仪器对象指针，根据图形单元数组索引
	CInstrument* GetInstrumentByIndex(unsigned int uiIndex);
	// 得到图形单元，根据图形区坐标
	CGraphInstrumentData* GetGraphInstrumentDataByPosIndex(unsigned int uiPosX, unsigned int uiPosY);
	// 得到仪器对象指针，根据图形区坐标
	CInstrument* GetInstrumentByPosIndex(unsigned int uiPosX, unsigned int uiPosY);
	// 得到行索引对应的测线号
	int GetLineNbByRowIndex(unsigned int uiRowIndex);
	// 得到列索引对应的测点号
	int GetPointNbByColumnIndex(unsigned int uiColumnIndex);
	// 设置单元格行坐标和测线对应关系
	void SetLineNbList();
	// 设置单元格列坐标和测点对应关系
	void SetPointNbList();

	// 加载仪器图形视图仪器图形单元队列
	void LoadSiteData();
	// 设置仪器的位置，递归
	void SetInstrumentPos(CInstrument* pInstrument, unsigned int uiDirection);
	// 把选中区域转换为绝对排列字符串
	CString ConvertSelectAreaToAbsoluteSpreadString(CRect* pSelectAreaIndex);

	// 画网格图
	void DrawGrid(CDC* pDC, int iPaperX, int iPaperY, int iGridX, int iGridY);
	// 画测线标尺和测点标尺
	void DrawMarkLineNbAndPointNb(CDC* pDC, int iGridX, int iGridY);
	// 画测线标尺，测线
	void DrawMarkLineNb(CDC* pDC, int iGridY);
	// 画测点标尺，测点
	void DrawMarkPointNb(CDC* pDC, int iGridX);
	// 画图形单元连接线
	void DrawUnitLinkLineAll(CDC* pDC, int iGridX, int iGridY);
	// 画图形单元
	void DrawUnitAll(CDC* pDC, int iGridX, int iGridY, unsigned int uiTestType, bool bShowCross, bool bShowPower, bool bShowCollect);
	// 得到图形单元尺寸
	void GetUnitSize(int iGridX, int &iUnitWidth, int &iUnitHight, int &iUnitSizeMode);
};
