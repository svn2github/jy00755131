// IC_TESTDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "d:\program files (x86)\codejock software\mfc\xtreme toolkitpro v15.3.1\source\controls\edit\xtphexedit.h"
#include <vector>
#include "afxcmn.h"
#include "LandedDlg.h"
#include "Uart.h"
using std::vector;
using std::string;
// 定义默认缓冲区大小
#define DefaultBufSize	10240
// 定义默认缓冲区数值
#define DefaultBufValue	0xff
// 定义SCR地址
#define SCRAddr			1039
// 定义IRC4M地址
#define IRC4MAddr		1038
// 定义IRC1K地址
#define IRC1KAddr		1037
// 定义读数据模式
// #define ReadMode		0
// 定义写数据模式
// #define WriteMode		1
// CIC_TESTDlg dialog
class CIC_TESTDlg : public CDialog
{
// Construction
public:
	CIC_TESTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_IC_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 信息列表控制变量
	CListBox m_ctrlListMsg;
	// 烧写文件信息显示控制变量
	CXTPHexEdit m_ctrlEditHex;
	// 烧写数据
	LPBYTE  m_pProData;
	// Hex数据地址最大值
	unsigned int m_uiAddrMax;
	// Hex数据地址最小值
	unsigned int m_uiAddrMin;
	// SCR的值
	BYTE m_bySCR;
	// IRC4M的值
	BYTE m_byIRC4M;
	// IRC1K的值
	BYTE m_byIRC1K;
	// 重置控件
	void RefreshControls(void);
	// 串口接收数据回调函数
	static void CALLBACK OnUartRead(void* pFatherPtr, BYTE buf);
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnResetMsg();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnDestroy();

	// 烧写模式下拉菜单控制变量
	CComboBox m_ctrlComboProMode;
	// 端口号编辑控件控制变量
	CEdit m_ctrlComNo;
	// 登陆
	void OnLanded(void);
	afx_msg void OnBnClickedBtnLanded();
	// 重置界面
	void RefreshView(void);
	// 显示控件
	void ShowControls(int iStyle);
	// SCR的Spin控件控制变量
	CSpinButtonCtrl m_ctrlSpinSCR;
	CEdit m_ctrlEditSCR;
	CComboBox m_ctrlComboSCR;
	// IRC4M的Spin控件控制变量
	CSpinButtonCtrl m_ctrlSpinIRC4M;
	CEdit m_ctrlEditIRC4M;
	CComboBox m_ctrlComboIRC4M;
	// IRC1K的Spin控件控制变量
	CSpinButtonCtrl m_ctrlSpinIRC1K;
	CEdit m_ctrlEditIRC1K;
	CComboBox m_ctrlComboIRC1K;
	// 读回菜单控制变量
	CComboBox m_ctrlComboReadback;
	// 串口通讯类成员
	CUart m_oUart;
	// 发送数据队列
	CList<BYTE> m_olsSendData;
	// 读命令接收字节数
	unsigned int m_uiRecDataNum;
	// 接收数据队列
	CList<BYTE> m_olsRecData;
	afx_msg void OnBnClickedBtnOpencom();
	afx_msg void OnBnClickedBtnClosecom();
	afx_msg void OnBnClickedBtnReadbackDo();
	afx_msg void OnBnClickedBtnProgramDo();
	afx_msg void OnBnClickedBtnSCRDo();
	afx_msg void OnBnClickedBtnIrc4mDo();
	afx_msg void OnBnClickedBtnIrc1kDo();
	afx_msg void OnBnClickedBtnSavefile();
	// SCR在文件中位置
	unsigned int m_uiSCRPosition;
	// IRC4M在文件中的位置
	unsigned int m_uiIRC4MPosition;
	// IRC1K在文件中的位置
	unsigned int m_uiIRC1KPosition;
	// 烧写文件路径
	CString m_strProFilePath;
	// 定义读数据模式
	enum {ReadMode, WriteMode};
	// 向串口写模式
	bool m_bModeWrite;
	// 需要接收到的ADC数据
	unsigned int m_uiRecDataNeed;
	// 忙状态
	bool m_bBusy;
};
