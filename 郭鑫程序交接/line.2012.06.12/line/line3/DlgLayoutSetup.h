#pragma once

#include "TabMarker.h"
#include "TabAux.h"
#include "TabDetour.h"
#include "TabMute.h"
#include "TabBlastMachine.h"

#include "SiteData.h"

/**
CDlgLayoutSetup 布局设置对话框类
*/
class CDlgLayoutSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgLayoutSetup)

public:
	CDlgLayoutSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLayoutSetup();

// 对话框数据
	enum { IDD = 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

public:	// 属性
	/** 现场数据*/
	CSiteData* m_pSiteData;
	/** 属性页容器*/
	CBCGPTabWnd m_oTabWnd;
	/** 属性页标记点*/
	CTabMarker m_oTabMarker;
	/** 属性页辅助道*/
	CTabAux m_oTabAux;
	/** 属性页迂回道*/
	CTabDetour m_oTabDetour;
	/** 属性页哑点*/
	CTabMute m_oTabMute;
	/** 属性页爆炸机控制器*/
	CTabBlastMachine m_oTabBlastMachine;

public:	// 方法
	// 响应鼠标单击按钮；Apply
	afx_msg void OnBnClickedButtonApply();
	// 响应鼠标单击按钮；Reset
	afx_msg void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；Add
	afx_msg void OnBnClickedButtonAdd();
	// 响应鼠标单击按钮；Change
	afx_msg void OnBnClickedButtonChange();
	// 响应鼠标单击按钮；Delete
	afx_msg void OnBnClickedButtonDelete();
	// 响应鼠标单击按钮；Clear
	afx_msg void OnBnClickedButtonClear();

	// 响应仪器图形视图弹出菜单设置标记点
	void OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置辅助道
	void OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置迂回道
	void OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight);
	// 响应检波器图形视图弹出菜单Mute设置
	void OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint);
};
