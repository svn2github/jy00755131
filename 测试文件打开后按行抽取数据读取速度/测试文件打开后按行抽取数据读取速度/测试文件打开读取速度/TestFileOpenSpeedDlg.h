// TestFileOpenSpeedDlg.h : 头文件
//

#pragma once


// CTestFileOpenSpeedDlg 对话框
class CTestFileOpenSpeedDlg : public CDialog
{
// 构造
public:
	CTestFileOpenSpeedDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CFile file;
	wchar_t* m_pbufRead;
	unsigned int m_uiSamplingRate;
	unsigned int m_uiFindLineCount;
	unsigned int m_uiBufSize;
	ULONGLONG m_ullPointer;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	// 打开文件读取数据
	void OpenFile(CString csOpenFilePath);
};
