#pragma once


// CDlgCrew 对话框
#include "..\common\ConfigParseXML.h"
class CDlgCrew : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCrew)

public:
	CDlgCrew(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCrew();

// 对话框数据
	enum { IDD = IDD_CREWDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCrewName;
	int m_nSampleRate;
	int m_nGain3301;
	int m_nFilterFIR;
	int m_nFilterIIR;
	int m_nMux3301;
	int m_nMode5373;
	int m_nAttr5373;
public:
	void SetParams(CFDUConfig* pFDUConfig);
	void ApplyParams(CFDUConfig* pFDUConfig);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonapply();
	afx_msg void OnBnClickedButtonreset();
};
