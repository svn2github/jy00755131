// MatrixTestSoftDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "Parameter.h"
#include "TabUartToUdp.h"
#include "TabPortMonitoring.h"
#include "TabSample.h"
#include "TabADCSettings.h"
#include "TabADCDataShow.h"
#include "LogFile.h"


// CMatrixTestSoftDlg dialog
class CMatrixTestSoftDlg : public CDialog
{
// Construction
public:
	CMatrixTestSoftDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MATRIXTESTSOFT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	// TabContrl控件的控制变量
	CTabCtrl m_cTabCtrlItems;
	// CTabUartToUdp类的变量
	CTabUartToUdp m_oTabUartToUdp;
	// CTabPortMonitoring类的变量
	CTabPortMonitoring m_oTabPortMonitoring;
	// CTabGraphView类的变量
	CTabSample m_oTabSample;
	// CTabADCSettings类的变量
	CTabADCSettings m_oTabADCSettings;
	// CTabADCDataShow类的变量
	CTabADCDataShow m_oTabADCDataShow;
	// CLogFile类的变量
	CLogFile m_oLogFile;
protected: 
	// 初始化选项卡控件
	void InitTabControlItems(void);
	// 载入XML配置文件
	void LoadXmlFile(CString csXmlFileName);
	// 打开XML配置文件
	BOOL OpenXmlFile(CString csXmlFileName);
	// XMLDOM文件对象
	CXMLDOMDocument m_oXMLDOMDocument;
	// 载入串口转UDP配置
	bool LoadUartToUDPSetup(void);
	// 载入UDP端口监视配置
	bool LoadUDPPortMonitoringSetup(void);
	// 载入采样配置
	bool LoadSampleSetup(void);
	// 载入ADC参数设置配置
	bool LoadADCSettingsSetup(void);
	// 关闭程序配置文件
	void CloseXmlFile(void);
	// 保存串口转UDP配置
	bool SaveUartToUDPSetup(void);
	// 保存UDP端口监视配置
	bool SaveUDPPortMonitoringSetup(void);
	// 保存采样设置
	bool SaveSampleSetup(void);
	// 保存ADC参数设置配置
	bool SaveADCSettingsSetup(void);
	// 保存配置文件
	void SaveXmlFile(void);
public:
	afx_msg void OnTcnSelchangeTabItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
