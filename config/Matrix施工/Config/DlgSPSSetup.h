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
private:
	// SPS_R文件路径
	CString m_csSPSFilePath;
	// 查找SPS文件路径
	void OnSelectSPSFilePath(void);
	// 本地默认Operation XML路径
	CString m_csLocalOptXMLFile;
	// 本地默认Line XML路径
	CString m_csLocalLineXMLFile;
	// 本地默认LineInit TXT路径
	CString m_csLocalLineInitTXTFile;
	// 程序当前路径
	wchar_t m_wcExeCurrentPath[MAX_PATH];
	// 生成施工XML
	void OnGenOptXML();
	// 生成测线XML
	void OnGenLineXML();
	// 生成测线仿真排列文件
	void OnGenLineInitTXT();
};