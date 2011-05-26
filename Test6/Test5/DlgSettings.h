#pragma once
#include "afxwin.h"
#include "DlgStudentsSetting.h"
#include "DlgTeachersSetting.h"


// CDlgSettings 对话框

class CDlgSettings : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSettings)

public:
	CDlgSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSettings();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctrlTabLocation;
	CBCGPTabWnd			m_wndTab;
	CDlgStudentsSetting	m_DlgStudents;
	CDlgTeachersSetting	m_DlgTeachers;
	virtual BOOL OnInitDialog(void);
};
