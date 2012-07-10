#pragma once
#include "Parameter.h"
#include "SetupGridCtrl.h"
// CDlgProcessTypeSetup 对话框

class CDlgProcessTypeSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessTypeSetup)

public:
	CDlgProcessTypeSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessTypeSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROCESSSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioStandard();
	afx_msg void OnBnClickedRadioAdvanced();
	afx_msg void OnCbnSelchangeComboProcesstypeselect();
	afx_msg void OnBnClickedButtonAuxAdd();
	afx_msg void OnBnClickedButtonAuxChange();
	afx_msg void OnBnClickedButtonAuxDelete();
	afx_msg void OnBnClickedButtonAcqAdd();
	afx_msg void OnBnClickedButtonAcqChange();
	afx_msg void OnBnClickedButtonAcqDelete();
	afx_msg void OnBnClickedButtonProAdd();
	afx_msg void OnBnClickedButtonProChange();
	afx_msg void OnBnClickedButtonProDelete();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnDestroy();
public:
	// 处理类型选项
	int m_radioProcessSetup;
	// 处理类型选择
	int m_cbindexProcessType;
	// Plug选项的数值变量
	int m_cbindexPlug;
	// 设备类型选项的数值变量
	int m_cbindexBoxType;
	// 串号的数值变量
	int m_editindexSerialNb;
	// 记录时间数值变量
	int m_editindexRecordLength;
	// TB时间窗口的数值变量
	int m_editindexTBWindow;
	// PeakTime的数值变量
	int m_editindexPeakTime;
	// Raw的布尔型变量
	BOOL m_bCheckRaw;
	// CorrelWith的字符串变量
	CString m_strCorrelWith;
	// AuxesFromDSD的布尔型变量
	BOOL m_bAuxesFromDSD;
public:
	CGridCtrlList		m_wndAuxListGrid;
	CGridCtrlEdit		m_wndAuxEditGrid;
	CGridCtrlList		m_wndAcqListGrid;
	CGridCtrlEdit		m_wndAcqEditGrid;
	CGridCtrlList		m_wndProTypeListGrid;
	CGridCtrlEdit		m_wndProTypeEditGrid;
	// 显示处理类型窗口
	void OnShowProcessTypeWindow(int iProcessType);
	// 显示Aux窗口
	void OnShowAuxWindow(void);
	// 显示Process Type窗口
	void OnShowProTypeWindow(void);
	// 设置Impulsive处理类型控件的位置
	void SetProcessImpulsiveCtrlsRect(void);
};
