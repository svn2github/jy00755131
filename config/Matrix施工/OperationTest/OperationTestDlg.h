// OperationTestDlg.h : 头文件
//

#pragma once

#include "FDUSocket.h"
#include "FDUDevice.h"
#include "afx.h"
#include "maincrosssocket.h"
#include "FDUDownSocket.h"
// COperationTestDlg 对话框
class COperationTestDlg : public CDialog
{
// 构造
public:
	COperationTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPERATIONTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFDUSocket* m_pFDUSocket;
	CArray<CFDUDevice*,CFDUDevice*>	m_arrFDU;



	bool CreateFDUSocket(void);
	afx_msg void OnDestroy();
	int InitFDUDevice(int nCount);
	int m_nTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnstart();
	afx_msg void OnBnClickedBtnstop();
	// 采样长度
	int m_nTimeLen;
	// 已采样次数
	int m_nTimeCount;
	int SendFDUData(CFDUDevice* pDevice,BOOL bLost);
	CFile m_File;


	int ProcessRetrans(char* pBuf);
	
	UINT m_nFDUNum;
	afx_msg void OnBnClickedButtonsetfdu();
	int ReadXML(void);
	afx_msg void OnBnClickedBtnreadxml();
	int SaveXML(void);
	afx_msg void OnBnClickedButton1();
	void SaveMatrixData(BYTE* pBuf, int nSize);
	void SaveMatrixDownData(BYTE* pBuf, int nSize);


	// -------FDU上行命令帧处理
	CMainCrossSocket		m_sockMainCross;
	DWORD m_dwFrmCount;
	CString m_strMainIP;
	CString m_strError;
	CFile m_ReFile;

	// -------FDU下行命令帧处理
	CMainCrossDownSocket	m_sockMainCrossDown;
	DWORD m_dwFDUFrmCount;
	CString m_strFDUIP;
	CFile m_TxFile;

	
	CString m_strFDUError;
};
