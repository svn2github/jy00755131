#pragma once


#include "..\common\OperaStruct.h"
#include "DlgProcessImpulsive.h"
#include "SetupGridCtrl.h"
#include "ProcessTypes.h"
// CDlgProcessType 对话框

class CDlgProcessType : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessType)

public:
	CDlgProcessType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessType();

// 对话框数据
	enum { IDD = IDD_PROCESSTYPEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/** 标准还是高级 */
	int							m_ProcessSetup;
	/** 对话框的位置 */
	CStatic						m_wndDlgLocation;
	/** 处理类型选择 */
	CBCGPComboBox				m_ctrlProcessType;
	/** 脉冲处理类型对话框 */
	CDlgProcessImpulsive		m_dlgImpulsive;
	
	CStatic						m_ctrlRefraction;
	CStatic						m_ctrlEditGridLocation;
	CStatic						m_ctrlListGridLocation;

	CGridCtrlList				m_wndListGrid;
	CGridCtrlEdit				m_wndEditGrid;
	
	// 爆炸机所连的采集站SN
	DWORD m_dwFDUSN;
	// 采样长度
	DWORD m_dwRecordLen;
	// TB 窗口文本标识显示框
	CStatic m_ctrlTBWindow;
	// TB 窗口编辑框
	CEdit m_EditTbWindow;
	// TB 窗口
	DWORD m_dwTBWindow;	
	// 反射延时
	DWORD m_dwRefractionDelay;

private:
	// 保存在该对话框中的处理类型
	CProcessTypes		m_AllProcesses;
	// 装载处理类型属性
	void    ShowProcessDialog(int nProcessType);
	void	AppendRecord(CProcessType* pProcessType);
	void	LoadProcess(CProcessType* pProcess);
	void	LoadImpulsive(CProcessImpulsive* pProcess);
	//void	LoadStackImpulsive(CProcessStackImpulsive* pProcess);

	// 保存处理类型属性
	void	ApplyImpulsive(CProcessImpulsive* pProcess);
	CProcessType*	ApplyRecord(int nListRow);	
protected:
	afx_msg LRESULT OnGridItemClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridItemDblClk(WPARAM wParam, LPARAM lParam);	
public:
	virtual BOOL OnInitDialog();
	void	Load(void);
	
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtnchange();
	afx_msg void OnBnClickedBtndelete();
	afx_msg void OnBnClickedBtnapply();
	afx_msg void OnBnClickedBtnreset();
};
