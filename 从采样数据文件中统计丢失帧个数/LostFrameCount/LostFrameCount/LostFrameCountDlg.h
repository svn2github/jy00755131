// LostFrameCountDlg.h : 头文件
//

#pragma once


// CLostFrameCountDlg 对话框
class CLostFrameCountDlg : public CDialog
{
// 构造
public:
	CLostFrameCountDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CLostFrameCountDlg();

// 对话框数据
	enum { IDD = IDD_LOSTFRAMECOUNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCallostdataframe();
	// 文件开始序号
	UINT m_uiFileStartNb;
	// 文件结束序号
	UINT m_uiFileEndNb;
	// 单个设备每文件存储数据个数
	UINT m_uiSaveADCDataFrameNum;
	// 总设备个数
	UINT m_uiInstrumentTotalNum;
	// 采样设备个数
	UINT m_uiInstrumentSampleNum;
	// 采样帧的大小
	UINT m_uiFrameSize;
	afx_msg void OnBnClickedButtonFilepath();
	// 文件夹路径
	CString m_csFilePath;
	// 计算丢失数据帧个数
	void OnCalLostDataFrameNum(void);
	// 丢失帧个数指针
	unsigned int * m_uipLostFrameCount;
	// 处理最后一个保存的数据文件
	void OnCalLastFile(unsigned int uiLastFileNb);
	// 处理自动保存完得数据文件
	void OnCalFinishFile(unsigned int uiFinishFileNb);
};
