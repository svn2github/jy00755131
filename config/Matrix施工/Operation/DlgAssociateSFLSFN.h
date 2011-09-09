#pragma once


// CDlgAssociateSFLSFN 对话框

class CDlgAssociateSFLSFN : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgAssociateSFLSFN)

public:
	CDlgAssociateSFLSFN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAssociateSFLSFN();

// 对话框数据
	enum { IDD = IDD_ASSOCIATESPLSPNDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fSourceLine;
	float m_fSourceNumber;
};
