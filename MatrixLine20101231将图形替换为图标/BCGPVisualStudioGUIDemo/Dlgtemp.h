#pragma once


// CDlgtemp 对话框

class CDlgtemp : public CDialog
{
	DECLARE_DYNAMIC(CDlgtemp)

public:
	CDlgtemp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgtemp();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
