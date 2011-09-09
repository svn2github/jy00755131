#pragma once


// CDlgProcessImpulsive 对话框
#include "SetupGridCtrl.h"
#include "..\common\OperaStruct.h"

class CDlgProcessImpulsive : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessImpulsive)

public:
	CDlgProcessImpulsive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessImpulsive();

// 对话框数据
	enum { IDD = IDD_PROCESSIMPULSIVEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/** 网格坐标 */
	CStatic				m_ctrlEditGridLocation;
	/** 网格坐标 */
	CStatic				m_ctrlListGridLocation;
	/** 列表框网格，显示所有辅助道 */
	CGridCtrlList		m_wndListGrid;
	/** 编辑一条辅助道记录 */
	CGridCtrlEdit		m_wndEditGrid;

private:
	CProcessImpulsive*		m_pProcess;
	void	AppendRecord(AUXICHANNEL* pAuxiChannel);
	bool	ApplyRecord(int nRow,AUXICHANNEL* pAuxiChannel);
protected:
	afx_msg LRESULT OnGridItemClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridItemDblClk(WPARAM wParam, LPARAM lParam);	
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtnchange();
	afx_msg void OnBnClickedBtndelete();
public:
	virtual BOOL OnInitDialog();
	void	Load(CProcessImpulsive* pProcess);
    void    Apply(CProcessImpulsive* pProcess);
	
};
