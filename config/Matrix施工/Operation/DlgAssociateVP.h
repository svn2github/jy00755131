#pragma once


// CDlgAssociateVP 对话框

class CDlgAssociateVP : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgAssociateVP)

public:
	CDlgAssociateVP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAssociateVP();

// 对话框数据
	enum { IDD = IDD_ASSOCIATEVPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	/** 炮点的Nb */
	UINT m_dwShotNb;
};
