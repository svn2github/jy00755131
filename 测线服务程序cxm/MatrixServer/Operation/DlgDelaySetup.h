#pragma once
#include "afxcmn.h"


// CDlgDelaySetup 对话框

class CDlgDelaySetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgDelaySetup)

public:
	CDlgDelaySetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDelaySetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_DELAYSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
	// 控件的数值变量
	float m_feditAcq;
	// 控件的数值变量
	float m_feditVP;
	// 控件的控制变量
	CSliderCtrl m_ctrlsliderAcq;
	// 控件的控制变量
	CSliderCtrl m_ctrlsliderVP;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSliderAcq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVp(NMHDR *pNMHDR, LRESULT *pResult);
};
