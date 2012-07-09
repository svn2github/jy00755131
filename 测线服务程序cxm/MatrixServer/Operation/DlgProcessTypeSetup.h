#pragma once
// 数据处理对象类型
#define PROCESS_IMPULSIVE		0		// 脉冲
#define PROCESS_STACKIMPULSIVE	1		// 脉冲叠加
#define PROCESS_CORRELAFTER		2		// 叠加后相关
#define PROCESS_CORRELBEFORE	3		// 叠加前相关
#define PROCESS_STACK			4		// 可控震源地震叠加
// 处理设置选项
#define PROCESS_STANDARD		0		// 标准
#define PROCESS_ADVANCE			1		// 高级

// CDlgProcessTypeSetup 对话框

class CDlgProcessTypeSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessTypeSetup)

public:
	CDlgProcessTypeSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessTypeSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROCESSSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 处理类型选项
	int m_radioProcessSetup;
	afx_msg void OnBnClickedRadioStandard();
	afx_msg void OnBnClickedRadioAdvanced();
	// 处理类型选择
	int m_cbindexProcessType;
	afx_msg void OnCbnSelchangeComboProcesstypeselect();
};
