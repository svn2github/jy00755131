#pragma once


// CDlgUserInfo 对话框
#include "..\common\ConfigParseXML.h"
class CDlgUserInfo : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgUserInfo)

public:
	CDlgUserInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserInfo();

// 对话框数据
	enum { IDD = IDD_USERINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户信息
	CString m_strUserInfo;
	afx_msg void OnBnClickedBtnok();
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedBtnreset();
	virtual BOOL OnInitDialog();
};
