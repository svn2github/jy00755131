#pragma once
#include "afxwin.h"
#include "SetupGridCtrl.h"
// vivi 2010.11.9
// CDlgImpulsive 对话框

class CDlgImpulsive : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgImpulsive)

public:
	CDlgImpulsive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImpulsive();

// 对话框数据
	enum { IDD = IDD_IMPULSIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	// 设置对话框尺寸为IDC_STATICPICTURE的尺寸
	CRect dlgRect;
	// 控件IDC_COMBO_PULG的控制变量
	int m_comboPulg;
	// 控件IDC_COMBO_BOXTYPE的控制变量
	int m_comboBoxType;

	// 辅助记录道编辑
	CStatic m_ctrlEditGridLocation;
	// 辅助记录道列表
	CStatic m_ctrlGridLocation;
	// 辅助记录道控件
	CGridCtrlEdit* m_wndEditGrid;
	CGridCtrlList* m_wndListGrid;
public:
	BOOL OnInitDialog(void);
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg LRESULT OnGridItemClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridItemDblClk(WPARAM wParam, LPARAM lParam);	
	afx_msg void OnDestroy();
	// 载入脉冲类型设置
	void Load(void);
	// 加入一条辅助记录道记录
	void AppendRecord(AUXICHANNEL m_arrAuxiChannel);
};
