#pragma once
#include "afxwin.h"
#include "SetupGridCtrl.h"


// CDlgTeachersSetting 对话框

class CDlgTeachersSetting : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgTeachersSetting)

public:
	CDlgTeachersSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTeachersSetting();

// 对话框数据
	enum { IDD = IDD_DLGTEACHERSSETTING };

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
	void Load(CString csteacher);
	// 载入一个教师个人信息
	void LoadTeacher(CString csteacher);
};
