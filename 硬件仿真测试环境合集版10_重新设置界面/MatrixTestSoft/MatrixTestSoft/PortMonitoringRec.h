#pragma once


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

	DECLARE_MESSAGE_MAP()

};
