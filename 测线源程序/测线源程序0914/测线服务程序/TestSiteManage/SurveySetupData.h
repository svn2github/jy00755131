#pragma once

#include "LineList.h"
#include "PointList.h"
#include "ChannelList.h"
#include "SensorList.h"
#include "PointCodeList.h"

/**
*@brief 地形设置类
*/
class CSurveySetupData
{
public:
	CSurveySetupData();
	~CSurveySetupData();

public: // 属性
	/** XMLDOM文件对象指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 测线数量*/
	unsigned int m_uiLineCount;
	/** 测线头指针*/
	CLineSetupData* m_pLineHead;
	/** 测线尾指针*/
	CLineSetupData* m_pLineTail;

	/** 测点数量*/
	unsigned int m_uiPointCount;
	/** 测点头指针*/
	CPointSetupData* m_pPointHead;
	/** 测点尾指针*/
	CPointSetupData* m_pPointTail;

	/** 测道数量*/
	unsigned int m_uiChannelCount;
	/** 测道头指针*/
	CChannelSetupData* m_pChannelHead;
	/** 测道尾指针*/
	CChannelSetupData* m_pChannelTail;

	/** 测线数组总数*/
	unsigned int m_uiLineCountAll;
	/** 测线队列*/
	CLineList m_oLineList;

	/** 测点数组总数*/
	unsigned int m_uiPointCountAll;
	/** 测点队列*/
	CPointList m_oPointList;

	/** 测道数组总数*/
	unsigned int m_uiChannelCountAll;
	/** 测道队列*/
	CChannelList m_oChannelList;

	/** 检波器列表对象*/
	CSensorList m_oSensorList;
	/** 点代码列表对象*/
	CPointCodeList m_oPointCodeList;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
	// 解析测线
	void ParseLine(CLineSetupData* pLineSetupData);
	// 解析测点
	void ParsePoint(CPointSetupData* pPointSetupData);
	// 解析测道
	void ParseChannel(CChannelSetupData* pChannelSetupData);
	// 解析测线的测点定义
	void ParseLineReceiverSection(CLineSetupData* pLineSetupData);
	// 解析测线段
	void ParseLineReceiverSectionPart(CLineSetupData* pLineSetupData, CString strPart);
	// 链接
	void Link();
	// 链接测线
	void LinkLine();
	// 链接测点
	void LinkPoint();
	// 链接测道
	void LinkChannel();
	// 得到测道、根据IP地址
	CChannelSetupData* GetChannel(unsigned int uiIP);
	// 得到测道，根据测线号、测点号、测道号
	CChannelSetupData* GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel);
	// 得到测点，根据测点索引号
	CPointSetupData* GetPoint(unsigned int uiIndexPoint);
	// 得到测点，根据测线号、测点号
	CPointSetupData* GetPoint(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 得到测线
	CLineSetupData* GetLine(unsigned int uiNbLine);

	// 根据增量和方向，得到测点的相邻点
	CPointSetupData* GetPointByIncrementAndDirection(CPointSetupData* pPointSetupData, unsigned int uiIncrement, unsigned int uiDirection);
	// 解析排列，得到IP地址队列，采集道
	bool ParseSpreadToIPListByAbsoluteSpread(CList<unsigned int, unsigned int>* plsIP, CString strAbsoluteSpread);
	// 得到IP地址队列，参加新加检波器的初始测试
	bool GetIPListForSensorTestOnField(CList<unsigned int, unsigned int>* plsIP);
	// 解析排列，得到IP地址队列，采集道
	bool ParseSpreadToIPListByAbsoluteSpreadPart(CList<unsigned int, unsigned int>* plsIP, CString strPart);

	//根据判据号和IP地址，得到测试判据
	bool GetTestLimitValue(unsigned int uiTestLimitIndex, unsigned int uiIP, float& fLimit);
};
