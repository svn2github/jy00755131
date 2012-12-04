#pragma once
#include "afxwin.h"


// CLandedDlg 对话框

class CLandedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLandedDlg)

public:
	CLandedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLandedDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// 用户名控制变量
	CComboBox m_ctrlUserName;
	// 密码控制变量
	CEdit m_ctrlPassword;
	virtual void OnOK();
	// 登陆
	void OnLand(void);
};
