#pragma once
#include "afxwin.h"


// CActiveSourceCtrl 对话框

class CActiveSourceCtrl : public CBCGPFormView
{

protected:
	CActiveSourceCtrl();   // 标准构造函数
	DECLARE_DYNCREATE(CActiveSourceCtrl)

public:
// 对话框数据
	enum { IDD = IDD_ACTIVESOURCEVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	virtual ~CActiveSourceCtrl();
	DECLARE_MESSAGE_MAP()
public:
	BOOL CreateView(CWnd* pParent, CRect rect, CCreateContext* pContext);
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnBnClickedButtongo();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonabort();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// IDC_BUTTONGO按键控制变量
	CButton m_btnGo;
	// IDC_BUTTONSTOP按键控制变量
	CButton m_btnStop;
	// IDC_BUTTONABORT按键控制变量
	CButton m_btnAbort;
	// IDC_STATIC_ACTIVESOURCEGROUP控件控制变量
	CStatic m_staticActiveSourceGroup;
	// IDC_STATIC_BLASTER控件控制变量
	CStatic m_staticBlaster;
	// IDC_STATIC_UPHOLE控件控制变量
	CStatic m_staticUphole;
	// IDC_STATIC_TB控件控制变量
	CStatic m_staticTB;
	// IDC_EDIT_UPHOLE控件控制变量
	CEdit m_editUphole;
	// IDC_EDIT_TB控件控制变量
	CEdit m_editTB;
	// 爆炸机状态
	int m_iBlasterStatus;
	// 爆炸机状态-未点火
	HICON m_oBlasterIconNoFire;
	// 爆炸机状态-正常
	HICON m_oBlasterIconNormal;
	// 爆炸机状态-警告
	HICON m_oBlasterIconWarning;
};
