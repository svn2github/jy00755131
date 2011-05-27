#pragma once
#include "PortMonitoringRecThread.h"
#include <afxmt.h>
// CPortMonitoringSend 对话框

class CPortMonitoringSend : public CDialog
{
	DECLARE_DYNAMIC(CPortMonitoringSend)

public:
	CPortMonitoringSend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPortMonitoringSend();

// 对话框数据
	enum { IDD = IDD_DIALOG_PORTMONITORING_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:
	// 定义临界区变量
	CCriticalSection m_Sec;
public:
	// 线程类指针
	CPortMonitoringRecThread* m_pRecThread;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
