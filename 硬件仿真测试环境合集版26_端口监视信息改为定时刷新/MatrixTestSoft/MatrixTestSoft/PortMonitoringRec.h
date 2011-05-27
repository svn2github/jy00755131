#pragma once
#include "PortMonitoringSendThread.h"
#include <afxmt.h>
// CPortMonitoringRec 对话框

class CPortMonitoringRec : public CDialog
{
	DECLARE_DYNAMIC(CPortMonitoringRec)

public:
	CPortMonitoringRec(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPortMonitoringRec();

// 对话框数据
	enum { IDD = IDD_DIALOG_PORTMONITORING_REC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	// 线程类指针
	CPortMonitoringSendThread* m_pSendThread;
	// 定义临界区变量
	CCriticalSection m_Sec;
};
