#pragma once


// CDlgStackImpulsive 对话框

class CDlgStackImpulsive : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgStackImpulsive)

public:
	CDlgStackImpulsive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStackImpulsive();

// 对话框数据
	enum { IDD = IDD_STACKIMPULSIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog(void);
	//设置对话框尺寸为IDC_STATICPICTURE的尺寸
	CRect dlgRect;
};
