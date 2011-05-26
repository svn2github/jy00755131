#pragma once

#include "MainFrm.h"
#include "SiteData.h"

// CSensorListCtrl 视图

class CSensorListCtrl : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CSensorListCtrl)

public:
	CSensorListCtrl();
	virtual ~CSensorListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

public: // 属性
	CSiteData*	m_pSiteData;	// 现场数据对象

public: // 方法
	// 现场数据对象初始化
	void OnSiteDataInit();
	// 加载现场测道数据队列
	void OnLoadSiteDataAndFillToChannelList();
	// 重新加载现场测道数据
	void OnReloadSiteDataCMD();
	// 为新的测试数据刷新显示
	void OnRefreshForNewTestValue(int iTestType);

	// 生成表格头
	void CreateColumTitle();
	// 设置一行的数据
	void SetRowData(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData);
	// 设置一行的基本数据
	void SetRowDataOfBase(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData);
	// 设置一行的测试数据
	void SetRowDataOfTest(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData);
	// 设置一行的测试数据，依据测试类型
	void SetRowDataOfTestByTestType(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData, int iTestType);
	// 得到背景颜色，依据测试检查结果
	COLORREF GetColourByCheck(byte byCheck);
	// 得到标记文字，依据逻辑值
	COleVariant GetMarkerStringByLogic(bool bValue);
};


