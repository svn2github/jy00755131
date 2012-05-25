#pragma once


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
	afx_msg void OnBnClickedButtongo();
};
