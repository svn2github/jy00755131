#pragma once

// 在对话框退出前，向父窗口发送消息，通知父窗口该无模式对话框准备退出
#define WM_DLGLOGINDESTROY (WM_USER + 100)


// CDlgLoginAdminServer 对话框
class CDlgLoginAdminServer : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgLoginAdminServer)

public:
	CDlgLoginAdminServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLoginAdminServer();

// 对话框数据
	enum { IDD = IDD_CONNECTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedBtnconnect();
	afx_msg void OnDestroy();

public:
	CString m_strLabel;
	CString m_strURL;
	CString m_strUserName;
	CString m_strPassword;
	
};
