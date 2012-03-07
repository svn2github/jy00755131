#pragma once


// CDlgAcquisitionControl 对话框
/**
 * @class CDlgAcquisitionControl
 * @brief 响应ActiveSource视图中Stop和Abort按钮。弹出该对话框供用户选择后续施工顺序。
 *
 * 定义一个炮点对象属性，是放炮表中一条记录的数据结构。
 */
class CDlgAcquisitionControl : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgAcquisitionControl)

public:
	CDlgAcquisitionControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAcquisitionControl();

// 对话框数据
	enum { IDD = IDD_ACTIVESOURCECONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnend();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
public:
	int		m_nSelectedButton;
protected:
	virtual void OnCancel();
};
