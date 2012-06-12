// SerialIODlg.h : 头文件
//

#pragma once
#include "afxwin.h"

typedef struct __tagBLASTER_PROTOCOL
{
	char sp_[6];
	char sl_[8];
	char sn_[8];
	char si_;
	char seq_[3];
}BLASTER_PROTOCOL, *BLASTER_PROTOCOL_PTR;

// CSerialIODlg 对话框
class CSerialIODlg : public CDialog
{
// 构造
public:
	CSerialIODlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_SERIALIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	void output_log(CString const& log);
	void com_output(CString const& result_text);
	void parse_and_output_info(CString const& info);
	void echo_status(CString const& info);

protected:
	HICON m_hIcon;
	bool be_startup_;
	HANDLE com_port_; 
	CString recv_info_;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	CEdit output_edit_;
	CComboBox combobox_;
	afx_msg void OnBnClickedOk();
	CButton button_;
	CButton gps_check_;
};
