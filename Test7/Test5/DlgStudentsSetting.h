#pragma once
#include "afxwin.h"
#include "SetupGridCtrl.h"


// CDlgStudentsSetting 对话框

class CDlgStudentsSetting : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgStudentsSetting)

public:
	CDlgStudentsSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStudentsSetting();

// 对话框数据
	enum { IDD = IDD_DLGSTUDENTSSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctrlEditGridLocation;
	CStatic m_ctrlGridLocation;
	CGridCtrlList			m_wndListGrid;
	CGridCtrlEdit			m_wndEditGrid;
	virtual BOOL OnInitDialog(void);
	// 载入表格数据
	void Load(CString csstudents);
	// 载入一个学生个人信息
	void LoadStudent(CString csstudent);
};
