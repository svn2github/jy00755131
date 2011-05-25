#pragma once

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
	virtual BOOL OnInitDialog();
	CString m_strComment;
};
