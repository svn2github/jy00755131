#pragma once


// CDlgNewUser 对话框

class CDlgNewUser : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewUser)

public:
	CDlgNewUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewUser();

// 对话框数据
	enum { IDD = IDD_NEWUSERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户名
	CString m_strUserName;
	// 用户密码
	CString m_strPassword;
};
