#pragma once
#include "afxcmn.h"


// CDlgResultsOfAckedVP 对话框
/**
 * @class CDlgResultsOfAckedVP
 * @brief 显示采集排列点名结果
 *
 */
class CDlgResultsOfAckedVP : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgResultsOfAckedVP)

public:
	CDlgResultsOfAckedVP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResultsOfAckedVP();

// 对话框数据
	enum { IDD = IDD_RESULTSOFACKEDVP };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

public:
	/** 显示列表框 */
	CListBox		m_ctrlListResult;
	/** 总的无应答数量*/
	CString			m_strResult;
	/** 需要显示的无应答采集站信息 */
	CStringArray	m_arrResult;
	
};
