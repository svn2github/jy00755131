// XMLTESTDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CXMLTESTDlg 对话框
class CXMLTESTDlg : public CDialog
{
// 构造
public:
	CXMLTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XMLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	// XMLDOM文件对象
	CXMLDOMDocument m_oXMLDOMDocument;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReadxml();

public:
	void OpenXMLFile(CString PathName);
	void CloseXMLFile();
	afx_msg void OnBnClickedButtonWritexml();
	// 编辑框控件
	CEdit m_EditXML;
};
