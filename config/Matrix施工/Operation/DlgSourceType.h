#pragma once
#include "afxwin.h"
#include "DlgSourceExplo.h"
#include "DlgSourceVibro.h"
// CDlgSourceType 对话框

class CDlgSourceType : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSourceType)

public:
	CDlgSourceType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSourceType();

// 对话框数据
	enum { IDD = IDD_SOURCESETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// Tab控件的大小
	CStatic				m_ctrlTabLocation;
	CBCGPButton			m_btnApply;
	CBCGPButton			m_btnReset;

	CBCGPTabWnd			m_wndTab;
	CDlgSourceExplo		m_DlgExplo;
	CDlgSourceVibro		m_DlgVibro;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedBtnreset();
private:
	bool CheckSourceNb(CBCGPGridCtrl* pGridOne,CBCGPGridCtrl* pGridTwo);
};
