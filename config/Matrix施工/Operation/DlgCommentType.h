#pragma once
#include "ShotComment.h"
#include "SetupGridCtrl.h"
// CDlgCommentType 对话框

class CDlgCommentType : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCommentType)

public:
	CDlgCommentType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCommentType();

// 对话框数据
	enum { IDD = IDD_COMMENTTYPESETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CStatic				m_ctrlEditGridLocation;
	CStatic				m_ctrlListGridLocation;
	CGridCtrlList		m_wndListGrid;
	CGridCtrlEdit		m_wndEditGrid;
	CString				m_strComment;
private:
	void  AppendRecord(CShotComment*	pComment);
	bool  ApplyRecord(int nRow,CShotComment*	pComment);
public:
	virtual BOOL OnInitDialog();
	void    Load(void);
	void	Apply(CShotComments *pAllComments);

protected:
	afx_msg LRESULT OnGridItemClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridItemDblClk(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtnchange();
	afx_msg void OnBnClickedBtndelete();
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedBtnreset();
};
