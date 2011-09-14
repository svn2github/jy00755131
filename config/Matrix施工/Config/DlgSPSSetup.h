#pragma once
#include "FraseSPSToXML.h"

// DlgSPSSetup 对话框
class CDlgSPSSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSPSSetup)

public:
	CDlgSPSSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSPSSetup();

	// 对话框数据
	enum { IDD = IDD_SPSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFraseSPSToXML m_oFraseSPSToXML;
public:
	afx_msg void OnBnClickedBtnloadsps();
	afx_msg void OnBnClickedBtngenoptxml();
	afx_msg void OnBnClickedBtngenlinexml();
private:
	// SPS_R文件路径
	CString m_csSPSRFilePath;
	// SPS_X文件路径
	CString m_csSPSXFilePath;
	// 查找SPS_R文件路径
	void OnSelectSPSRFilePath(void);
	// 查找SPS_X文件路径
	void OnSelectSPSXFilePath(void);
	// 本地默认Operation XML路径
	CString m_csLocalOptXMLFile;
	// 本地默认Line XML路径
	CString m_csLocalLineXMLFile;
};