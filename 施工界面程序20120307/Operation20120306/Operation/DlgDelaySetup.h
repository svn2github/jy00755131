#pragma once
#include "afxcmn.h"


// CDlgDelaySetup 对话框

class CDlgDelaySetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDelaySetup)

public:
	CDlgDelaySetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDelaySetup();

// 对话框数据
	enum { IDD = IDD_DELAYSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTRBNThumbPosChangingSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult);

	float m_fEndOfAcq;
	float m_fEndOfVP;
	
	CSliderCtrl m_SliderEndOfAcq;
	CSliderCtrl m_SliderEndOfVP;
	virtual BOOL OnInitDialog();
};
