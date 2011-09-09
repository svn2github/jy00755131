#pragma once


// CDlgSEGDSetup 对话框

class CDlgSEGDSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSEGDSetup)

public:
	CDlgSEGDSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEGDSetup();

// 对话框数据
	enum { IDD = IDD_SEGDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
