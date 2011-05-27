#pragma once


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

	DECLARE_MESSAGE_MAP()
};
