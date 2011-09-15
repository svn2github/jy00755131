#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "TabPage.h"
#include "SiteData.h"
/**
CTabSelectShotPoint Tab属性页炮点选择设置类
*/
class CTabSelectShotPoint : public CTabPage
{
	DECLARE_DYNAMIC(CTabSelectShotPoint)

public:
	CTabSelectShotPoint();
	virtual ~CTabSelectShotPoint();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnGridCtrlListLButtonDown();	// 鼠标单击列表
	afx_msg void OnGridCtrlListDblClk();	// 鼠标单击双击列表

public:	// 属性
	/** 输入行*/
	CGridCtrlEdit m_oGridCtrlEdit;
	/** 列表*/
	CGridCtrlList m_oGridCtrlList;

	/** 索引号队列*/
	CList<int, int> m_olsNb;
	/** 索引号*/
	unsigned int m_uiTestNb;
	/** 现场数据*/
	CSiteData* m_pSiteData;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 重置
	void OnReset(void);
};
